/* $CORTO_GENERATED
 *
 * Publisher.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "dDDS/dDDS.h"

/* $header() */
#include "dDDS_intern.h"
/* $end */

corto_int16 _dDDS_Publisher_construct(dDDS_Publisher this) {
/* $begin(dDDS/Publisher/construct) */
    DDS_DomainParticipant dp;
    DDS_PublisherQos qos;
    DDS_Publisher pub;
    DDS_ReturnCode_t status;

    /* DDS entities will be created in scopes of each other, so enforce that
     * the created object is scoped */
    if (!corto_checkAttr(this, CORTO_ATTR_SCOPED)) {
        corto_seterr("DDS entities must be scoped objects (use createChild)");
        goto error;
    }

    /* Get DomainParticipant */
    dp = dDDS_getEntity(corto_parentof(this), dDDS_DomainParticipant_o);
    if (!dp) {
        corto_seterr("dDDS/Topic/construct: %s", corto_lasterr());
        goto error;
    }

    memset(&qos, 0, sizeof(DDS_PublisherQos));

    /* Copy partitions into DDS default publisher Qos */
    status = DDS_DomainParticipant_get_default_publisher_qos (dp, &qos);
    if (status != DDS_RETCODE_OK) {
        corto_seterr(
          "dDDS/Publisher/construct: could not get default Publisher qos");
        goto error;
    }
    
    /* Create publisher */
    pub = DDS_DomainParticipant_create_publisher(dp, &qos, NULL, DDS_STATUS_MASK_NONE);
    if (!pub) {
        corto_seterr("dDDS/Publisher/construct: failed to create Publisher",
            corto_fullpath(NULL, this));
        goto error;
    }

    /* Set entity handle of object */
    corto_olsSet(this, DDDS_ENTITY_HANDLE, pub);

    return 0;
error:
    return -1;
/* $end */
}

corto_void _dDDS_Publisher_destruct(dDDS_Publisher this) {
/* $begin(dDDS/Publisher/destruct) */
    DDS_Publisher pub;
    DDS_ReturnCode_t status;
    corto_object dp_o;
    DDS_DomainParticipant dp;

    /* Get DomainParticipant */
    dp_o = corto_parentof(this);
    if (!corto_instanceof(dDDS_DomainParticipant_o, dp_o)) {
        corto_seterr(
          "dDDS/Publisher/destruct: parent of topic '%s' is not of type dDDS/DomainParticipant",
          corto_fullpath(NULL, this));
    }

    /* Get entity handle of DomainParticipant */
    dp = corto_olsGet(corto_parentof(this), DDDS_ENTITY_HANDLE);
    if (!dp) {
        corto_seterr(
          "dDDS/Publisher/destruct: invalid handle to DomainParticipant",
          corto_fullpath(NULL, this));
    }

    /* Delete publisher */
    pub = corto_olsGet(this, DDDS_ENTITY_HANDLE);
    if (pub) {
        status = DDS_DomainParticipant_delete_publisher(dp, pub);
        if (status != DDS_RETCODE_OK) {
            corto_error("dDDS/Publisher/destruct: failed to delete publisher %s",
            corto_fullpath(NULL, this));
        }
    }

/* $end */
}
