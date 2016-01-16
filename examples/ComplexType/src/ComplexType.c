#include "ComplexType.h"

int ComplexTypeMain(int argc, char *argv[]) {
    corto_string xml;

    /* Create simple Point struct */
    dDDS_Module Foo_o = dDDS_ModuleCreateChild(root_o, "Foo");
    dDDS_Struct Point_o = dDDS_StructDeclareChild(Foo_o, "Point");
    dDDS_MemberCreateChild(Point_o, "x", dDDS_Long_o);
    dDDS_MemberCreateChild(Point_o, "y", dDDS_Long_o);
    dDDS_StructDefine(Point_o);

    /* Complex struct */
    dDDS_Struct Polygon_o = dDDS_StructDeclareChild(Foo_o, "Polygon");
    dDDS_Member id_o = dDDS_MemberCreateChild(Polygon_o, "id", dDDS_Long_o);
    dDDS_Member_setKey(id_o, 1); // First key member- number indicates ordering
    dDDS_MemberCreateChild(Polygon_o, "name", dDDS_String_o);
    dDDS_Sequence PointSeq_o = dDDS_SequenceCreate(Point_o, 0);
    dDDS_MemberCreateChild(Polygon_o, "points", PointSeq_o);
    dDDS_StructDefine(Polygon_o);

    /* Print Xml for complex struct */
    printf("# IDL:\nstruct Polygon {\n  long id;\n  string name;\n  sequence<Point> points;\n};\n\n# XML:\n");
    xml = dDDS_Struct_metaXml(Polygon_o);
    printf("%s\n", xml);
    dDDS_free(xml);

    return 0;
}
