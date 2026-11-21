/*-------------------------------------------------------------------------
 |
 |  File Name   :  ColorChannel.cpp
 |
 |  Description :
 |
 |   This module  implements the class 'ColorChannel'  with functions for
 |   setting, modifying and reading curve  points and samples for a color
 |   or  gray channel.   It contains  functions for  the conversion  from
 |   points to curves, handling and creating of lookup tables.
 |
 |  Developer : Oskar Leirich                Creation date : 17.Feb.2025
 |  Modified  : Oskar Leirich                Last changes  : 18.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of ColorChannel:
 |    ColorChannel              ~ColorChannel             addPoint
 |    ascendingX                calcSamplePnts            checkLinear
 |    createLine                createLinear              createSamples
 |    createSigmoid             createSpline              createSplines
 |    delPoint                  evalPoints                evalSpline
 |    findPoint                 getChannelInfo            initDefault
 |    isEqual                   limitPoint                resetPoints
 |    setBlackPnt               setChannel                setCurvePoints
 |    setCurveType              setLowHigh                setPoint
 |    setWhitePnt               sigmoidToSpline
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "ColorChannel.h"
#include "Spline.h"

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const float cPntTolerance = 0.050f;
const float cPntRound     = 0.025f;
const float cPntCompare   = + 1e-4f;
const int   cSplinePoints = 7;

/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  ColorChannel
 |
 |  Description :
 |
 |   The constructor initializes the color channel member variables.
 |
 +-------------------------------------------------------------------------*/

ColorChannel::ColorChannel ()
{

  initDefault ();

}               /*----- ColorChannel -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  ColorChannel
 |
 |  Description :
 |
 |   The destructor is emtpy.
 |
 +-------------------------------------------------------------------------*/

ColorChannel::~ColorChannel ()
{

}               /*-----  ~ColorChannel  -----*/


/*-------------------------------------------------------------------------
 |
 |  Memberfunction :  ascendingX  of  ColorChannel
 |
 |  Description :
 |
 |   This function  is a comparison  function which is used  for sorting.
 |   It returns true when the x  coordinate of the first point is smaller
 |   than the second x coordinate.
 |
 +-------------------------------------------------------------------------*/

bool ColorChannel::ascendingX ( QPointF firstObj, QPointF secondObj )
{

  if ( firstObj.x () < secondObj.x () )
    return true;
  else
    return false;

}               /*-----  ascendingX  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : initDefault  of  ColorChannel
 |
 |  Description :
 |
 |   This function  initializes the  curve with type  linear and  the two
 |   points black and white and creates an 8 bit lookup table.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::initDefault ( )
{

  numBins = 256;
  sigma   = 0.07f;
  offset  = 0.0;

  samples.clear ();
  delta = 1.0 / ( numBins - 1 );

  resetPoints ();

  cvLut.create ( 1, numBins, CV_8UC1 );

}               /*-----  initDefault  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  checkLinear  of  ColorChannel
 |
 |  Description :
 |
 |   This function  checks the points  to decide  if they contain  only a
 |   black and white point or spline points.
 |
 +-------------------------------------------------------------------------*/

bool ColorChannel::checkLinear ()
{
  bool linear;

  if ( ( points.count () == 2 )             and
       ( isEqual ( 1.0, points [0].y () ) ) and
       ( isEqual ( 1.0, points [1].y () ) ) )
  {
    linear = true;

    curveType == cTypeLinear;
  }
  else
  {
    linear = false;

    curveType == cTypeSpline;
  }

  return linear;

}               /*-----  checkLinear  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  calcSamplePnts  of  ColorChannel
 |
 |  Description :
 |
 |   This function  creates sample  value points  from the  lookup table.
 |   The lookup table is scaled with 255, the sample points are stores as
 |   float in the range from 0.0 to 1.0.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::calcSamplePnts ()
{
  QPointF point;
  double  scale;
  float   x, y;

  samples.clear ();

  scale = 1.0 / 255.0;   // change range and resolution

  for ( int i = 0; i < numBins; i++ )
  {
    x = scale * i;
    y = scale * cvLut.at <uchar> (i);

    point.setX (x);
    point.setY (y);

    samples.append (point);
  }

}               /*-----  calcSamplePnts  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLine  of  ColorChannel
 |
 |  Description :
 |
 |   This function  creates a lookup table  from the levels curve  if the
 |   type is linear.  It always has a point at the beginnning (0) and end
 |   (100) on the x-coordinate.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::createLine ( QPointF begPnt, QPointF endPnt )
{
  float x,  y;
  float x0, x1;
  float y0, y1;
  int   begIdx;
  int   endIdx;

  x0 = begPnt.x ();
  x1 = endPnt.x ();

  y0 = begPnt.y ();
  y1 = endPnt.y ();

  begIdx = x0 * 255;
  endIdx = x1 * 255;

  for ( int i = begIdx; i < endIdx; i++ )
  {
    x = 1.0 * delta * i;
    y = y0 + ( x - x0 ) * ( y1 - y0 ) / ( x1 - x0 );

    // scaling to 256 must be done here because of uchar
    cvLut.at <uchar> (i) = 255.0 * y;
  }

}               /*-----  createLine  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLinear  of  ColorChannel
 |
 |  Description :
 |
 |   This function creates a lookup table if the curve type is linear. It
 |   always  has a  point at  the  beginnning (0)  and end  (100) on  the
 |   x-coordinate.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::createLinear ()
{
  float x0, x1;
  float x, y;

  x0 = points [0].x ();
  x1 = points [1].x ();

  for ( int i = 0; i < numBins; i++ )
  {
    x = 1.0 * delta * i;

    if ( x <= x0 )
      y = 0.0;
    else
    {
      if ( x >= x1 )
        y = 1;
      else   // generic linear interpolation with y0 = 0.0 and y1 = 1.0
        y = 0.0 + ( x - x0 ) * ( 1.0 - 0.0 ) / ( x1 - x0 );
    }

    // scaling to 256 must be done here because of uchar
    cvLut.at <uchar> (i) = 255.0 * y;
  }

}               /*-----  createLinear  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createSigmoid  of  ColorChannel
 |
 |  Description :
 |
 |   This function creates a lookup table with the shape of a sigmoid and
 |   stores the points also in a 'sigmoid' vector.
 |
 |              1
 |   y = -----------------
 |                 x - 0.5
 |               - -------
 |          1 + e   sigma
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::createSigmoid ()
{
  QPointF point;
  double  exp;
  double  offs;
  float   x, y;

  sigmoid.clear ();

  offs = offset / 10;

  for ( int i = 0; i < numBins; i++ )
  {
    x = 1.0 * delta * i;

    exp = - ( x - 0.5 ) / sigma + offs;
    y = 1.0 / ( 1.0 + qExp (exp) );

    point.setX (x);
    point.setY (y);

    sigmoid.append (point);

    // scaling to 256 must be done here because of uchar
    cvLut.at <uchar> (i) = 255.0 * y;
  }

}               /*-----  createSigmoid  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createSpline  of  ColorChannel
 |
 |  Description :
 |
 |   This function  creates a lookup table  from the levels curve  if the
 |   type is  spline.  It  calculates a natural  spline within  the given
 |   points.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::createSpline ( L_PointF inPoints )
{
  Spline spline;
  float  delta;
  float  xdelta;
  float  x, x0, y;
  int    numPnts;

  numPnts = inPoints.size ();

  if ( ! spline.setPoints (inPoints) )
    return;

  x0 = inPoints [0].x ();

  // difference between first and last point
  xdelta  = inPoints.last ().x () - x0;
  numPnts = qRound ( xdelta * numBins );

  delta = ( inPoints.last ().x () - x0 ) / ( numPnts - 1 );

  for ( int i = 0; i < numPnts; i++ )
  {
    x = x0 + i * delta;
    y = spline.getValueY (x);

    y = qBound ( 0.0, y, 1.0 );

    // scaling to 256 must be done here because of uchar
    cvLut.at <uchar> (i) = 255.0 * y;
  }

}               /*-----  createSpline  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createSplines  of  ColorChannel
 |
 |  Description :
 |
 |   This function  creates a lookup table  from the levels curve  if the
 |   type is spline.  It can contain  three sections: a linear one, then
 |   a spline section and again a linear one at the end.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::createSplines ()
{
  L_PointF inPoints;
  QPointF  point;
  int      cnt;
  float    y;

  inPoints.clear ();

  cnt   = points.count ();
  point = points.first ();

  y = point.y ();

  // evaluate if the curve is linear at the beginning
  if (lineBeg)
  {
    if ( point.x () > 0 )
      createLine ( QPointF ( 0, y ), point );

    for ( int i = 0; i < splineBeg; i++ )
    {
      createLine ( points [i], points [i+1] );
    }
  }

  // evaluate if the curve is linear at the end
  if (lineEnd)
  {
    point = points.last ();

    y = point.y ();

    for ( int i = splineEnd + 1; i < cnt; i++ )
    {
      createLine ( points [ i - 1 ], points [i] );
    }

    createLine ( point, QPointF ( 1.0, y ) );
  }

  // create spline points in the middle
  for ( int i = splineBeg; i <= splineEnd; i++ )
  {
    inPoints.append ( points [i] );
  }

  if (splines)
    createSpline (inPoints);

}               /*-----  createSplines  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : createSamples  of  ColorChannel
 |
 |  Description :
 |
 |   This function creates the sample values from the points depending on
 |   the curve type.  The sample points  are stored in the 'samples' list
 |   of points  and in  the 'cvLut'  lookup table.   The lookup  table is
 |   scaled with  256 because the lookup  table is of type  'uchar', with
 |   float it does  not work what is very strange,  the sample points are
 |   stores as float in the range from 0.0 to 1.0.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::createSamples ()
{

  samples.clear ();
  delta = 1.0 / ( numBins - 1 );

  evalPoints ();   // update the points information

  if ( curveType == cTypeSigmoid )
  {
    createSigmoid ();
  }

  if ( curveType == cTypeLinear )
  {
    createLinear ();
  }

  if ( curveType == cTypeSpline )
  {
    createSplines ();
  }

  calcSamplePnts ();

}               /*-----  createSamples  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : addPoint  of  ColorChannel
 |
 |  Description :
 |
 |   This  function adds  a  point to  the  list and  sorts  them by  the
 |   x-coordinate. The point index in the list is returned.
 |
 +-------------------------------------------------------------------------*/

int ColorChannel::addPoint ( QPointF point )
{
  QPointF pnt;
  int     idx;

  limitPoint (point);     // limit to allowed range

  points.append (point);  // add point

  std::sort ( points.begin (), points.end (), ColorChannel::ascendingX );

  idx = findPoint (point);

  return idx;

}               /*-----  addPoint  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : delPoint  of  ColorChannel
 |
 |  Description :
 |
 |   This function searches for the closest point and deletes it from the
 |   list if the distance is below a given tolerance.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::delPoint ( QPointF point )
{
  int idx;

  idx = findPoint (point);

  if ( idx >= 0 )
    points.removeAt (idx);

}               /*-----  delPoint  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : findPoint  of  ColorChannel
 |
 |  Description :
 |
 |   This function compares  the given point with all points  in the list
 |   and returns the index  if there is a point with  a small distance to
 |   the given point.
 |
 +-------------------------------------------------------------------------*/

int ColorChannel::findPoint ( QPointF point )
{
  QPointF pnt;
  int     idx;

  idx = -1;

  for ( int i = 0; i < points.count (); i++ )
  {
    pnt = points [i];

    if ( ( fabs ( point.x () - pnt.x () ) < cPntTolerance ) and
         ( fabs ( point.y () - pnt.y () ) < cPntTolerance ) )
    {
      idx = i;
    }
  }

  return idx;

}               /*-----  findPoint  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setPoint  of  ColorChannel
 |
 |  Description :
 |
 |   This function replaces the point with the given index and then sorts
 |   the points by the  x-coordinate or adds the point at  the end if the
 |   index is outside the range.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::setPoint ( int & index, QPointF point )
{

  limitPoint (point);     // limit to allowed range

  if ( ( index >= 0 ) and ( index < points.count () ) )
  {
    points.replace ( index, point );

    std::sort ( points.begin (), points.end (), ColorChannel::ascendingX );

    index = findPoint (point);
  }
  else
    index = addPoint (point);

}               /*-----  setPoint  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setPoint  of  ColorChannel
 |
 |  Description :
 |
 |   This function replaces the given point  in the list when it is found
 |   in the list and sorts the points by the x-coordinate. If not then it
 |   is just added to the list.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::setPoint ( QPointF point )
{
  int index;

  limitPoint (point);     // limit to allowed range

  index = findPoint (point);

  if ( index >= 0 )
  {
    points.replace ( index, point );

    std::sort ( points.begin (), points.end (), ColorChannel::ascendingX );
  }
  else
    addPoint (point);

  checkLinear ();

}               /*-----  setPoint  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : limitPoint  of  ColorChannel
 |
 |  Description :
 |
 |   This function limits and rounds the x and y value of the given point
 |   to the range 0.0  upto 1.0. Rounding means if a  points is close the
 |   edges 0 and 1 then it is  clipped to it.  This avoids a confusion if
 |   the curve type is linear of a spline.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::limitPoint ( QPointF & point )
{
  float x, y;

  x = point.x ();
  y = point.y ();

  if ( fabs ( x - 0.0 ) < cPntRound )
    x = 0;

  if ( fabs ( x - 1.0 ) < cPntRound )
    x = 1.0;

  if ( fabs ( y - 0.0 ) < cPntRound )
    y = 0;

  if ( fabs ( y - 1.0 ) < cPntRound )
    y = 1.0;

  point.setX (x);
  point.setY (y);

}               /*-----  limitPoint  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  evalSpline  of  ColorChannel
 |
 |  Description :
 |
 |   This function creates  a spline series from the levels  curve if the
 |   type is not linear.  It always has a point at the beginnning (0) and
 |   end (100) on the x-coordinate.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::evalSpline ()
{
  QPointF  point;
  int      cnt;
  float    y1;
  float    y;

  cnt       = points.count ();
  splineBeg = 0;
  splineEnd = cnt - 1;

  point = points.first ();

  y = point.y ();

  // evaluate if the curve is linear at the beginning
  if (lineBeg)
  {
    for ( int i = 1; i < cnt; i++ )
    {
      y1 = points [i].y ();

      if ( not isEqual ( y1, y ) )
      {
        splineBeg = i - 1;   // store previous point index for spline
        break;
      }
    }
  }

  // evaluate if the curve is linear at the end
  if (lineEnd)
  {
    point = points.last ();

    for ( int i = cnt - 2; i > splineBeg; i-- )
    {
      y1 = points [i].y ();

      if ( not isEqual ( y1, y ) )
      {
        splineEnd = i + 1;
        break;
      }
    }
  }

}               /*-----  evalSpline  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : evalPoints  of  ColorChannel
 |
 |  Description :
 |
 |   This  function evaluates  it the  points  build a  linear or  spline
 |   curve. In case of a sigmoid it creates a lookup table with the shape
 |   of a sigmoid.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::evalPoints ( )
{
  QPointF point;
  float   x, y;
  int     idx;

  if ( curveType == cTypeSigmoid )
  {
    createSigmoid ();
    return;
  }

  lineBeg   = false;
  lineEnd   = false;
  splines   = false;

  splineBeg = -1;
  splineEnd = -1;

  point = points.first ();

  if ( point.x () > 0 )   // x of first point not on 0
    lineBeg = true;
  else
  {
    if ( isEqual ( points [1].y (), point.y () ) )   // check 2nd point
      lineBeg = true;
  }

  point = points.last ();

  if ( point.x () < 1.0 )   // x of last point not on 0
    lineEnd = true;
  else
  {
    idx = points.count () - 2;

    if ( isEqual ( points [idx].y (), point.y () ) )  // check point before
      lineEnd = true;
  }

  for ( int i = 1; i < points.count () - 1; i++ )
  {
    point = points [i];

    x = point.x ();
    y = point.y ();

    if ( ( point.y () > 0 ) and ( point.y () < 1.0 ) )
    {
      splines = true;
    }
  }

  if (splines)
  {
    curveType = cTypeSpline;

    evalSpline ();
  }
  else
    curveType = cTypeLinear;

}               /*-----  evalPoints  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : isEqual  of  ColorChannel
 |
 |  Description :
 |
 |   This function checks if the two  given values are identical within a
 |   certain tolerance.
 |
 +-------------------------------------------------------------------------*/

bool ColorChannel::isEqual ( float y1, float y2 )
{

  if ( fabs ( y1 - y2 ) < cPntCompare )
    return true;
  else
    return false;

}               /*-----  isEqual  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : resetPoints  of  ColorChannel
 |
 |  Description :
 |
 |   This function  initializes the  curve with type  linear and  the two
 |   points black and white.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::resetPoints ()
{

  lineBeg   = false;
  lineEnd   = false;
  splines   = false;

  lowInput  = 0.0;
  highInput = 1.0;

  points.clear ();

  points.append ( QPointF ( 0, 0 ) );
  points.append ( QPointF ( 1, 1 ) );

}               /*-----  resetPoints  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : getChannelInfo  of  ColorChannel
 |
 |  Description :
 |
 |   This function  copies the internal  member variables to  the channel
 |   info.   The caller  is responsible  to update  the information  when
 |   needed.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::getChannelInfo ( ChannelInfo & info )
{

  info.points    = points;
  info.sigmoid   = sigmoid;
  info.curveType = curveType;

  info.lowInput  = lowInput;
  info.highInput = highInput;

  info.blackPnt  = 0.0;
  info.whitePnt  = 1.0;

  if ( info.curveType == cTypeLinear )
  {
    info.blackPnt = points [0].x ();
    info.whitePnt = points [1].x ();
  }

  info.lineBeg   = lineBeg;
  info.lineEnd   = lineEnd;
  info.splines   = splines;
  info.sigma     = sigma;
  info.offset    = offset;
  info.splineBeg = splineBeg;
  info.splineEnd = splineEnd;
  info.numBins   = numBins;

}               /*-----  getChannelInfo  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setCurvePoints  of  ColorChannel
 |
 |  Description :
 |
 |   This  function  sets the  line  or  spline  points to  the  'points'
 |   depending on the curve type.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::setCurvePoints ()
{

  // store current points
  if ( curveType == cTypeLinear )
    linePts = points;

  if ( curveType == cTypeSpline )
    splinePts = points;

}               /*-----  setCurvePoints  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setCurveType  of  ColorChannel
 |
 |  Description :
 |
 |   This function sets the curve type to the given type. Before that the
 |   points of  the original type  are stored and  the points of  the new
 |   type are restored.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::setCurveType ( QString type )
{

  if ( curveType == type )
    return;

  setCurvePoints ();

  curveType = type;

  // restore current points
  if ( ( type == cTypeLinear ) and ( linePts.count () == 2 ) )
    points = linePts;

  if ( ( type == cTypeSpline ) and ( splinePts.count () > 2 ) )
    points = splinePts;

}               /*-----  setCurveType  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setBlackPnt  of  ColorChannel
 |
 |  Description :
 |
 |   This function sets  the black point to the given  value. There is no
 |   need to check if the x-value is  at the corner, if the curve type is
 |   linear then there are only two points.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::setBlackPnt ( float x )
{

  if ( points.count () > 2 )   // set back to linear
    resetPoints ();

  points [0].setX (x);

  linePts = points;

}               /*-----  setBlackPnt  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setWhitePnt  of  ColorChannel
 |
 |  Description :
 |
 |   This function sets  the white point to the given  value. There is no
 |   need to check if the x-value is  at the corner, if the curve type is
 |   linear then there are only two points.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::setWhitePnt ( float x )
{

  if ( points.count () > 2 )   // set back to linear
    resetPoints ();

  points [1].setX (x);

  linePts = points;

}               /*-----  setWhitePnt  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setLowHigh  of  ColorChannel
 |
 |  Description :
 |
 |   This function sets the low and high  input value. This is used for a
 |   auto input level adjustment.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::setLowHigh ( float low, float high )
{

  resetPoints ();

  lowInput  = low;
  highInput = high;

  points [0].setX (low);
  points [1].setX (high);

  linePts = points;

}               /*-----  setLowHigh  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setChannelMat  of  ColorChannel
 |
 |  Description :
 |
 |   This function takes  the given channel of an image  and makes a copy
 |   of it.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::setChannelMat ( cv::Mat & mat )
{

  cvChannel = mat.clone ();   // make a copy

}               /*-----  setChannelMat  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  sigmoidToSpline  of  ColorChannel
 |
 |  Description :
 |
 |   This function converts the sigmoid to spline points.
 |
 +-------------------------------------------------------------------------*/

void ColorChannel::sigmoidToSpline ()
{
  QPointF point;
  int     numPnts;

  if ( curveType != cTypeSigmoid )
    createSigmoid ();

  points.clear ();

  numPnts = qRound ( (double) numBins / cSplinePoints );

  for ( int i = 1; i < sigmoid.count (); i++ )
  {
    if ( ( i mod numPnts ) == 0 )
    {
      point = sigmoid [i];
      points.append (point);
    }
  }

  curveType = cTypeSpline;

  evalPoints ();

  createSplines ();

}               /*-----  sigmoidToSpline  -----*/

