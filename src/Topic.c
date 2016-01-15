/* $CORTO_GENERATED
 *
 * Topic.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "dDDS/dDDS.h"

/* $header() */
#include "dDDS_intern.h"
/* $end */

corto_int16 _dDDS_Topic_construct(dDDS_Topic this) {
/* $begin(dDDS/Topic/construct) */
    DDS_DomainParticipant dp;
    DDS_TypeSupport ts;
    DDS_Topic topic;
    DDS_ReturnCode_t result;
    corto_id typeName;
    corto_string xml;

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

    /* Get metadescriptor */
    xml = dDDS_Struct_metaXml(this->type);

    /* Create typename */
    corto_path(typeName, root_o, this->type, "::");

    /* Obtain typesupport */
    ts = DDS_TypeSupport__alloc(
        typeName,
        "",
        xml);

    /* Inject type */
    result = DDS__FooTypeSupport_register_type(ts, dp, typeName);
    if(result != DDS_RETCODE_OK) {
        corto_seterr(
          "dDDS/Topic/construct: failed to inject type '%s'", typeName);
        goto error;
    }

    /* Create topic */
    topic = DDS_DomainParticipant_create_topic(
        dp,
        corto_nameof(this),
        typeName,
        DDS_TOPIC_QOS_DEFAULT,
        NULL,
        0);
    if(!topic){
        corto_seterr(
            "dDDS/Topic/construct: failed to create topic '%s' (name=%s, typeName=%s)",
            corto_fullpath(NULL, this),
            corto_nameof(this),
            typeName);
        goto error;
    }

    /* Set entity handle */
    corto_olsSet(this, DDDS_ENTITY_HANDLE, topic);

    return 0;
error:
    return -1;
/* $end */
}

corto_void _dDDS_Topic_destruct(dDDS_Topic this) {
/* $begin(dDDS/Topic/destruct) */

    /* << Insert implementation >> */

/* $end */
}
