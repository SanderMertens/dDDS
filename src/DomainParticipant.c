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

void* dDDS_DomainParticipant_threadMonitor(void *userData) {
    dDDS_DomainParticipant this = userData;
    corto_string osplTypesFilename =
      corto_envparse(
        "$CORTO_HOME/etc/corto/$CORTO_VERSION/packages/dDDS/etc/ospltypes.xml");
    if (!osplTypesFilename) {
        corto_error("threadMonitor: can't resolve path to ospltypes.xml");
        goto error;
    }

    corto_string osplTypes = corto_fileLoad(osplTypesFilename);
    if (!osplTypes) {
        corto_error("threadMonitor: failed to load '%s'", osplTypesFilename);
        goto error;
    }

    if (dDDS_fromMetaXml(osplTypes)) {
        corto_error("threadMonitor: can't parse '%s': %s",
            osplTypesFilename,
            corto_lasterr());
        goto error;
    }

    corto_type topicInfo = corto_resolve(NULL, "/kernelModule/v_topicInfo");
    if (!topicInfo) {
        corto_error("threadMonitor: cannot resolve v_topicInfo");
        goto error;
    }

    /* Create topic */
    dDDS_Topic dcpsTopic = dDDS_TopicCreateChild(
        this,
        "DCPSTopic",
        topicInfo,
        "key.localId,key.systemId");
    if (!dcpsTopic) {
        corto_error("threadMonitor: failed to create topic");
        goto error;
    }

error:
    return NULL;
}

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

dDDS_Publisher _dDDS_DomainParticipant_defaultPublisher(dDDS_DomainParticipant this) {
/* $begin(dDDS/DomainParticipant/defaultPublisher) */
    dDDS_Publisher pub_o = corto_resolve(this, "defaultPublisher");

    if (!pub_o) {
        dDDS_StringSeq partitions = {0, NULL};
        pub_o = dDDS_PublisherCreateChild(
            this,
            "defaultPublisher",
            partitions);
    } else {
        corto_release(pub_o);
    }

    return pub_o;
/* $end */
}

dDDS_Subscriber _dDDS_DomainParticipant_defaultSubscriber(dDDS_DomainParticipant this) {
/* $begin(dDDS/DomainParticipant/defaultSubscriber) */
    dDDS_Subscriber sub_o = corto_resolve(this, "defaultSubscriber");

    if (!sub_o) {
        dDDS_StringSeq partitions = {0, NULL};
        sub_o = dDDS_SubscriberCreateChild(
            this,
            "defaultSubscriber",
            partitions);
    } else {
        corto_release(sub_o);
    }

    return sub_o;
/* $end */
}

corto_void _dDDS_DomainParticipant_destruct(dDDS_DomainParticipant this) {
/* $begin(dDDS/DomainParticipant/destruct) */
    DDS_ReturnCode_t result;
    DDS_DomainParticipant dp;

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

corto_void _dDDS_DomainParticipant_requireTopic(dDDS_DomainParticipant this, corto_string topic) {
/* $begin(dDDS/DomainParticipant/requireTopic) */
    corto_lock(this);

    /* Add topic to list of required topics */
    corto_llAppend(this->watchForTopics, topic);

    /* If thread hasn't started yet, start it now  */


    corto_unlock(this);
/* $end */
}
