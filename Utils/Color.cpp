/*-------------------------------------------------------------------------
 |
 |  File name   :  Color.cpp
 |
 |  Description :
 |
 |   This  file contains  the implementation  for the  Color class  which
 |   contains  a  collection  of  static   QColors  and  a  color  picker
 |   helper. It  can generate random  colors, has defined colors  by name
 |   and holds a color ring buffer.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 01.Feb.2023
 |  Modified  : Oskar Leirich                Last changes  : 23.Dec.2023
 |
 |
 |  This file contains the following functions:
 |    Color                     ~Color                    random
 |
 +-------------------------------------------------------------------------*/

#include "Color.h"

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

// colors by name definition
const QColor Color::steelblue            = QColor ( 0x46, 0x82, 0xB4 );
const QColor Color::royalblue            = QColor ( 0x04, 0x16, 0x90 );
const QColor Color::cornflowerblue       = QColor ( 0x64, 0x95, 0xED );
const QColor Color::lightsteelblue       = QColor ( 0xB0, 0xC4, 0xDE );
const QColor Color::mediumslateblue      = QColor ( 0x7B, 0x68, 0xEE );
const QColor Color::slateblue            = QColor ( 0x6A, 0x5A, 0xCD );
const QColor Color::darkslateblue        = QColor ( 0x48, 0x3D, 0x8B );
const QColor Color::midnightblue         = QColor ( 0x19, 0x19, 0x70 );
const QColor Color::navy                 = QColor ( 0x00, 0x00, 0x80 );
const QColor Color::darkblue             = QColor ( 0x00, 0x00, 0x8B );
const QColor Color::mediumblue           = QColor ( 0x00, 0x00, 0xCD );
const QColor Color::blue                 = QColor ( 0x00, 0x00, 0xFF );
const QColor Color::dodgerblue           = QColor ( 0x1E, 0x90, 0xFF );
const QColor Color::deepskyblue          = QColor ( 0x00, 0xBF, 0xFF );
const QColor Color::lightskyblue         = QColor ( 0x87, 0xCE, 0xFA );
const QColor Color::skyblue              = QColor ( 0x87, 0xCE, 0xEB );
const QColor Color::lightblue            = QColor ( 0xAD, 0xD8, 0xE6 );
const QColor Color::powderblue           = QColor ( 0xB0, 0xE0, 0xE6 );
const QColor Color::azure                = QColor ( 0xF0, 0xFF, 0xFF );
const QColor Color::lightcyan            = QColor ( 0xE0, 0xFF, 0xFF );
const QColor Color::paleturquoise        = QColor ( 0xAF, 0xEE, 0xEE );
const QColor Color::mediumturquoise      = QColor ( 0x48, 0xD1, 0xCC );
const QColor Color::lightseagreen        = QColor ( 0x20, 0xB2, 0xAA );
const QColor Color::darkcyan             = QColor ( 0x00, 0x8B, 0x8B );
const QColor Color::teal                 = QColor ( 0x00, 0x80, 0x80 );
const QColor Color::cadetblue            = QColor ( 0x5F, 0x9E, 0xA0 );
const QColor Color::darkturquoise        = QColor ( 0x00, 0xCE, 0xD1 );
const QColor Color::aqua                 = QColor ( 0x00, 0xFF, 0xFF );
const QColor Color::cyan                 = QColor ( 0x00, 0xFF, 0xFF );
const QColor Color::turquoise            = QColor ( 0x40, 0xE0, 0xD0 );
const QColor Color::aquamarine           = QColor ( 0x7F, 0xFF, 0xD4 );
const QColor Color::mediumaquamarine     = QColor ( 0x66, 0xCD, 0xAA );
const QColor Color::darkseagreen         = QColor ( 0x8F, 0xBC, 0x8F );
const QColor Color::mediumseagreen       = QColor ( 0x3C, 0xB3, 0x71 );
const QColor Color::seagreen             = QColor ( 0x2E, 0x8B, 0x57 );
const QColor Color::darkgreen            = QColor ( 0x00, 0x64, 0x00 );
const QColor Color::green                = QColor ( 0x00, 0x80, 0x00 );
const QColor Color::forestgreen          = QColor ( 0x22, 0x8B, 0x22 );
const QColor Color::limegreen            = QColor ( 0x32, 0xCD, 0x32 );
const QColor Color::lime                 = QColor ( 0x00, 0xFF, 0x00 );
const QColor Color::chartreuse           = QColor ( 0x7F, 0xFF, 0x00 );
const QColor Color::lawngreen            = QColor ( 0x7C, 0xFC, 0x00 );
const QColor Color::greenyellow          = QColor ( 0xAD, 0xFF, 0x2F );
const QColor Color::yellowgreen          = QColor ( 0x9A, 0xCD, 0x32 );
const QColor Color::palegreen            = QColor ( 0x98, 0xFB, 0x98 );
const QColor Color::lightgreen           = QColor ( 0x90, 0xEE, 0x90 );
const QColor Color::springgreen          = QColor ( 0x00, 0xFF, 0x7F );
const QColor Color::mediumspringgreen    = QColor ( 0x00, 0xFA, 0x9A );
const QColor Color::darkolivegreen       = QColor ( 0x55, 0x6B, 0x2F );
const QColor Color::olivedrab            = QColor ( 0x6B, 0x8E, 0x23 );
const QColor Color::olive                = QColor ( 0x80, 0x80, 0x00 );
const QColor Color::darkkhaki            = QColor ( 0xBD, 0xB7, 0x6B );
const QColor Color::darkgoldenrod        = QColor ( 0xB8, 0x86, 0x0B );
const QColor Color::goldenrod            = QColor ( 0xDA, 0xA5, 0x20 );
const QColor Color::gold                 = QColor ( 0xFF, 0xD7, 0x00 );
const QColor Color::yellow               = QColor ( 0xFF, 0xFF, 0x00 );
const QColor Color::khaki                = QColor ( 0xF0, 0xE6, 0x8C );
const QColor Color::palegoldenrod        = QColor ( 0xEE, 0xE8, 0xAA );
const QColor Color::blanchedalmond       = QColor ( 0xFF, 0xEB, 0xCD );
const QColor Color::moccasin             = QColor ( 0xFF, 0xE4, 0xB5 );
const QColor Color::wheat                = QColor ( 0xF5, 0xDE, 0xB3 );
const QColor Color::navajowhite          = QColor ( 0xFF, 0xDE, 0xAD );
const QColor Color::burlywood            = QColor ( 0xDE, 0xB8, 0x87 );
const QColor Color::tan                  = QColor ( 0xD2, 0xB4, 0x8C );
const QColor Color::rosybrown            = QColor ( 0xBC, 0x8F, 0x8F );
const QColor Color::sienna               = QColor ( 0xA0, 0x52, 0x2D );
const QColor Color::saddlebrown          = QColor ( 0x8B, 0x45, 0x13 );
const QColor Color::chocolate            = QColor ( 0xD2, 0x69, 0x1E );
const QColor Color::peru                 = QColor ( 0xCD, 0x85, 0x3F );
const QColor Color::sandybrown           = QColor ( 0xF4, 0xA4, 0x60 );
const QColor Color::darkred              = QColor ( 0x8B, 0x00, 0x00 );
const QColor Color::maroon               = QColor ( 0x80, 0x00, 0x00 );
const QColor Color::brown                = QColor ( 0xA5, 0x2A, 0x2A );
const QColor Color::firebrick            = QColor ( 0xB2, 0x22, 0x22 );
const QColor Color::indianred            = QColor ( 0xCD, 0x5C, 0x5C );
const QColor Color::lightcoral           = QColor ( 0xF0, 0x80, 0x80 );
const QColor Color::salmon               = QColor ( 0xFA, 0x80, 0x72 );
const QColor Color::darksalmon           = QColor ( 0xE9, 0x96, 0x7A );
const QColor Color::lightsalmon          = QColor ( 0xFF, 0xA0, 0x7A );
const QColor Color::coral                = QColor ( 0xFF, 0x7F, 0x50 );
const QColor Color::tomato               = QColor ( 0xFF, 0x63, 0x47 );
const QColor Color::darkorange           = QColor ( 0xFF, 0x8C, 0x00 );
const QColor Color::orange               = QColor ( 0xFF, 0xA5, 0x00 );
const QColor Color::orangered            = QColor ( 0xFF, 0x45, 0x00 );
const QColor Color::crimson              = QColor ( 0xDC, 0x14, 0x3C );
const QColor Color::red                  = QColor ( 0xFF, 0x00, 0x00 );
const QColor Color::deeppink             = QColor ( 0xFF, 0x14, 0x93 );
const QColor Color::fuchsia              = QColor ( 0xFF, 0x00, 0xFF );
const QColor Color::magenta              = QColor ( 0xFF, 0x00, 0xFF );
const QColor Color::hotpink              = QColor ( 0xFF, 0x69, 0xB4 );
const QColor Color::lightpink            = QColor ( 0xFF, 0xB6, 0xC1 );
const QColor Color::pink                 = QColor ( 0xFF, 0xC0, 0xCB );
const QColor Color::palevioletred        = QColor ( 0xDB, 0x70, 0x93 );
const QColor Color::mediumvioletred      = QColor ( 0xC7, 0x15, 0x85 );
const QColor Color::purple               = QColor ( 0x80, 0x00, 0x80 );
const QColor Color::darkmagenta          = QColor ( 0x8B, 0x00, 0x8B );
const QColor Color::mediumpurple         = QColor ( 0x93, 0x70, 0xDB );
const QColor Color::blueviolet           = QColor ( 0x8A, 0x2B, 0xE2 );
const QColor Color::indigo               = QColor ( 0x4B, 0x00, 0x82 );
const QColor Color::darkviolet           = QColor ( 0x94, 0x00, 0xD3 );
const QColor Color::darkorchid           = QColor ( 0x99, 0x32, 0xCC );
const QColor Color::mediumorchid         = QColor ( 0xBA, 0x55, 0xD3 );
const QColor Color::orchid               = QColor ( 0xDA, 0x70, 0xD6 );
const QColor Color::violet               = QColor ( 0xEE, 0x82, 0xEE );
const QColor Color::plum                 = QColor ( 0xDD, 0xA0, 0xDD );
const QColor Color::thistle              = QColor ( 0xD8, 0xBF, 0xD8 );
const QColor Color::lavender             = QColor ( 0xE6, 0xE6, 0xFA );
const QColor Color::ghostwhite           = QColor ( 0xF8, 0xF8, 0xFF );
const QColor Color::aliceblue            = QColor ( 0xF0, 0xF8, 0xFF );
const QColor Color::mintcream            = QColor ( 0xF5, 0xFF, 0xFA );
const QColor Color::honeydew             = QColor ( 0xF0, 0xFF, 0xF0 );
const QColor Color::lightgoldenrodyellow = QColor ( 0xFA, 0xFA, 0xD2 );
const QColor Color::lemonchiffon         = QColor ( 0xFF, 0xFA, 0xCD );
const QColor Color::cornsilk             = QColor ( 0xFF, 0xF8, 0xDC );
const QColor Color::lightyellow          = QColor ( 0xFF, 0xFF, 0xE0 );
const QColor Color::ivory                = QColor ( 0xFF, 0xFF, 0xF0 );
const QColor Color::floralwhite          = QColor ( 0xFF, 0xFA, 0xF0 );
const QColor Color::linen                = QColor ( 0xFA, 0xF0, 0xE6 );
const QColor Color::oldlace              = QColor ( 0xFD, 0xF5, 0xE6 );
const QColor Color::antiquewhite         = QColor ( 0xFA, 0xEB, 0xD7 );
const QColor Color::bisque               = QColor ( 0xFF, 0xE4, 0xC4 );
const QColor Color::peachpuff            = QColor ( 0xFF, 0xDA, 0xB9 );
const QColor Color::papayawhip           = QColor ( 0xFF, 0xEF, 0xD5 );
const QColor Color::beige                = QColor ( 0xF5, 0xF5, 0xDC );
const QColor Color::seashell             = QColor ( 0xFF, 0xF5, 0xEE );
const QColor Color::lavenderblush        = QColor ( 0xFF, 0xF0, 0xF5 );
const QColor Color::mistyrose            = QColor ( 0xFF, 0xE4, 0xE1 );
const QColor Color::snow                 = QColor ( 0xFF, 0xFA, 0xFA );
const QColor Color::white                = QColor ( 0xFF, 0xFF, 0xFF );
const QColor Color::whitesmoke           = QColor ( 0xF5, 0xF5, 0xF5 );
const QColor Color::gainsboro            = QColor ( 0xDC, 0xDC, 0xDC );
const QColor Color::lightgray            = QColor ( 0xD3, 0xD3, 0xD3 );
const QColor Color::silver               = QColor ( 0xC0, 0xC0, 0xC0 );
const QColor Color::darkgray             = QColor ( 0xA9, 0xA9, 0xA9 );
const QColor Color::gray                 = QColor ( 0x80, 0x80, 0x80 );
const QColor Color::lightslategray       = QColor ( 0x77, 0x88, 0x99 );
const QColor Color::slategray            = QColor ( 0x70, 0x80, 0x90 );
const QColor Color::dimgray              = QColor ( 0x69, 0x69, 0x69 );
const QColor Color::darkslategray        = QColor ( 0x2F, 0x4F, 0x4F );
const QColor Color::black                = QColor ( 0x00, 0x00, 0x00 );

// the standard colors ring buffer
const QColor standardColors [] =
{
  Color::black,
  Color::blue,
  Color::red,
  Color::green,
  Color::darkred,
  Color::cyan,
  Color::darkgreen,
  Color::darkblue,
  Color::magenta,
  Color::darkcyan,
  Color::orange,
  Color::darkmagenta,
  Color::yellow,
  Color::orange,
  Color::darkgray

};


/*-------------------------------------------------------------------------
 |
 |  Member function :  random  of  Color
 |
 |  Description :
 |
 |   This function generates a random color.
 |
 |  Output parameter :
 |   (QColor)        :
 |
 +-------------------------------------------------------------------------*/

QColor Color::random ()
{

  return QColor ( 0xFF & rand (), 0xFF & rand (), 0xFF & rand () );

}               /*-----  random  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  color  of  Color
 |
 |  Description :
 |
 |   This function returns the next color from the ring buffer.
 |
 |  Output parameter :
 |   (QColor)        :
 |
 +-------------------------------------------------------------------------*/

QColor Color::color ()
{

  static int index = 0;

  return color ( index++ );

}               /*-----  color  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  color  of  Color
 |
 |  Description :
 |
 |   This function returns a specific color from the ring buffer.
 |
 |  Input parameters :
 |   i               : index of the color that shall be returned.
 |
 |  Output parameter :
 |   (QColor)        :
 |
 +-------------------------------------------------------------------------*/

QColor Color::color ( int i )
{

  return standardColors [ i % ( sizeof (standardColors) / sizeof (QColor) ) ];

}               /*-----  color  -----*/
