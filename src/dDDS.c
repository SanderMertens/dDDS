/* $CORTO_GENERATED
 *
 * dDDS.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "dDDS/dDDS.h"

/* $header() */
#include "dDDS_intern.h"

corto_int16 DDDS_ENTITY_HANDLE;

dDDS_Duration dDDS_Duration_Zero = {0, 0};
dDDS_Duration dDDS_Duration_Infinite = {-1, 0};
dDDS_Object dDDS_DefaultQos = NULL;

void* _dDDS_getEntity(corto_object o, corto_type type) {
    void* result = NULL;

    if (o && corto_instanceof(type, o)) {
        result = corto_olsGet(o, DDDS_ENTITY_HANDLE);
        if (!result) {
            corto_seterr("%s contains an invalid handle",
              corto_fullpath(NULL, o));
        }
    } else {
        corto_seterr("%s is not of type %s",
          corto_fullpath(NULL, o),
          corto_fullpath(NULL, type));
    }

    return result;
}

void dDDS_free(void* ptr) {
    corto_dealloc(ptr);
}

void dDDS_error(const char* fmt, ...) {
    va_list list;

    va_start(list, fmt);
    corto_errorv((char*)fmt, list);
    va_end(list);
}

const char* dDDS_lasterr() {
    return (const char*)corto_lasterr();
}

void dDDS_seterr(const char* fmt, ...) {
    va_list list;

    va_start(list, fmt);
    corto_seterrv((char*)fmt, list);
    va_end(list);
}

DDS_SampleStateKind dDDS_toSampleState(dDDS_SampleState s) {
    DDS_SampleStateKind result = 0;

    if (!s) {
        result = DDS_ANY_SAMPLE_STATE;
    } else {
        if (s & dDDS_Read) result |= DDS_READ_SAMPLE_STATE;
        if (s & dDDS_NotRead) result |= DDS_NOT_READ_SAMPLE_STATE;
    }

    return result;
}

DDS_ViewStateKind dDDS_toViewState(dDDS_ViewState s) {
    DDS_ViewStateKind result = 0;

    if (!s) {
        result = DDS_ANY_VIEW_STATE;
    } else {
        if (s & dDDS_New) result |= DDS_NEW_VIEW_STATE;
        if (s & dDDS_NotNew) result |= DDS_NOT_NEW_VIEW_STATE;
    }

    return result;
}

DDS_InstanceStateKind dDDS_toInstanceState(dDDS_InstanceState s) {
    DDS_InstanceStateKind result = 0;

    if (!s) {
        result = DDS_ANY_INSTANCE_STATE;
    } else {
        if (s & dDDS_Alive) result |= DDS_ALIVE_INSTANCE_STATE;
        if (s & dDDS_NotAlive) result |= DDS_NOT_ALIVE_INSTANCE_STATE;
        if (s & dDDS_Disposed) result |= DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE;
        if (s & dDDS_NoWriters) result |= DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE;
    }

    return result;
}
/* $end */

corto_void _dDDS_delete(dDDS_Object object) {
/* $begin(dDDS/delete) */

    corto_delete(object);

/* $end */
}

dDDS_ReturnCode _dDDS_fromMetaXml(dDDS_String xml) {
/* $begin(dDDS/fromMetaXml) */

    if (dDDS_metaXmlParse(xml)) {
        goto error;
    }

    return 0;
error:
    return -1;
/* $end */
}

corto_string _dDDS_toMetaXml(corto_struct type) {
/* $begin(dDDS/toMetaXml) */

    return dDDS_metaXmlGet(corto_type(type));

/* $end */
}

int dDDSMain(int argc, char* argv[]) {
/* $begin(main) */

    /* Create object local storage key to attach DDS entities to
     * Corto objects */
    DDDS_ENTITY_HANDLE = corto_olsKey(NULL);

    return 0;
/* $end */
}
