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
    dDDS_Subscriber pub = dDDS_SubscriberCreateChild(dp, "pub", partitions);
    if (!pub) goto error;

    dDDS_DataReader reader = dDDS_DataReaderCreateChild(pub, "reader", topic);
    if (!reader) goto error;

    dDDS_ObjectSeq *sampleSeq = dDDS_ObjectSeqCreate();
    if (!sampleSeq) goto error;

    while (1) {
        /* Write sample to DDS */
        if (dDDS_DataReader_read(reader, sampleSeq)) {
            corto_error("read failed: %s", corto_lasterr());
        }

        dDDS_ObjectSeqForeach(*sampleSeq, s) {
            dDDS_String json = dDDS_Object_json(s);
            printf("Sample received: %s\n", json);
            corto_dealloc(json);
        }

        corto_sleep(1, 0);
    }

    return 0;
error:
    corto_error("Reader failed: %s", corto_lasterr());
    return -1;
}
