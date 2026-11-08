/*-------------------------------------------------------------------------
 |
 |  Include-File :  HistogramWidget.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   HistogramWidget.
 |
 +-------------------------------------------------------------------------*/

#ifndef HistogramWidget_h
#define HistogramWidget_h

#include <QBarCategoryAxis>
#include <QBarSeries>
#include <QValueAxis>
#include <QBarSet>
#include <QChart>
#include <QChartView>

#include "../../Tools/Histogram.h"

/*-------------------------------------------------------------------------
 |
 |  Class :  HistogramWidget
 |
 |  Description :
 |
 |   This class  is derived  from QChartView and  is a  standalone widget
 |   that can display charts, it does not require a QGraphicsScene object
 |   to work.   It contains a bar  chart to display the  histogram of the
 |   given
 |
 +-------------------------------------------------------------------------*/

class HistogramWidget : public QChartView
{

  Q_OBJECT

  public:
    HistogramWidget ( QWidget * parent );
    virtual ~HistogramWidget ();

    void    create         ( cv::Mat image );
    void    update         ( cv::Mat image );

  protected:
    void    buildBarSet    ( QBarSet * barSet );
    void    createChart    ();
    void    calcHistogram  ();
    void    init           ();

  protected:
    Histogram    histogram;
    QChart     * barChart;
    QBarSeries * barSeries;

};

#endif
