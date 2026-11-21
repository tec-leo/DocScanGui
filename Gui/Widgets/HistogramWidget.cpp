/*-------------------------------------------------------------------------
 |
 |  File Name   :  HistogramWidget.cpp
 |
 |  Description :
 |
 |   This module contains the  class 'HistogramWidget' which implements a
 |   widget that can display charts, in  this case a bar chart to display
 |   the histogram of the Open CV matrix which contains an image.
 |
 |  Developer : Oskar Leirich                Creation date : 07.Jan.2025
 |  Modified  : Oskar Leirich                Last changes  : 16.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of HistogramWidget:
 |    HistogramWidget           ~HistogramWidget          buildBarSet
 |    create                    createChart               init
 |    update
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "HistogramWidget.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  HistogramWidget
 |
 |  Description :
 |
 |   The constructor  calls 'init'  to initialize  some variables  and it
 |   sets the render hint to Antialiasing.
 |
 |  Input parameters :
 |   parent          :  Pointer to the parent widget
 |
 +-------------------------------------------------------------------------*/

HistogramWidget::HistogramWidget ( QWidget * parent ) : QChartView (parent)
{

  setRenderHint ( QPainter::Antialiasing );

  init ();

}               /*----- HistogramWidget -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  HistogramWidget
 |
 |  Description :
 |
 |   The destructor deletes the bar chart if it exists.
 |
 +-------------------------------------------------------------------------*/

HistogramWidget::~HistogramWidget ()
{

  if (barChart)
    delete barChart;

}               /*-----  ~HistogramWidget  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : init  of  HistogramWidget
 |
 |  Description :
 |
 |   This function is called from the constructor and initializes the bar
 |   chart and series.
 |
 +-------------------------------------------------------------------------*/

void HistogramWidget::init ()
{

  barSeries = nullptr;
  barChart  = nullptr;

  barSeries = new QBarSeries ();

}               /*-----  init  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  buildBarSet  of  HistogramWidget
 |
 |  Description :
 |
 |   This function  adds the  values of the  calculated histogram  to the
 |   given BarSet.  The values are normalized  to the number of pixels in
 |   the image.
 |
 +-------------------------------------------------------------------------*/

void HistogramWidget::buildBarSet ( QBarSet * barSet )
{
  float   histY;
  L_Float histValues;
  int     pixels;

  histValues = histogram.getValues ();
  pixels     = histogram.getPixels ();

  for ( int i = 0; i < histValues.count (); i++ )
  {
    // normalize to 100 percent
    histY  = 100.0 * histValues [i] / pixels;

    barSet->append (histY);
  }

}               /*-----  buildBarSet  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  create  of  HistogramWidget
 |
 |  Description :
 |
 |   This function  creates a histogram  of a grayscale or  colored image
 |   and shows  it as  a bar chart.   The ownership of  the bar  chart is
 |   passed to the chart view and  the ownership of the previous chart is
 |   released.
 |
 +-------------------------------------------------------------------------*/

void HistogramWidget::create ( cv::Mat image )
{

  if (barChart)
    delete barChart;

  barChart = new QChart ();

  histogram.create (image);

  createChart   ();

  // sets the bar chart of this QChartView.
  setChart (barChart);

}               /*-----  create  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : createChart  of  HistogramWidget
 |
 |  Description :
 |
 |   This function creates  the bar chart from the calculated  cv mat and
 |   adds the values  to the series with every value  as own category. It
 |   creates a set of bar sets and  adds it to the bar series which takes
 |   the ownership of the bar sets.
 |
 +-------------------------------------------------------------------------*/

void HistogramWidget::createChart ()
{
  QValueAxis * axisX;
  QValueAxis * axisY;
  QBarSet    * barSet;

  barSeries = new QBarSeries ();
  barChart->removeAllSeries ();

  barSet = new QBarSet ( "1" );   // first bar set
  barSet->setPen   ( QPen ( Color::blue ) );
  barSet->setColor ( Color::lightblue );

  buildBarSet (barSet);

  barSeries->append (barSet);  // series takes the ownership of the barSet
  barChart->addSeries (barSeries);
  barChart->setTitle ("Histogram");
  barChart->setAnimationOptions (QChart::SeriesAnimations);

  axisX = new QValueAxis  ();
  axisX->setRange         ( 1, 100 );
  axisX->setTickCount     (10);
  axisX->applyNiceNumbers ();

  axisY = new QValueAxis  ();
  axisY->setRange         ( 1, 100.0 );
  axisY->setTickCount     (10);
  axisY->applyNiceNumbers ();

  barSeries->attachAxis   (axisX);
  barSeries->attachAxis   (axisY);

  barChart->addAxis ( axisX, Qt::AlignBottom );
  barChart->addAxis ( axisY, Qt::AlignLeft );

  barChart->legend ()->setVisible (false);

}               /*-----  createChart  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  update  of  HistogramWidget
 |
 |  Description :
 |
 |   This function updates the histogram  with a new image.
 |
 +-------------------------------------------------------------------------*/

void HistogramWidget::update ( cv::Mat image )
{
  QBarSet * barSet;

  barChart->removeAllSeries ();

  barSet = new QBarSet ( "1" );   // first bar set
  barSet->setPen   ( QPen ( Color::blue ) );
  barSet->setColor ( Color::lightblue );

  histogram.create (image);

  buildBarSet (barSet);

  barSeries = new QBarSeries ();

  barSeries->append (barSet);  // series takes the ownership of the barSet
  barChart->addSeries (barSeries);

}               /*-----  update  -----*/

