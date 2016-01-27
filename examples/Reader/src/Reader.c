#include "Reader.h"

int ReaderMain(int argc, char *argv[]) {

    /* Create type */
    dDDS_ModuleCreateChild_auto(root_o, Foo);
    dDDS_StructDeclareChild_auto(Foo_o, Point);
    dDDS_MemberCreateChild_auto(Point_o, x, dDDS_Long_o);
    dDDS_MemberCreateChild_auto(Point_o, y, dDDS_Long_o);
    dDDS_StructDefine(Point_o);

    /* Create DDS entities */
    dDDS_DomainParticipantCreateChild_auto(root_o, dp, 0);
    dDDS_TopicCreateChild_auto(dp_o, Points, Point_o, "");
    dDDS_DataReaderCreateChild_auto(dp_o, reader, Points_o);
    dDDS_ObjectSeqCreate_auto(sampleSeq);

    /* Enter main loop */
    while (TRUE) {
        dDDS_DataReader_takeAny(reader_o, sampleSeq_o);
        dDDS_ObjectSeqForeach(*sampleSeq_o, s) {
            dDDS_String json = dDDS_Object_json(s);
            printf("[reader] sample received: %s\n", json);
            dDDS_free(json);
        }

        if (!sampleSeq_o->length) printf("[reader] no data received.\n");
        corto_sleep(1, 0);
    }

    /* Cleanup */
    dDDS_delete(sampleSeq_o);

    return 0;
}
