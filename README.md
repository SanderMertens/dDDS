
# dDDS (dynamic DDS)

This project is a prototype for a dynamic DDS API which allows for the creation of types at runtime. The API provides
OpenSpliceDDS compatible metadescriptors based on dynamically created types.

The API allows for creating dynamic readers and writers, and send/receive samples over DDS. The API uses JSON to get and set values. Not all DDS features are supported yet. See 'Limitations' for what is not yet possible.

## Building dDDS

Clone repository (if not using the GitHub desktop client)
```
$ git clone https://github.com/SanderMertens/dDDS.git
```

Install Corto
```
$ curl https://corto.io/install | sh
```

Build the package
```
$ corto build dDDS
```

## Run examples
### SimpleType
```
$ corto run dDDS/examples/SimpleType
```
Output:
```
# IDL:
struct Point {
  long x;
  long y;
};

# XML:
<MetaData version="1.0.0"><Module name="Foo"><Struct name="Point"><Member name="x"><Long/></Member><Member name="y"><Long/></Member></Struct></Module></MetaData>
```

### ComplexType
```
$ corto run dDDS/examples/ComplexType
```
Output:
```
# IDL:
struct Polygon {
  long id;
  string name;
  sequence<Point> points;
};

# XML:
<MetaData version="1.0.0"><Module name="Foo"><Struct name="Point"><Member name="x"><Long/></Member><Member name="y"><Long/></Member></Struct><Struct name="Polygon"><Member name="id"><Long/></Member><Member name="name"><String/></Member><Member name="points"><Array><Type name="Point"/></Array></Member></Struct></Module></MetaData>
```

## Architecture
The API, except for the XML serializer, is based  exclusively on code generation. The `dDDS.cx` file contains type defintions that wrap
the Corto typesystem in dDDS types. This effectively hides the Corto typesystem, and makes the API consistent with how types are 
defined in IDL.

This is what the API looks like:
```c
dDDS_Module Foo_o = dDDS_ModuleCreateChild(root_o, "Foo");
dDDS_Struct Point_o = dDDS_StructDeclareChild(Foo_o, "Point");
dDDS_MemberCreateChild(Point_o, "x", dDDS_Long_o);
dDDS_MemberCreateChild(Point_o, "y", dDDS_Long_o);
dDDS_StructDefine(Point_o);
```
This code represents the IDL type
```idl
struct Point {
  long x;
  long y;
};
```

The code is a good example of what kind of code is generated:
 * Type definitions (`dDDS_Module`, `dDDS_Struct`)
 * Type objects (`dDDS_Long_o`)
 * Creation functions (`*CreateChild`)
 * Forward declaration functions (`*DeclareChild`, `Define`)

The types in this example are all named. Alternatively, you can also create anonymous types:
```c
dDDS_Module Foo_o = dDDS_ModuleCreateChild(root_o, "Foo");
dDDS_Struct Polygon_o = dDDS_StructDeclareChild(Foo_o, "Polygon");
dDDS_Sequence PointSeq_o = dDDS_SequenceCreate(Point_o, 0); // Anonymous sequence type
dDDS_MemberCreateChild(Point_o, "points", PointSeq_o);
dDDS_StructDefine(Polygon_o);
```
Representing the IDL type
```idl
struct Polygon {
  sequence<Point> points;
};
```
Code generation guarantees consistency throughout and makes it incredibly simple to add new capabilities to the API. 
All of the above has been realized with just 31 lines of code (see `dDDS.cx`).

## Limitations
### Missing IDL types
The prototype doesn't yet map the full IDL typesystem. What's missing is:
 * Unions
 * Typedefs
These types are not natively supported in Corto, thus a mapping would have to be created. 

### Unsupported types when reading / writing
The current API assumes that the layout of the samples is equal in DDS and Corto. This is true for 80% of the cases, however, sequences are different. Therefore when a user would attempt to read/write a type containing a sequence, the API will most likely crash.

### QoS settings
The API uses default QoS settings at the moment. A user cannot yet specify his own settings.

### Import types from running DDS system
At this point, the API cannot yet import types from a running DDS system.

### Metadescriptor limitations
Due to limitations in the metadescriptor format, the following type can be created in the API but not in XML:
```
struct Foo
struct Bar {
    sequence<Foo> member;
};
struct Foo {
    long member;
};
```
