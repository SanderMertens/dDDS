#include "SimpleType.h"

int SimpleTypeMain(int argc, char *argv[]) {
    corto_string xml;

    /* Create simple Point struct */
    dDDS_Module Foo_o = dDDS_ModuleCreateChild(root_o, "Foo");
    dDDS_Struct Point_o = dDDS_StructDeclareChild(Foo_o, "Point");
    dDDS_MemberCreateChild(Point_o, "x", dDDS_Long_o);
    dDDS_MemberCreateChild(Point_o, "y", dDDS_Long_o);
    dDDS_StructDefine(Point_o);

    /* Print Xml for simple struct */
    xml = dDDS_Struct_metaXml(Point_o);
    printf("# IDL:\nstruct Point {\n  long x;\n  long y;\n};\n\n# XML:\n");
    printf("%s\n", xml);
    dDDS_free(xml);

    return 0;
}
