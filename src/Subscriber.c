/* $CORTO_GENERATED
 *
 * Subscriber.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "dDDS/dDDS.h"

/* $header() */
 #include "dDDS_intern.h"
 /* $end */

corto_int16 _dDDS_Subscriber_construct(dDDS_Subscriber this) {
/* $begin(dDDS/Subscriber/construct) */
     DDS_DomainParticipant dp;
     DDS_SubscriberQos qos;
     DDS_Subscriber sub;
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

     memset(&qos, 0, sizeof(DDS_SubscriberQos));

     /* Copy partitions into DDS default subscriber Qos */
     status = DDS_DomainParticipant_get_default_subscriber_qos (dp, &qos);
     if (status != DDS_RETCODE_OK) {
         corto_seterr(
           "dDDS/Subscriber/construct: could not get default Subscriber qos");
         goto error;
     }

     /* Create subscriber */
     sub = DDS_DomainParticipant_create_subscriber(dp, &qos, NULL, DDS_STATUS_MASK_NONE);
     if (!sub) {
         corto_seterr("dDDS/Subscriber/construct: failed to create Subscriber",
             corto_fullpath(NULL, this));
         goto error;
     }

     /* Set entity handle of object */
     corto_olsSet(this, DDDS_ENTITY_HANDLE, sub);

     return 0;
 error:
     return -1;
 /* $end */
}

corto_void _dDDS_Subscriber_destruct(dDDS_Subscriber this) {
/* $begin(dDDS/Subscriber/destruct) */
     DDS_Subscriber sub;
     DDS_ReturnCode_t status;
     corto_object dp_o;
     DDS_DomainParticipant dp;

     /* Get DomainParticipant */
     dp_o = corto_parentof(this);
     if (!corto_instanceof(dDDS_DomainParticipant_o, dp_o)) {
         corto_seterr(
           "dDDS/Subscriber/destruct: parent of topic '%s' is not of type dDDS/DomainParticipant",
           corto_fullpath(NULL, this));
     }

     /* Get entity handle of DomainParticipant */
     dp = corto_olsGet(corto_parentof(this), DDDS_ENTITY_HANDLE);
     if (!dp) {
         corto_seterr(
           "dDDS/Subscriber/destruct: invalid handle to DomainParticipant",
           corto_fullpath(NULL, this));
     }

     /* Delete subscriber */
     sub = corto_olsGet(this, DDDS_ENTITY_HANDLE);
     if (sub) {
         status = DDS_DomainParticipant_delete_subscriber(dp, sub);
         if (status != DDS_RETCODE_OK) {
             corto_error("dDDS/Subscriber/destruct: failed to delete subscriber %s",
             corto_fullpath(NULL, this));
         }
     }

 /* $end */
}
