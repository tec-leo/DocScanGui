 /*-------------------------------------------------------------------------
 |
 |  File Name   :  CurvesDialog.cpp
 |
 |  Description :
 |
 |   This module implements a dialog class which creates a histogram of a
 |   grayscale or colored image and allows  to modify a lookup table in a
 |   graphical view by moving points of a curve.
 |
 |  Developer : Oskar Leirich                Creation date : 07.Jan.2025
 |  Modified  : Oskar Leirich                Last changes  : 14.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of CurvesDialog:
 |    CurvesDialog              ~CurvesDialog             blockControls
 |    closeEvent                connectSlots              createLayout
 |    createObjects             enterEvent                evalAutoLevel
 |    evalMousePos              focusInEvent              focusOutEvent
 |    fromGui                   getColorAtPoint           initCurves
 |    initDialog                leaveEvent                onAutoLevel
 |    onBlackPnt                onCancel                  onCheckCurve
 |    onCurveChanged            onLoadCurve               onOk
 |    onReset                   onSaveCurve               onSigmoid
 |    onToSpline                onValueChanged            onWhitePnt
 |    setCurveType              setImage                  toGui
 |    updateHistogram
 |
 +-------------------------------------------------------------------------*/

#include "CurvesDialog.h"
#include "ListSelectDlg.h"
#include "../ToolsWidget.h"
#include "../Main/MainWnd.h"
#include "../Config/ParManager.h"

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const int cNoColorPick = 0;
const int cPickerBlack = 1;
const int cPickerWhite = 2;


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  CurvesDialog
 |
 |  Description :
 |
 |   The constructor initializes some variables, sets the dialog icon and
 |   title.
 |
 |  Input parameters :
 |   parent          :  Pointer to the parent widget
 |
 +-------------------------------------------------------------------------*/

CurvesDialog::CurvesDialog ( QWidget * parent ) : QDialog (parent)
{
  RegFile * regFile;
  QSize     size;

  setWindowIcon  ( QIcon ( ":/Icons/Histogram" ) );
  setWindowTitle ( "Curves and Histogram" );

  // accepts focus by both tabbing and clicking
  setFocusPolicy (Qt::StrongFocus);

  // receive mouse move events without a click
  setMouseTracking (true);

  initDialog ();

  regFile = mainWnd->getRegFile ();

  size  = regFile->value ( "CurvesSize" ).toSize ();

  if ( size.width () > 0 )
    resize (size);
  else
    resize ( 700, 800 );

}               /*----- CurvesDialog -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  CurvesDialog
 |
 |  Description :
 |
 |   The destructor deletes all dynamically created objects like buttons,
 |   controls, cursors etc.
 |
 +-------------------------------------------------------------------------*/

CurvesDialog::~CurvesDialog ()
{
  RegFile * regFile;
  QSize     dim;

  regFile = mainWnd->getRegFile ();

  dim     = size ();

  regFile->setValue ( "CurvesSize", dim );

  delete  btnCancel;
  delete  btnOk;

  delete  blackCursor;
  delete  whiteCursor;

  delete  btnBlackPnt;
  delete  btnSaveAs;
  delete  btnWhitePnt;

  delete  btnAutoLevel;
  delete  btnSigmoid;
  delete  btnSigConv;
  delete  btnReset;

  delete  txtBlackPnt;
  delete  txtWhitePnt;
  delete  txtRadius;
  delete  txtSigmaVal;
  delete  txtOffsetVal;

  delete  histWidget;
  delete  curveWidget;

}               /*-----  ~CurvesDialog  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : blockControls  of  CurvesDialog
 |
 |  Description :
 |
 |   This function blocks  or unblocks the controls in  the gui depending
 |   on the state of 'block'.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::blockControls ( bool block )
{

  txtBlackPnt ->blockSignals (block);
  txtWhitePnt ->blockSignals (block);

  txtSigmaVal ->blockSignals (block);
  txtOffsetVal->blockSignals (block);

  cbCurves->blockSignals (block);
  cbTypes->blockSignals (block);

}               /*-----  blockControls  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : closeEvent  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  is called  when the 'X-Cross'  button or  'Cancel' is
 |   pressed.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::closeEvent ( QCloseEvent * )
{

  mainWnd->setPickerCursor (nullptr);

  emit finished (QDialog::Rejected);

}               /*-----  closeEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Memberfunction :  createObjects  of  CurvesDialog
 |
 |  Description :
 |
 |   This function creates the gui objects like buttons, histogram etc.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::createObjects ()
{

  // hotspot of the cursor is half of the pixmap size
  blackCursor  = new QCursor ( QPixmap ( ":/Icons/CursorBlack" ), -1, -1 );
  whiteCursor  = new QCursor ( QPixmap ( ":/Icons/CursorWhite" ), -1, -1 );

  histWidget   = new HistogramWidget (this);
  curveWidget  = new CurvesWidget    (this);

  txtRadius  = new QLineEdit ();
  txtRadius->setMaximumWidth (45);

  txtSigmaVal  = new QLineEdit ();
  txtSigmaVal->setMaximumWidth (45);

  txtOffsetVal = new QLineEdit ();
  txtOffsetVal->setMaximumWidth (45);

  txtBlackPnt  = new QDoubleSpinBox ();
  txtBlackPnt->setRange          ( 0.0, 100.0 );
  txtBlackPnt->setDecimals       (2);
  txtBlackPnt->setMaximumWidth   (100);

  txtWhitePnt  = new QDoubleSpinBox ();
  txtWhitePnt->setRange          ( 0.0, 100.0 );
  txtWhitePnt->setDecimals       (2);
  txtWhitePnt->setMaximumWidth   (100);

  btnBlackPnt  = new QPushButton (this);
  btnWhitePnt  = new QPushButton (this);

  btnBlackPnt->setIcon ( QIcon ( ":/Icons/PickerBlack" ) );
  btnWhitePnt->setIcon ( QIcon ( ":/Icons/PickerWhite" ) );

  btnBlackPnt->setIconSize ( QSize ( 22, 22 ) );
  btnWhitePnt->setIconSize ( QSize ( 22, 22 ) );

  btnAutoLevel = new QPushButton ( "Auto Levels", this );
  btnAutoLevel->setIcon ( QIcon  ( ":/Icons/Linear" ) );
  btnAutoLevel->setIconSize ( QSize ( 22, 22 ) );
  btnAutoLevel->setToolTip       ( "Automatic input level evaluation" );

  btnSigmoid   = new QPushButton (this);
  btnSigmoid->setIcon ( QIcon ( ":/Icons/Sigmoid" ) );
  btnSigmoid->setIconSize ( QSize ( 22, 22 ) );
  btnSigmoid->setToolTip         ( "Create a sigmoid with a slope and offset" );

  btnSigConv   = new QPushButton (this);
  btnSigConv->setIcon ( QIcon ( ":/Icons/Spline" ) );
  btnSigConv->setIconSize ( QSize ( 22, 22 ) );
  btnSigConv->setToolTip         ( "Convert the sigmoid to a cubic spline" );

  btnReset     = new QPushButton ( "Reset", this );
  btnReset->setIcon ( QIcon ( ":/Icons/Reset" ) );
  btnReset->setToolTip           ( "Resets the curve" );

  btnCancel   = new QPushButton ( "Cancel", this );
  btnCancel->setIcon ( QIcon ( ":/Icons/Cancel" ) );
  btnOk       = new QPushButton ( "Ok", this );
  btnOk->setIcon ( QIcon ( ":/Icons/OkClean" ) );

  btnSaveAs = new QPushButton (this);
  btnSaveAs->setIcon       ( QIcon ( ":/Icons/SaveAs" ) );
  btnSaveAs->setIconSize   ( QSize ( 22, 22 ) );
  btnSaveAs->setToolTip    ( "Save curve as preset with new name" );

  // creating the curves preset combobox
  cbCurves  = new QComboBox  (this);
  cbCurves->setEditable      (false);
  cbCurves->setMinimumWidth  (100);
  cbCurves->clear ();

  // creating the curves type combobox
  cbTypes  = new QComboBox  (this);
  cbTypes->setEditable      (false);
  cbTypes->setMinimumWidth  (90);
  cbTypes->clear ();
  cbTypes->addItem (cTypeLinear);
  cbTypes->addItem (cTypeSigmoid);
  cbTypes->addItem (cTypeSpline);

}               /*-----  createObjects  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLayout  of  CurvesDialog
 |
 |  Description :
 |
 |   This function creates the layout for the curves dialog by adding the
 |   controls to the box layouts.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::createLayout ()
{
  QVBoxLayout  * vBox;
  QHBoxLayout  * hBox1;
  QHBoxLayout  * hBox2;
  QHBoxLayout  * hBox3;
  QHBoxLayout  * btnBox;

  vBox   = new QVBoxLayout ();
  hBox1  = new QHBoxLayout ();
  hBox2  = new QHBoxLayout ();
  hBox3  = new QHBoxLayout ();
  btnBox = new QHBoxLayout ();

  hBox1->addWidget  (btnBlackPnt);
  hBox1->addWidget  (txtBlackPnt);
  hBox1->addStretch ();
  hBox1->addWidget  ( new QLabel ( "Picker radius :" ) );
  hBox1->addWidget  (txtRadius);
  hBox1->addStretch ();
  hBox1->addWidget  (btnWhitePnt);
  hBox1->addWidget  (txtWhitePnt);

  hBox2->addWidget  (btnAutoLevel);
  hBox2->addStretch ();
  hBox2->addWidget  (btnSigmoid);
  hBox2->addSpacing (4);
  hBox2->addWidget  ( new QLabel ( "Sigma :" ) );
  hBox2->addWidget  (txtSigmaVal);
  hBox2->addSpacing (4);
  hBox2->addWidget  ( new QLabel ( "Offset :" ) );
  hBox2->addWidget  (txtOffsetVal);
  hBox2->addSpacing (4);
  hBox2->addWidget  (btnSigConv);
  hBox2->addStretch ();
  hBox2->addWidget  (btnReset);

  hBox3->addWidget  ( new QLabel ( "Curve Types :" ) );
  hBox3->addWidget  (cbTypes);
  hBox3->addStretch ();

  hBox3->addWidget  ( new QLabel ( "Curve Presets :" ) );
  hBox3->addWidget  (cbCurves);
  hBox3->addSpacing (4);
  hBox3->addWidget  (btnSaveAs);
  hBox3->addStretch ();

  btnBox->addStretch ();
  btnBox->addWidget  (btnCancel);
  btnBox->addWidget  (btnOk);

  vBox->addWidget  (histWidget);
  vBox->addSpacing (4);
  vBox->addLayout  (hBox1);
  vBox->addSpacing (4);
  vBox->addLayout  (hBox2);
  vBox->addLayout  (hBox3);
  vBox->addSpacing (4);
  vBox->addWidget  (curveWidget);
  vBox->addSpacing (8);
  vBox->addLayout  (btnBox);

  setLayout (vBox);

}               /*-----  createLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  connectSlots  of  CurvesDialog
 |
 |  Description :
 |
 |   This function creates the signal slot connections.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::connectSlots ()
{

  // connections for combo boxes
  connect ( curveWidget,  & CurvesWidget::curveChanged,
            this,         & CurvesDialog::onCheckCurve );

  connect ( cbTypes,      & QComboBox::currentIndexChanged,
            this,         & CurvesDialog::onCurveChanged );

  connect ( cbCurves,     & QComboBox::currentIndexChanged,
            this,         & CurvesDialog::onLoadCurve );

  // connections for push buttons
  connect ( btnCancel,    & QPushButton::pressed,
            this,         & CurvesDialog::onCancel );

  connect ( btnCancel,    & QPushButton::pressed,
            this,         & CurvesDialog::onCancel );

  connect ( btnOk,        & QPushButton::pressed,
            this,         & CurvesDialog::onOk );

  connect ( btnReset,     & QPushButton::pressed,
            this,         & CurvesDialog::onReset );

  connect ( btnAutoLevel, & QPushButton::pressed,
            this,         & CurvesDialog::onAutoLevel );

  connect ( btnSaveAs,    & QPushButton::pressed,
            this,         & CurvesDialog::onSaveCurve );

  connect ( btnSigmoid,   & QPushButton::pressed,
            this,         & CurvesDialog::onSigmoid );

  connect ( btnSigConv,   & QPushButton::pressed,
            this,         & CurvesDialog::onToSpline );

  connect ( btnBlackPnt,  & QPushButton::pressed,
            this,         & CurvesDialog::onBlackPnt );

  connect ( btnWhitePnt,  & QPushButton::pressed,
            this,         & CurvesDialog::onWhitePnt );

  connect ( txtBlackPnt,  & QDoubleSpinBox::valueChanged,
            this,         & CurvesDialog::onValueChanged );

  connect ( txtWhitePnt,  & QDoubleSpinBox::valueChanged,
            this,         & CurvesDialog::onValueChanged );

}               /*-----  connectSlots  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : initCurves  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  loads the current  curves and  makes a backup  of the
 |   current curve and gray channel.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::initCurves ()
{

  curves      = appCurves->getCurrent ();
  channel     = curves->getChannelGray ();

  curveBackup = * curves;
  chanBackup  = * channel;    // make a backup copy

  text = channel->getCurveType ();

  setCurveType (text);

}               /*-----  initCurves  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : initDialog  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  is called  from the  constructor and  initializes the
 |   curves,  objects and  some member  variables.  Then  it creates  and
 |   updates the layout and makes the signal - slot connections.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::initDialog ()
{

  appConfig   = parMan->getAppConfig ();
  appCurves   = parMan->getAppCurves ();

  colPicker   = cNoColorPick;  // no color picker is active
  isGray      = true;
  hasFocus    = true;
  blackPnt    = 0.0;
  whitePnt    = 100.0;
  offsetVal   = 0.0;
  sigmaVal    = 0.7f;

  createObjects ();
  createLayout  ();

  initCurves ();  // call after createObjects

  toGui ();

  curveWidget->setChannel ( channel, Color::blue );

  connectSlots  ();

}               /*-----  initDialog  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fromGui  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  reads the  values of  some widgets  from the  gui and
 |   updates the member variables with this values.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::fromGui ()
{

  sigmaVal   = txtSigmaVal->text  ().toFloat ();
  offsetVal  = txtOffsetVal->text ().toFloat ();

  blackPnt   = txtBlackPnt ->text ().toFloat ();
  whitePnt   = txtWhitePnt ->text ().toFloat ();

  appConfig->pickRadius = txtRadius->text ().toInt ();

  // black and whitepoint in gray channel are already set

  channel->setSigma  (sigmaVal);
  channel->setOffset (offsetVal);

}               /*-----  fromGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  toGui  of  CurvesDialog
 |
 |  Description :
 |
 |   This function updates some widgets  like the curves combobox and the
 |   point and sigmoid controls.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::toGui ()
{
  ChannelInfo info;
  L_String    names;

  channel->getChannelInfo (info);

  blackPnt  = info.blackPnt * 100.0;
  whitePnt  = info.whitePnt * 100.0;
  sigmaVal  = info.sigma;
  offsetVal = info.offset;

  blockControls (true);

  txtBlackPnt->setValue (blackPnt);
  txtWhitePnt->setValue (whitePnt);
  txtRadius  ->setText  ( QString::number (appConfig->pickRadius) );

  txtSigmaVal ->setText ( QString::number (sigmaVal) );
  txtOffsetVal->setText ( QString::number (offsetVal) );

  if ( cbCurves->count () < 1 )
  {
    names = appCurves->getCurveNames ();
    cbCurves->addItems (names);
  }

  cbCurves->setCurrentText ( curves->getCurveName () );
  cbTypes->setCurrentText ( info.curveType);

  blockControls (false);

}               /*-----  toGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  enterEvent  of  CurvesDialog
 |
 |  Description :
 |
 |   This  function is  a  reimplemented event  handler  for mouse  enter
 |   events, the event is sent to the widget when the mouse cursor enters
 |   the widget.
 |
 |  Input parameters :
 |   event           : mouse enter event
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::enterEvent ( QEnterEvent * event )
{

  mainWnd->updateStatusBar ( "Status", "Enter widget" );

  if ( colPicker > cNoColorPick )
  {
    mainWnd->setPickerCursor (nullptr);
  }

  QWidget::enterEvent (event);

}               /*-----  enterEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  leaveEvent  of  CurvesDialog
 |
 |  Description :
 |
 |   This  function is  a  reimplemented event  handler  for mouse  leave
 |   events, the event is sent to the widget when the mouse cursor leaves
 |   the widget.
 |
 |  Input parameters :
 |   event           : mouse leave event
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::leaveEvent ( QEvent * event )
{

  mainWnd->updateStatusBar ( "Status", "Leave widget" );

  if ( colPicker == cPickerBlack )
    mainWnd->setPickerCursor (blackCursor);

  if ( colPicker == cPickerWhite )
    mainWnd->setPickerCursor (whiteCursor);

  QWidget::leaveEvent (event);

}               /*-----  leaveEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  focusInEvent  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  is the  event handler to  receive keyboard  and mouse
 |   focus events for  the widget. It is called then  the widget receives
 |   the focus e.g.  by a mouse press  event, the event is  passed in the
 |   event parameter.
 |
 |  Input parameters :
 |   event           : focus event
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::focusInEvent ( QFocusEvent * event )
{

  if ( colPicker > cNoColorPick )
  {
    colPicker = cNoColorPick;
  }

  mainWnd->setPickerCursor (nullptr);

  QWidget::focusInEvent (event);

}               /*-----  focusInEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  focusOutEvent  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  is the  event handler to  receive keyboard  and mouse
 |   focus events for the widget. It is called when the widget looses the
 |   focus, the event is passed in the event parameter.
 |
 |  Input parameters :
 |   event           : mouse event
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::focusOutEvent ( QFocusEvent * event )
{

  //  mainWnd->updateStatusBar ( "Status", "Focus - out of widget" );

  QWidget::focusOutEvent (event);

}               /*-----  focusOutEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : evalAutoLevel  of  CurvesDialog
 |
 |  Description :
 |
 |   This  function  evaluates the  optimal  values  for the  lowest  and
 |   highest input value for the given channel. It uses a percentage of a
 |   hysteresis when history  values have a 'peak'. This  method is taken
 |   from Gimp.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::evalAutoLevel ( ColorChannel * level )
{
  cv::Mat * channel;
  Histogram hist;
  L_Float   histValues;
  float     newCount;
  float     percent;
  float     nextPerc;
  float     bias;
  float     lowInput;
  float     highInput;
  float     histVal;
  float     histNext;
  float     histPrev;
  int       histBins;

  channel = level->getChannelMat ();

  hist.create ( * channel );

  histValues = hist.getNormalized ( "Sum" );
  histBins   = histValues.count () - 1;  // - 1 because of for loop

  bias       = 0.006f;
  newCount   = 0.0;
  lowInput   = 0.0;
  highInput  = 1.0;

  // set the low input, -1 is already in histBins
  for ( int i = 0; i < histBins; i++ )
  {
    histVal  = histValues [i];
    histNext = histValues [ i + 1 ];

    newCount = newCount + histVal;
    percent  = newCount;

    nextPerc = ( newCount + histNext );

    if ( fabs ( percent - bias ) < fabs ( nextPerc - bias ) )
    {
      lowInput = (float) ( i + 1 ) / histBins;
      break;
    }
  }

  newCount = 0.0;

  // set the high input
  for ( int i = histBins; i > 0; i-- )
  {
    histVal  = 1.0 * histValues [i];
    histPrev = 1.0 * histValues [ i - 1 ];

    newCount = newCount + histVal;
    percent  = newCount;

    nextPerc = ( newCount + histPrev );

    if ( fabs ( percent - bias ) < fabs ( nextPerc - bias ) )
    {
      highInput = (float) ( i - 1 ) / histBins;
      break;
    }
  }

  // sets the low and high input, the same as black and white point
  level->setLowHigh ( lowInput, highInput );

}               /*-----  evalAutoLevel  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  evalMousePos  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  is called from  the main  window after a  mouse press
 |   event  happened in  image  scene.   The mouse  position  is used  to
 |   evaluate the  color value of the  image at this position.  The color
 |   then defined the current black or white point value.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::evalMousePos ( QPointF pos )
{
  cv::Vec3b color;
  cv::Point point;
  int       intensitiy;
  float     value;

  if ( colPicker <= cNoColorPick )   // no color picker active
    return;

  point.x = pos.x ();
  point.y = pos.y ();

  if ( ( point.x >= image.cols ) or ( point.y >= image.rows ) )
    return;

  intensitiy = 0;

  color = getColorAtPoint (point);

  if (isGray)
  {
    intensitiy = image.at < uchar > (point);

    value = 1.0 * intensitiy / 255;
    intensitiy = value * 100.0;

    if ( colPicker == cPickerBlack )    // black point
      channel->setBlackPnt (value);
    else
      channel->setWhitePnt (value);
  }
  else
  {
    if ( colPicker == cPickerBlack )    // black point
      curves->setBlackPnt (color);

    if ( colPicker == cPickerWhite )    // white point
      curves->setWhitePnt (color);

    intensitiy = 100.0 * curves->getLuminosity (color);
  }

  blockControls (true);

  if ( colPicker == cPickerBlack )    // black point
    txtBlackPnt->setValue (intensitiy);
  else
    txtWhitePnt->setValue (intensitiy);

  blockControls (false);

  //  emit applyCurves ();    // inform main window to update the image view

  curveWidget->updateChart ();

}               /*-----  evalMousePos  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getColorAtPoint  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  evaluates the color of  the image at the  given mouse
 |   position. It allows  to use a rectangle around the  point to apply a
 |   simple blur filter before returning the color.
 |
 +-------------------------------------------------------------------------*/

cv::Vec3b CurvesDialog::getColorAtPoint ( cv::Point point )
{
  cv::Mat   dstMat;
  cv::Vec3b color;
  cv::Rect  rect;
  cv::Size  size;
  Point2f   center;
  int       x, y;
  int       dif;
  int       radius;

  if (isGray)
    color [0] = image.at < uchar > (point);
  else
    color = image.at < cv::Vec3b > (point);

  radius = txtRadius->text ().toInt ();

  if ( radius <= 1 )
    return color;

  dif = radius - 1;

  x = point.x - dif;
  y = point.y - dif;

  if ( ( ( x - dif ) < 0 ) or ( ( x + dif ) >= image.cols ) or
       ( ( y - dif ) < 0 ) or ( ( y + dif ) >= image.rows ) )
  {
    return color;
  }

  rect.x = x - dif;
  rect.y = y - dif;

  rect.width  = 2 * dif + 1;
  rect.height = 2 * dif + 1;

  center = point;

  cv::getRectSubPix ( image, rect.size (), center, dstMat );

  size.height = rect.height;
  size.width  = rect.width;

  cv::blur ( dstMat, dstMat, size );

  point.x = dif;
  point.y = dif;

  if (isGray)
    color [0] = dstMat.at < uchar > (point);
  else
    color = dstMat.at < cv::Vec3b > (point);

  return color;

}               /*-----  getColorAtPoint  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onCancel  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  is called  when the 'Cancel'  button is  pressed.  It
 |   resets the points in the level curve and resets the cursor before it
 |   closes the dialog.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::onCancel ()
{

  appCurves->setCurrent (curveBackup);

  mainWnd->setPickerCursor (nullptr);

  reject ();  // indicates 'cancel' in the finished signal

}               /*-----  onCancel  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onOk  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  is called when the  'Ok' button is pressed.   It does
 |   not reset the points in the level curve and resets the cursor before
 |   it closes the dialog.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::onOk ()
{

  fromGui ();

  mainWnd->setPickerCursor (nullptr);

  accept ();  // indicates 'ok' in the finished signal

}               /*-----  onOk  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onAutoLevel  of  CurvesDialog
 |
 |  Description :
 |
 |   This function is called when the  'Auto Level' button is pressed. It
 |   uses 'evalAutoLevel'  to evaluate the  optimal values for  the black
 |   and white point for every channel.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::onAutoLevel ()
{
  ChannelInfo info;

  fromGui ();

  curves->setImage (image);

  channel = curves->getChannelGray ();  // default channel for dialog

  channel->resetPoints ();       // sets to linear default

  if (isGray)
  {
    evalAutoLevel (channel);

    channel->getChannelInfo (info);

    txtBlackPnt->setValue ( 100 * info.lowInput );
    txtWhitePnt->setValue ( 100 * info.highInput );

    curves->setApplyColors (false);
  }
  else
  {
    channel = curves->getChannelRed ();
    evalAutoLevel (channel);

    channel = curves->getChannelGreen ();
    evalAutoLevel (channel);

    channel = curves->getChannelBlue ();
    evalAutoLevel (channel);

    curves->setApplyColors (true);

    channel = curves->getChannelGray ();
  }

  channel->setCurveType (cTypeLinear);

  // evaluate and merge color channels if necessary
  mainWnd->getToolsWidget ()->onApplyCurves ();

  if ( not isGray )
  {
    evalAutoLevel (channel);  // apply new gray channel

    channel->getChannelInfo (info);

    txtBlackPnt->setValue ( 100 * info.lowInput );
    txtWhitePnt->setValue ( 100 * info.highInput );
  }

  curveWidget->updateChart (false);   // calls also apply curves

  curves->setApplyColors (false);

  setCurveType (cTypeLinear);

}               /*-----  onAutoLevel  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onCurveChanged  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  is called when the  index of the combo  box for curve
 |   types  has changed.  Then it  updates the  new type  to the  current
 |   channel and also to the curveWidget.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::onCurveChanged ()
{

  text = cbTypes->currentText ();
  channel->setCurveType (text);

  curveWidget->updateChart ();

}               /*-----  onCurveChanged  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onCheckCurve  of  CurvesDialog
 |
 |  Description :
 |
 |   This function is  called from the 'curveWidget' when  e.g. a sigmoid
 |   was changed to a spline by adding a new point.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::onCheckCurve ()
{

  text = channel->getCurveType ();

  setCurveType (text);

}               /*-----  onCheckCurve  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onReset  of  CurvesDialog
 |
 |  Description :
 |
 |   This  function is  called when  the 'Reset'  button is  pressed.  It
 |   resets the  points in the level  curve and updates the  chart in the
 |   curveWidget.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::onReset ()
{

  colPicker = cNoColorPick;  // no color picker is active

  appCurves->setCurrent (curveBackup);

  * channel = chanBackup;    // restore backup

  curveWidget->reset ();

  toGui ();

}               /*-----  onReset  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onSigmoid  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  is called  when the 'Sigmoid'  button is  pressed. It
 |   then  reads the  widget values  like  sigma and  offset values  from
 |   gui. Then  is reset the  points in the  level curve, sets  the curve
 |   type to sigmoid and calls updateChart from the curve dialog.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::onSigmoid ()
{

  fromGui ();

  curves->resetColors ();

  setCurveType (cTypeSigmoid);

  curveWidget->updateChart ();

}               /*-----  onSigmoid  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onToSpline  of  CurvesDialog
 |
 |  Description :
 |
 |   This function converts  the sigmoid into points for  cubic spline if
 |   the current curve type is sigmoid.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::onToSpline ()
{

  fromGui ();

  channel->sigmoidToSpline ();

  curveWidget->updateChart ();

}               /*-----  onToSpline  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onBlackPnt  of  CurvesDialog
 |
 |  Description :
 |
 |   This  function is  called  when  the black  color  picker button  is
 |   pressed.  This means the black color picker is now active.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::onBlackPnt ()
{

  if ( colPicker == cPickerBlack )
    return;

  fromGui ();

  curves->resetColors ();

  if ( cbTypes->currentText () != cTypeLinear )
  {
    setCurveType (cTypeLinear);
  }

  colPicker = cPickerBlack;  // no color picker is active

  mainWnd->setPickerCursor (blackCursor);

}               /*-----  onBlackPnt  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onWhitePnt  of  CurvesDialog
 |
 |  Description :
 |
 |   This  function is  called  when  the white  color  picker button  is
 |   pressed.  This means the white color picker is now active.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::onWhitePnt ()
{

  if ( colPicker == cPickerWhite )
    return;

  fromGui ();

  curves->resetColors ();

  if ( cbTypes->currentText () != cTypeLinear )
  {
    setCurveType (cTypeLinear);
  }

  colPicker = cPickerWhite;  // no color picker is active

  mainWnd->setPickerCursor (whiteCursor);

}               /*-----  onWhitePnt  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onLoadCurve  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  is called when the  combo box for defined  curves has
 |   changed the selection. Then it loads the current curve settings from
 |   the current 'ColorCurves'.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::onLoadCurve ()
{
  ColorCurves * colCurve;

  text = cbCurves->currentText ();

  colCurve = appCurves->selectCurve (text);

  if (colCurve)
  {
    * curves = * colCurve;

    channel = curves->getChannelGray ();

    toGui ();

    curveWidget->setChannel ( channel, Color::blue );
  }

}               /*-----  onLoadCurve  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onSaveCurve  of  CurvesDialog
 |
 |  Description :
 |
 |   This  function  opens an  dialog  which  shows all  available  curve
 |   settings from the current 'ColorCurves'.  It allows to provide a new
 |   name for the current curve and saves the it with the new name.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::onSaveCurve ()
{
  ListSelectDlg * listDlg;
  L_String        curveNames;

  curveNames = appCurves->getCurveNames ();

  listDlg = new ListSelectDlg ( this, curveNames, "Curve Presets", true );

  if ( listDlg->exec () == QDialog::Accepted )
  {
    text = listDlg->getSelected ();

    if ( cbCurves->findText (text) < 0 )
      cbCurves->addItem (text);

    appCurves->addCurve ( curves, text );
  }

  delete listDlg;

}               /*-----  onSaveCurve  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onValueChanged  of  CurvesDialog
 |
 |  Description :
 |
 |   This function is called when the  value of one of the double spinner
 |   boxes  has changed  it's value.   After identifying  the sender  the
 |   value is changed in curve setting  and the curves widget is informed
 |   about an update.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::onValueChanged ( double value )
{
  QDoubleSpinBox * spinBox;

  // QObject::sender() returns the object which sent the signal
  spinBox = qobject_cast < QDoubleSpinBox * > ( QObject::sender () );

  if ( not spinBox )
    return;

  if ( spinBox == txtBlackPnt )
  {
    channel->setBlackPnt ( value / 100.0 );
    txtBlackPnt->setValue (value);
  }

  if ( spinBox == txtWhitePnt )
  {
    channel->setWhitePnt ( value / 100.0 );
    txtWhitePnt->setValue (value);
  }

  // emit applyCurves ();    // inform main window to update the image view
  curveWidget->updateChart ();

}               /*-----  onValueChanged  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setCurveType  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  selects a text in  the combo box for  curve types but
 |   blocks the signals from the combobox.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::setCurveType ( QString type )
{

  channel->setCurveType (type);

  cbTypes->blockSignals (true);

  cbTypes->setCurrentText (type);

  cbTypes->blockSignals (false);

}               /*-----  setCurveType  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setImage  of  CurvesDialog
 |
 |  Description :
 |
 |   This function  creates a histogram  of a grayscale or  colored image
 |   and allows  to modify a lookup  table in a graphical  view by moving
 |   points of a curve.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::setImage ( cv::Mat image )
{

  this->image = image.clone ();

  imageMat.evalMatInfo (image);

  if ( imageMat.getChannels () == 1 )
  {
    isGray = true;
  }
  else
  {
    isGray = false;
  }

  curves->setImage (image);

  histWidget->create (image);

}               /*-----  setImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  updateHistogram  of  CurvesDialog
 |
 |  Description :
 |
 |   This function updates the histogram with the new image.
 |
 +-------------------------------------------------------------------------*/

void CurvesDialog::updateHistogram ( cv::Mat image )
{

  histWidget->update (image);

}               /*-----  updateHistogram  -----*/

