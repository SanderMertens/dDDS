/* $CORTO_GENERATED
 *
 * DomainParticipant.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "dDDS/dDDS.h"

/* $header() */
#include "dDDS_intern.h"
/* $end */

corto_int16 _dDDS_DomainParticipant_construct(dDDS_DomainParticipant this) {
/* $begin(dDDS/DomainParticipant/construct) */
    DDS_DomainParticipant dp;

    /* DDS entities will be created in scopes of each other, so enforce that
     * the created object is scoped */
    if (!corto_checkAttr(this, CORTO_ATTR_SCOPED)) {
        corto_seterr("DDS entities must be scoped objects (use createChild)");
        goto error;
    }

    /* Create participant */
    dp = DDS_DomainParticipantFactory_create_participant(
        DDS_TheParticipantFactory,
        this->id,
        DDS_PARTICIPANT_QOS_DEFAULT,
        NULL,
        DDS_STATUS_MASK_NONE);
    if(!dp) {
        corto_seterr(
          "dDDS/DomainParticipant/construct: create_participant failed for domain %d.", this->id);
        goto error;
    }

    /* Attach entity to object */
    corto_olsSet(this, DDDS_ENTITY_HANDLE, dp);

    return 0;
error:
    return -1;
/* $end */
}

corto_void _dDDS_DomainParticipant_destruct(dDDS_DomainParticipant this) {
/* $begin(dDDS/DomainParticipant/destruct) */
    DDS_ReturnCode_t result;
    DDS_DomainParticipant dp;

    printf("Destruct\n");

    dp = corto_olsGet(this, DDDS_ENTITY_HANDLE);
    if (dp) {
        /* Delete remaining entities of participant */
        result = DDS_DomainParticipant_delete_contained_entities(dp);
        if (result != DDS_RETCODE_OK) {
            corto_error(
              "dDDS/DomainParticipant/destruct: failed to delete contained entities");
        }

        /* Delete domainparticipant */
        /*result = DDS_DomainParticipantFactory_delete_participant(DDS_TheParticipantFactory, dp);
        if (result != DDS_RETCODE_OK) {
            corto_error(
              "dDDS/DomainParticipant/destruct: failed to delete participant.");
        }*/
    }

/* $end */
}
