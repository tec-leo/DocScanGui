/*-------------------------------------------------------------------------
 |
 |  Include-File :  CurvesWidget.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   CurvesWidget.
 |
 +-------------------------------------------------------------------------*/

#ifndef CurvesWidget_h
#define CurvesWidget_h

#include <QLineSeries>
#include <QSplineSeries>
#include <QValueAxis>
#include <QChart>
#include <QChartView>

#include "../Tools/ColorChannel.h"

/*-------------------------------------------------------------------------
 |
 |  Class :  CurvesWidget
 |
 |  Description :
 |
 |   This class  is derived  from QChartView and  is a  standalone widget
 |   that can display charts, it does not require a QGraphicsScene object
 |   to work.  It contains a chart  to display the curves from the levels
 |   dialog.  The  curves can be  line segments or spline  curves between
 |   points.
 |
 +-------------------------------------------------------------------------*/

class CurvesWidget : public QChartView
{

  friend class CurvesDialog;

  Q_OBJECT

  public:
    CurvesWidget ( QWidget * parent );
    virtual ~CurvesWidget ();

    void    reset                 ();
    void    init                  ();
    void    setChannel            ( ColorChannel * channel, QColor color );
    void    updateChart           ( bool apply = true );

  signals:
    void    curveChanged          ();
    void    applyCurves           ();

  protected:
    QImage  createMarker          ( float size, QColor color );
    void    createChart           ();
    void    createLines           ();
    void    createSplines         ();
    void    createSigmoid         ();
    void    createSeries          ();
    void    createSamples         ( L_PointF splinePnts );

    QPointF calcPoint             ( QPointF mousePos );
    void    addLineSeries         ( bool showPoints = true );
    void    addSplineSeries       ( bool showPoints = true );
    void    makeLineSeries        ( QColor col );
    void    makeSplineSeries      ( QColor col );
    void    mouseDoubleClickEvent ( QMouseEvent * event );
    void    mousePressEvent       ( QMouseEvent * event );
    void    mouseMoveEvent        ( QMouseEvent * event );
    void    mouseReleaseEvent     ( QMouseEvent * event );
    void    scalePoints           ( L_PointF orgPoints );

  protected:
    QChart        * curveChart;
    QValueAxis    * axisX;
    QValueAxis    * axisY;
    QSplineSeries * splineSeries;
    QLineSeries   * lineSeries;
    ColorChannel  * channel;
    QPointF       begMousePos;
    QPointF       curMousePos;
    QPointF       endMousePos;
    QPointF       activePoint;
    ChannelInfo   channelInfo;
    QString       curveBefore;
    QColor        color;     // color for the curve
    QString       text;
    L_PointF      points;    // scaled points of the current curve
    bool          moving;

};

#endif
