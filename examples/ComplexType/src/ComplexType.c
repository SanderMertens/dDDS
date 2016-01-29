#include "ComplexType.h"

/* ComplexType example
 *
 * This example demonstrates how to create a complex dynamic type, and display
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

int ComplexTypeMain(int argc, char *argv[]) {
    char* xml;

    /* Create simple Point struct */
    dDDS_ModuleCreateChild_auto(root_o, Foo);
    dDDS_StructDeclareChild_auto(Foo, Point);
    dDDS_MemberCreateChild_auto(Point, x, dDDS_Long_o);
    dDDS_MemberCreateChild_auto(Point, y, dDDS_Long_o);
    dDDS_StructDefine(Point);

    /* Create Color enumeration */
    dDDS_EnumDeclareChild_auto(Foo, Color);
    dDDS_ConstantCreateChild_auto(Color, Red, 0);
    dDDS_ConstantCreateChild_auto(Color, Orange, 1);
    dDDS_ConstantCreateChild_auto(Color, Yellow, 2);
    dDDS_ConstantCreateChild_auto(Color, Green, 3);
    dDDS_ConstantCreateChild_auto(Color, Blue, 4);
    dDDS_ConstantCreateChild_auto(Color, Purple, 5);
    dDDS_EnumDefine(Color);

    /* Complex struct */
    dDDS_StructDeclareChild_auto(Foo, Polygon);
    dDDS_MemberCreateChild_auto(Polygon, id, dDDS_Long_o);
    dDDS_MemberCreateChild_auto(Polygon, name, dDDS_String_o);
    dDDS_MemberCreateChild_auto(Polygon, color, Color);
    dDDS_SequenceCreate_auto(PointSeq, Point, 0);
    dDDS_MemberCreateChild_auto(Polygon, points, PointSeq);
    dDDS_StructDefine(Polygon);

    /* Print Xml */
    printf(
      "# IDL:\n"\
      "enum Color {\n"\
      "  Red,\n"\
      "  Orange,\n"\
      "  Yellow,\n"\
      "  Green,\n"\
      "  Blue,\n"\
      "  Purple,\n"\
      "};\n\n"
      "struct Polygon {\n"\
      "  long id;\n"\
      "  string name;\n"\
      "  Color color;\n"\
      "  sequence<Point> points;\n"\
      "};\n\n"\
      "# XML:\n");

    xml = dDDS_toMetaXml(Polygon);
    printf("%s\n", xml);
    dDDS_free(xml);

    return 0;
}
