/* $CORTO_GENERATED
 *
 * Object.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "dDDS/dDDS.h"

/* $header() */
#include "json.h"
/* $end */

dDDS_String _dDDS_Object_json(dDDS_Object sample) {
/* $begin(dDDS/Object/json) */

    return json_fromCorto(sample);

/* $end */
}

dDDS_Object _dDDS_Object_new(dDDS_Struct type, corto_string json) {
/* $begin(dDDS/Object/new) */

    /* Forward declare sample */
    corto_object o = corto_declare(type);

    /* Deserialize JSON */
    if (json_toCorto(o, json)) {
        corto_seterr("failed to deserialize json: %s", corto_lasterr());
        corto_delete(o);
        goto error;
    }

    /* Define object */
    if (corto_define(o)) {
        corto_seterr("failed to define Object of type %s (%s)",
            corto_fullpath(NULL, type),
            corto_lasterr());
        corto_delete(o);
        goto error;
    }

    return o;
error:
    return NULL;
/* $end */
}

corto_void _dDDS_Object_set(dDDS_Object sample, corto_string json) {
/* $begin(dDDS/Object/set) */

    /* Deserialize JSON */
    if (json_toCorto(sample, json)) {
        corto_error("failed to deserialize json: %s", corto_lasterr());
    }

/* $end */
}
