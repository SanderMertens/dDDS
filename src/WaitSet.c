/* $CORTO_GENERATED
 *
 * WaitSet.c
 *
 * Only code written between the begin and end tags will be preserved
 * when the file is regenerated.
 */

#include "dDDS/dDDS.h"

/* $header() */
#include "dDDS_intern.h"
/* $end */

dDDS_ReturnCode _dDDS_WaitSet_attach(dDDS_WaitSet this, dDDS_Condition cond) {
/* $begin(dDDS/WaitSet/attach) */
    DDS_WaitSet ws = corto_olsGet(this, DDDS_ENTITY_HANDLE);
    DDS_Condition c = corto_olsGet(cond, DDDS_ENTITY_HANDLE);
    dDDS_ReturnCode result;

    if ((result = DDS_WaitSet_attach_condition(ws, c))) {
        goto error;
    }

    dDDS_ConditionListAppend(this->conditions, cond);

    return dDDS_Ok;
error:
    return result;
/* $end */
}

corto_int16 _dDDS_WaitSet_construct(dDDS_WaitSet this) {
/* $begin(dDDS/WaitSet/construct) */
    DDS_WaitSet ws = DDS_WaitSet__alloc();

    corto_olsSet(this, DDDS_ENTITY_HANDLE, ws);

    return 0;
/* $end */
}

corto_void _dDDS_WaitSet_destruct(dDDS_WaitSet this) {
/* $begin(dDDS/WaitSet/destruct) */
    DDS_WaitSet ws = corto_olsGet(this, DDDS_ENTITY_HANDLE);

    DDS_free(ws);

/* $end */
}

dDDS_ReturnCode _dDDS_WaitSet_detach(dDDS_WaitSet this, dDDS_Condition cond) {
/* $begin(dDDS/WaitSet/detach) */
    DDS_WaitSet ws = corto_olsGet(this, DDDS_ENTITY_HANDLE);
    DDS_Condition c = corto_olsGet(cond, DDDS_ENTITY_HANDLE);
    DDS_ReturnCode_t result;

    if ((result = DDS_WaitSet_detach_condition(ws, c))) {
        goto error;
    }

    dDDS_ConditionListRemove(this->conditions, cond);

    return dDDS_Ok;
error:
    return result;
/* $end */
}

dDDS_ReturnCode _dDDS_WaitSet_dispatch(dDDS_WaitSet this, corto_time *timeout) {
/* $begin(dDDS/WaitSet/dispatch) */
    DDS_WaitSet ws = corto_olsGet(this, DDDS_ENTITY_HANDLE);
    DDS_ConditionSeq *seq = DDS_ConditionSeq__alloc();
    DDS_Duration_t t = {timeout->sec, timeout->nanosec};
    DDS_ReturnCode_t result;

    result = DDS_WaitSet_wait(ws, seq, &t);

    DDS_free(seq);

    return result;
/* $end */
}
