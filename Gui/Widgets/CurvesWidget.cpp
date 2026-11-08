/*-------------------------------------------------------------------------
 |
 |  File Name   :  CurvesWidget.cpp
 |
 |  Description :
 |
 |   This module contains the class  'CurvesWidget' which is derived from
 |   QChartView and  is a standalone  widget that can display  charts, it
 |   does not  require a  QGraphicsScene object to  work.  It  contains a
 |   chart to display the curves from  the curves dialog.  The curves can
 |   be line segments or spline curves between points.
 |
 |  Developer : Oskar Leirich                Creation date : 02.Mar.2025
 |  Modified  : Oskar Leirich                Last changes  : 16.Aug.2026
 |
 |
 |  This unit contains following member functions :
 |   of CurvesWidget:
 |    CurvesWidget              ~CurvesWidget             addLineSeries
 |    addSplineSeries           calcPoint                 createChart
 |    createLines               createMarker              createSeries
 |    createSigmoid             createSplines             init
 |    makeLineSeries            makeSplineSeries          mouseMoveEvent
 |    mousePressEvent           mouseReleaseEvent         scalePoints
 |    setChannel                  updateChart
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "CurvesWidget.h"
#include "../ToolsWidget.h"
#include "../../Main/MainWnd.h"
#include "../../Tools/Histogram.h"

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const  float  cMarkerWidth = 2;
const  int    cMarkerSize  = 3;
const  float  cPenWidth    = 2.0;
const  int    cColorPoints = 7;


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  CurvesWidget
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 |  Input parameters :
 |   parent          :  Pointer to the parent window
 |
 +-------------------------------------------------------------------------*/

CurvesWidget::CurvesWidget ( QWidget * parent ) : QChartView (parent)
{

  setRenderHint ( QPainter::Antialiasing );

  init ();

}               /*----- CurvesWidget -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  CurvesWidget
 |
 |  Description :
 |
 |   The destructor deletes all  dynamically created objects if there are
 |   any.
 |
 +-------------------------------------------------------------------------*/

CurvesWidget::~CurvesWidget ()
{

  if (curveChart)
    delete curveChart;

}               /*-----  ~CurvesWidget  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : init  of  CurvesWidget
 |
 |  Description :
 |
 |   This function is called from the constructor and initializes the bar
 |   chart and series.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::init ()
{
  ToolsWidget * toolsWnd;

  toolsWnd = mainWnd->getToolsWidget ();

  lineSeries   = nullptr;
  splineSeries = nullptr;
  moving       = false;

  curveChart   = new QChart ();

  createChart ();

  connect ( this,     & CurvesWidget::applyCurves,
            toolsWnd, & ToolsWidget::onApplyCurves );

}               /*-----  init  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : reset  of  CurvesWidget
 |
 |  Description :
 |
 |   This  function is  called when  the 'Reset'  button is  pressed.  It
 |   resets the  points in the level  curve and updates the  chart in the
 |   curveWidget.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::reset ()
{

  updateChart ();

}               /*-----  reset  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : addLineSeries  of  CurvesWidget
 |
 |  Description :
 |
 |   This function  adds the lines series  to the chart and  attaches the
 |   axes to  the line series.  It then makes  the points visible  or not
 |   depending on the parameter showPoints.
 |
 |   The documentation is  a litte bit confusing but it  says if the axes
 |   are attached  to the series  before the  series are attached  to the
 |   chart, then the axes have ranges  that exactly fit the series to the
 |   plot area of the chart.
 |
 |   This can be  confusing if the same chart also  displays other series
 |   that have properly attached axes, so  always make sure that the axes
 |   are attached  to the  series after  the series  are attached  to the
 |   chart.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::addLineSeries ( bool showPoints )
{

  lineSeries->setPointsVisible (showPoints);

  curveChart->addSeries (lineSeries);

  lineSeries->attachAxis (axisX);
  lineSeries->attachAxis (axisY);

}               /*-----  addLineSeries  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : addSplineSeries  of  CurvesWidget
 |
 |  Description :
 |
 |   This function adds the splines series  to the chart and attaches the
 |   axes to the spline series.  It  then makes the points visible or not
 |   depending on the parameter showPoints.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::addSplineSeries ( bool showPoints )
{

  splineSeries->setPointsVisible (showPoints);

  curveChart->addSeries (splineSeries);

  splineSeries->attachAxis (axisX);
  splineSeries->attachAxis (axisY);

}               /*-----  addSplineSeries  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createMarker  of  CurvesWidget
 |
 |  Description :
 |
 |   This function  creates a  marker point as  rectangle with  the given
 |   size and color and returns it a QImage.
 |
 |  Input parameters :
 |   size            : Size (width and height) of the rectangle
 |   color           : color for the rectangle
 |
 |  Output parameter :
 |   (QImage)        : Image which can be used for a light marker
 |
 +-------------------------------------------------------------------------*/

QImage CurvesWidget::createMarker ( float size, QColor color )
{
  QPainter painter;

  QImage image ( size, size, QImage::Format_RGB32 );

  painter.begin ( & image );
  painter.fillRect ( 0, 0, size, size, color );
  painter.end ();

  return image;

}               /*-----  createMarker  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLines  of  CurvesWidget
 |
 |  Description :
 |
 |   This function  creates a line series  from the curve if  the type is
 |   linear.  It always  has a point at the beginnning  (0) and end (100)
 |   on the x-coordinate.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::createLines ()
{
  QPointF point;

  makeLineSeries (color);

  scalePoints ( channelInfo.points );

  point = points.first ();

  if ( point.x () > 0 )
    lineSeries->append ( 0, point.y () );

  for ( int i = 0; i < points.count (); i++ )
  {
    point = points [i];
    lineSeries->append (point);
  }

  point = points.last ();

  if ( point.x () < 100.0 )
    lineSeries->append ( 100.0, point.y () );

  addLineSeries ();

}               /*-----  createLines  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createSamples  of  CurvesWidget
 |
 |  Description :
 |
 |   This function creates a line series from the given points.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::createSamples ( L_PointF samples )
{
  QPointF point;

  makeLineSeries (color);

  lineSeries->setPointsVisible (false);
  lineSeries->setMarkerSize (0.3);

  scalePoints (samples);   // scales to points

  for ( int i = 0; i < points.count (); i++ )
  {
    point = points [i];
    lineSeries->append (point);
  }

  addLineSeries ();

}               /*-----  createSamples  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createSigmoid  of  CurvesWidget
 |
 |  Description :
 |
 |   This function  creates a series of  points from the curve  which has
 |   the shape of a sigmoid. The sample points for this sigmoid are taken
 |   from the curve.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::createSigmoid ()
{

  makeSplineSeries (color);

  scalePoints ( channelInfo.sigmoid );

  for ( int i = 0; i < points.count (); i++ )
  {
    splineSeries->append ( points [i] );
  }

  addSplineSeries (false);

}               /*-----  createSigmoid  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createSplines  of  CurvesWidget
 |
 |  Description :
 |
 |   This function creates a spline series  from the curve if the type is
 |   not linear.   It always has  a point at  the beginnning (0)  and end
 |   (100) on the x-coordinate.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::createSplines ()
{
  QPointF point;
  int     cnt;
  int     splineBeg;
  int     splineEnd;
  float   x, y;

  scalePoints ( channelInfo.points );

  makeSplineSeries (color);

  cnt       = points.count ();
  splineBeg = channelInfo.splineBeg;
  splineEnd = channelInfo.splineEnd;

  point = points.first ();

  x = point.x ();
  y = point.y ();

  // evaluate if the curve is linear at the beginning
  if ( channelInfo.lineBeg )
  {
    makeLineSeries (color);

    if ( x > 0 )
      lineSeries->append ( 0, y );

    for ( int i = 0; i <= splineBeg; i++ )
    {
      x = points [i].x ();
      y = points [i].y ();

      lineSeries->append ( x, y );
    }

    addLineSeries ();
  }

  // evaluate if the curve is linear at the end
  if ( channelInfo.lineEnd )
  {
    makeLineSeries (color);

    for ( int i = splineEnd; i < cnt; i++ )
    {
      x = points [i].x ();
      y = points [i].y ();

      lineSeries->append ( x, y );
    }

    lineSeries->append ( 100, y );
  }

  // create spline points in the middle
  for ( int i = splineBeg; i <= splineEnd; i++ )
  {
    x = points [i].x ();
    y = points [i].y ();

    splineSeries->append ( x, y );
  }

  addSplineSeries ();    // add splines and show points

  if ( channelInfo.lineEnd )   // we have linear end ?
    addLineSeries ();

}               /*-----  createSplines  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createSeries  of  CurvesWidget
 |
 |  Description :
 |
 |   This function evaluates  the curveType and depending on  the type it
 |   then creates  line, spline or a  mixture of line and  spline series.
 |   If  the type  is sigmoid,  then the  points are  ignored and  a line
 |   series is created, but with no marker points.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::createSeries ()
{
  L_PointF splinePts;
  QString  curveType;

  channel->evalPoints ();   // update the points information

  curveType = channel->getCurveType ();

  channel->getChannelInfo (channelInfo);

  if ( curveType == cTypeSigmoid )
  {
    createSigmoid ();
    return;
  }

  if ( curveType == cTypeLinear )
  {
    createLines ();
    return;
  }

  if ( curveType == cTypeSpline )
  {
    createSplines ();
    return;
  }

}               /*-----  createSeries  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : createChart  of  CurvesWidget
 |
 |  Description :
 |
 |   This function only creates the curve  chart with the axes, no series
 |   are added  the the chart  here. This is  done in updateChart  when a
 |   update is made the to channel curve.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::createChart ()
{

  curveChart->removeAllSeries ();

  axisX = new QValueAxis  ();
  axisX->setRange         ( 0, 100 );
  axisX->setTickCount     (10);
  axisX->applyNiceNumbers ();
  axisX->setTitleText     ( "x" );

  axisY = new QValueAxis  ();
  axisY->setRange         ( 0, 100 );
  axisY->setTickCount     (10);
  axisY->applyNiceNumbers ();
  axisY->setTitleText     ( "y" );

  curveChart->addAxis ( axisX, Qt::AlignBottom );
  curveChart->addAxis ( axisY, Qt::AlignLeft );

  curveChart->legend ()->setVisible (false);

  // sets the curve chart of this QChartView.
  setChart (curveChart);

}               /*-----  createChart  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : calcPoint  of  CurvesWidget
 |
 |  Description :
 |
 |   This function calculates the point  coordinates from the given mouse
 |   position in the widget.
 |
 +-------------------------------------------------------------------------*/

QPointF CurvesWidget::calcPoint ( QPointF mousePos )
{
  QPointF point;
  QRectF  rect;
  float   w, h;
  float   x, y;

  // the rectangle within the chart is drawn
  rect = curveChart->plotArea ();

  w = rect.width  ();
  h = rect.height ();

  x = mousePos.x () - rect.x ();
  y = h - mousePos.y () + rect.y ();

  x = x / w;
  y = y / h;

  x = qBound ( 0.0, x, 1.0 );
  y = qBound ( 0.0, y, 1.0 );

  point.setX (x);
  point.setY (y);

  return point;

}               /*-----  calcPoint  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : makeLineSeries  of  CurvesWidget
 |
 |  Description :
 |
 |   This function creates  a lines series and sets the  pen and size for
 |   the curve marker. It then makes the points visible.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::makeLineSeries ( QColor col )
{
  QPen pen;

  pen.setColor  (col);
  pen.setWidthF (cPenWidth);

  lineSeries = new QLineSeries ();

  lineSeries->setMarkerSize (cMarkerSize);
  lineSeries->setPointsVisible (true);
  lineSeries->setPen (pen);

}               /*-----  makeLineSeries  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : makeSplineSeries  of  CurvesWidget
 |
 |  Description :
 |
 |   This function creates a splines series and sets the pen and size for
 |   the curve marker. It then makes the points visible.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::makeSplineSeries ( QColor col )
{
  QPen pen;

  pen.setColor  (col);
  pen.setWidthF (cPenWidth);

  splineSeries = new QSplineSeries ();

  splineSeries->setMarkerSize (cMarkerSize);
  splineSeries->setPointsVisible (true);
  splineSeries->setPen (pen);

}               /*-----  makeSplineSeries  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  mouseDoubleClickEvent  of  CurvesWidget
 |
 |  Description :
 |
 |   This function is reimplemented to  receive mouse double click events
 |   for the item. It deletes the point under the mouse if there is one.
 |
 |  Input parameters :
 |   mouseEvent      : pointer to a QMouseEvent
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::mouseDoubleClickEvent ( QMouseEvent * event )
{
  QPointF point;

  curMousePos = event->position ();

  point = calcPoint (curMousePos);

  channel->delPoint (point);

  moving = false;

  updateChart ();

}               /*-----  mouseDoubleClickEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  mousePressEvent  of  CurvesWidget
 |
 |  Description :
 |
 |   This function is the event handler for the mouse press event.
 |
 |  Input parameters :
 |   event           : Graphics scene mouse event
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::mousePressEvent ( QMouseEvent * event )
{

  channel->getChannelInfo (channelInfo);

  curveBefore = channelInfo.curveType;

  if ( curveBefore == cTypeSigmoid )
    return;

  begMousePos = event->position ();

  curMousePos = begMousePos;
  endMousePos = begMousePos;

  moving      = true;

  activePoint = calcPoint (curMousePos);

  channel->setPoint (activePoint);

  updateChart ();

  event->accept ();

}               /*-----  mousePressEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  mouseMoveEvent  of  CurvesWidget
 |
 |  Description :
 |
 |   This function is the event handler for the mouse move event.
 |
 |  Input parameters :
 |   event           : Graphics scene mouse event
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::mouseMoveEvent ( QMouseEvent * event )
{

  if ( not moving )
    return;

  begMousePos = event->position ();

  channel->delPoint (activePoint);

  activePoint = calcPoint (curMousePos);

  channel->setPoint (activePoint);

  points = channelInfo.points;

  updateChart ();

  event->accept ();

}               /*-----  mouseMoveEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  mouseReleaseEvent  of  CurvesWidget
 |
 |  Description :
 |
 |   This function is the event handler for the mouse release event.
 |
 |  Input parameters :
 |   event           : Graphics scene mouse event
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::mouseReleaseEvent ( QMouseEvent * event )
{
  QPointF point;

  if ( not moving )
    return;

  moving = false;

  endMousePos = event->position ();
  curMousePos = endMousePos;

  channel->delPoint (activePoint);

  activePoint = calcPoint (curMousePos);

  channel->setPoint (activePoint);

  updateChart ();   // call get channel info

  points = channelInfo.points;

  if ( channelInfo.curveType != curveBefore )
    emit curveChanged ();

  event->accept ();

}               /*-----  mouseReleaseEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : updateChart  of  CurvesWidget
 |
 |  Description :
 |
 |   This  function is  called  when an  update is  made  the to  channel
 |   curve. Then the  line or spline series are added  the the chart. The
 |   axes are  already created  in create  chart.  The  parameter 'apply'
 |   tells if also the signal 'applyCurves'  should be sent.  This is the
 |   default behavior only  when it is called from  'AutoLevel' then this
 |   is already done before.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::updateChart ( bool apply )
{

  curveChart->removeAllSeries ();

  createSeries ();

  if (apply)
    emit applyCurves ();    // inform tools widget to update the image view

}               /*-----  updateChart  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setChannel  of  CurvesWidget
 |
 |  Description :
 |
 |   This function  is called  from the  curves dialog  when a  new curve
 |   should be drawn, the curve selection has changed (for colors) or the
 |   points and values have been updated.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::setChannel ( ColorChannel * channel, QColor color )
{

  this->color   = color;
  this->channel = channel;

  updateChart ();

}               /*-----  setChannel  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  scalePoints  of  CurvesWidget
 |
 |  Description :
 |
 |   This function  is called  from the  curves dialog  when a  new curve
 |   should be drawn, the curve selection has changed (for colors) or the
 |   points and values have been updated.
 |
 +-------------------------------------------------------------------------*/

void CurvesWidget::scalePoints ( L_PointF orgPoints )
{

  points = orgPoints;

  for ( int i = 0; i < points.count (); i++ )
  {
    points [i] = 100.0 * orgPoints [i];
  }

}               /*-----  scalePoints  -----*/

