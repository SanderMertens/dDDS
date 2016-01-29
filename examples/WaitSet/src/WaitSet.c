#include "WaitSet.h"

void callback(dDDS_Object this, dDDS_Object cond)
{
    dDDS_ReadCondition rc = dDDS_ReadCondition(cond);
    dDDS_DataReader dr = dDDS_DataReader(dDDS_Object_parentof(cond));
    dDDS_ObjectSeq *sampleSeq = dDDS_ObjectSeq(this);

    dDDS_DataReader_takeCondition(dr, sampleSeq, -1, rc);
    dDDS_ObjectSeqForeach(*sampleSeq, s) {
        dDDS_String json = dDDS_Object_json(s);
        printf("[waitset] %s received: %s\n",
          dDDS_Object_nameof(dDDS_Object_typeof(s)),
          json);

        dDDS_free(json);
    }
}

int WaitSetMain(int argc, char *argv[])
{
    /* Create type */
    dDDS_ModuleCreateChild_auto(root_o, Foo);
    dDDS_StructDeclareChild_auto(Foo, Point);
    dDDS_MemberCreateChild_auto(Point, x, dDDS_Long_o);
    dDDS_MemberCreateChild_auto(Point, y, dDDS_Long_o);
    dDDS_StructDefine(Point);

    /* Create DDS entities */
    dDDS_DomainParticipantCreateChild_auto(root_o, dp, 0);
    dDDS_TopicCreateChild_auto(dp, PointTopic, Point, "");
    dDDS_DataReaderCreateChild_auto(dp, reader, PointTopic);

    /* Initialize delegate that will be invoked when condition triggers.
     * Provide sampleSeq as delegate instance so it can be reused. */
    dDDS_ObjectSeqCreate_auto(sampleSeq);
    dDDS_ConditionActionInitCInstance_auto(action, sampleSeq, callback);
    dDDS_ReadConditionCreateChild_auto(
        reader, rc, &action, dDDS_Any, dDDS_Any, dDDS_Any);

    /* Setup waitset */
    dDDS_WaitSetCreateChild_auto(root_o, ws);
    dDDS_WaitSet_attach(ws, rc);

    /* Enter wait loop */
    while (TRUE) {
        dDDS_WaitSet_dispatch(ws, &dDDS_Duration_Infinite);
    }

    /* Cleanup */
    dDDS_ConditionActionDeinit(&action);
    dDDS_delete(sampleSeq);
    dDDS_delete(ws);

    return 0;
}
