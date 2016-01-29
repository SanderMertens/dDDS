/* dDDS.h
 *
 * This file contains generated code. Do not modify!
 */

#ifndef DDDS_H
#define DDDS_H

#include "corto/corto.h"
#include "dDDS/_type.h"
#include "dDDS/_api.h"
#include "dDDS/_meta.h"
#include "dDDS/_interface.h"

#ifdef __cplusplus
extern "C" {
#endif


/* $header() */
#include "dDDS/MetaXml.h"

/* Free heap object. Use dDDS_delete for dDDS_Object and entities.  */
DDDS_EXPORT void dDDS_free(void* ptr);

/* Report error to stderr */
DDDS_EXPORT void dDDS_error(const char* fmt, ...);

/* Set error, don't report to stderr */
DDDS_EXPORT void dDDS_seterr(const char* fmt, ...);

/* Retrieve last error set by seterr */
DDDS_EXPORT const char* dDDS_lasterr(void);

/* Hide corto/core/time */
typedef corto_time dDDS_Time;
typedef corto_time dDDS_Duration;

DDDS_EXPORT extern dDDS_Duration dDDS_Duration_Zero;
DDDS_EXPORT extern dDDS_Duration dDDS_Duration_Infinite;
DDDS_EXPORT extern dDDS_Object dDDS_DefaultQos;
/* $end */

DDDS_EXPORT corto_void _dDDS_delete(dDDS_Object object);
#define dDDS_delete(object) _dDDS_delete(object)

DDDS_EXPORT dDDS_ReturnCode _dDDS_fromMetaXml(dDDS_String xml);
#define dDDS_fromMetaXml(xml) _dDDS_fromMetaXml(xml)

DDDS_EXPORT corto_string _dDDS_toMetaXml(corto_struct type);
#define dDDS_toMetaXml(type) _dDDS_toMetaXml(corto_struct(type))
#include "dDDS/Array.h"
#include "dDDS/Condition.h"
#include "dDDS/ConditionAction.h"
#include "dDDS/Constant.h"
#include "dDDS/DataReader.h"
#include "dDDS/DataReaderQos.h"
#include "dDDS/DataWriter.h"
#include "dDDS/DataWriterQos.h"
#include "dDDS/DeadlineQosPolicy.h"
#include "dDDS/DestinationOrderQosPolicy.h"
#include "dDDS/DomainParticipant.h"
#include "dDDS/DomainParticipantFactoryQos.h"
#include "dDDS/DomainParticipantQos.h"
#include "dDDS/DurabilityQosPolicy.h"
#include "dDDS/DurabilityServiceQosPolicy.h"
#include "dDDS/EntityFactoryQosPolicy.h"
#include "dDDS/EntityQos.h"
#include "dDDS/Enum.h"
#include "dDDS/GroupDataQosPolicy.h"
#include "dDDS/HistoryQosPolicy.h"
#include "dDDS/InconsistentTopicStatus.h"
#include "dDDS/LatencyBudgetQosPolicy.h"
#include "dDDS/LifespanQosPolicy.h"
#include "dDDS/LivelinessChangedStatus.h"
#include "dDDS/LivelinessLostStatus.h"
#include "dDDS/LivelinessQosPolicy.h"
#include "dDDS/Member.h"
#include "dDDS/Module.h"
#include "dDDS/Object.h"
#include "dDDS/OfferedDeadlineMissedStatus.h"
#include "dDDS/OfferedIncompatibleQosStatus.h"
#include "dDDS/OwnershipQosPolicy.h"
#include "dDDS/OwnershipStrengthQosPolicy.h"
#include "dDDS/PresentationQosPolicy.h"
#include "dDDS/PublicationMatchedStatus.h"
#include "dDDS/Publisher.h"
#include "dDDS/PublisherQos.h"
#include "dDDS/QosPolicyCount.h"
#include "dDDS/ReadCondition.h"
#include "dDDS/ReaderDataLifecycleQosPolicy.h"
#include "dDDS/ReliabilityQosPolicy.h"
#include "dDDS/RequestedDeadlineMissedStatus.h"
#include "dDDS/RequestedIncompatibleQosStatus.h"
#include "dDDS/ResourceLimitsQosPolicy.h"
#include "dDDS/SampleLostStatus.h"
#include "dDDS/SampleRejectedStatus.h"
#include "dDDS/Sequence.h"
#include "dDDS/Struct.h"
#include "dDDS/Subscriber.h"
#include "dDDS/SubscriberQos.h"
#include "dDDS/SubscriptionMatchedStatus.h"
#include "dDDS/TimeBasedFilterQosPolicy.h"
#include "dDDS/Topic.h"
#include "dDDS/TopicDataQosPolicy.h"
#include "dDDS/TopicQos.h"
#include "dDDS/TransportPriorityQosPolicy.h"
#include "dDDS/Typedef.h"
#include "dDDS/UserDataQosPolicy.h"
#include "dDDS/WaitSet.h"
#include "dDDS/WriterDataLifecycleQosPolicy.h"

#ifdef __cplusplus
}
#endif
#endif
