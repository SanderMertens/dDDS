/* $CORTO_GENERATED
 *
 * ReadCondition.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "dDDS/dDDS.h"

/* $header() */
#include "dDDS_intern.h"
/* $end */

corto_int16 _dDDS_ReadCondition_construct(dDDS_ReadCondition this) {
/* $begin(dDDS/ReadCondition/construct) */
    DDS_DataReader dr;
    DDS_ReadCondition rc;

    /* DDS entities will be created in scopes of each other, so enforce that
     * the created object is scoped */
    if (!corto_checkAttr(this, CORTO_ATTR_SCOPED)) {
        corto_seterr("DDS entities must be scoped objects (use createChild)");
        goto error;
    }

    /* Get DataReader */
    dr = dDDS_getEntity(corto_parentof(this), dDDS_DataReader_o);
    if (!dr) {
        corto_seterr("dDDS/ReadCondition/construct: %s", corto_lasterr());
        goto error;
    }

    /* Create ReadCondition */
    rc = DDS_DataReader_create_readcondition(
        dr,
        dDDS_toSampleState(this->sampleState),
        dDDS_toViewState(this->viewState),
        dDDS_toInstanceState(this->instanceState));
    if (!rc) {
        corto_seterr(
          "dDDS/ReadCondition/construct: failed to create readcondition %s",
          corto_fullpath(NULL, this));
        goto error;
    }

    /* Set entity handle */
    corto_olsSet(this, DDDS_ENTITY_HANDLE, rc);

    return 0;
error:
    return -1;
/* $end */
}

corto_void _dDDS_ReadCondition_destruct(dDDS_ReadCondition this) {
/* $begin(dDDS/ReadCondition/destruct) */
    DDS_DataReader dr;
    DDS_ReadCondition rc;

    /* Get DataReader */
    dr = dDDS_getEntity(corto_parentof(this), dDDS_DataReader_o);
    if (!dr) {
        corto_error("dDDS/ReadCondition/construct: %s", corto_lasterr());
    }

    rc = corto_olsGet(this, DDDS_ENTITY_HANDLE);
    if (rc) {
        if (DDS_DataReader_delete_readcondition(dr, rc) != DDS_RETCODE_OK) {
            corto_error(
              "dDDS/ReadCondition/destruct: failed to delete readcondition %s",
              corto_fullpath(NULL, this));
        }
    }

/* $end */
}
