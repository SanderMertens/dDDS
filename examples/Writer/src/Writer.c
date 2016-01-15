#include "Writer.h"

int WriterMain(int argc, char *argv[]) {
    int x = 0, y = 0;
    char json[256];

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
    dDDS_Publisher pub = dDDS_PublisherCreateChild(dp, "pub", partitions);
    if (!pub) goto error;

    dDDS_DataWriter writer = dDDS_DataWriterCreateChild(pub, "writer", topic);
    if (!writer) goto error;

    /* Create sample and provide initial value in JSON */
    dDDS_Object sample = dDDS_Object_new(Point_o, "{}");
    if (!sample) goto error;

    while (1) {
        /* Set new value */
        sprintf(json, "{\"x\":%d,\"y\":%d}", x += 1, y += 2);
        dDDS_Object_set(sample, json);

        /* Write sample to DDS */
        dDDS_DataWriter_write(writer, sample);
        corto_sleep(1, 0);
    }

    return 0;
error:
    corto_error("Writer failed: %s", corto_lasterr());
    return -1;
}
