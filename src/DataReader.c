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

static DDS_SampleStateKind dDDS_toSampleState(dDDS_SampleState s) {
    DDS_SampleStateKind result = 0;

    if (!s) {
        result = DDS_ANY_SAMPLE_STATE;
    } else {
        if (s & dDDS_Read) result |= DDS_READ_SAMPLE_STATE;
        if (s & dDDS_NotRead) result |= DDS_NOT_READ_SAMPLE_STATE;
    }

    return result;
}

static DDS_ViewStateKind dDDS_toViewState(dDDS_ViewState s) {
    DDS_ViewStateKind result = 0;

    if (!s) {
        result = DDS_ANY_VIEW_STATE;
    } else {
        if (s & dDDS_New) result |= DDS_NEW_VIEW_STATE;
        if (s & dDDS_NotNew) result |= DDS_NOT_NEW_VIEW_STATE;
    }

    return result;
}

static DDS_InstanceStateKind dDDS_toInstanceState(dDDS_InstanceState s) {
    DDS_InstanceStateKind result = 0;

    if (!s) {
        result = DDS_ANY_INSTANCE_STATE;
    } else {
        if (s & dDDS_Alive) result |= DDS_ALIVE_INSTANCE_STATE;
        if (s & dDDS_NotAlive) result |= DDS_NOT_ALIVE_INSTANCE_STATE;
        if (s & dDDS_Disposed) result |= DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE;
        if (s & dDDS_NoWriters) result |= DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE;
    }

    return result;
}

static corto_int16 dDDS_DataReader_doRead(
    dDDS_DataReader this,
    dDDS_ObjectSeq *sampleSeq,
    corto_uint32 length,
    dDDS_SampleState sampleState,
    dDDS_ViewState viewState,
    dDDS_InstanceState instanceState,
    corto_bool take)
{
    DDS_ReturnCode_t status;
    DDS_SampleStateKind ss = dDDS_toSampleState(sampleState);
    DDS_ViewStateKind vs = dDDS_toViewState(viewState);
    DDS_InstanceStateKind is = dDDS_toInstanceState(instanceState);

    if (length == -1) {
        length = DDS_LENGTH_UNLIMITED;
    }

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
            length,
            ss,
            vs,
            is);
    } else {
        status = DDS_DataReader_read (
            reader,
            ddsSeq,
            infoSeq,
            length,
            ss,
            vs,
            is);
    }
    if ((status != DDS_RETCODE_OK) && (status != DDS_RETCODE_NO_DATA)) {
        corto_seterr("dDDS/DataReader/read: read failed (%d)", status);
        goto error;
    }

    /* Set size of sequence to be big enough for # of returned samples */
    dDDS_ObjectSeqSize(sampleSeq, ddsSeq->_length);

    /* Copy out values */
    corto_int32 i = 0;
    void *ptr = ddsSeq->_buffer;
    for (i = 0; i < ddsSeq->_length; i++) {
        dDDS_Object o = dDDS_Object_new(this->topic->type, "{}");
        corto_copyp(o, this->topic->type, ptr);
        sampleSeq->buffer[i] = o;
        ptr = CORTO_OFFSET(ptr, corto_type(this->topic->type)->size);
    }

    /* Return loan */
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
    dDDS_Subscriber sub_o;
    DDS_Topic topic;
    DDS_DataReader reader;
    corto_object parent;

    /* DDS entities will be created in scopes of each other, so enforce that
     * the created object is scoped */
    if (!corto_checkAttr(this, CORTO_ATTR_SCOPED)) {
        corto_seterr("DDS entities must be scoped objects (use createChild)");
        goto error;
    }

    /* Accept both DomainParticipants and Subscribers as parent */
    parent = corto_parentof(this);
    if (corto_instanceof(dDDS_Subscriber_o, parent)) {
        sub_o = parent;
        sub = dDDS_getEntity(parent, dDDS_Subscriber_o);

    } else if (corto_instanceof(dDDS_DomainParticipant_o, parent)) {
        sub_o = dDDS_DomainParticipant_defaultSubscriber(parent);
        if (!sub_o) {
            corto_seterr("dDDS/DataReader/construct: %s", corto_lasterr());
            goto error;
        }

        sub = dDDS_getEntity(sub_o, dDDS_Subscriber_o);
    } else {
        corto_seterr(
            "dDDS/DataReader/construct: invalid type '%s' for parent '%s'",
            corto_fullpath(NULL, corto_typeof(parent)),
            corto_fullpath(NULL, parent));
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

    /* Set subscriber */
    corto_setref(&this->subscriber, sub_o);

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
        DDS_Subscriber sub = dDDS_getEntity(this->subscriber, dDDS_Subscriber_o);
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

dDDS_ReturnCode _dDDS_DataReader_read(dDDS_DataReader this, dDDS_ObjectSeq *sampleSeq, corto_uint32 length, dDDS_SampleState sampleState, dDDS_ViewState viewState, dDDS_InstanceState instanceState) {
/* $begin(dDDS/DataReader/read) */

    return dDDS_DataReader_doRead(
      this, sampleSeq, length, sampleState, viewState, instanceState, FALSE);
/* $end */
}

dDDS_ReturnCode _dDDS_DataReader_readAny(dDDS_DataReader this, dDDS_ObjectSeq *sampleSeq) {
/* $begin(dDDS/DataReader/readAny) */

    return dDDS_DataReader_doRead(
      this, sampleSeq, -1, dDDS_Any, dDDS_Any, dDDS_Any, FALSE);
/* $end */
}

dDDS_ReturnCode _dDDS_DataReader_take(dDDS_DataReader this, dDDS_ObjectSeq *sampleSeq, corto_uint32 length, dDDS_SampleState sampleState, dDDS_ViewState viewState, dDDS_InstanceState instanceState) {
/* $begin(dDDS/DataReader/take) */

    return dDDS_DataReader_doRead(
      this, sampleSeq, length, sampleState, viewState, instanceState, TRUE);
/* $end */
}

dDDS_ReturnCode _dDDS_DataReader_takeAny(dDDS_DataReader this, dDDS_ObjectSeq *sampleSeq) {
/* $begin(dDDS/DataReader/takeAny) */

    return dDDS_DataReader_doRead(
      this, sampleSeq, -1, dDDS_Any, dDDS_Any, dDDS_Any, TRUE);
/* $end */
}
