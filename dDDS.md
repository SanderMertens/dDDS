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
### read(/dDDS/ObjectSeq& sampleSeq,uint32 length,/dDDS/SampleState sampleState,/dDDS/ViewState viewState,/dDDS/InstanceState instanceState)
#### sampleSeq
#### length
#### sampleState
#### viewState
#### instanceState
#### Returns
### readAny(/dDDS/ObjectSeq& sampleSeq)
#### sampleSeq
#### Returns
### subscriber
### take(/dDDS/ObjectSeq& sampleSeq,uint32 length,/dDDS/SampleState sampleState,/dDDS/ViewState viewState,/dDDS/InstanceState instanceState)
#### sampleSeq
#### length
#### sampleState
#### viewState
#### instanceState
#### Returns
### takeAny(/dDDS/ObjectSeq& sampleSeq)
#### sampleSeq
#### Returns
### topic

## DataWriter
### construct()
#### Returns
### destruct()
### publisher
### topic
### write(/dDDS/Object sample)
#### sample
#### Returns

## DomainParticipant
### construct()
#### Returns
### defaultPublisher()
#### Returns
### defaultSubscriber()
#### Returns
### destruct()
### id
### requireTopic(string topic)
#### topic
### watchForTopics

## Double

## Enum

## Float

## InstanceState
### Alive
### Any
### Disposed
### NotAlive
### NoWriters

## Long

## LongLong

## Member
### type

## Module

## Object
### json(/dDDS/Object sample)
#### sample
#### Returns
### new(/dDDS/Struct type,string json)
#### type
#### json
#### Returns
### set(/dDDS/Object sample,string json)
#### sample
#### json

## ObjectSeq

## Octet

## Publisher
### construct()
#### Returns
### destruct()
### partitions

## ReadCondition
### construct()
#### Returns
### destruct()
### instanceState
### sampleState
### viewState

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

## SampleState
### Any
### NotRead
### Read

## Sequence

## Short

## String

## Struct

## Subscriber
### construct()
#### Returns
### destruct()
### partitions

## Topic
### construct()
#### Returns
### destruct()
### key
### type

## Typedef
### type

## ULong

## ULongLong

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
