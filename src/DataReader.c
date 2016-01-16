/* $CORTO_GENERATED
 *
 * DataReader.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "dDDS/dDDS.h"

/* $header() */
#include "dDDS_intern.h"

corto_int16 dDDS_DataReader_doRead(
    dDDS_DataReader this,
    dDDS_ObjectSeq *sampleSeq,
    corto_bool take)
{
    DDS_ReturnCode_t status;

    DDS_DataReader reader = dDDS_getEntity(this, dDDS_DataReader_o);
    if (!reader) {
        corto_seterr("dDDS/DataReader/read: invalid handle");
        goto error;
    }

    if (!sampleSeq) {
        corto_seterr("dDDS/DataReader/read: invalid sample sequence provided");
        goto error;
    }

    DDS_sequence ddsSeq = corto_calloc(sizeof(DDS_SampleInfoSeq));
    ddsSeq->_release = FALSE;
    DDS_SampleInfoSeq *infoSeq = DDS_SampleInfoSeq__alloc();
    infoSeq->_release = FALSE;

    if (take) {
        status = DDS_DataReader_take (
            reader,
            ddsSeq,
            infoSeq,
            DDS_LENGTH_UNLIMITED,
            DDS_ANY_SAMPLE_STATE,
            DDS_ANY_VIEW_STATE,
            DDS_ANY_INSTANCE_STATE);
    } else {
        status = DDS_DataReader_read (
            reader,
            ddsSeq,
            infoSeq,
            DDS_LENGTH_UNLIMITED,
            DDS_ANY_SAMPLE_STATE,
            DDS_ANY_VIEW_STATE,
            DDS_ANY_INSTANCE_STATE);
    }
    if ((status != DDS_RETCODE_OK) && (status != DDS_RETCODE_NO_DATA)) {
        corto_seterr("dDDS/DataReader/read: read failed (%d)", status);
        goto error;
    }

    /* Copy out values */
    dDDS_ObjectSeqSize(sampleSeq, ddsSeq->_length);

    corto_int32 i = 0;
    void *ptr = ddsSeq->_buffer;
    for (i = 0; i < ddsSeq->_length; i++) {
        dDDS_Object o = dDDS_Object_new(this->topic->type, "{}");
        corto_copyp(o, this->topic->type, ptr);
        sampleSeq->buffer[i] = o;
        ptr = CORTO_OFFSET(ptr, corto_type(this->topic->type)->size);
    }

    status = DDS_DataReader_return_loan(reader, ddsSeq, infoSeq);
    if (status != DDS_RETCODE_OK) {
        corto_seterr("dDDS/DataReader/read: return_loan failed");
        goto error;
    }

    return 0;
error:
    return -1;
}

/* $end */

corto_int16 _dDDS_DataReader_construct(dDDS_DataReader this) {
/* $begin(dDDS/DataReader/construct) */
    DDS_Subscriber sub;
    DDS_Topic topic;
    DDS_DataReader reader;

    /* DDS entities will be created in scopes of each other, so enforce that
     * the created object is scoped */
    if (!corto_checkAttr(this, CORTO_ATTR_SCOPED)) {
        corto_seterr("DDS entities must be scoped objects (use createChild)");
        goto error;
    }

    /* Get Subscriber */
    sub = dDDS_getEntity(corto_parentof(this), dDDS_Subscriber_o);
    if (!sub) {
        corto_seterr("dDDS/DataReader/construct: %s", corto_lasterr());
        goto error;
    }

    /* Get entity handle of Topic */
    topic = dDDS_getEntity(this->topic, dDDS_Topic_o);
    if (!topic) {
        corto_seterr("dDDS/DataReader/construct: %s", corto_lasterr());
        goto error;
    }

    /* Create DataReader */
    reader = DDS_Subscriber_create_datareader(
        sub,
        topic,
        DDS_DATAREADER_QOS_USE_TOPIC_QOS,
        NULL,
        DDS_STATUS_MASK_NONE);
    if (!reader) {
        corto_seterr("dDDS/DataReader/construct: failed to create DataReader",
            corto_fullpath(NULL, this));
        goto error;
    }

    /* Set entity handle of object */
    corto_olsSet(this, DDDS_ENTITY_HANDLE, reader);

    return 0;
error:
    return -1;
/* $end */
}

corto_void _dDDS_DataReader_destruct(dDDS_DataReader this) {
/* $begin(dDDS/DataReader/destruct) */
    DDS_DataReader reader = dDDS_getEntity(this, dDDS_DataReader_o);
    if (reader) {
        DDS_Subscriber sub = dDDS_getEntity(this, dDDS_Subscriber_o);
        DDS_ReturnCode_t status;
        if (!sub) {
            corto_error("dDDS/DataReader/destruct: %s", corto_lasterr());
        } else {
            status = DDS_Subscriber_delete_datareader(sub, reader);
            if (status != DDS_RETCODE_OK) {
                corto_error(
                    "dDDS/DataReader/destruct: failed to delete DataReader");
            }
        }
    }
/* $end */
}

corto_int16 _dDDS_DataReader_read(dDDS_DataReader this, dDDS_ObjectSeq *sampleSeq) {
/* $begin(dDDS/DataReader/read) */
    return dDDS_DataReader_doRead(this, sampleSeq, FALSE);
/* $end */
}

corto_int16 _dDDS_DataReader_take(dDDS_DataReader this, dDDS_ObjectSeq *sampleSeq) {
/* $begin(dDDS/DataReader/take) */
    return dDDS_DataReader_doRead(this, sampleSeq, TRUE);
/* $end */
}
