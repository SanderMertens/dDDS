/*
 * dDDS_metaserializer.c
 *
 *  Created on: Dec 14, 2012
 *      Author: sander
 */

#include "dDDS/dDDS.h"
#include "dDDS/MetaXml.h"


/* #define SER_DEBUG */

/* Quick hack to be able to debug the serializer */
corto_struct dDDS_Time_t_o = NULL;

/* --------------------- Serialization driving functions -------------------- */

typedef struct dDDS_context_s* dDDS_context;
struct dDDS_context_s {
    corto_ll items; /* corto_ll<dDDS_item> - Dependency ordered list of contextItems. */
    corto_ll declarations; /* corto_ll<dDDS_item> - Declarations - for cycle detection. */

    /* Variables used for printing */
    corto_object module; /* Used for keeping track of current scope while printing. */
    corto_ll inlineProcessed; /* corto_ll<corto_type> - Keeps a list of the inline types defined in the current top-level type. This list is needed
                         because types can be used more than once within a given type-scope and the algorithm must be able
                         to determine whether a type should be defined or whether it already is. */
    corto_ll xmlbuff;  /* corto_ll<corto_char*> This contains a list of string-buffers, which together form the metadescriptor. The
                        final step is to aggegrate these buffers into one string. */

    /* Module optimization */
    corto_ll modules; /* corto_ll<dDDS_moduleItem> - This list will be populated with used modules when types are parsed with modules. Per module
                       the number of types is counted. At print-time, arrays are allocated of size typeCount * sizeof(dDDS_item).
                       These arrays will at any time hold the unprinted root-type items with dependency count 0. */
};

/* This structure is only used in the final module optimization step. */
typedef struct dDDS_item_s* dDDS_item;
typedef struct dDDS_moduleItem_s* dDDS_moduleItem;
struct dDDS_moduleItem_s {
    corto_object self;
    corto_uint32 typeCount; /* Worst case amount of types that can be stored at once in this module. */
    dDDS_item* array; /* Array which at any time contains the unprinted root-type items with dependency count 0 */
    dDDS_item* start; /* Pointer to the next item that should be printed. */
    dDDS_item* end; /* Pointer which indicates the next free element in the list. */
};

struct dDDS_item_s {
    corto_type self;
    corto_int32 refcount; /* This is the number of unresolved(unprinted) dependencies */
    corto_ll dependees; /* corto_ll<dDDS_item> */
    dDDS_moduleItem module; /* Pointer to module-item. This overcomes the need for doing
                             module-lookups at print-time, when a type must be added
                             to the type-array of a module when it reaches dependency-count 0. */
};


/* --------------------- Serialization utilities --------------------- */

/* Find the root-type (NULL if type is not an inlined type) */
corto_type
dDDS_utilRootType(
    corto_type type)
{
    corto_object result, prev;

    prev = NULL;

    if(corto_checkAttr(type, CORTO_ATTR_SCOPED)) {
        result = corto_parentof(type);
        while(result && (corto_class_instanceof(corto_type_o, result))) {
            prev = result;

            /* If result isn't a module, there must be higher levels in the hierarchy. */
            result = corto_parentof(result);
            corto_assert(result != NULL, "missing parent.");
        }
    }

    return corto_type(prev);
}

/* Check whether an object is inlined.
 * Collections and primitives are conceptually always inlined,
 * though the implementation might place them in a scope.
 */
static corto_bool
dDDS_utilIsInlined(
    corto_type type)
{
    corto_bool inlined;

    inlined = FALSE;

    if(corto_type(type) == type) {
        inlined = (type->kind == CORTO_COLLECTION) || ((type->kind == CORTO_PRIMITIVE) && (corto_primitive(type)->kind != CORTO_ENUM));
    }

    if(!inlined) {
        inlined = corto_class_instanceof(corto_type_o, corto_parentof(type));
    }

    return inlined;
}

static dDDS_item
dDDS_itemNew(
    corto_type self)
{
    dDDS_item result;

    result = corto_alloc(sizeof(struct dDDS_item_s));
    result->self = self;
    result->refcount = 0;
    result->dependees = NULL;
    result->module = NULL;

    return result;
}

static void
dDDS_itemFree(
    dDDS_item item)
{
    if(item->dependees) {
        corto_llFree(item->dependees);
    }
    corto_dealloc(item);
}

static void
dDDS_moduleItemFree(
    dDDS_moduleItem item)
{
    if(item->array) {
        corto_dealloc(item->array);
    }
    corto_dealloc(item);
}

static dDDS_moduleItem
dDDS_moduleItemNew(
    corto_object module)
{
    dDDS_moduleItem result;

    result = corto_alloc(sizeof(struct dDDS_moduleItem_s));
    result->array = NULL;
    result->start = NULL;
    result->end = NULL;
    result->typeCount = 0;
    result->self = module;

    return result;
}

static dDDS_context
dDDS_contextNew(void)
{
    dDDS_context result;

    result = corto_alloc(sizeof(struct dDDS_context_s));
    result->items = corto_llNew();
    result->declarations = corto_llNew();

    result->module = NULL;
    result->xmlbuff = corto_llNew();
    result->inlineProcessed = corto_llNew();
    result->modules = corto_llNew();

    return result;
}

static int
dDDS_freeItems(
    void* o,
    void* udata)
{
    dDDS_item item;
    CORTO_UNUSED(udata);

    item = o;
    dDDS_itemFree(item);

    return 1;
}

static int
dDDS_freeModuleItems(
    void* o,
    void* udata)
{
    dDDS_moduleItem item;
    CORTO_UNUSED(udata);

    item = o;
    dDDS_moduleItemFree(item);

    return 1;
}

static void
dDDS_contextFree(
    dDDS_context context)
{
    /* Free items */
    corto_llWalk(context->items, dDDS_freeItems, NULL);
    corto_llFree(context->items);

    /* Free declarations */
    corto_llFree(context->declarations);

    /* Free modules */
    corto_llWalk(context->modules, dDDS_freeModuleItems, NULL);
    corto_llFree(context->modules);

    corto_assert(context->xmlbuff == NULL, "xmlbuffer is not NULL");

    corto_dealloc(context);
}

/* Add a dependee to the dependee list of an item */
static void
dDDS_itemAddDependee(
    dDDS_item item,
    dDDS_item dependee)
{
    if(!item->dependees) {
        item->dependees = corto_llNew();
    }
    corto_llInsert(item->dependees, dependee);

    /* Increase the refcount of the dependee. */
    dependee->refcount++;
    corto_assert(item->dependees != NULL, "dependees is empty.");
}

/* Add item to module array */
static void
dDDS_itemAddToModule(
    dDDS_item item) {
    dDDS_moduleItem module = item->module;

    if(!module->array) {
        module->array = corto_alloc(module->typeCount * sizeof(dDDS_item));
        module->start = module->array;
        module->end = module->array;
    }

    *(module->end) = item;
    module->end++;

    corto_assert(module->end <= (module->array + module->typeCount), "type must not exceed bounds for module.");
}

struct dDDS_itemLookup_t {
    corto_type type;
    dDDS_item result;
};

static int
dDDS_itemDerefDependee(
    void* o, void* udata)
{
    CORTO_UNUSED(udata);

    if(!--((dDDS_item)o)->refcount) {
        dDDS_itemAddToModule(o);
    }
    return 1;
}

/* Deref dependees. Dependees are deref'd when a dependency (the item passed to this function) is
 * resolved. When a dependee reaches refcount zero, it can be processed itself. */
static void
dDDS_itemDerefDependees(
    dDDS_item item)
{
    if(item->dependees) {
        corto_llWalk(item->dependees, dDDS_itemDerefDependee, NULL);
    }
}

int
dDDS_contextLookupAction(
    void* o,
    void* userData)
{
    struct dDDS_itemLookup_t* data;

    data = userData;
    if(((dDDS_item)o)->self == data->type) {
        data->result = o;
    }

    return data->result == NULL;
}

/* Check whether an item is already processed */
static dDDS_item
dDDS_contextIsProcessed(
    dDDS_context context,
    corto_type type)
{
    struct dDDS_itemLookup_t walkData;

    walkData.type = type;
    walkData.result = NULL;
    corto_llWalk(context->items, dDDS_contextLookupAction, &walkData);

    return walkData.result;
}

/* Check for cycles */
static dDDS_item
dDDS_contextCheckCycles(
    dDDS_context context,
    corto_type type)
{
    struct dDDS_itemLookup_t walkData;

    walkData.type = type;
    walkData.result = NULL;
    corto_llWalk(context->declarations, dDDS_contextLookupAction, &walkData);

    return walkData.result;
}

struct dDDS_contextFindModule_t {
    corto_object find;
    dDDS_moduleItem result;
};

static int
dDDS_contextFindModule(
    void* o,
    void* userData)
{
    struct dDDS_contextFindModule_t* data;

    data = userData;
    if(((dDDS_moduleItem)o)->self == data->find) {
        data->result = o;
    }

    return data->result == NULL;
}

/* Mark an item as processed, add to the context->items list */
static void
dDDS_contextProcessed(
    dDDS_context context,
    dDDS_item item)
{
    struct dDDS_contextFindModule_t walkData;
    corto_assert(!dDDS_contextIsProcessed(context, item->self), "function called on already processed object.");
    corto_llAppend(context->items, item);

    /* Find corresponding module */
    walkData.find = corto_parentof(item->self);
    walkData.result = NULL;
    corto_llWalk(context->modules, dDDS_contextFindModule, &walkData);

    /* If module is not found, create new module object. */
    if(!walkData.result) {
        walkData.result = dDDS_moduleItemNew(walkData.find);
        corto_llInsert(context->modules, walkData.result);
    }

    /* Administrate extra type for module. */
    walkData.result->typeCount++;

    /* Store pointer in item to module, so that during printing no lookups are required. */
    item->module = walkData.result;
}

/* Mark an item as declared, add to the context->declarations list */
static void
dDDS_contextDeclare(
    dDDS_context context,
    dDDS_item item)
{
    corto_assert(!dDDS_contextIsProcessed(context, item->self), "item passed to dDDS_contextDeclare that is already processed.");
    corto_llAppend(context->declarations, item);
}

/* Serialize a type */
static int
dDDS_serializeType(
    dDDS_context context,
    dDDS_item rootType,
    corto_type type,
    corto_bool allowCycles,
    dDDS_item* out);

/* Serialize dependencies of a typedef */
static int
dDDS_serializeTypedefDependencies(
    dDDS_context context,
    dDDS_item rootType,
    corto_bool allowCycles,
    corto_type type)
{
    dDDS_item alias;

    alias = NULL;

    /* Resolve dependency */
    if(dDDS_serializeType(context, rootType, type, allowCycles, &alias)) {
        goto error;
    }

    /* Add typedef to dependee list of dependency */
    if(alias) {
        dDDS_itemAddDependee(alias, rootType);
    }

    return 0;
error:
    return -1;
}

/* Serialize dependencies of a collection. */
static int
dDDS_serializeCollectionDependencies(
    dDDS_context context,
    dDDS_item rootType,
    corto_collection type)
{
    dDDS_item subType;
    corto_bool allowCycles;

    subType = NULL;
    allowCycles = FALSE;

    /* Check if type is a sequence or array to find out if cycles are allowed. */
    if(type->kind == CORTO_SEQUENCE) {
        allowCycles = TRUE;
    }

    /* Resolve dependency */
    if(dDDS_serializeType(context, rootType, type->elementType, allowCycles, &subType)) {
        goto error;
    }

    /* Add collection to dependee list of subType */
    if(subType) {
        dDDS_itemAddDependee(subType, rootType);
    }

    return 0;
error:
    return -1;
}

/* Serialize dependencies for a struct */
static int
dDDS_serializeStructureDependencies(
    dDDS_context context,
    dDDS_item rootType,
    corto_interface type)
{
    corto_member member;
    dDDS_item memberType;
    corto_uint32 i;

    memberType = NULL;

    if(corto_class_instanceof(corto_struct_o, type)) {
        if(corto_interface(type)->base) {
            dDDS_serializeStructureDependencies(context, rootType, corto_interface(type)->base);
        }
    }

    /* Walk members to resolve dependencies of struct. */
    for(i=0; i<type->members.length; i++) {
        member = type->members.buffer[i];
        /* Serialize memberType, do not allow cycles. */
        if(!member->type->reference) {
            if(dDDS_serializeType(context, rootType, member->type, FALSE, &memberType)) {
                goto error;
            }

            if(memberType) {
                dDDS_itemAddDependee(memberType, rootType);
            }
        }
    }

    return 0;
error:
    return -1;
}

/* Serialize dependencies of a type.
 *
 * Parameter 'rootType' is typically the same as the 'type' parameter.
 * However, for inline types these two differ. The distinction
 * enables that dependencies are always added to the
 * top-level type. */
static int
dDDS_serializeTypeDependencies(
    dDDS_context context,
    dDDS_item rootType,
    corto_bool allowCycles,
    corto_type type)
{
    int result;

    result = 0;

    /* Forward to the correct dependency-resolve function, depending on metaKind */
    if(type != corto_type(type)) {
        /* Allowance of cycles is transparently forwarded to typedefs. */
        result = dDDS_serializeTypedefDependencies(context, rootType, allowCycles, corto_type(type));
    }else {
        switch(type->kind) {
        case CORTO_COLLECTION:
            result = dDDS_serializeCollectionDependencies(context, rootType, corto_collection(type));
            break;
        case CORTO_PRIMITIVE:
            switch(corto_primitive(type)->kind) {
            case CORTO_ENUM:
                /* Enumerations have no dependencies */
                break;
            default:
                corto_assert(0, "Invalid type-kind for dependency-routine.");
                break;
            }
            break;
        case CORTO_COMPOSITE:
            result = dDDS_serializeStructureDependencies(context, rootType, corto_interface(type));
            break;
        default:
            corto_assert(0, "Invalid type-kind.");
            break;
        }
    }

    return result;
}

/* Serialize a type */
static int
dDDS_serializeType(
    dDDS_context context,
    dDDS_item rootType,
    corto_type type,
    corto_bool allowCycles,
    dDDS_item* out)
{
    dDDS_item item;
    corto_bool isTypedef;

#ifdef SER_DEBUG
    static corto_uint32 indent = 0;
    printf("%*s>> type %s\n", indent*3, " ", corto_fullpath(NULL, type));
    indent++;
#endif

    item = NULL;

    if(type != corto_type(type)) {
        isTypedef = TRUE;
    }else {
        isTypedef = FALSE;
    }

    switch(type->kind) {
    case CORTO_PRIMITIVE:
        /* Primitives and collections are always treated as inline types, and are generated when a type is printed. */
        if(!isTypedef && (corto_primitive(type)->kind != CORTO_ENUM)) {
            break;
        }
        /* no break */
    case CORTO_COMPOSITE:
        /* Check if type is the c_time type in the rootscope which is not handled as a seperate type, but will result in a <Time/>. */
    {
        corto_id id;
        corto_path(id, NULL, type, "::");
        if(!strcmp("::DDS::Time_t", id)) {
            break; /* Don't handle c_time */
        }
    }
    /* no break */
    /* Fallthrough on purpose. */
    case CORTO_COLLECTION: /* Collections are always treated as inline types, but do have dependencies. */
        /* Check if item is already processed */
        if(!(item = dDDS_contextIsProcessed(context, type))) {
            /* Check for invalid cycles. Don't process type if a cycle is detected. */
            if(!isTypedef && (type->kind == CORTO_COMPOSITE) && dDDS_contextCheckCycles(context, type)) {
                if(!allowCycles) {
                    corto_error("dDDS_serializerXMLTypeInfoSerialize: unsupported cycle detected!");
                    goto error;
                }
            }else {
                /* Don't process inlined types in graph. These are resolved when the parent-type is printed, which is always in correct
                 * dependency order, which is the order of members(structs) or cases(unions). Collections are also considered
                 * inlined, because the actual location of a collection type is implementation specific. */
                if(!dDDS_utilIsInlined(type)) {
                    /* Here, the usage of 'rootType' is prohibited since this is not an inlined type. */

                    /* Create contextItem */
                    item = dDDS_itemNew(type);

                    /* Add forward-declaration marker for structs and unions, to allow references to self, so that
                     * the cycles can be detected. IDL does not allow other types to be forward-declared, thus
                     * cannot introduce cyclic references. */
                    if(!isTypedef && (type->kind == CORTO_COMPOSITE)) {
                        dDDS_contextDeclare(context, item);
                    }

                    /* Resolve dependencies of type */
                    if(dDDS_serializeTypeDependencies(context, item, allowCycles, type)) {
                        /* An error occurred, most likely an unsupported cyclic dependency */
                        dDDS_itemFree(item);
                        goto error;
                    }

                    /* If success, add item to processed list */
                    if(item) {
                        dDDS_contextProcessed(context, item);
                    }
                    break;
                }else {
                    corto_type typeRoot;

                    corto_assert(rootType != NULL, "rootType is zero.");

                    /* Still need to resolve dependencies for inlined types */
                    if(dDDS_serializeTypeDependencies(context, rootType, allowCycles, type)) {
                        goto error;
                    }

                    /* If rootType is referencing an inlined type that is defined in the scope of another type,
                     * rootType is implicitly dependent on the rootType of the inlined type. */
                    if(rootType->self != (typeRoot = dDDS_utilRootType(type))) {
                        dDDS_item typeRootItem = NULL;

                        /* An empty typeRoot means that the type is not stored as inlined object, but conceptually it is.
                         * This will typically occur for intern collectiontypes like c_string, who are defined
                         * in the root. */
                        if(typeRoot) {
                            /* Serialize rootType of type */
                            if(dDDS_serializeType(context, NULL, typeRoot, allowCycles, &typeRootItem)) {
                                goto error;
                            }

                            /* Because the 'typeRoot' is the root type of 'type', this can never be an inlined
                             * type, neither can it be a primitive type (which is not serialized). Thus, typeRootItem
                             * must always be set.
                             */
                            corto_assert(typeRootItem != NULL, "see comment.");

                            /* Add the root-type of the type to 'rootType' */
                            dDDS_itemAddDependee(typeRootItem, rootType);
                        }
                    }
                }
            }
        }
        break;
    default:
        /* The serializer shouldn't call serializeType with metaKinds other than the ones described above. */
        corto_assert(0, "invalid typekind.");
        break;
    }

    /* Return item if out is set */
    if(out) {
        *out = item;
    }

#ifdef SER_DEBUG
    indent--;
    printf("%*s<< finish\n", indent*3, " ");
#endif

    return 0;
error:
    return -1;
}

#define dDDS_XML_BUFFER (512)

/* Append to buffer-list. Parameter 'str' is never larger than dDDS_XML_BUFFER bytes. */
static void
dDDS_printXmlAppend(
    dDDS_context context,
    char* str /* char[dDDS_XML_BUFFER] */)
{
    char* lastBuffer;
    corto_uint32 strLen, buffLen, spaceLeft;

    /* Get first buffer from list, create if it didn't exist */
    lastBuffer = corto_llGet(context->xmlbuff, 0);
    if(!lastBuffer) {
        lastBuffer = corto_alloc(dDDS_XML_BUFFER + 1);
        *lastBuffer = '\0';
        corto_llInsert(context->xmlbuff, lastBuffer);
    }

    strLen = strlen(str);
    buffLen = strlen(lastBuffer);
    spaceLeft = dDDS_XML_BUFFER - buffLen;

    /* If length of string is larger than the space left in the buffer, allocate new buffer. */
    if(strLen >= spaceLeft) {
        char* nextBuffer;
        /* Fill remaining bytes of current buffer. */
        memcpy(lastBuffer + buffLen, str, spaceLeft);
        lastBuffer[dDDS_XML_BUFFER] = '\0';

        /* Do the remaining string in the next buffer. */
        nextBuffer = corto_alloc(dDDS_XML_BUFFER + 1);
        memcpy(nextBuffer, str + spaceLeft, strLen - spaceLeft + 1 /* 0-terminator */);

        /* Insert new buffer in iterator */
        corto_llInsert(context->xmlbuff, nextBuffer);

    /* ... otherwise, just append the string to the current buffer. */
    }else {
        strcat(lastBuffer, str);
    }
}

static void
dDDS_printXmlAggegrate(
    dDDS_context context,
    corto_char** result)
{
    corto_iter iter;
    corto_uint32 size;
    corto_char* chunk;

    size = corto_llSize(context->xmlbuff);

    /* Allocate memory for string */
    *result = corto_alloc(size * (dDDS_XML_BUFFER) + 1);

    /* Copy buffers in final structure. */
    iter = corto_llIter(context->xmlbuff);
    while(corto_iterHasNext(&iter)) {
        chunk = corto_iterNext(&iter);
#ifdef SER_DEBUG
        printf("   ### %d: %s\n", strlen(chunk), chunk);
#endif
        memcpy((corto_char*)(*result) + dDDS_XML_BUFFER * (size-1), chunk, dDDS_XML_BUFFER);
        corto_dealloc(chunk);
        size--;
    }

}

/* Print xml */
static void
dDDS_printXml(
    dDDS_context context,
    char* fmt,
    ...)
{
    va_list args;
    corto_char buffer[dDDS_XML_BUFFER]; /* Large buffer on stack, safe since function is never called recursively. */
    int length;

    va_start(args, fmt);
    length = vsnprintf(buffer, dDDS_XML_BUFFER, fmt, args); /* C99 function */
    if(length >= dDDS_XML_BUFFER) {
        corto_critical("dDDS_serializerXMLTypeInfoSerialize: buffer too small!");
    }

    dDDS_printXmlAppend(context, buffer);

    va_end(args);
}

/* Print type. */
static void
dDDS_printXmlType(
    dDDS_context context,
    corto_type current,
    corto_type type);

/* Print typedef */
static void
dDDS_printXmlTypedef(
    dDDS_context context,
    corto_type type)
{
    dDDS_printXml(context, "<TypeDef name=\"%s\">", corto_nameof(type));
    dDDS_printXmlType(context, corto_type(type), type);
    dDDS_printXml(context, "</TypeDef>");
}

/* Print primitive */
static void
dDDS_printXmlPrimitive(
    dDDS_context context,
    corto_primitive type)
{
    switch(type->kind) {
    case CORTO_BOOLEAN:
        dDDS_printXml(context, "<Boolean/>");
        break;
    case CORTO_CHARACTER:
        switch(type->width) {
        case CORTO_WIDTH_8:
            dDDS_printXml(context, "<Char/>");
            break;
        case CORTO_WIDTH_16:
            dDDS_printXml(context, "<WChar/>");
            break;
        default:
            corto_error("Character width not supported when converting to DDS type.");
            break;
        }
        break;
    case CORTO_BINARY:
        dDDS_printXml(context, "<Octet/>");
        break;
    case CORTO_INTEGER:
        switch(type->width) {
        case CORTO_WIDTH_8:
            dDDS_printXml(context, "<Char/>");
            break;
        case CORTO_WIDTH_16:
            dDDS_printXml(context, "<Short/>");
            break;
        case CORTO_WIDTH_32:
            dDDS_printXml(context, "<Long/>");
            break;
        case CORTO_WIDTH_64:
            dDDS_printXml(context, "<LongLong/>");
            break;
        default:
            corto_error("Integer width unsupported when converting to DDS type.");
            break;
        }
        break;
    case CORTO_UINTEGER:
        switch(type->width) {
        case CORTO_WIDTH_8:
            dDDS_printXml(context, "<Char/>");
            break;
        case CORTO_WIDTH_16:
            dDDS_printXml(context, "<UShort/>");
            break;
        case CORTO_WIDTH_32:
            dDDS_printXml(context, "<ULong/>");
            break;
        case CORTO_WIDTH_64:
            dDDS_printXml(context, "<ULongLong/>");
            break;
        default:
            corto_error("Unsigned integer width unsupported when converting to DDS type.");
            break;
        }
        break;
    case CORTO_FLOAT:
        switch(type->width) {
        case CORTO_WIDTH_32:
            dDDS_printXml(context, "<Float/>");
            break;
        case CORTO_WIDTH_64:
            dDDS_printXml(context, "<Double/>");
            break;
        default:
            corto_error("Floating-point width unsupported when converting to DDS type.");
            break;
        }
        break;
    case CORTO_BITMASK:
        dDDS_printXml(context, "<Long/>");
        break;
    default:
        /* Use this serializer only to serialize userdata */
        {
            corto_critical("Invalid primitive type '%s'.", corto_fullpath(NULL, type));
        }
        break;
    }
}

/* Print enumeration */
static void
dDDS_printXmlEnumeration(
    dDDS_context context,
    corto_enum type)
{
    corto_uint32 i;
    corto_constant* constant;
    corto_uint32 value;

    dDDS_printXml(context, "<Enum name=\"%s\">", corto_nameof(type));

    /* Walk constants */
    for(i=0; i<type->constants.length; i++) {
        constant = type->constants.buffer[i];
        value = *constant;
        dDDS_printXml(context, "<Element name=\"%s\" value=\"%d\"/>", corto_nameof(constant), value);
    }

    dDDS_printXml(context, "</Enum>");
}

/* Print structure */
static void
dDDS_printXmlStructure(
    dDDS_context context,
    corto_interface type,
    corto_bool serializeBase)
{
    corto_uint32 i;
    corto_member member;

    if(!serializeBase) {
        dDDS_printXml(context, "<Struct name=\"%s\">", corto_nameof(type));
    }

    if(corto_class_instanceof(corto_struct_o, type)) {
        if(corto_interface(type)->base) {
            dDDS_printXmlStructure(context, corto_interface(type)->base, TRUE);
        }
    }

    /* Walk members of struct */
    for(i=0; i<type->members.length; i++) {
        member = type->members.buffer[i];

        /* Serialize member and member type */
        dDDS_printXml(context, "<Member name=\"%s\">", corto_nameof(member));
        if(member->type->reference) {
            dDDS_printXml(context,"<Long/>");
        }else {
               dDDS_printXmlType(context, corto_type(type), member->type);
        }
        dDDS_printXml(context, "</Member>");
    }

    if(!serializeBase) {
        dDDS_printXml(context, "</Struct>");
    }
}

/* Print collection */
static void
dDDS_printXmlCollection(
    dDDS_context context,
    corto_type current,
    corto_collection type) /* Strings are not handled by this function! */
{
    corto_char* elementName;
    corto_uint32 size = 0;

    elementName = NULL;

    switch(type->kind) {
    case CORTO_LIST:
    case CORTO_MAP:
        dDDS_printXml(context, "<Long/>"); /* Placeholder for lists and maps */
        return;
    default:
        break;
    }

    /* Print collection header */
    switch(type->kind) {
    case CORTO_SEQUENCE:
        elementName = "Sequence";
        break;
    case CORTO_ARRAY:
        elementName = "Array";
        break;
    default:
        corto_error("dDDS_printXmlCollection: unsupported collectionkind for DDS-serializer.");
        break;
    }

    dDDS_printXml(context, "<%s", elementName);

    /* Print collection size, subType and footer. Pass current rootType to
     * dDDS_printXmlType so inlined types that are used as subtype are defined within
     * the inline collection. */
    switch(type->kind) {
    case CORTO_SEQUENCE:
    case CORTO_ARRAY:
        size = corto_collection(type)->max;
        break;
    default:
        corto_error("dDDS_printXmlCollection: unsupported collectionkind for DDS-serializer(2).");
        break;
    }

    if(size) {
        dDDS_printXml(context, " size=\"%d\">", size);
    }else {
        dDDS_printXml(context, ">");
    }
    dDDS_printXmlType(context, current, type->elementType);
    dDDS_printXml(context, "</%s>", elementName);
}

/* Print text */
static void
dDDS_printXmlText(
    dDDS_context context,
    corto_text type) /* Strings are not handled by this function! */
{
    corto_char* elementName;
    corto_uint32 size;

    elementName = "String";

    dDDS_printXml(context, "<%s", elementName);
    size = type->length;

    if(size) {
        dDDS_printXml(context, " length=\"%d\"/>", size);
    }else {
        dDDS_printXml(context, "/>");
    }
}

#define dDDS_MAX_SCOPE_DEPTH (64) /* Should be on the safe side */

/* Function builds a scope-stack from root to module */
static void
dDDS_utilModuleStack(
    corto_object module,
    corto_object* stack /* corto_object[dDDS_MAX_SCOPE_DEPTH] */)
{
    corto_uint32 count;
    corto_object ptr;

    corto_assert(module != NULL, "NULL passed for module to dDDS_utilModuleStack");

    /* Count scope depth */
    ptr = module;
    count = 1; /* For self */
    while((ptr = corto_parentof(ptr))) {
        count++;
    }

    if(count > dDDS_MAX_SCOPE_DEPTH) {
        corto_error("dDDS_printXmlCollection: unsupported scope-depth (depth=%d, max=%d).", count, dDDS_MAX_SCOPE_DEPTH);
    }
    corto_assert(count <= dDDS_MAX_SCOPE_DEPTH, "MAX_SCOPE_DEPTH overflow.");

    /* Fill module stack */
    ptr = module;
    while(count) {
        stack[count-1] = ptr;
        ptr = corto_parentof(ptr);
        count--;
    }

    /* ptr should be NULL */
    corto_assert(!ptr, "ptr is NULL.");
}

/* Find first common module in two module-stacks */
static corto_object
dDDS_utilFirstCommonModule(
    corto_object from,
    corto_object to,
    corto_object* fromStack,
    corto_object* toStack,
    corto_uint32* i_out)
{
    corto_object fromPtr, toPtr;
    corto_uint32 i;

    /* fromPtr and toPtr will initially point to base */
    i = 0;
    do {
        fromPtr = fromStack[i];
        toPtr = toStack[i];
        i++;
    }while((fromPtr != from) && (toPtr != to) && (fromStack[i] == toStack[i]));

    /* Common module is now stored in fromPtr and toPtr. */

    if(i_out) {
        *i_out = i;
    }

    return fromPtr;
}

/* Print typeref */
static void
dDDS_printXmlTyperef(
    dDDS_context context,
    corto_type type)
{
    /* Print typeref. Use relative names (if possible) to obtain the shortest possible reference to another type. */
    if(corto_parentof(type) != context->module) {
        corto_object fromStack[dDDS_MAX_SCOPE_DEPTH], toStack[dDDS_MAX_SCOPE_DEPTH];
        corto_object from, to, common;
        corto_uint32 i;

        /* Get first common module between current module and the referenced type. */
        from = context->module;
        to = corto_parentof(type);
        dDDS_utilModuleStack(from, fromStack);
        dDDS_utilModuleStack(to, toStack);
        dDDS_utilFirstCommonModule(from, to, fromStack, toStack, &i);

        dDDS_printXml(context, "<Type name=\"");

        /* Print modules from the common module until the current */
        i--;
        do {
            common = toStack[i];
            i++;
            if(corto_nameof(common)) {
                dDDS_printXml(context, "%s", corto_nameof(common));
            }
            dDDS_printXml(context, "::");
        }while(common != to);

        /* Print the typename */
        dDDS_printXml(context, "%s\"/>", corto_nameof(type));
    }else {
        /* If module of type is equal to the current, just print the typename. */
        dDDS_printXml(context, "<Type name=\"%s\"/>", corto_nameof(type));
    }
}

/* Open module. This function finds the shortest path from the current module to the next,
 * and opens and closes modules where necessary. */
static void
dDDS_printXmlModuleOpen(
    dDDS_context context,
    corto_object to)
{
    corto_object from;

    /* If context->module is NULL, start from root */
    from = context->module;
    if(!from) {
        from = root_o;
    }

    /* If from and to are not equal, find shortest path between modules. */
    if(from != to) {
        corto_object fromStack[dDDS_MAX_SCOPE_DEPTH], toStack[dDDS_MAX_SCOPE_DEPTH];
        corto_object fromPtr, toPtr;
        corto_uint32 i;

        /* Find common module. First build up a scope-stack for the two modules which
         * are ordered base -> <module>. Then walk through these stacks to find the
         * last common module. */
        dDDS_utilModuleStack(from, fromStack);
        dDDS_utilModuleStack(to, toStack);
        fromPtr = toPtr = dDDS_utilFirstCommonModule(from, to, fromStack, toStack, &i);

        /* Walk down from module 'from' to 'toPtr' */
        fromPtr = from;
        while(fromPtr != toPtr) {
            dDDS_printXml(context, "</Module>");
            fromPtr = corto_parentof(fromPtr);
        }

        /* Walk from toPtr to 'to' */
        while(toPtr != to) {
            toPtr = toStack[i];
            dDDS_printXml(context, "<Module name=\"%s\">", corto_nameof(toPtr));
            i++;
        }

        /* Update context->module */
        context->module = to;
    }
}

/* Close module */
static void dDDS_printXmlModuleClose(
    dDDS_context context)
{
    corto_object ptr;

    if(context->module) {
        ptr = context->module;
        while((ptr = corto_parentof(ptr))) {
            dDDS_printXml(context, "</Module>");
        }

        context->module = NULL;
    }
}

/* Print type. */
static void
dDDS_printXmlType(
    dDDS_context context,
    corto_type current,
    corto_type type)
{
    if(type == corto_type(dDDS_Time_t_o)) {
        dDDS_printXml(context, "<Time/>");
    }else {
        corto_bool parseTypeRef = TRUE;

        /* If object is defined outside the current scope and is not a collection or primitive, serialize a typeref.
         * Also, if the type is already defined serialize a typeref. This prevents inline types to be defined multiple
         * times if they are used multiple times within a module-scoped type. */
        if(corto_type(type) == type) {
            switch(type->kind) {
            case CORTO_PRIMITIVE:
            case CORTO_COLLECTION:
                parseTypeRef = FALSE;
                break;
            default:
                if(corto_parentof(type) == current) {
                    if(!context->inlineProcessed || !corto_llHasObject(context->inlineProcessed, type)) {
                        parseTypeRef = FALSE;
                    }
                }
                break;
            }
        }else {
            if(corto_parentof(type) == current) {
                if(!context->inlineProcessed || !corto_llHasObject(context->inlineProcessed, type)) {
                    parseTypeRef = FALSE;
                }
            }
        }

        if(parseTypeRef) {
            dDDS_printXmlTyperef(context, type);
        }else {
            /* Serialize type-definition. */
            if(corto_type(type) != type) {
                dDDS_printXmlTypedef(context, type);
            }else {
                switch(type->kind) {
                case CORTO_PRIMITIVE:
                    switch(corto_primitive(type)->kind) {
                    case CORTO_ENUM:
                        dDDS_printXmlEnumeration(context, corto_enum(type));
                        break;
                    case CORTO_TEXT:
                        dDDS_printXmlText(context, corto_text(type));
                        break;
                    default:
                        dDDS_printXmlPrimitive(context, corto_primitive(type));
                        break;
                    }
                    break;
                case CORTO_COLLECTION:
                    dDDS_printXmlCollection(context, current, corto_collection(type));
                    break;
                case CORTO_COMPOSITE:
                    /* Make an exception for c_time. */
                    dDDS_printXmlStructure(context, corto_interface(type), FALSE);
                    break;
                default:
                    /* This may not happen. Types other than the ones listed
                     * above cannot be printed. */
                    corto_critical("Invalid typekind for dDDS_printXmlType.");
                    break;
                }
            }

            /* Mark type as processed. */
            if(!context->inlineProcessed) {
                context->inlineProcessed = corto_llNew();
            }
            corto_llInsert(context->inlineProcessed, type);
        }
    }
}

/* Top-level printroutine. */
static void
dDDS_printXmlItem(
    dDDS_item item,
    dDDS_context context)
{
    if(context->inlineProcessed) {
        corto_llFree(context->inlineProcessed);
        context->inlineProcessed = NULL;
    }

#ifdef SER_DEBUG
    {
        printf("%s\n", corto_fullpath(NULL, item->self));
    }
#endif

    /*if(item->self != corto_type(item->self)) {

    } else */{
        switch(item->self->kind) {
        case CORTO_COMPOSITE:
        case CORTO_PRIMITIVE:
            dDDS_printXmlModuleOpen(context, corto_parentof(item->self));
            dDDS_printXmlType(context, corto_parentof(item->self), item->self);
            break;
        default:
            /* Typedefs are not yet supported */

            /*if(item->self->type != corto_type(item->self->real)) {
                dDDS_printXmlModuleOpen(context, corto_parentof(item->self));
                dDDS_printXmlType(context, corto_type(corto_parentof(item->self)), item->self);
            }else {
                 * This may not happen. Types other than the ones listed
                 * above cannot be directly printed.
                corto_critical("dDDS_printXmlItem: invalid typeKind for serializer.");
            }*/
            break;
        }
    }

    /* Dereference dependees of item. This will populate module-objects with types
     * that reach refcount 0. */
    dDDS_itemDerefDependees(item);
}

static int
dDDS_addInitialItems(
    void* o,
    void* udata)
{
    dDDS_item item;
    CORTO_UNUSED(udata);

    item = o;

    /* If refcount of item is zero, it means that it has no unresolved dependencies, thus that it can
     * be processed. */
    if(!item->refcount){
        dDDS_itemAddToModule(item);
    }

    return 1;
}

struct dDDS_findLargestModule_t {
    dDDS_moduleItem largest;
};

static int
dDDS_findLargestModule(
    void* o,
    void* userData)
{
    dDDS_moduleItem module, largest;
    struct dDDS_findLargestModule_t* data;

    module = o;
    data = userData;
    largest = data->largest;

    /* Find largest module. */
    if(!largest) {
        /* Only set largest if module has types. */
        if(module->end - module->start) {
            data->largest = module;
        }
    }else {
        if((largest->end - largest->start) < (module->end - module->start)) {
            data->largest = module;
        }
    }

    return 1;
}

/* Output types in optimized module order. The algorithm attempts to reduce the number
 * of module transitions by selecting the module with the most types in each iteration.
 * The algorithm stops when all modules are empty.
 */
static void
dDDS_printModules(
    dDDS_context context)
{
    struct dDDS_findLargestModule_t walkData;
    dDDS_moduleItem module;

    /* There must be at least one type, and as such at least one module. */
    corto_assert(context->modules != NULL, "context->modules is NULL.");

    /* Find initial largest module */
    walkData.largest = NULL;
    corto_llWalk(context->modules, dDDS_findLargestModule, &walkData);

    /* There must be at least one module with types. */
    corto_assert(walkData.largest != NULL, "No modules with types?");

    do {
        module = walkData.largest;

        /* Walk types */
        while(module->start != module->end) {
            /* If the printing of an item causes other types to be 'unlocked' (refcount becomes 0) in the current module,
             * the type will be added to the 'module' object, causing the module->end pointer to shift. This
             * automatically causes these types to be processed within the current iteration.
             */
            dDDS_printXmlItem(*(module->start), context);
            module->start++;
        }

        /* Lookup next largest module */
        walkData.largest = NULL;
        corto_llWalk(context->modules, dDDS_findLargestModule, &walkData);
    }while(walkData.largest);
}

#ifdef SER_DEBUG
static int
dDDS_printTypes(
    void* o,
    void* udata)
{
    CORTO_UNUSED(o);
    CORTO_UNUSED(udata);

    printf("%s\n", corto_fullpath(NULL, ((dDDS_item)o)->self));

    return 1;
}
#endif

static void
dDDS_printXmlDescriptor(
    dDDS_context context,
    corto_char** result)
{
#ifdef SER_DEBUG
    printf("=== Dependency ordered: (%d types).\n", corto_llSize(context->items));
    corto_llWalk(context->items, dDDS_printTypes, NULL);
#endif

    /* Set initial module to base */
    context->module = root_o;

    /* Insert initial types with refcount 0 in module objects */
    corto_llWalk(context->items, dDDS_addInitialItems, NULL);

    /* Print xml */
    dDDS_printXml(context, "<MetaData version=\"1.0.0\">");

    /* Print types, ordered by modules. */
#ifdef SER_DEBUG
    printf("=== Module ordered output:\n");
#endif
    dDDS_printModules(context);

    /* Clean inlineProcessed. */
    if(context->inlineProcessed) {
        corto_llFree(context->inlineProcessed);
        context->inlineProcessed = NULL;
    }

    /* Close last used module */
    dDDS_printXmlModuleClose(context);
    dDDS_printXml(context, "</MetaData>");

    /* Aggegrate xml-buffers */
#ifdef SER_DEBUG
    printf("\n=== XML buffers:\n");
#endif
    dDDS_printXmlAggegrate(context, result);

    /* Free xmlbuff list */
    corto_llFree(context->xmlbuff);
    context->xmlbuff = NULL;

#ifdef SER_DEBUG
    printf("\n=== XML MetaDescriptor:\n");
    printf("'%s'\n", *result);
#endif
}

corto_string
dDDS_metaXmlGet(
    corto_type type)
{
    dDDS_context context;
    corto_char* result;
#ifdef SER_DEBUG
    corto_time t, start, stop;

    corto_timeGet(&start);
    printf("=== Start serializing..\n");
#endif

    context = dDDS_contextNew();
    result = NULL;

    /* Serialize type */
    if(dDDS_serializeType(context, NULL, type, FALSE, NULL)) {
        goto error;
    }

    /* Print XML */
    dDDS_printXmlDescriptor(context, &result);

    /* Free context */
    dDDS_contextFree(context);

#ifdef SER_DEBUG
    corto_timeGet(&stop);
    t = corto_timeSub(stop, start);
    printf("=== Serializing finished in %d.%09d seconds.\n", t.tv_sec, t.tv_nsec);
#endif

    return result;
error:
    return NULL;
}
