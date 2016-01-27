#include "WaitSet.h"

void action(dDDS_Object sampleSeq, dDDS_Object cond)
{
    dDDS_ReadCondition rc = dDDS_ReadCondition(cond);
    dDDS_DataReader dr = dDDS_DataReader(dDDS_parentof(cond));

    dDDS_DataReader_takeCondition(dr, sampleSeq, rc);
    dDDS_ObjectSeqForeach(*sampleSeq, s) {
        dDDS_String json = dDDS_Object_json(s);
        printf("[waitset] sample received: %s\n", json);
        dDDS_free(json);
    }
}

int WaitSetMain(int argc, char *argv[])
{
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

    /* Create condition & callback
     * Allocate sampleSeq once, reuse in callback. */
    dDDS_ObjectSeqCreate_auto(sampleSeq);
    dDDS_ConditionAction rcAction;
    dDDS_ConditionActionInitNative(&rcAction, action, sampleSeq_o);
    dDDS_ReadConditionCreateChild_auto(
      reader_o, rc, rcAction, dDDS_NotRead, dDDS_Any, dDDS_Any);

    /* Setup waitset */
    dDDS_WaitSetCreate(ws);
    dDDS_WaitSet_attach(ws_o, rc_o);

    /* Enter wait loop */
    while (TRUE) {
        dDDS_Duration timeout = {-1, -1};
        dDDS_WaitSet_dispatch(ws_o, &timeout);
    }

    /* Cleanup */
    dDDS_ConditionActionDeinit(&rcAction);
    dDDS_delete(sampleSeq_o);
    dDDS_delete(ws_o);

    return 0;
}
