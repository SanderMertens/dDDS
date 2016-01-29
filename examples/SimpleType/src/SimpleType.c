#include "SimpleType.h"

/* SimpleType example
 *
 * This example demonstrates how to create a simple dynamic type, and display
 * the OpenSplice XML metadescriptor.
 *
 * The example uses generated _auto macro's which results in more concise code.
 * These macro's 'emulate' C++'s auto behavior by automatically declaring a
 * variable of the correct type.
 *
 * Without _auto:
 *   dDDS_Struct Point = dDDS_StructDeclareChild(dp, "Point");
 *
 * With _auto:
 *   dDDS_StructDeclareChild_auto(Foo, Point);
 *
 */

int SimpleTypeMain(int argc, char *argv[]) {
    corto_string xml;

    /* Create simple Point struct */
    dDDS_ModuleCreateChild_auto(root_o, Foo);
    dDDS_StructDeclareChild_auto(Foo, Point);
    dDDS_MemberCreateChild_auto(Point, x, dDDS_Long_o);
    dDDS_MemberCreateChild_auto(Point, y, dDDS_Long_o);
    dDDS_StructDefine(Point);

    /* Print Xml */
    xml = dDDS_toMetaXml(Point);
    printf("# IDL:\nstruct Point {\n  long x;\n  long y;\n};\n\n# XML:\n");
    printf("%s\n", xml);
    dDDS_free(xml);

    return 0;
}
