/*-------------------------------------------------------------------------
 |
 |  Include-File :  Color.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  Color.
 |
 +-------------------------------------------------------------------------*/

#ifndef Color_h
#define Color_h

/*-------------------------------------------------------------------------
 |
 |  Class      : Color
 |  Base class :
 |
 |  Description :
 |
 |   This  class contains  a collection  of  static QColors  and a  color
 |   picker helper. It can generate  random colors, has defined colors by
 |   name and holds a standard color ring buffer.
 |
 +-------------------------------------------------------------------------*/

class Color
{

  public:
    static QColor random ();
    static QColor color ();
    static QColor color ( int i );

    static const QColor steelblue;
    static const QColor royalblue;
    static const QColor cornflowerblue;
    static const QColor lightsteelblue;
    static const QColor mediumslateblue;
    static const QColor slateblue;
    static const QColor darkslateblue;
    static const QColor midnightblue;
    static const QColor navy;
    static const QColor darkblue;
    static const QColor mediumblue;
    static const QColor blue;
    static const QColor dodgerblue;
    static const QColor deepskyblue;
    static const QColor lightskyblue;
    static const QColor skyblue;
    static const QColor lightblue;
    static const QColor powderblue;
    static const QColor azure;
    static const QColor lightcyan;
    static const QColor paleturquoise;
    static const QColor mediumturquoise;
    static const QColor lightseagreen;
    static const QColor darkcyan;
    static const QColor teal;
    static const QColor cadetblue;
    static const QColor darkturquoise;
    static const QColor aqua;
    static const QColor cyan;
    static const QColor turquoise;
    static const QColor aquamarine;
    static const QColor mediumaquamarine;
    static const QColor darkseagreen;
    static const QColor mediumseagreen;
    static const QColor seagreen;
    static const QColor darkgreen;
    static const QColor green;
    static const QColor forestgreen;
    static const QColor limegreen;
    static const QColor lime;
    static const QColor chartreuse;
    static const QColor lawngreen;
    static const QColor greenyellow;
    static const QColor yellowgreen;
    static const QColor palegreen;
    static const QColor lightgreen;
    static const QColor springgreen;
    static const QColor mediumspringgreen;
    static const QColor darkolivegreen;
    static const QColor olivedrab;
    static const QColor olive;
    static const QColor darkkhaki;
    static const QColor darkgoldenrod;
    static const QColor goldenrod;
    static const QColor gold;
    static const QColor yellow;
    static const QColor khaki;
    static const QColor palegoldenrod;
    static const QColor blanchedalmond;
    static const QColor moccasin;
    static const QColor wheat;
    static const QColor navajowhite;
    static const QColor burlywood;
    static const QColor tan;
    static const QColor rosybrown;
    static const QColor sienna;
    static const QColor saddlebrown;
    static const QColor chocolate;
    static const QColor peru;
    static const QColor sandybrown;
    static const QColor darkred;
    static const QColor maroon;
    static const QColor brown;
    static const QColor firebrick;
    static const QColor indianred;
    static const QColor lightcoral;
    static const QColor salmon;
    static const QColor darksalmon;
    static const QColor lightsalmon;
    static const QColor coral;
    static const QColor tomato;
    static const QColor darkorange;
    static const QColor orange;
    static const QColor orangered;
    static const QColor crimson;
    static const QColor red;
    static const QColor deeppink;
    static const QColor fuchsia;
    static const QColor magenta;
    static const QColor hotpink;
    static const QColor lightpink;
    static const QColor pink;
    static const QColor palevioletred;
    static const QColor mediumvioletred;
    static const QColor purple;
    static const QColor darkmagenta;
    static const QColor mediumpurple;
    static const QColor blueviolet;
    static const QColor indigo;
    static const QColor darkviolet;
    static const QColor darkorchid;
    static const QColor mediumorchid;
    static const QColor orchid;
    static const QColor violet;
    static const QColor plum;
    static const QColor thistle;
    static const QColor lavender;
    static const QColor ghostwhite;
    static const QColor aliceblue;
    static const QColor mintcream;
    static const QColor honeydew;
    static const QColor lightgoldenrodyellow;
    static const QColor lemonchiffon;
    static const QColor cornsilk;
    static const QColor lightyellow;
    static const QColor ivory;
    static const QColor floralwhite;
    static const QColor linen;
    static const QColor oldlace;
    static const QColor antiquewhite;
    static const QColor bisque;
    static const QColor peachpuff;
    static const QColor papayawhip;
    static const QColor beige;
    static const QColor seashell;
    static const QColor lavenderblush;
    static const QColor mistyrose;
    static const QColor snow;
    static const QColor white;
    static const QColor whitesmoke;
    static const QColor gainsboro;
    static const QColor lightgray;
    static const QColor silver;
    static const QColor darkgray;
    static const QColor gray;
    static const QColor lightslategray;
    static const QColor slategray;
    static const QColor dimgray;
    static const QColor darkslategray;
    static const QColor black;

};

/*------------------------------------------*/
/*   T Y P E   -  D e c l a r a t i o n s   */
/*------------------------------------------*/

  typedef QList < Color  > L_Color;

#endif
