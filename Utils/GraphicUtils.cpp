/*-------------------------------------------------------------------------
 |
 |  File Name   :  GraphicUtils.cpp
 |
 |  Description :
 |
 |   This  modul contains  the class  'GraphicUtils' which  provides some
 |   functions  for clipping,  rectangle conversion,  point distance  and
 |   more.
 |
 |  Developer : Oskar Leirich                Creation date : 22.Jul.2025
 |  Modified  : Oskar Leirich                Last changes  : 20.Nov.2026
 |
 |  This unit contains following member functions :
 |   of GraphicUtils :
 |    GraphicUtils              ~GraphicUtils             clipLiangBrasky
 |    clipSutherland            clipTestLiangBrasky       clipTestSutherland
 |    containsPoint             convRectF                 findIntersect
 |    getDistance               getPointsRectF            getRectFMoveBy
 |    setWinDimension
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "GraphicUtils.h"

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const  int  INSIDE = 0;  // 0000
const  int  LEFT   = 1;  // 0001
const  int  RIGHT  = 2;  // 0010
const  int  BOTTOM = 4;  // 0100
const  int  TOP    = 8;  // 1000


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  GraphicUtils
 |
 |  Description :
 |
 |   The constructor initializes the some member variables.
 |
 +-------------------------------------------------------------------------*/

GraphicUtils::GraphicUtils ()
{

  xwmin = 0;
  xwmax = 0;
  ywmin = 0;
  ywmax = 0;

}               /*-----  GraphicUtils  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  of  GraphicUtils
 |
 |  Description :
 |
 |   This is the default constructor.
 |
 +-------------------------------------------------------------------------*/

GraphicUtils::~GraphicUtils ()
{

}               /*-----  ~GraphicUtils  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  containsPoint  of  GraphicUtils
 |
 |  Description :
 |
 |   This function  checks if the  given widget contains the  given point
 |   (in global screen coordinates) and returns the result of the test.
 |
 |  Input parameters :
 |   widget          :  pointer to the widget
 |   point           :  point to test (in global screen coordinates)
 |
 +-------------------------------------------------------------------------*/

bool GraphicUtils::containsPoint ( QWidget * widget, QPoint point )
{
  QPointF pos;
  QPointF pnt;
  QRectF  rect;

  pos  = widget->mapToGlobal ( widget->pos () );
  pnt  = widget->mapToGlobal (point);

  rect = widget->rect ();
  rect.translate (pnt);

  return rect.contains (pnt);

}               /*-----  containsPoint  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  clipTestLiangBrasky  of  GraphicUtils
 |
 |  Description :
 |
 |   This function  implements the Liang-Barsky line  clipping algorithm.
 |   This algorithm is more efficient than Cohen-Sutherland line clipping
 |   algorithm  and can  be  extended to  3-Dimensional  clipping and  is
 |   considered to be the faster parametric line-clipping algorithm.
 |
 |  Input parameters :
 |   p               :
 |   q               :
 |   t0              :
 |   t1              :
 |
 |  Output parameter :
 |   (bool)          :
 |
 +-------------------------------------------------------------------------*/

bool GraphicUtils::clipTestLiangBrasky ( const float & p, const float & q,
                                         double & t0, double & t1 )
{
  double t;

  if ( p < 0 )
  {
    t = q / p;

    if ( t > t1 )
      return false;

    t0 = max ( t0, t );
  }
  else
  {
    if ( p > 0 )
    {
      t = q / p;

      if ( t < t0 )
        return false;

      t0 = min ( t1, t );
    }
    else
    {
      if ( q < 0 )
        return false;
    }
  }

  return true;

}               /*-----  clipTestLiangBrasky  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  clipLiangBrasky  of  GraphicUtils
 |
 |  Description :
 |
 |   This function  implements the Liang-Barsky line  clipping algorithm.
 |   This algorithm is more efficient than Cohen-Sutherland line clipping
 |   algorithm  and can  be  extended to  3-Dimensional  clipping and  is
 |   considered to be the faster parametric line-clipping algorithm.
 |
 |  Input parameters :
 |   p               :
 |   q               :
 |
 |  Output parameter :
 |   (bool)          :
 |
 +-------------------------------------------------------------------------*/

bool GraphicUtils::clipLiangBrasky ( QPointF & p, QPointF & q )
{
  double t0;
  double t1;
  float  dx;
  float  dy;

  t0 = 0.0;
  t1 = 1.0;
  dx = q.x () - p.x ();

  if ( clipTestLiangBrasky ( -dx, p.x () - xwmin, t0, t1 ) )
  {
    if ( clipTestLiangBrasky ( dx, xwmax - p.x (), t0, t1 ) )
    {
      dy = q.y () - p.y ();

      if ( clipTestLiangBrasky ( -dy, p.y () - ywmin, t0, t1 ) )
      {
        if ( clipTestLiangBrasky ( dy, ywmax - p.y (), t0, t1 ) )
        {
          if ( t0 > 0.0 )
          {
            p.setX ( p.x () + t0 * dx );
            p.setY ( p.y () + t0 * dy );
          }

          if ( t1 < 1.0 )
          {
            q.setX ( p.x () + t1 * dx );
            q.setY ( p.y () + t1 * dy );
          }
          return true;
        }
      }
    }
  }

  return false;

}               /*-----  clipLiangBrasky  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  clipTestSutherland  of  GraphicUtils
 |
 |  Description :
 |
 |   This function  implements the  Cohen-Sutherland line  clipping.  The
 |   algorithm divides  a two-dimensional space  into 9 regions  and then
 |   efficiently  determines the  lines and  portions of  lines that  are
 |   visible in the central region of interest (the viewport).
 |
 |  Input parameters :
 |   x               :
 |   y               :
 |
 |  Output parameter :
 |   (int)           :
 |
 +-------------------------------------------------------------------------*/

int GraphicUtils::clipTestSutherland ( const float & x, const float & y )
{
  int code;

  code = INSIDE;       // initialized as being inside of clip window

  if ( x < xwmin )     // to the left of clip window
    code |= LEFT;
  else
  {
    if ( x > xwmax )   // to the right of clip window
      code |= RIGHT;
  }

  if ( y < ywmin )     // below the clip window
    code |= TOP;
  else
  {
    if ( y > ywmax )   // above the clip window
      code |= BOTTOM;
  }

  return code;

}               /*-----  clipTestSutherland  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  clipSutherland  of  GraphicUtils
 |
 |  Description :
 |
 |   This  function  implements the  the  Cohen-Sutherland algorithm  for
 |   clipping. The algorithm divides a  2D space into 9 regions, of which
 |   only the middle part (viewport) is visible.
 |
 |   The  algorithm includes,  excludes  or partially  includes the  line
 |   based on where:
 |    Both endpoints are  in the viewport region
 |                        (bitwise  OR of endpoints== 0): trivial accept.
 |    Both endpoints are on the same non-visible region
 |                        (bitwise AND of endpoints != 0): trivial reject.
 |    Both endpoints are in different regions:
 |      In case of this non trivial situation the algorithm finds one of
 |      the two points that are outside the viewport region
 |      (there will be at least one point outside).
 |      The intersection of the outpoint and extended viewport border is
 |      then calculated (i.e. with the parametric equation for the line)
 |      and this new point replaces the outpoint. The algorithm repeats
 |      until a trivial accept or reject occurs.
 |
 |  Input parameters :
 |   p               :
 |   q               :
 |
 |  Output parameter :
 |   (bool)          :
 |
 +-------------------------------------------------------------------------*/

GraphicUtils::PointCheck GraphicUtils::clipSutherland ( QPointF & p, QPointF & q )
{
  int   outcodeOut;
  int   outcode0;
  int   outcode1;
  float x,  y;
  float x0, y0;
  float x1, y1;

  x = 0;
  y = 0;

  x0 = p.x ();
  y0 = p.y ();
  x1 = q.x ();
  y1 = q.y ();

  outcode0 = clipTestSutherland ( x0, y0 );
  outcode1 = clipTestSutherland ( x1, y1 );

  // logical or is 0. Trivially accept and get out of loop
  if ( not ( outcode0 | outcode1 ) )
    return PointCheck::Inside;

  // logical and is not 0. Trivially reject and get out of loop
  if ( outcode0 & outcode1 )
    return PointCheck::Outside;

  while (true)
  {
    if ( not ( outcode0 | outcode1 ) )
    {
      break;
    }
    else
    {
      if ( outcode0 & outcode1 )
      { // logical and is not 0. Trivially reject and get out of loop
        break;
      }
      else
      {
        // failed both tests, so calculate the line segment to clip
        // from an outside point to an intersection with clip edge

        // At least one endpoint is outside the clip rectangle; pick it.
        if (outcode0)
          outcodeOut = outcode0;
        else
          outcodeOut = outcode1;

        // Now find the intersection point;
        // use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
        if ( outcodeOut & BOTTOM )
        { // point is above the clip rectangle
          x = x0 + ( x1 - x0 ) * ( ywmax - y0 ) / ( y1 - y0 );
          y = ywmax;
        }
        else
        {
          if ( outcodeOut & TOP )
          { // point is below the clip rectangle
            x = x0 + ( x1 - x0 ) * ( ywmin - y0 ) / ( y1 - y0 );
            y = ywmin;
          }
          else
          {
            if ( outcodeOut & RIGHT )
            { // point is to the right of clip rectangle
              y = y0 + ( y1 - y0 ) * ( xwmax - x0 ) / ( x1 - x0 );
              x = xwmax;
            }
            else
            { // point is to the left of clip rectangle
              if ( outcodeOut & LEFT )
              { // point is to the left of clip rectangle
                y = y0 + ( y1 - y0 ) * ( xwmin - x0 ) / ( x1 - x0 );
                x = xwmin;
              }
            }
          }
        }

        // Now we move outside point to intersection point to clip
        // and get ready for next pass.
        if ( outcodeOut == outcode0 )
        {
          x0 = x;
          y0 = y;
          outcode0 = clipTestSutherland ( x0, y0 );
        }
        else
        {
          x1 = x;
          y1 = y;
          outcode1 = clipTestSutherland ( x1, y1 );
        }
      }
    }
  }

  p.setX (x0);
  p.setY (y0);
  q.setX (x1);
  q.setY (y1);

  return PointCheck::Changed;

}               /*-----  clipSutherland  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  convRectF  of  GraphicUtils
 |
 |  Description :
 |
 |   This function converts a QRectF to a QRect.
 |
 +-------------------------------------------------------------------------*/

QRect GraphicUtils::convRectF ( QRectF rectF )
{
  QRect rect;
  int   top;
  int   bottom;
  int   left;
  int   right;

  top    = VarUtils::round ( rectF.top    () );
  bottom = VarUtils::round ( rectF.bottom () );
  left   = VarUtils::round ( rectF.left   () );
  right  = VarUtils::round ( rectF.right  () );

  rect.setLeft   (top);
  rect.setRight  (bottom);
  rect.setBottom (left);
  rect.setTop    (right);

  if ( not rect.isValid () )
  {
    rect = rect.normalized ();
  }

  return rect;

}               /*-----  convRectF  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findIntersect  of  GraphicUtils
 |
 |  Description :
 |
 |   This function searches if there  is a intersection between the given
 |   line and the  rectangle within the start and end  point of the line.
 |   When a bounded intersection is  found then the intersection point is
 |   valid.
 |
 |  Input parameters :
 |   line            : Line to test
 |   rect            : Rectangle to test
 |   point           : Intersection point if there is one
 |
 |  Output parameter :
 |   (bool)          : Intersection found (true) or not
 |
 +-------------------------------------------------------------------------*/

bool GraphicUtils::findIntersect ( QLineF line, QRectF rect, QPointF & point )
{
  QLineF line1;

  // test the line on top of the rectangle
  line1.setPoints ( rect.topLeft (), rect.topRight () );

  if ( line.intersects ( line1, & point ) == QLineF::BoundedIntersection )
    return true;

  // test the line on right side of the rectangle
  line1.setPoints ( rect.bottomRight (), rect.topRight () );

  if ( line.intersects ( line1, & point ) == QLineF::BoundedIntersection )
    return true;

  // test the line on bottom of the rectangle
  line1.setPoints ( rect.bottomLeft (), rect.bottomRight () );

  if ( line.intersects ( line1, & point ) == QLineF::BoundedIntersection )
    return true;

  // test the line on left side of the rectangle
  line1.setPoints ( rect.bottomLeft (), rect.topLeft () );

  if ( line.intersects ( line1, & point ) == QLineF::BoundedIntersection )
    return true;

  return false;

}               /*-----  findIntersect  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getDistance  of  GraphicUtils
 |
 |  Description :
 |
 |   This function returns the distance between the two given points.
 |
 +-------------------------------------------------------------------------*/

float GraphicUtils::getDistance ( QPointF p1, QPointF p2 )
{
  float distance;
  float difx;
  float dify;

  difx = p1.x () - p2.x ();
  dify = p1.y () - p2.y ();

  distance = std::sqrt ( ( difx * difx ) + ( dify * dify ) );

  return distance;

}               /*-----  getDistance  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getPointsRectF  of  GraphicUtils
 |
 |  Description :
 |
 |   This function returns the points of the given rectangle in a list of
 |   points.  The order of the points is top left, right and bottom left,
 |   right.
 |
 +-------------------------------------------------------------------------*/

L_PointsF GraphicUtils::getPointsRectF ( QRectF rect )
{
  L_PointsF points;

  points.resize (4);
  points [0] = rect.topLeft     ();   // first point
  points [1] = rect.topRight    ();   // second point
  points [2] = rect.bottomLeft  ();   // third point
  points [3] = rect.bottomRight ();   // fourth point

  return points;

}               /*-----  getPointsRectF  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getRectFMoveBy  of  GraphicUtils
 |
 |  Description :
 |
 |   This  function returns  the difference  positions of  the two  given
 |   rectangles.  This can be used to move a rectange, line, or points by
 |   the calculated values.
 |
 +-------------------------------------------------------------------------*/

L_PointsF GraphicUtils::getRectFMoveBy ( QRectF rect1, QRectF rect2 )
{
  L_PointsF pntsDif;
  L_PointsF points1;
  L_PointsF points2;
  QPointF   pnt;
  float     x, y;

  points1 = getPointsRectF (rect1);
  points2 = getPointsRectF (rect2);

  for ( int i = 0; i < 4; i++ )
  {
    x = points1 [i].x () - points2 [i].x ();
    y = points1 [i].y () - points2 [i].y ();

    pnt.setX (x);
    pnt.setY (y);

    pntsDif.append (pnt);
  }

  return pntsDif;

}               /*-----  getRectFMoveBy  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setWinDimension  of  GraphicUtils
 |
 |  Description :
 |
 |   This function stores the window dimension fors the clipping functions.
 |
 +-------------------------------------------------------------------------*/

void GraphicUtils::setWinDimension ( float xwmin, float xwmax,
                                     float ywmin, float ywmax )
{

  this->xwmin = xwmin;
  this->xwmax = xwmax;
  this->ywmin = ywmin;
  this->ywmax = ywmax;

}               /*-----  setWinDimension  -----*/

