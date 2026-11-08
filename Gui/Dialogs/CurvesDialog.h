/*-------------------------------------------------------------------------
 |
 |  Include-File :  CurvesDialog.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   CurvesDialog.
 |
 +-------------------------------------------------------------------------*/

#ifndef CurvesDialog_h
#define CurvesDialog_h

#include "../Widgets/HistogramWidget.h"
#include "../Widgets/CurvesWidget.h"
#include "../Tools/ImageMat.h"
#include "../Config/AppCurves.h"

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class  AppConfig;

/*-------------------------------------------------------------------------
 |
 |  Class :  CurvesDialog
 |
 |  Description :
 |
 |   This class  is derived  from QDialog  and creates  a histogram  of a
 |   grayscale or colored image and allows  to modify a lookup table in a
 |   graphical view by moving points of a curve.
 |
 +-------------------------------------------------------------------------*/

class CurvesDialog : public QDialog
{

  Q_OBJECT

  public:
    CurvesDialog ( QWidget * parent );
    virtual ~CurvesDialog ();

    void      setImage        ( cv::Mat image );
    cv::Mat   getImage        ()   { return imgOut; }
    void      updateHistogram ( cv::Mat image );

  signals:
    void      applyCurves     ();

  public slots:
    void      evalMousePos    ( QPointF pos );

  protected slots:
    void      onCurveChanged  ();
    void      onCheckCurve    ();
    void      onLoadCurve     ();
    void      onSaveCurve     ();

    void      onAutoLevel     ();
    void      onBlackPnt      ();
    void      onWhitePnt      ();
    void      onToSpline      ();
    void      onCancel        ();
    void      onOk            ();
    void      onReset         ();
    void      onSigmoid       ();
    void      onValueChanged  ( double value );

  protected:
    void      blockControls   ( bool block );
    void      closeEvent      ( QCloseEvent * event );
    void      createObjects   ();
    void      createLayout    ();
    void      connectSlots    ();
    void      initCurves      ();
    void      initDialog      ();
    void      fromGui         ();
    void      toGui           ();
    void      evalAutoLevel   ( ColorChannel * level );
    cv::Vec3b getColorAtPoint ( cv::Point point );
    void      setCurveType    ( QString type );

    protected:    // overwritten from widget
    void      enterEvent      ( QEnterEvent * event );
    void      leaveEvent      ( QEvent      * event );
    void      focusInEvent    ( QFocusEvent * event );
    void      focusOutEvent   ( QFocusEvent * event );

  protected:
    HistogramWidget * histWidget;
    CurvesWidget    * curveWidget;

    AppCurves       * appCurves;
    AppConfig       * appConfig;
    QComboBox       * cbCurves;
    QComboBox       * cbTypes;

    ColorCurves     * curves;
    ColorChannel    * channel;
    ColorChannel      chanBackup;
    ColorCurves       curveBackup;

    cv::Mat           image;
    cv::Mat           imgOut;
    ImageMat          imageMat;

    QPushButton     * btnCancel;
    QPushButton     * btnOk;
    QPushButton     * btnBlackPnt;
    QPushButton     * btnWhitePnt;
    QPushButton     * btnAutoLevel;
    QPushButton     * btnSigmoid;
    QPushButton     * btnSigConv;
    QPushButton     * btnReset;
    QPushButton     * btnSaveAs;

    QCursor         * blackCursor;
    QCursor         * whiteCursor;
    QDoubleSpinBox  * txtBlackPnt;
    QDoubleSpinBox  * txtGammaVal;
    QDoubleSpinBox  * txtWhitePnt;

    QLineEdit       * txtRadius;
    QLineEdit       * txtSigmaVal;
    QLineEdit       * txtOffsetVal;

    float             blackPnt;
    float             whitePnt;
    float             sigmaVal;    // sigma value for sigmoid
    float             offsetVal;   // offset value for sigmoid

    int               colPicker;
    bool              isGray;
    bool              hasFocus;
    QString           text;

};

#endif
