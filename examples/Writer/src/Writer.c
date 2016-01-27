#include "Writer.h"

int WriterMain(int argc, char *argv[]) {
    char json[256];
    int x = 0, y = 0;

    /* Create type */
    dDDS_ModuleCreateChild_auto(root_o, Foo);
    dDDS_StructDeclareChild_auto(Foo_o, Point);
    dDDS_MemberCreateChild_auto(Point_o, x, dDDS_Long_o);
    dDDS_MemberCreateChild_auto(Point_o, y, dDDS_Long_o);
    dDDS_StructDefine(Point_o);

    /* Create DDS entities */
    dDDS_DomainParticipantCreateChild_auto(root_o, dp, 0);
    dDDS_TopicCreateChild_auto(dp_o, Points, Point_o, "");
    dDDS_DataWriterCreateChild_auto(dp_o, writer, Points_o);

    /* Create sample */
    dDDS_Object sample_o = dDDS_Object_new(Point_o, "{}");

    /* Enter main loop */
    while (1) {
        sprintf(json, "{\"x\":%d,\"y\":%d}", x += 1, y += 2);
        dDDS_Object_set(sample_o, json);

        printf("[writer] write sample '%s'\n", json);
        dDDS_DataWriter_write(writer_o, sample_o);
        corto_sleep(1, 0);
    }

    /* Cleanup */
    dDDS_delete(sample_o);

    return 0;
}
