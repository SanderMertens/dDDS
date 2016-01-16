/* $CORTO_GENERATED
 *
 * dDDS.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "dDDS/dDDS.h"

/* $header() */
corto_int16 DDDS_ENTITY_HANDLE;

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
/* $end */

corto_void _dDDS_delete(dDDS_Object object) {
/* $begin(dDDS/delete) */

    /* << Insert implementation >> */

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
