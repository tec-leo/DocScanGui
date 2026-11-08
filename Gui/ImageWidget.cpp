/*-------------------------------------------------------------------------
 |
 |  File Name   :  ImageWidget.cpp
 |
 |  Description :
 |
 |   This  module  contains the  class  'ImageWidget'  which contains  an
 |   'ImageView' to show a QGraphicsView, zoom controls for the image and
 |   image file loading and saving or exporting controls.
 |
 |  Developer : Oskar Leirich                Creation date : 22.Apr.2025
 |  Modified  : Oskar Leirich                Last changes  : 08.Sep.2026
 |
 |
 |  This unit contains following member functions :
 |   of ImageWidget:
 |    ImageWidget               ~ImageWidget              addImage
 |    addZoomValue              connectSlots              contextMenuEvent
 |    createLayout              createMenu                createObjects
 |    evalZoomFactor            executeJob                fromGui
 |    getExportDir              init                      loadImage
 |    onExportImage             onFileChanged             onImageToPdf
 |    onProcFinished            onReloadImage             onSaveImage
 |    onSaveImageAs             onSliderChanged           onViewMoved
 |    onZoomChanged             onZoomIn                  onZoomOut
 |    saveImage                 scaleView                 toGui
 |    wheelEvent
 |
 +-------------------------------------------------------------------------*/

#include "ImageWidget.h"
#include "ImageEdit.h"
#include "ToolsWidget.h"
#include "Controls/FileSelect.h"
#include "Controls/PathSelect.h"
#include "../Config/ParManager.h"
#include "../Main/MainWnd.h"

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const int cZoomNumStart  =   3;   // index where the numbers start in zoom combo
const int cZoomNewIndex  =   8;   // new values start from this index in zoom combo
const int cMinimumZoom   =  10;   // minimum zoom factor
const int cMaximumZoom   = 150;   // maximum zoom factor
const int cMaxZoomCount  =  12;   // maximum zoom items

/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  ImageWidget
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 |  Input parameters :
 |   parent          :  Pointer to the parent window
 |
 +-------------------------------------------------------------------------*/

ImageWidget::ImageWidget ( QWidget * parent ) : QWidget (parent)
{

  init ();

}               /*----- ImageWidget -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  ImageWidget
 |
 |  Description :
 |
 |   The destructor deletes all  dynamically created objects if there are
 |   any.
 |
 +-------------------------------------------------------------------------*/

ImageWidget::~ImageWidget ()
{

  imageView->deleteItems ();

  if ( process != nullptr )
    delete process;

  // image load, save and export
  delete  cbImage;
  delete  cbExport;

  delete  btnImgReload;
  delete  btnImgSave;
  delete  btnImgSaveAs;
  delete  btnImgExport;
  delete  btnImgToPdf;

  delete  comboZoom;
  delete  sliderZoom;

  delete  btnZoomIn;
  delete  btnZoomOut;

  delete  imageEdit;

}               /*-----  ~ImageWidget  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  addImage  of  ImageWidget
 |
 |  Description :
 |
 |   This function forwards 'addImage' to the view and forces a rescaling.
 |
 |  Input parameters :
 |   img             : Pointer to an image
 |   name            : Name of the layer
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::addImage ( QImage * img, QString name )
{

  imageView->addImage ( img, name );

  scaleView ();

}               /*-----  addImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createMenu  of  ImageWidget
 |
 |  Description :
 |
 |   This function creates the contextMenu.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::createMenu ()
{

  contextMenu = imageEdit->createMenu ();

}               /*-----  createMenu  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  contextMenuEvent  of  ImageWidget
 |
 |  Description :
 |
 |   This function receives a context menu event for the image widget.
 |
 |  Input parameters :
 |   event           : pointer to a QContextMenuEvent
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::contextMenuEvent ( QContextMenuEvent * event )
{

  // in global coordinates
  contextMenu->exec ( event->globalPos () );

}               /*-----  contextMenuEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  connectSlots  of  ImageWidget
 |
 |  Description :
 |
 |   This  function  creates the  signal  slot  connections for  buttons,
 |   widgets, sliders etc.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::connectSlots ()
{

  // connections for load, save and export the image
  connect ( cbImage,      & FileSelect::textChanged,
            this,         & ImageWidget::onFileChanged );

  connect ( btnImgReload, & QPushButton::pressed,
            this,         & ImageWidget::onReloadImage );

  connect ( btnImgSave,   & QPushButton::pressed,
            this,         & ImageWidget::onSaveImage );

  connect ( btnImgSaveAs, & QPushButton::pressed,
            this,         & ImageWidget::onSaveImageAs );

  connect ( btnImgToPdf,  & QPushButton::pressed,
            this,         & ImageWidget::onImageToPdf );

  // connections for page size and zooming
  connect ( comboZoom,    & QComboBox::currentIndexChanged,
            this,         & ImageWidget::onZoomChanged );

  connect ( sliderZoom,   & QSlider::valueChanged,
            this,         & ImageWidget::onSliderChanged );

  connect ( btnZoomIn,    & QToolButton::pressed,
            this,         & ImageWidget::onZoomIn );

  connect ( btnZoomOut,   & QToolButton::pressed,
            this,         & ImageWidget::onZoomOut );

  connect ( imageView,    & ImageView::textDropped,
            this,         & ImageWidget::onFileChanged );

  connect ( imageScene,   & ImageScene::moveView,
            this,         & ImageWidget::onViewMoved, Qt::QueuedConnection );

}               /*-----  connectSlots  -----*/


/*-------------------------------------------------------------------------
 |
 |  Memberfunction :  createObjects  of  ImageWidget
 |
 |  Description :
 |
 |   This  function creates  the imag  e view,  the combo  boxes and  pus
 |   buttons for load, save, export and the zoom controls.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::createObjects ()
{

  regFile   = mainWnd->getRegFile ();

  imageView = new ImageView ();
  imageView->setMinimumSize ( 400, 500 );

  imageScene = imageView->getImageScene ();
  imageEdit  = new ImageEdit (this);

  // image load, save and export
  cbImage = new FileSelect  (this);
  cbImage->setHistory       ( "Image", "File" );
  cbImage->setLabel         ( "Image File  : " );
  cbImage->setIconName      ( ":/Icons/Open" );
  cbImage->setIconSize      (24);
  cbImage->setMustExist     (true);
  cbImage->create           ();
  cbImage->setMinimumWidth  (200);
  cbImage->setMaximumWidth  (500);

  cbExport = new PathSelect (this);
  cbExport->setHistory      ( "Export", "Path" );
  cbExport->setLabel        ( "Export Path : " );
  cbExport->setIconSize     (24);
  cbExport->create          ();
  cbExport->setMinimumWidth (200);
  cbExport->setMaximumWidth (500);

  btnImgReload = new QPushButton (this);
  btnImgReload->setIcon      ( QIcon ( ":/Icons/Update" ) );
  btnImgReload->setIconSize  ( QSize ( 28, 28 ) );
  btnImgReload->setToolTip   ( "Reload last image - Ctrl-R" );
  btnImgReload->setShortcut  ( QKeySequence ( Qt::CTRL | Qt::Key_R ) );

  btnImgSave  = new QPushButton  (this);
  btnImgSave->setIcon        ( QIcon ( ":/Icons/Save" ) );
  btnImgSave->setIconSize    ( QSize ( 28, 28 ) );
  btnImgSave->setToolTip     ( "Save image - Ctrl-S" );
  btnImgSave->setStatusTip   ( "Save image - Ctrl-S" );
  btnImgSave->setShortcut    ( QKeySequence ( Qt::CTRL | Qt::Key_S ) );

  btnImgSaveAs = new QPushButton (this);
  btnImgSaveAs->setIcon      ( QIcon ( ":/Icons/SaveAs" ) );
  btnImgSaveAs->setIconSize  ( QSize ( 28, 28 ) );
  btnImgSaveAs->setToolTip   ( "Save image as - Shift-Ctrl-S" );
  btnImgSaveAs->setStatusTip ( "Save image as - Shift-Ctrl-S" );
  btnImgSaveAs->setShortcut  ( QKeySequence ( Qt::SHIFT | Qt::CTRL | Qt::Key_S ) );

  btnImgExport = new QPushButton (this);
  btnImgExport->setIcon      ( QIcon ( ":/Icons/Export" ) );
  btnImgExport->setIconSize  ( QSize ( 28, 28 ) );
  btnImgExport->setToolTip   ( "Export image - Ctrl-E" );
  btnImgExport->setStatusTip ( "Export image - Ctrl-E" );
  btnImgExport->setShortcut  ( QKeySequence ( Qt::CTRL | Qt::Key_E ) );

  btnImgToPdf = new QPushButton (this);
  btnImgToPdf->setIcon      ( QIcon ( ":/Icons/OcrPdf" ) );
  btnImgToPdf->setIconSize  ( QSize ( 28, 28 ) );
  btnImgToPdf->setToolTip   ( "Convert image to Pdf with OCR" );
  btnImgToPdf->setStatusTip ( "Convert image to Pdf with OCR" );

  // creating the zoom slider, combobox and buttons
  comboZoom  = new QComboBox     (this);
  comboZoom->addItems            (zoomValues);
  comboZoom->setEditable         (true);
  comboZoom->setCurrentIndex     (0);
  comboZoom->setMinimumWidth     (100);
  comboZoom->setMaxCount         (cMaxZoomCount);

  sliderZoom = new QSlider       (Qt::Horizontal);
  sliderZoom->setMinimum         (cMinimumZoom);
  sliderZoom->setMaximum         (cMaximumZoom);
  sliderZoom->setSingleStep      (5);
  sliderZoom->setPageStep        (20);
  sliderZoom->setTickInterval    (10);
  sliderZoom->setTickPosition    (QSlider::NoTicks);
  sliderZoom->setMaximumWidth    (500);
  sliderZoom->setValue           (100);

  btnZoomIn  = new QToolButton   ();
  btnZoomOut = new QToolButton   ();

  btnZoomIn ->setShortcut ( Qt::Key_Plus );
  btnZoomOut->setShortcut ( Qt::Key_Minus );

  btnZoomIn->setAutoRaise  ( true );
  btnZoomIn->setFixedSize  ( 24, 24 );
  btnZoomIn->setIcon       ( QIcon ( ":/Icons/ZoomIn" ) );
  btnZoomIn->setToolTip    ( "Zoom in" );
  btnZoomIn->setShortcut   ( Qt::CTRL | Qt::Key_Plus );

  btnZoomOut->setAutoRaise  ( true );
  btnZoomOut->setFixedSize  ( 24, 24 );
  btnZoomOut->setIcon       ( QIcon ( ":/Icons/ZoomOut" ) );
  btnZoomOut->setToolTip    ( "Zoom out" );
  btnZoomOut->setShortcut   ( Qt::CTRL | Qt::Key_Minus );

}               /*-----  createObjects  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLayout  of  ImageWidget
 |
 |  Description :
 |
 |   This function creates all widgets and sets up the main window.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::createLayout ()
{
  QHBoxLayout * hBox1;
  QHBoxLayout * hBox2;
  QHBoxLayout * hZoom;
  QVBoxLayout * vBox1;

  hBox1 = new QHBoxLayout ();
  hBox2 = new QHBoxLayout ();
  hZoom = new QHBoxLayout ();
  vBox1 = new QVBoxLayout ();

  hBox1->addWidget  (cbImage);
  hBox1->addSpacing (5);
  hBox1->addWidget  (btnImgReload);
  hBox1->addWidget  (btnImgSave);
  hBox1->addWidget  (btnImgSaveAs);
  hBox1->addStretch ();

  hBox2->addWidget  (cbExport);
  hBox2->addSpacing (5);
  hBox2->addWidget  (btnImgExport);
  hBox2->addWidget  (btnImgToPdf);
  hBox2->addStretch ();

  // add zoom area
  hZoom->addStretch ();
  hZoom->addWidget  (comboZoom);
  hZoom->addWidget  (btnZoomOut);
  hZoom->addWidget  (sliderZoom);
  hZoom->addWidget  (btnZoomIn);

  // layout middle, combine vertical
  vBox1->addLayout  (hBox1);
  vBox1->addLayout  (hBox2);
  vBox1->addSpacing (5);
  vBox1->addWidget  (imageView);
  vBox1->addSpacing (5);
  vBox1->addLayout  (hZoom);

  setLayout (vBox1);

}               /*-----  createLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : init  of  ImageWidget
 |
 |  Description :
 |
 |   This member function is called  from the constructor, it initializes
 |   some variables, acquires the  pointers to the application parameter.
 |   scene.  Then it  creates  the objects,  layout  and established  the
 |   signal slot connections.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::init ()
{

  appConfig   = parMan->getAppConfig ();
  fileConfig  = parMan->getFileConfig ();

  msgLog      = mainWnd->getMsgLogger ();

  imageTools.setConfig (appConfig);

  comboZoom   = nullptr;
  contextMenu = nullptr;
  process     = nullptr;

  zoomValues.clear ();

  zoomValues.append ( "Fit Page" );
  zoomValues.append ( "Fit Width" );
  zoomValues.append ( "Fit Height" );
  zoomValues.append ( "25 %" );
  zoomValues.append ( "50 %" );
  zoomValues.append ( "100 %" );
  zoomValues.append ( "150 %" );

  zoomIndex = 0;
  zoomImage = zoomValues [0];

  imageLoaded = false;
  zoomFactor  = 1;
  cmdMode     = false;

  createObjects ();
  createLayout  ();

  createMenu    ();

  imageName = cbImage->getFileName ();

  if ( regFile->contains ( "ImageFile" ) )
  {
    text = regFile->value ( "ImageFile" ).toString ();

    if ( text != imageName )           // should be identical
      cbImage->addFile (imageName);    // show as first file
  }

  connectSlots ();

}               /*-----  init  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getExportDir  of  ImageWidget
 |
 |  Description :
 |
 |   This function  returns the  export directory, it  is taken  from the
 |   path select combobox.
 |
 +-------------------------------------------------------------------------*/

QString ImageWidget::getExportDir ()
{

  return cbExport->getPath ();

}               /*-----  getExportDir  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fromGui  of  ImageWidget
 |
 |  Description :
 |
 |   This  function reads  the values  of some  widget from  the gui  and
 |   updates the member variables with this values.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::fromGui ()
{

  cmdMode = mainWnd->getCmdMode ();

  zoomImage = comboZoom->currentText ();
  appConfig->zoomImage = zoomImage;

  regFile->setValue ( "ImageFile", imageName );

}               /*-----  fromGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  toGui  of  ImageWidget
 |
 |  Description :
 |
 |   This function  updates some widgets  in the  gui from values  of the
 |   member variables. The image is loaded  after a signal from the combo
 |   box when the text has changed during initial load.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::toGui ()
{

  // not in init, because of creation order
  toolsWidget = mainWnd->getToolsWidget ();
  imageEdit->setToolsWidget (toolsWidget);
  imageEdit->toGui ();

  appConfig = parMan->getAppConfig ();

  zoomImage = appConfig->zoomImage;

  if ( zoomValues.indexOf (zoomImage) < 0 )
    zoomImage = zoomValues [0];

  addZoomValue (zoomImage);

  cbImage->clearFiles ();

}               /*-----  toGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : addZoomValue  of  ImageWidget
 |
 |  Description :
 |
 |   This function adds a  new zoom value to the combobox  if it does not
 |   already exist. Then it is set as the current text which also updates
 |   the index.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::addZoomValue ( QString value )
{

  if ( comboZoom->findText (value) < 0 )
  {
    if ( comboZoom->count () >= cMaxZoomCount )
      comboZoom->removeItem (cZoomNewIndex);

    comboZoom->addItem (value);
    zoomIndex = cZoomNewIndex;
    comboZoom->setCurrentIndex (zoomIndex);
    comboZoom->setCurrentText (value);
  }
  else
    zoomIndex = zoomValues.indexOf (value);

}               /*-----  addZoomValue  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : evalZoomFactor  of  ImageWidget
 |
 |  Description :
 |
 |   This function  checks if the zoom  value in the combobox  is correct
 |   and calculates a zoom  factor from it. If it is  wrong then the text
 |   is deleted and the old index restored. The text can only be wrong if
 |   it was entered manually.
 |
 |  Output parameter :
 |   (bool)          : true - valid zoom factor and changed value
 |
 +-------------------------------------------------------------------------*/

bool ImageWidget::evalZoomFactor ( bool forceScale )
{
  int  index;
  bool changed;

  index = comboZoom->currentIndex ();

  if ( ( index == zoomIndex ) and ( not forceScale ) )
    return false;

  changed = true;

  if ( index >= cZoomNumStart )
  {
    text = zoomImage;
    text.replace ( "%", "" );
    text = text.trimmed ();

    if ( ( index >= zoomValues.count () ) and
         ( not StringUtils::validReal (text) ) )
    {
      // restore old index
      comboZoom->setCurrentIndex (zoomIndex);
      changed = false;
    }
    else
    {
      zoomImage = text + " %";
      zoomFactor = text.toDouble ();
    }
  }
  else    // page width and fit
  {
    zoomFactor = 100.0 * imageView->getScale (zoomImage);
  }

  return changed;

}               /*-----  evalZoomFactor  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  executeJob  of  ImageWidget
 |
 |  Description :
 |
 |   This function  is called when a  job was initiated from  the command
 |   line interface  or a job file.   The image widget executes  the jobs
 |   like loading and saving an image.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::executeJob ( JobInfo jobInfo )
{
  QString  name;
  M_StrStr infoMap;

  infoMap = jobInfo.infoMap;

  cmdMode = true;    // used when loading images from a list or directory

  if ( jobInfo.topic != "image" )
  {
    return;
  }

  if ( jobInfo.command == "load-image" )
  {
    if ( infoMap.contains ( "image-file" ) )
    {
      name = infoMap [ "image-file" ];

      if ( QFile::exists (name) )
      {
        imageName = name;
        loadImage ();
      }
    }
  }

  if ( jobInfo.command == "save-image" )
  {
    if ( infoMap.contains ( "image-file" ) )
      imageName = infoMap [ "image-file" ];

    onSaveImage ();
  }

  if ( jobInfo.command == "export-dir" )
  {
    if ( infoMap.contains ( "export-dir" ) )
    {
      name = infoMap [ "export-dir" ];
      cbExport->addPath (name);
    }
  }

  if ( jobInfo.command == "export-image" )
  {
    if ( infoMap.contains ( "export-dir" ) )
    {
      name = infoMap [ "export-dir" ];
      cbExport->addPath (name);
    }

    onExportImage ();
  }

}               /*-----  executeJob  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  loadImage  of  ImageWidget
 |
 |  Description :
 |
 |   This function loads  the image from file if it  exists and calls the
 |   tools widget to load the image  from file into the OpenCV mat. After
 |   that it is scaled with the current zoom settings.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::loadImage ()
{
  QImage * img;

  imageLoaded = false;

  if ( not QFile::exists (imageName) )
    return;

  // tools widget makes all color and format conversions
  imageLoaded = toolsWidget->loadImage ();

  if ( not imageLoaded )
    return;

  img = toolsWidget->getConvImage ();

  imageView->loadImage (img);   // copies a QImage to a QPixmap

  text = "Image " + imageName + " loaded";
  mainWnd->updateStatusBar ( "Info", text );

  scaleView (true);

  delete img;  // it was created in imageMat

}               /*-----  loadImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  saveImage  of  ImageWidget
 |
 |  Description :
 |
 |   This  function  saves the  image  with  the  given file  name  (with
 |   complete  path) and  updates  the  image name  for  the  case it  is
 |   updated.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::saveImage ( QString newName )
{

  imageName = newName;   // update file name

  if ( not fileUtils.makeFilePath (imageName) )
  {
    text  = "Cannot create directory for " + imageName;
    msgLog->showError ( "Save Image", text, " from Image Widget" );

    return;
  }

  toolsWidget->saveImage (newName);

}               /*-----  saveImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onViewMoved  of  ImageWidget
 |
 |  Description :
 |
 |   This function is called from the  image scene when the scene or view
 |   is moved  by pressing and  holding the  left mouse button.  It calls
 |   translate of the view to act like a scroller change.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::onViewMoved ( QPointF delta )
{

  imageView->translate ( delta.x (), delta.y () );
  //  imageView->translate ( 50, 50 );

}               /*-----  onViewMoved  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onFileChanged  of  ImageWidget
 |
 |  Description :
 |
 |   This  function is  called when  the selected  file has  changed.  It
 |   loads the file even if it has  the same name as before. This assures
 |   that all intermediate changes are forgotten.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::onFileChanged ( QString newFile )
{

  imageName = newFile;

  text = cbImage->getFileName ();

  if ( newFile != imageName )
  {
    cbImage->blockSignals (true);
    cbImage->addFile (imageName);
    cbImage->blockSignals (false);
  }

  cbImage->save ();

  regFile->setValue ( "ImageFile", imageName );

  loadImage ();   // load also OpenCV images for run chain

}               /*-----  onFileChanged  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onExportImage  of  ImageWidget
 |
 |  Description :
 |
 |   This function saves the image in the export directory.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::onExportImage ()
{
  QString tmpName;

  fromGui ();     // because of zoom values

  text = cbExport->getPath ();

  if ( not fileUtils.makePath (text) )
  {
    text  = "Export directory " + text + " does not exist";
    msgLog->showError ( "Export Image", text, " from Image Widget" );

    return;
  }

  tmpName = text + "/" + fileUtils.getBaseName ( imageName, true );

  saveImage (tmpName);

}               /*-----  onExportImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onImageToPdf  of  ImageWidget
 |
 |  Description :
 |
 |   This function run the ocr program  which converts the image to a pdf
 |   with ocr.   The option for  optimize is  not changed, default  is 1,
 |   which means safe, lossless optimizations.
 |
 |   Example :
 |    ocrmypdf -l en --optimize=1 --image-dpi=300 ScanDoc.png ScanDoc.pdf
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::onImageToPdf ()
{
  L_String args;
  L_String parts;
  QString  arg;
  QString  path;

  if ( process == nullptr )
  {
    process = new QProcess ();

    connect ( process, & QProcess::finished,
              this,    & ImageWidget::onProcFinished );
  }

  args.clear ();

  arg = fileConfig->ocrConfig;

  arg.replace ( "<lang>", fileConfig->language );

  if ( fileConfig->imageDpi != 0 )
  {
    text.setNum ( fileConfig->imageDpi );
    arg.replace ( "<dpi>", text );
    args.append (arg);
  }

  arg   = strUtils.getQuoted (imageName);
  args.append (arg);

  parts   = fileUtils.splitFileName (imageName);
  pdfName = parts [0] + "/" + parts[1] + ".pdf";

  arg = strUtils.getQuoted (pdfName);
  args.append (arg);

  path = strUtils.getQuoted ( fileConfig->ocrFile );

  arg = args.join ( " " );
  arg = path + " " + arg;

  procTitle = "OCR Pdf";
  procMsg   = "Creating " + pdfName;

  if ( not cmdMode )
    mainWnd->showMessageBox ( procTitle, procMsg, 5 );

  process->startCommand (arg);

}               /*-----  onImageToPdf  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onProcFinished  of  ImageWidget
 |
 |  Description :
 |
 |   This function is called (signaled) when the process finishes.  After
 |   the process has finished, the  buffers in QProcess are still intact.
 |   You can still read any data that the process may have written before
 |   it finished.
 |
 |  Input parameters :
 |   exitCode        : Exit code of the process
 |   exitStatus      : Exit status - Normal or crash
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::onProcFinished ( int, QProcess::ExitStatus exitStatus )
{

  mainWnd->closeMessageBox ();

  if ( exitStatus == QProcess::CrashExit )
    text = "Process crashed";
  else
    text = "Process finished";

  mainWnd->updateStatusBar ( "Info", text );

  process->close ();

  if ( fileConfig->openPdf )
    QDesktopServices::openUrl ( QUrl ( "file:///" + pdfName ) );

}               /*-----  onProcFinished  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onReloadImage  of  ImageWidget
 |
 |  Description :
 |
 |   This function  is called  when the 'Reload'  button is  pressed.  It
 |   reloads  the selected  image  by calling  loadImage  from the  image
 |   widget.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::onReloadImage ()
{

  mainWnd->fromGui ();

  imageScene->removeItems ();

  loadImage ();

}               /*-----  onReloadImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onSaveImage  of  ImageWidget
 |
 |  Description :
 |
 |   This function saves the loaded image with the same name.  This means
 |   the original image will be overwritten and then loaded again.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::onSaveImage ()
{

  fromGui ();    // save zoom values

  saveImage (imageName);

  loadImage ();

  text = "Image " + imageName + " saved";
  mainWnd->updateStatusBar ( "Info", text );

}               /*-----  onSaveImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onSaveImageAs  of  ImageWidget
 |
 |  Description :
 |
 |   This function saves the image with a new name.  The save file dialog
 |   starts in the same folder and with the same name as the input file.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::onSaveImageAs ()
{
  QString filter;

  fromGui ();    // save zoom values

  filter = "*.png;*.jpg;*.tiff";

  fileUtils.selectFile ( imageName, filter, false );

  cbImage->setMustExist (false);
  cbImage->addFile (imageName);    // show as first file
  cbImage->setMustExist (true);

  saveImage (imageName);

  loadImage ();

  text = "Image saved as " + imageName;
  mainWnd->updateStatusBar ( "Info", text );

}               /*-----  onSaveImageAs  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onSliderChanged  of  ImageWidget
 |
 |  Description :
 |
 |   This function is called when the zoom slider has changed its values.
 |   Then the zoom factor for the images are changed to the new value.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::onSliderChanged ( int value )
{

  if ( (int) zoomFactor != value )
  {
    text = QString::number (value) + " %";

    addZoomValue (text);

    zoomFactor = value;

    imageView->scaleView ( zoomFactor / 100.0 );
  }

}               /*-----  onSliderChanged  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  wheelEvent  of  ImageWidget
 |
 |  Description :
 |
 |   This function  is the event handler  for the mouse wheel  event. The
 |   wheel event  parameter provides  information about the  wheel event,
 |   such as:
 |    delta      () - The amount of wheel scrolling, positive scrolls up
 |    angleDelta () - The angle of the wheel rotation.
 |    pixelDelta () - The pixel distance the wheel scrolled horizontally
 |                    and vertically.
 |
 |  Input parameters :
 |   event           : QWidget mouse wheel event
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::wheelEvent ( QWheelEvent * event )
{
  int delta;

  //  event->ignore ();   // default is scroll
  if ( imageEdit->getInsertMode () != cInsertNone )
    return;

  event->accept ();

  delta = event->angleDelta ().y ();

  if ( delta > 0 )
    onZoomIn ();
  else
    onZoomOut ();

}               /*-----  wheelEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onZoomChanged  of  ImageWidget
 |
 |  Description :
 |
 |   This function is  called when the in the zoom  combobox another item
 |   was selected.   Then the zoom factor  for the images are  changed to
 |   the new value.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::onZoomChanged ( int /* index */ )
{

  zoomImage = comboZoom->currentText ();

  if ( evalZoomFactor () )   // value has changed
  {
    sliderZoom->blockSignals (true);
    sliderZoom->setValue ( (int) zoomFactor );
    sliderZoom->blockSignals (false);

    imageView->scaleView ( zoomFactor / 100.0 );
  }

}               /*-----  onZoomChanged  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onZoomIn  of  ImageWidget
 |
 |  Description :
 |
 |   This function increases the zoom factor,  this means it zooms in the
 |   image view.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::onZoomIn ()
{
  int scale;

  scale = qRound ( zoomFactor * 1.2 );

  sliderZoom->setValue (scale);

}               /*-----  zoomIn  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onZoomOut  of  ImageWidget
 |
 |  Description :
 |
 |   This function decreases the zoom factor,  this means it zooms out of
 |   the image view.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::onZoomOut ()
{
  int scale;

  scale = qRound ( zoomFactor * 0.8 );

  sliderZoom->setValue (scale);

}               /*-----  onZoomOut  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  scaleView  of  ImageWidget
 |
 |  Description :
 |
 |   This function scales the view with the current zoomfactor.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::scaleView ( bool forceScale )
{

  evalZoomFactor (forceScale);

  imageView->scaleView ( zoomFactor / 100.0 );

}               /*-----  scaleView  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  scaleView  of  ImageWidget
 |
 |  Description :
 |
 |   This  function   recalculates  the  current  zoomfactor   and  scene
 |   rectangle for the given OpenCV image.
 |
 +-------------------------------------------------------------------------*/

void ImageWidget::scaleView ( Mat newMat )
{
  QImage * img;

  img = imageMat.convMatImage (newMat);

  imageView->updateImage (img);

  evalZoomFactor (true);

  imageView->scaleView ( zoomFactor / 100.0 );

//  delete image;  // it was created in imageMat

}               /*-----  scaleView  -----*/
