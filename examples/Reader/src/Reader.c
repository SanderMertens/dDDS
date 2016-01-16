#include "Reader.h"

int ReaderMain(int argc, char *argv[]) {

    /* Create Point type */
    dDDS_Module Foo_o = dDDS_ModuleCreateChild(root_o, "Foo");
    dDDS_Struct Point_o = dDDS_StructDeclareChild(Foo_o, "Point");
    dDDS_MemberCreateChild(Point_o, "x", dDDS_Long_o);
    dDDS_MemberCreateChild(Point_o, "y", dDDS_Long_o);
    dDDS_StructDefine(Point_o);

    /* Create dynamic DDS entities */
    dDDS_DomainParticipant dp = dDDS_DomainParticipantCreateChild(NULL, "dp", 0);
    if (!dp) goto error;

    /* This is where the magic happens: use the dynamic Point type */
    dDDS_Topic topic = dDDS_TopicCreateChild(dp, "Points", Point_o);
    if (!topic) goto error;

    dDDS_StringSeq partitions = {0, NULL};
    dDDS_Subscriber sub = dDDS_SubscriberCreateChild(dp, "pub", partitions);
    if (!sub) goto error;

    dDDS_DataReader reader = dDDS_DataReaderCreateChild(sub, "reader", topic);
    if (!reader) goto error;

    dDDS_ObjectSeq *sampleSeq = dDDS_ObjectSeqCreate();
    if (!sampleSeq) goto error;

    while (1) {
        if (dDDS_DataReader_take(reader, sampleSeq)) {
            dDDS_error("[reader] read failed: %s", dDDS_lasterr());
        }

        dDDS_ObjectSeqForeach(*sampleSeq, s) {
            dDDS_String json = dDDS_Object_json(s);
            printf("[reader] sample received: %s\n", json);
            dDDS_free(json);
        }

        if (!sampleSeq->length) printf("[reader] no data received.\n");

        corto_sleep(1, 0);
    }

    return 0;
error:
    dDDS_error("[reader] failed: %s", dDDS_lasterr());
    return -1;
}
