
# dDDS (dynamic DDS)

This project is a prototype for a dynamic DDS API which allows for the creation of types at runtime. The API provides
OpenSpliceDDS compatible metadescriptors based on dynamically created types.

## Building dDDS

Install Corto
```
curl https://corto.io/install | sh
```

Build the package
```
corto build dDDS
```

## Run examples
### SimpleType
```
corto run dDDS/examples/SimpleType
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
corto run dDDS/examples/ComplexType
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

From the `dDDS.cx` file the complete dDDS API is generated. An example:
```c
dDDS_Module Foo_o = dDDS_ModuleCreateChild(root_o, "Foo");
dDDS_Struct Point_o = dDDS_StructDeclareChild(Foo_o, "Point");
dDDS_MemberCreateChild(Point_o, "x", dDDS_Long_o);
dDDS_MemberCreateChild(Point_o, "y", dDDS_Long_o);
dDDS_StructDefine(Point_o);
```
This code is a good example of what kind of code is generated:
 * Type definitions (dDDS_Module, dDDS_Struct)
 * Type objects (dDDS_Long_o)
 * Creation functions (*CreateChild)
 * Forward declaration functions (*DeclareChild, Define)

The types in this example are all named. Alternatively, you can also create anonymous types:
```c
dDDS_Module Foo_o = dDDS_ModuleCreateChild(root_o, "Foo");
dDDS_Struct Polygon_o = dDDS_StructDeclareChild(Foo_o, "Polygon");
dDDS_Sequence PointSeq_o = dDDS_SequenceCreate(Point_o, 0); // Anonymous sequence type
dDDS_MemberCreateChild(Point_o, "points", PointSeq_o);
dDDS_StructDefine(Polygon_o);
```
Code generation guarantees consistency throughout and makes it incredibly simple to add new capabilities to the API. 
All of the above has been realized with just 23 lines of code (see `dDDS.cx`).
