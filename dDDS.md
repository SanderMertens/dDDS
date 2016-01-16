# /dDDS
Dynamic DDS API.

### delete(/dDDS/Object object)
Delete a dDDS entity or `dDDS/Object`.

#### object

### metaXml(struct type)
Generate an OpenSplice XML metadescriptor from a type.
#### type
The type for which to generate the descriptor.
#### Returns
The XML metadescriptor. `NULL` if failed.

## Array
IDL array

## Char
IDL character

## Constant
Enumeration constant.
### init()
#### Returns

## DataReader
Dynamic DataReader.

### construct()
#### Returns
### destruct()
### read(/dDDS/ObjectSeq& sampleSeq)
#### sampleSeq
#### Returns
### take(/dDDS/ObjectSeq& sampleSeq)
#### sampleSeq
#### Returns
### topic

## DataWriter
Dynamic DataWriter.

### construct()
#### Returns
### destruct()
### topic
### write(object sample)
#### sample
#### Returns

## DomainParticipant
Dynamic DomainParticipant.
### construct()
#### Returns
### destruct()
### id

## Double
IDL Double

## Enum
IDL Enumeration

## Float
IDL Floating point

## Long
IDL 32bit signed integer

## LongLong
IDL 64bit signed integer

## Member
Member of dynamic struct
### key
The key id determines the order in which keyfields appear.
### setKey(int32 id)
Set the key id.
#### id
### type
The type of the member.

## Module
IDL Module.

## Object
A dynamic object. Can be used to hold a reference to a dDDS entity or object of
any type.
### json(/dDDS/Object sample)
Retrieve value of object in JSON.
#### sample
#### Returns

### new(/dDDS/Struct type,string json)
Create new dynamic object.
#### type
The type of the object.
#### json
The initial value of the object in JSON.
#### Returns
The object if success, NULL if fails.

### set(/dDDS/Object sample,string json)
Set value of the object in JSON.
#### sample
The object.
#### json
The new value in JSON.

## ObjectSeq
Sequence of dynamic objects.

## Octet
IDL octet.

## Publisher
Dynamic publisher.
### construct()
#### Returns
### destruct()
### partitions

## Sequence
IDL Sequence.

## Short
IDL 16bit signed integer.

## String
IDL string.

## Struct
IDL struct.

## Subscriber
Dynamic subscriber.
### construct()
#### Returns
### destruct()
### partitions

## Topic
Dynamic topic.
### construct()
#### Returns
### destruct()
### type
The topic type.

## ULong
IDL 32bit unsigned integer.

## ULongLong
IDL 64bit unsigned integer.

## UShort
IDL 16bit unsigned integer.
