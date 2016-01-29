#include "Writer.h"

/* Writer example
 *
 * This example demonstrates how a dynamic DataWriter can be used. Note how the
 * value of a DynamicObject is set with JSON. JSON is just one example of how
 * a sample's value can be set.
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

int WriterMain(int argc, char *argv[])
{
    char json[256];
    int vx = 0, vy = 0;

    /* Create type */
    dDDS_ModuleCreateChild_auto(root_o, Foo);
    dDDS_StructDeclareChild_auto(Foo, Point);
    dDDS_MemberCreateChild_auto(Point, x, dDDS_Long_o);
    dDDS_MemberCreateChild_auto(Point, y, dDDS_Long_o);
    dDDS_StructDefine(Point);

    /* Create DDS entities */
    dDDS_DomainParticipantCreateChild_auto(root_o, dp, 0, dDDS_DefaultQos);
    dDDS_TopicCreateChild_auto(dp, PointTopic, Point, "", dDDS_DefaultQos);
    dDDS_DataWriterCreateChild_auto(dp, writer, PointTopic, dDDS_DefaultQos);

    /* Create sample */
    dDDS_Object sample = dDDS_Object_new(Point, "{}");

    /* Enter main loop */
    while (1) {
        sprintf(json, "{\"x\":%d,\"y\":%d}", vx += 1, vy += 2);
        dDDS_Object_set(sample, json);

        printf("[writer] write sample '%s'\n", json);
        dDDS_DataWriter_write(writer, sample);
        corto_sleep(1, 0);
    }

    /* Cleanup */
    dDDS_delete(sample);

    return 0;
}
