/* $CORTO_GENERATED
 *
 * DataWriter.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "dDDS/dDDS.h"

/* $header() */
#include "dDDS_intern.h"
/* $end */

corto_int16 _dDDS_DataWriter_construct(dDDS_DataWriter this) {
/* $begin(dDDS/DataWriter/construct) */
    DDS_Publisher pub;
    DDS_Topic topic;
    DDS_DataWriter writer;

    /* DDS entities will be created in scopes of each other, so enforce that
     * the created object is scoped */
    if (!corto_checkAttr(this, CORTO_ATTR_SCOPED)) {
        corto_seterr("DDS entities must be scoped objects (use createChild)");
        goto error;
    }

    /* Get Publisher */
    pub = dDDS_getEntity(corto_parentof(this), dDDS_Publisher_o);
    if (!pub) {
        corto_seterr("dDDS/DataWriter/construct: %s", corto_lasterr());
        goto error;
    }

    /* Get entity handle of Topic */
    topic = dDDS_getEntity(this->topic, dDDS_Topic_o);
    if (!topic) {
        corto_seterr("dDDS/DataWriter/construct: %s", corto_lasterr());
        goto error;
    }

    /* Create DataWriter */
    writer = DDS_Publisher_create_datawriter(
        pub,
        topic,
        DDS_DATAWRITER_QOS_USE_TOPIC_QOS,
        NULL,
        DDS_STATUS_MASK_NONE);
    if (!writer) {
        corto_seterr("dDDS/DataWriter/construct: failed to create DataWriter",
            corto_fullpath(NULL, this));
        goto error;
    }

    /* Set entity handle of object */
    corto_olsSet(this, DDDS_ENTITY_HANDLE, writer);

    return 0;
error:
    return -1;
/* $end */
}

corto_void _dDDS_DataWriter_destruct(dDDS_DataWriter this) {
/* $begin(dDDS/DataWriter/destruct) */
    DDS_DataWriter writer = dDDS_getEntity(this, dDDS_DataWriter_o);
    if (writer) {
        DDS_Publisher pub = dDDS_getEntity(this, dDDS_Publisher_o);
        DDS_ReturnCode_t status;
        if (!pub) {
            corto_error("dDDS/DataWriter/destruct: %s", corto_lasterr());
        } else {
            status = DDS_Publisher_delete_datawriter(pub, writer);
            if (status != DDS_RETCODE_OK) {
                corto_error(
                    "dDDS/DataWriter/destruct: failed to delete DataWriter");
            }
        }
    }
/* $end */
}

corto_void _dDDS_DataWriter_write(dDDS_DataWriter this, corto_object sample) {
/* $begin(dDDS/DataWriter/write) */
    DDS_DataWriter writer = dDDS_getEntity(this, dDDS_DataWriter_o);
    if (writer) {
        DDS_ReturnCode_t status;
        status = DDS_DataWriter_write(writer, sample, DDS_HANDLE_NIL);
        if (status != DDS_RETCODE_OK) {
            corto_error("dDDS/DataWriter/write: write failed");
        }
    }

/* $end */
}
