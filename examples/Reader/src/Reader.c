#include "Reader.h"

/* Reader example
 *
 * This example demonstrates how a dynamic DataReader can be used. The example
 * uses a convenience function called 'takeAny' that is a shortcut for getting
 * all data, regardless of sample, view or instance state.
 *
 * The example uses generated _auto macro's which results in more concise code.
 * These macro's 'emulate' C++'s auto behavior by automatically declaring a
 * variable of the correct type.
 *
 * Without _auto:
 *   dDDS_Struct Point = dDDS_StructDeclareChild(dp, "Point");
 *
 * With _auto:
 *   dDDS_StructDeclareChild_auto(Foo, Point);
 *
 */

int ReaderMain(int argc, char *argv[])
{
    /* Create type */
    dDDS_ModuleCreateChild_auto(root_o, Foo);
    dDDS_StructDeclareChild_auto(Foo, Point);
    dDDS_MemberCreateChild_auto(Point, x, dDDS_Long_o);
    dDDS_MemberCreateChild_auto(Point, y, dDDS_Long_o);
    dDDS_StructDefine(Point);

    /* Create DDS entities */
    dDDS_DomainParticipantCreateChild_auto(root_o, dp, 0, dDDS_DefaultQos);
    dDDS_TopicCreateChild_auto(dp, PointTopic, Point, "", dDDS_DefaultQos);
    dDDS_DataReaderCreateChild_auto(dp, reader, PointTopic, dDDS_DefaultQos);
    dDDS_ObjectSeqCreate_auto(sampleSeq);

    /* Enter main loop */
    while (TRUE) {
        dDDS_DataReader_takeAny(reader, sampleSeq);
        dDDS_ObjectSeqForeach(*sampleSeq, s) {
            dDDS_String json = dDDS_Object_json(s);
            printf("[reader] sample received: %s\n", json);
            dDDS_free(json);
        }

        if (!sampleSeq->length) printf("[reader] no data received.\n");
        corto_sleep(1, 0);
    }

    /* Cleanup */
    dDDS_delete(sampleSeq);

    return 0;
}
