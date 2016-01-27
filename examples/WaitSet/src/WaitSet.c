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
    dDDS_Module Foo_o = dDDS_ModuleCreateChild(root_o, "Foo");
    dDDS_Struct Point_o = dDDS_StructDeclareChild(Foo_o, "Point");
    dDDS_MemberCreateChild(Point_o, "x", dDDS_Long_o);
    dDDS_MemberCreateChild(Point_o, "y", dDDS_Long_o);
    dDDS_StructDefine(Point_o);

    /* Create DDS entities */
    dDDS_DomainParticipant dp = dDDS_DomainParticipantCreateChild(NULL, "dp", 0);
    dDDS_Topic topic = dDDS_TopicCreateChild(dp, "Points", Point_o, "");
    dDDS_DataReader reader = dDDS_DataReaderCreateChild(dp, "reader", topic);

    /* Create condition & callback
     * Allocate sampleSeq once, reuse in callback. */
    dDDS_ObjectSeq sampleSeq = dDDS_ObjectSeqCreate();
    dDDS_ConditionAction rcAction;
    dDDS_ConditionActionInitNative(&rcAction, action, sampleSeq);
    dDDS_ReadCondition rc = dDDS_ReadConditionCreateChild(
        reader, "rc", rcAction, dDDS_NotRead, dDDS_Any, dDDS_Any);

    /* Setup waitset */
    dDDS_WaitSet ws = dDDS_WaitSetCreate();
    dDDS_WaitSet_attach(ws, rc);

    /* Enter wait loop */
    while (TRUE) {
        dDDS_Duration timeout = {-1, -1};
        dDDS_WaitSet_dispatch(ws, &timeout);
    }

    return 0;
}
