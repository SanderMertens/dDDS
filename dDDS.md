# /dDDS
Dynamic DDS API.

### delete(/dDDS/Object object)
#### object
### fromMetaXml(/dDDS/String xml)
#### xml
#### Returns
### toMetaXml(struct type)
#### type
#### Returns

## Array

## Char

## Condition
### onTrigger

## ConditionAction

## Constant
### init()
#### Returns

## DataReader
### construct()
#### Returns
### destruct()
### qos
### read(/dDDS/ObjectSeq& sampleSeq,/dDDS/ULong length,/dDDS/SampleState sampleState,/dDDS/ViewState viewState,/dDDS/InstanceState instanceState)
#### sampleSeq
#### length
#### sampleState
#### viewState
#### instanceState
#### Returns
### readAny(/dDDS/ObjectSeq& sampleSeq)
#### sampleSeq
#### Returns
### readCondition(/dDDS/ObjectSeq& sampleSeq,/dDDS/ULong length,/dDDS/Condition cond)
#### sampleSeq
#### length
#### cond
#### Returns
### subscriber
### take(/dDDS/ObjectSeq& sampleSeq,/dDDS/ULong length,/dDDS/SampleState sampleState,/dDDS/ViewState viewState,/dDDS/InstanceState instanceState)
#### sampleSeq
#### length
#### sampleState
#### viewState
#### instanceState
#### Returns
### takeAny(/dDDS/ObjectSeq& sampleSeq)
#### sampleSeq
#### Returns
### takeCondition(/dDDS/ObjectSeq& sampleSeq,/dDDS/ULong length,/dDDS/Condition cond)
#### sampleSeq
#### length
#### cond
#### Returns
### topic

## DataReaderQos
### deadline
### destination_order
### durability
### durability_service
### from_topic(/dDDS/Topic topic)
#### topic
### get_default()
### history
### latency_budget
### lifespan
### liveliness
### ownership
### reader_data_lifecycle
### reliability
### resource_limits
### time_based_filter
### transport_priority
### user_data

## DataWriter
### construct()
#### Returns
### destruct()
### publisher
### qos
### topic
### write(/dDDS/Object sample)
#### sample
#### Returns

## DataWriterQos
### construct()
#### Returns
### deadline
### destination_order
### durability
### durability_service
### from_topic(/dDDS/Topic topic)
#### topic
### get_default()
### history
### init()
#### Returns
### latency_budget
### lifespan
### liveliness
### ownership
### ownership_strength
### reliability
### resource_limits
### transport_priority
### user_data
### writer_data_lifecycle

## DeadlineQosPolicy
### period

## DestinationOrderQosPolicy
### kind

## DestinationOrderQosPolicyKind
### BY_RECEPTION_TIMESTAMP
### BY_SOURCE_TIMESTAMP

## DomainId_t

## DomainParticipant
### construct()
#### Returns
### defaultPublisher()
#### Returns
### defaultSubscriber()
#### Returns
### destruct()
### id
### qos
### requireTopic(string topic)
#### topic
### watchForTopics

## DomainParticipantFactoryQos
### entity_factory
### get_default()

## DomainParticipantQos
### entity_factory
### get_default()
### user_data

## Double

## DurabilityQosPolicy
### kind

## DurabilityQosPolicyKind
### Persistent
### Transient
### TransientLocal
### Volatile

## DurabilityServiceQosPolicy
### history_depth
### history_kind
### max_instances
### max_samples
### max_samples_per_instance
### service_cleanup_delay

## EntityFactoryQosPolicy
### autoenable_created_entities

## EntityQos
### __dummy
### get_default()

## Enum

## Float

## GroupDataQosPolicy
### value

## HistoryQosPolicy
### depth
### kind

## HistoryQosPolicyKind
### KeepAll
### KeepLast

## InconsistentTopicStatus
### total_count
### total_count_change

## InstanceHandle_t

## InstanceState
### Alive
### Any
### Disposed
### NotAlive
### NoWriters

## LatencyBudgetQosPolicy
### duration

## LifespanQosPolicy
### duration

## LivelinessChangedStatus
### alive_count
### alive_count_change
### last_publication_handle
### not_alive_count
### not_alive_count_change

## LivelinessLostStatus
### total_count
### total_count_change

## LivelinessQosPolicy
### kind
### lease_duration

## LivelinessQosPolicyKind
### Automatic
### ManualByParticipant
### ManualByTopic

## Long

## LongLong

## Member
### type

## Module

## Object
### json(/dDDS/Object sample)
#### sample
#### Returns
### nameof(/dDDS/Object this)
#### this
#### Returns
### new(/dDDS/Struct type,string json)
#### type
#### json
#### Returns
### parentof(/dDDS/Object this)
#### this
#### Returns
### set(/dDDS/Object sample,string json)
#### sample
#### json
### typeof(/dDDS/Object this)
#### this
#### Returns

## ObjectSeq

## Octet

## OfferedDeadlineMissedStatus
### last_instance_handle
### total_count
### total_count_change

## OfferedIncompatibleQosStatus
### last_policy_id
### policies
### total_count
### total_count_change

## OwnershipQosPolicy
### kind

## OwnershipQosPolicyKind
### Exclusive
### Shared

## OwnershipStrengthQosPolicy
### value

## PartitionQosPolicy

## PresentationQosPolicy
### accessScope
### AccessScopeKind
### coherent_access
### ordered_access

## PresentationQosPolicy/AccessScopeKind
### GroupScope
### InstanceScope
### TopicScope

## PublicationMatchedStatus
### current_count
### current_count_change
### last_subscription_handle
### total_count
### total_count_change

## Publisher
### construct()
#### Returns
### destruct()
### qos

## PublisherQos
### entity_factory
### get_default()
### group_data
### partition
### presentation

## QosPolicyCount
### count
### policy_id

## QosPolicyCountSeq

## QosPolicyId_t

## ReadCondition
### construct()
#### Returns
### destruct()
### instanceState
### sampleState
### viewState

## ReaderDataLifecycleQosPolicy
### autopurge_disposed_samples_delay
### autopurge_nowriter_samples_delay

## ReliabilityQosPolicy
### kind
### max_blocking_time

## ReliabilityQosPolicyKind
### BestEffort
### Reliable

## RequestedDeadlineMissedStatus
### last_instance_handle
### total_count
### total_count_change

## RequestedIncompatibleQosStatus
### last_policy_id
### policies
### total_count
### total_count_change

## ResourceLimitsQosPolicy
### max_instances
### max_samples
### max_samples_per_instance

## ReturnCode
### AlreadyDeleted
### BadParameter
### Error
### IllegalOperation
### ImmutablePolicy
### InconsistentPolicy
### NoData
### NotEnabled
### Ok
### OutOfResources
### PreconditionNotMet
### Timeout
### Unsupported

## SampleLostStatus
### total_count
### total_count_change

## SampleRejectedStatus
### last_instance_handle
### last_reason
### total_count
### total_count_change

## SampleRejectedStatusKind
### NotRejected
### RejectedByInstancesLimit
### RejectedBySamplesLimit
### RejectedBySamplesPerInstanceLimit

## SampleState
### Any
### NotRead
### Read

## Sequence

## Short

## Status
### DataAvailable
### DataOnReaders
### InconsistentTopic
### LivelinessChanged
### LivelinessLost
### OfferedDeadlineMissed
### OfferedIncompatibleQos
### PublicationChanged
### RequestedDeadlineMissed
### RequestedIncompatibleQos
### SampleLost
### SampleRejected
### SubscriptionMatched

## String

## Struct

## Subscriber
### construct()
#### Returns
### destruct()
### qos

## SubscriberQos
### entity_factory
### get_default()
### group_data
### partition
### presentation

## SubscriptionMatchedStatus
### current_count
### current_count_change
### last_publication_handle
### total_count
### total_count_change

## TimeBasedFilterQosPolicy
### minimum_separation

## Topic
### construct()
#### Returns
### destruct()
### key
### qos
### type

## TopicDataQosPolicy
### value

## TopicQos
### construct()
#### Returns
### deadline
### destination_order
### durability
### durability_service
### get_default()
### history
### init()
#### Returns
### latency_budget
### lifespan
### liveliness
### ownership
### reliability
### resource_limits
### topic_data
### transport_priority

## TransportPriorityQosPolicy
### value

## Typedef
### type

## ULong

## ULongLong

## UserDataQosPolicy
### value

## UShort

## ViewState
### Any
### New
### NotNew

## WaitSet
### attach(/dDDS/Condition cond)
#### cond
#### Returns
### conditions
### construct()
#### Returns
### destruct()
### detach(/dDDS/Condition cond)
#### cond
#### Returns
### dispatch(/corto/core/time timeout)
#### timeout
#### Returns

## WriterDataLifecycleQosPolicy
### autodispose_unregistered_instances
