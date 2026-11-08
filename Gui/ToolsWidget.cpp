/*-------------------------------------------------------------------------
 |
 |  File Name   :  ToolsWidget.cpp
 |
 |  Description :
 |
 |   This  module  implements  the  class  'ToolsWidget'  which  contains
 |   functions for  improving and  optimizing scanned documents  with the
 |   help of OpenCV. It allows to autorotate, deskew images and increases
 |   the contrast with several different methods.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 21.May.2025
 |  Modified  : Oskar Leirich                Last changes  : 14.Jun.2026
 |
 |
 |  This class contains following member functions :
 |   ToolsWidget :
 |    ToolsWidget               ~ToolsWidget              clearHistory
 |    connectSlots              createConfig              createImgLayer
 |    createLayout              createObjects             createToolsGrid
 |    executeJob                fromGui                   getConvImage
 |    getToolsList              init                      loadImage
 |    normInputImg              onApplyCurves             onBtnClicked
 |    onCurvesClosed            onCurvesDlg               onMousePos
 |    onRunChain                runChain                  runChainTo
 |    runDeskew                 runFilter                 runMerge
 |    runMorphTrans             runSharpen                runTool
 |    saveImage                 setRotation               showImageInfo
 |    toGui
 |
 +-------------------------------------------------------------------------*/

#include "ToolsWidget.h"
#include "Widgets/ButtonsWidget.h"
#include "Widgets/ScrollWidget.h"
#include "../Gui/Dialogs/CurvesDialog.h"
#include "../Gui/ImageWidget.h"
#include "../Main/MainWnd.h"
#include "../Config/ParManager.h"
#include "../Tools/ImageTools.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  ToolsWidget
 |
 |  Description :
 |
 |   The  constructor initializes  some variables,  sets the  main window
 |   icon and title.
 |
 +-------------------------------------------------------------------------*/

ToolsWidget::ToolsWidget ( QWidget * parent ) : QWidget (parent)
{

  init ();

}              /*-----  ToolsWidget  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  ToolsWidget
 |
 |  Description :
 |
 |   The destructor  clears the  history, destroys  the gui  controls and
 |   releases the Mat images.
 |
 +-------------------------------------------------------------------------*/

ToolsWidget::~ToolsWidget ()
{

  clearHistory ();

  if (curvesOpen)   // delete before deleting registry file
  {
    delete curvesDlg;
    curvesDlg = nullptr;
  }

  delete  toolButtons;
  delete  outList;

  delete  parGrid;

  matIn .release ();
  matMid.release ();
  matOut.release ();
  matOrg.release ();

}              /*----- ~ToolsWidget -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : init  of  ToolsWidget
 |
 |  Description :
 |
 |   This member function is called  from the constructor, it initializes
 |   some variables, acquires the pointers  to the image widget, view and
 |   scene.  Then it  creates  the objects,  layout  and established  the
 |   signal slot connections.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::init ()
{

  imgLoaded   = false;
  cmdMode     = false;

  curvesOpen  = false;
  curvesDlg   = nullptr;
  curves      = nullptr;

  appConfig   = parMan->getAppConfig ();
  appCurves   = parMan->getAppCurves ();
  fileConfig  = parMan->getFileConfig ();

  msgLog      = mainWnd->getMsgLogger ();

  imageTools.setConfig (appConfig);

  imageWidget = mainWnd->getImageWidget ();
  imageView   = imageWidget->getImageView ();
  imageScene  = imageView->getImageScene ();

  runList.append (cRunMorph);
  runList.append (cRunSharpen);
  runList.append (cRunBlur);
  runList.append (cRunCurves);
  runList.append (cRunRotate);
  runList.append (cRunCrop);
  runList.append (cRunMargins);

  jobList.append ( "Morphing" );
  jobList.append ( "Sharpen"  );
  jobList.append ( "Blur"     );
  jobList.append ( "Curves"   );
  jobList.append ( "Rotate"   );
  jobList.append ( "Crop"     );
  jobList.append ( "Margins"  );

  createObjects   ();
  createLayout    ();
  connectSlots    ();

}               /*-----  init  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  clearHistory  of  ToolsWidget
 |
 |  Description :
 |
 |   This  function releases  all  stored  cv mat  data  and deletes  the
 |   objects. The image mat are created in createImgLayer and can be used
 |   to implement layers for every new modification in the run chain.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::clearHistory ()
{

  // release and delete also the store mat images
  for ( int i = 0; i < imgStack.count (); i++ )
  {
    imgStack [i].release ();
  }

  imgStack.clear ();
  imgNames.clear ();
  outList->clear ();

}               /*-----  clearHistory  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  connectSlots  of  ToolsWidget
 |
 |  Description :
 |
 |   This  function  creates the  signal  slot  connections for  buttons,
 |   widgets, sliders etc.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::connectSlots ()
{

  connect ( imageScene,  & ImageScene::mousePressPos,
            this,        & ToolsWidget::onMousePos );

  connect ( toolButtons, & ButtonsWidget::buttonClicked,
            this,        & ToolsWidget::onBtnClicked );

}               /*-----  connectSlots  -----*/


/*-------------------------------------------------------------------------
 |
 |  Memberfunction :  createObjects  of  ToolsWidget
 |
 |  Description :
 |
 |   This  function  creates  the  gui  objects  like  images  tab,  file
 |   explorer, config pages etc.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::createObjects ()
{
  QString tooltip;

  toolButtons = new ButtonsWidget  (this);
  toolButtons->setIconSize (22);

  tooltip = "Morph image - Erode or dilate";
  toolButtons->addButton ( cToolMorph, tooltip );

  tooltip = "Sharpen image";
  toolButtons->addButton ( cToolSharp, tooltip );

  tooltip = "Blur image with filtering";
  toolButtons->addButton ( cToolBlur, tooltip );

  tooltip = "Curves, levels and histogram";
  toolButtons->addButton ( cToolCurves, tooltip );

  tooltip = "Rotate image with certain degress";
  toolButtons->addButton ( cToolRotate, tooltip );

  tooltip = "Crop image automatic or with values";
  toolButtons->addButton ( cToolCrop, tooltip );

  tooltip = "Add margins to the image";
  toolButtons->addButton ( cToolMargins, tooltip );

  parList   = appConfig->getParList ();
  parGrid   = new ParListGrid  (this);

  outList = new QListWidget (this);
  outList->setContextMenuPolicy ( Qt::CustomContextMenu );
  outList->hide ();

}               /*-----  createObjects  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createToolsGrid  of  ToolsWidget
 |
 |  Description :
 |
 |   This function creates a grid layout for the parameters of the OpenCV
 |   tools.  The  tools are image sharpening,  morphing, curves adaption,
 |   rotating, cropping etc.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::createToolsGrid ()
{
  QStringList items;
  ParItem   * itemPtr;
  ParItem     item;

  itemPtr = parList->getParItem ( "morphType" );
  itemPtr->infos ["maxWidth"] = "70";

  itemPtr = parList->getParItem ( "cropMargins" );
  itemPtr->infos ["maxWidth"] = "60";

  itemPtr = parList->getParItem ( "pageMargins" );
  itemPtr->infos ["maxWidth"] = "60";

  itemPtr = parList->getParItem ( "rotation" );
  itemPtr->infos ["maxWidth"] = "50";

  itemPtr = parList->getParItem ( "mergeAlign" );
  itemPtr->infos ["maxWidth"] = "60";

  // Group : Sharpening

  parList->beginGroup  ( "Gaussian Sharpening" );
  parList->beginLayout ( "grid" );

  parList->createLineEdit ( "alphaWeight", "Weight of 1st image (alpha)" );
  parList->createLineEdit ( "betaWeight",  "Weight of 2nd image (beta)" );
  parList->createLineEdit ( "gammaAdd",    "Additional offset (gamma)" );

  parList->endGroup ();

  //  parList->createStretch ();

  // Group: Morphing and filter

  parList->beginGroup  ( "Morphing Operations" );
  parList->beginLayout ( "grid" );

  items = appConfig->morphTypes;

  parList->createCombobox ( "morphType", items, appConfig->morphType );

  parList->createLineEdit ( "morphSize" );
  parList->createLineEdit ( "morphIter" );

  parList->endGroup ();

  //  parList->createStretch ();

  // Group: Blur Filtering

  parList->beginGroup  ( "Blur Filtering" );
  parList->beginLayout ( "grid" );

  items = appConfig->filterTypes;
  parList->createCombobox ( "filterType", items, appConfig->filterType );
  parList->createLineEdit ( "blurKernel" );

  parList->endGroup ();

  //  parList->createStretch ();

  // Group: Crop, Rotation and Margins" );

  parList->beginGroup  ( "Rotation and Margins" );
  parList->beginLayout ( "grid" );

  parList->createLineEdit ( "rotation" );
  parList->createCheckbox ( "useAutoRot" );

  parList->createLineEdit ( "cropMargins" );
  parList->createCheckbox ( "useAutoCrop" );

  parList->createLineEdit ( "pageMargins" );

  parList->endGroup ();

  // Group: Merging

  parList->beginGroup  ( "Merge two images" );
  parList->beginLayout ( "grid" );

  items = appConfig->mergeTypes;
  parList->createCombobox ( "mergeAlign", items, appConfig->mergeAlign );
  parList->createCheckbox ( "mergeVertical" );
  parList->createLineEdit ( "mergeGap" );

  parList->endGroup ();

  parList->createStretch ();

}               /*-----  createToolsGrid  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createConfig of  ToolsWidget
 |
 |  Description :
 |
 |   This function creates a configuration grid layout for the parameters
 |   for image loading, saving and OpenCV tools.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::createConfig ()
{

  // new parList for the dialog
  parList->setDlgParList ( & dlgParList );

  // grid for the tool chain
  createToolsGrid ();

  // add parList for the dialog controls
  parGrid->setParList ( & dlgParList );

}               /*-----  createConfig  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLayout  of  ToolsWidget
 |
 |  Description :
 |
 |   This function creates all widgets and sets up the main window.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::createLayout ()
{
  QVBoxLayout  * vBox1;
  ScrollWidget * parArea;

  createConfig ();

  vBox1   = new QVBoxLayout ();
  parArea = new ScrollWidget ( this, parGrid );

  toolButtons->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Maximum );
  // parArea->setSizePolicy ( QSizePolicy::Preferred, QSizePolicy::Minimum );

  // vertical layout left
  vBox1->addWidget  ( toolButtons, 1 );
  vBox1->addStretch ();
  vBox1->addWidget  ( parArea, 3 );
  vBox1->addSpacing (5);
  vBox1->addWidget  ( outList, 1 );

  setLayout (vBox1);

}               /*-----  createLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : createImgLayer  of  ToolsWidget
 |
 |  Description :
 |
 |   This function uses the given image to add a new image layer with the
 |   given name to the result view.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::createImgLayer ( QString name )
{
  QImage * image;
  Mat      matImg;

  matImg = matOut.clone ();

  imgStack.append (matImg);
  imgNames.append (name);
  outList->addItem (name);

  image = imageMat.convMatImage (matImg);

  imageWidget->addImage ( image, name );

  delete image;  // it was created in imageMat

  matIn = matOut;    // copy back as input for next image tool

}               /*-----  createImgLayer  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  executeJob  of  ToolsWidget
 |
 |  Description :
 |
 |   This function  is called when a  job was initiated from  the command
 |   line interface  or a job file.   The tools widget executes  the jobs
 |   like running a tools chain or  individual tools. It also handles the
 |   excecution more several files or all image files in a directory.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::executeJob ( JobInfo curJob )
{
  QDir     dir;
  QString  name;
  QString  exportDir;
  L_String dirFiles;
  L_String files;
  L_Int    toolList;
  bool     force;

  jobInfo = curJob;
  infoMap = jobInfo.infoMap;

  if ( jobInfo.topic != "tools" )
  {
    text = "Topic " + jobInfo.topic + " not known !!";
    msgLog->showError ( "ToolsWidget", "executeJob", text );

    return;
  }

  if ( not ( ( jobInfo.command == "run-chain" ) or
             ( jobInfo.command == "run-tools" ) ) )
  {
    text = "Command " + jobInfo.command + " not known !!";
    msgLog->showError ( "ToolsWidget", "executeJob", text );

    return;
  }

  cmdMode = true;    // used when loading images from a list or directory
  force   = true;    // force tools even when not selected

  // check export dir
  if ( infoMap.contains ( "export-dir" ) )
    exportDir = infoMap [ "export-dir" ];
  else
    exportDir = imageWidget->getExportDir ();

  if ( not fileUtils.makePath (exportDir) )
  {
    text = "Cannot create export directory : " + exportDir +
           ", using current export directory !!";

    msgLog->showError ( "ToolsWidget", "executeJob", text );

    exportDir = imageWidget->getExportDir ();
  }

  // check image files
  if ( infoMap.contains ( "image-files" ) )
  {
    name = infoMap [ "image-files" ];

    files = name.split ( "," );
  }

  // check image directory after files are evaluated
  if ( infoMap.contains ( "image-dir" ) )
  {
    name = infoMap [ "image-dir" ];
    dir.setPath (name);

    dirFiles = dir.entryList ( QDir::Files | QDir::NoDotAndDotDot );

    for ( int i = 0; i < dirFiles.count (); i++ )
    {
      text = name + "/" + dirFiles [i];
      files.append (text);
    }
  }

  if ( jobInfo.command == "run-tools" )
  {
    text = infoMap [ "tools" ];
    toolList = getToolsList (text);
    force    = true;    // force tools even when not selected
  }

  if ( jobInfo.command == "run-chain" )
  {
    text = infoMap [ "tools" ];
    toolList = runList;
    force    = false;    // only selected tools are executed
  }

  if ( files.count () > 0 )
  {
    for ( int i = 0; i < files.count (); i++ )
    {
      imageName = files [i];
      imageWidget->setImageName (imageName);

      if ( not QFile::exists (imageName) )
        continue;

      loadImage ();     // here it is checked if the image was loaded

      if ( not imgLoaded )
        continue;

      runChain ( toolList, force );

      name = exportDir + "/" + fileUtils.getBaseName ( imageName, true );
      saveImage (name);
    }
  }
  else
    runChain ( toolList, force );

}               /*-----  executeJob  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getToolsList  of  ToolsWidget
 |
 |  Description :
 |
 |   This function takes a comma separated string of short tool names and
 |   fills a integer list with the indexes of the tools.
 |
 +-------------------------------------------------------------------------*/

L_Int ToolsWidget::getToolsList ( QString tools )
{
  L_String jobNames;
  L_Int    jobTools;
  int      jobIdx;

  jobNames = tools.split ( "," );

  for ( int idx = 0; idx < jobNames.count (); idx++ )
  {
    text = jobNames [idx];

    jobIdx = jobList.indexOf (text);

    if ( jobIdx >= 0 )
      jobTools.append (jobIdx);
    else
    {
      text = "Tools " + text + " not found !!";
      msgLog->showError ( "ToolsWidget", "getToolsList", text );
    }

  }

  return jobTools;

}               /*-----  getToolsList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getConvImage  of  ToolsWidget
 |
 |  Description :
 |
 |   This function  converts the matOut to  a QImage. It does  not make a
 |   deep copy  of the mat  therefore the mat must  exist as long  as the
 |   QImage is used. Normally this means it is necessary to keep track of
 |   the Mat, but QImage is only used temporarily and deleted after it is
 |   copied to a QPixmap.
 |
 +-------------------------------------------------------------------------*/

QImage * ToolsWidget::getConvImage ()
{
  QImage * image;

  image = imageMat.convMatImage (matOut);

  return image;

}               /*-----  getConvImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fromGui  of  ToolsWidget
 |
 |  Description :
 |
 |   This  function reads  the values  of some  widget from  the gui  and
 |   updates the member variables with this values.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::fromGui ()
{

  cmdMode = mainWnd->getCmdMode ();

  appConfig->useSharpen  = toolButtons->getBtnActive (cRunSharpen);
  appConfig->useMorph    = toolButtons->getBtnActive (cRunMorph);
  appConfig->useBlur     = toolButtons->getBtnActive (cRunBlur);
  appConfig->useCurves   = toolButtons->getBtnActive (cRunCurves);
  appConfig->useRotate   = toolButtons->getBtnActive (cRunRotate);
  appConfig->useCropImg  = toolButtons->getBtnActive (cRunCrop);
  appConfig->addMargins  = toolButtons->getBtnActive (cRunMargins);

  parGrid ->fromGui ();

}               /*-----  fromGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  toGui  of  ToolsWidget
 |
 |  Description :
 |
 |   This function sets the tool buttons  and updates some widgets in the
 |   gui from the configuration parameters.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::toGui ()
{

  toolButtons->setBtnActive ( cRunSharpen, appConfig->useSharpen );
  toolButtons->setBtnActive ( cRunMorph,   appConfig->useMorph   );
  toolButtons->setBtnActive ( cRunBlur,    appConfig->useBlur    );
  toolButtons->setBtnActive ( cRunCurves,  appConfig->useCurves  );
  toolButtons->setBtnActive ( cRunRotate,  appConfig->useRotate  );
  toolButtons->setBtnActive ( cRunCrop,    appConfig->useCropImg );
  toolButtons->setBtnActive ( cRunMargins, appConfig->addMargins );

  parGrid->toGui ();

}               /*-----  toGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  normInputImg  of  ToolsWidget
 |
 |  Description :
 |
 |   This  function converts  the given  Mat  to the  format for  loading
 |   defined  in fileConfig.   Any alpha  channel is  removed, the  color
 |   order is  changed from BGR  to RGB.  Only  the 8 bit  unsigned image
 |   format is supported.  Floating point  formats are converted to 8 bit
 |   unsigned.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::normInputImg ( Mat & mat )
{

  imageMat.removeAlpha (mat);   // remove alpha

  if ( mat.channels () == 3 )     // change color order
    cvtColor ( mat, mat, COLOR_BGR2RGB );

  if ( mat.depth () != CV_8U )
    mat = imageMat.convTo8bit (mat);

  if ( fileConfig->loadGray )
    mat = imageMat.convToGray (mat);

}               /*-----  normInputImg  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  normOutputImg  of  ToolsWidget
 |
 |  Description :
 |
 |   This  function converts  the given  Mat  to the  format defined  for
 |   saving in fileConfig.   The color order is changed back  from RGB to
 |   BGR for  color images.  The image is  saved as png  or jpg  with the
 |   definded compression level.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |
 +-------------------------------------------------------------------------*/

Mat ToolsWidget::normOutputImg ( Mat mat )
{
  Mat normOut;

  if ( mat.channels () == 3 )     // change color order
  {
    cvtColor ( mat, normOut, COLOR_RGB2BGR );
  }
  else
  {
    normOut = mat.clone ();
  }

  return normOut;

}               /*-----  normOutputImg  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  loadImage  of  ToolsWidget
 |
 |  Description :
 |
 |   This function loads  the image from file if it  exists and shows the
 |   image info in the status bar if not called from the jobmanager.
 |
 +-------------------------------------------------------------------------*/

bool ToolsWidget::loadImage ()
{

  clearHistory ();

  imageName = imageWidget->getImageName ();

  matIn = cv::imread ( ToString (imageName), IMREAD_UNCHANGED );

  imgLoaded = not matIn.empty ();

  if ( matIn.empty () )   // cannot be loaded
    return false;

  normInputImg (matIn);

  matOrg = matIn.clone ();

  // store curves input before running chain, will be updated in chain
  matMid = matOrg.clone ();
  matIn  = matOrg.clone ();
  matOut = matOrg.clone ();  // must always be set if the chain is not run

  if ( not cmdMode )
    showImageInfo (matOrg);

  return true;

}               /*-----  loadImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  saveImage  of  ToolsWidget
 |
 |  Description :
 |
 |   This  function  saves the  image  with  the  given file  name  (with
 |   complete path). The image is  normalized before this means the color
 |   order  is changed  back  from RGB  to BGR  for  color images.   Also
 |   different output formats (jpg, png) are checked before saving.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::saveImage ( QString newName )
{
  L_String parts;
  QString  suffix;
  QString  saveName;
  Mat      saveMat;
  int      jpgComp;

  imageName = newName;

  saveMat = normOutputImg (matOut);

  if ( fileConfig->saveAsIs )
  {
    cv::imwrite ( ToString (imageName), saveMat );
    return;
  }

  parts  = fileUtils.splitFileName (imageName);
  suffix = parts [2];

  if ( fileConfig->saveType == "png" )
  {
    saveName = parts [0] + "/" + parts[1] + ".png";
    cv::imwrite ( ToString (saveName), saveMat );
  }
  else
  {
    saveName = parts [0] + "/" + parts[1] + ".jpg";

    jpgComp = fileConfig->saveJpgComp;
    cv::imwrite ( ToString (saveName), saveMat,
                  { cv::ImwriteFlags::IMWRITE_JPEG_QUALITY, jpgComp } );
  }

}               /*-----  saveImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  saveImage  of  ToolsWidget
 |
 |  Description :
 |
 |   This function  saves the  image with the  file name  'newName' (with
 |   complete path).  The image is normalized before this means the color
 |   order is changed back from RGB to BGR for color images.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::saveImage ( QString newName, Mat imgOut )
{
  Mat saveMat;

  saveMat = normOutputImg (imgOut);

  cv::imwrite ( ToString (newName), saveMat );

}               /*-----  saveImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onApplyCurves  of  ToolsWidget
 |
 |  Description :
 |
 |   This function is  called from the curves dialog when  some curves or
 |   black or white  points has changed.  Then the last  image of the run
 |   chain (without curves) is given to the color curves which calculates
 |   a new modified image. This is then shown immediately.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::onApplyCurves ()
{
  L_String names;
  cv::Mat  imgOut;

  curves = appCurves->getCurrent ();

  curves->applyCurve ( matMid, matOut );

  names = imageView->getLayerNames ();

  if ( not names.isEmpty () )
  {
    text = names.last ();

    if ( text == "Dialog Curves" )
      imageView->deleteLast ();
  }

  createImgLayer ( "Dialog Curves" );

  if (curvesOpen)
    curvesDlg->updateHistogram (matOut);

}               /*-----  onApplyCurves  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onCurvesDlg  of  ToolsWidget
 |
 |  Description :
 |
 |   This  function is  called  when  the curves  button  is pressed.  It
 |   creates a  new 'CurvesDialog'  if it  does not  yet exist,  sets the
 |   image and shows the dialog.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::onCurvesDlg ()
{

  if (curvesOpen)
    return;

  if ( not imageWidget->getImageLoaded () )
  {
    QMessageBox::warning ( this, "No image", "First load a valid image !" );
    return;
  }

  curves = appCurves->getCurrent ();

  mainWnd->fromGui ();   // update values from widget to internal variables

  if ( appConfig->runToolChain )
    runChainTo ( cRunCurves - 1 );

  matMid = matOut.clone ();   // store image for the curves dialog

  if ( curvesDlg == nullptr )
  {
    curvesDlg = new CurvesDialog (this);

    connect ( curvesDlg, & CurvesDialog::applyCurves,
              this,      & ToolsWidget::onApplyCurves );

    connect ( curvesDlg, & CurvesDialog::finished,
              this,      & ToolsWidget::onCurvesClosed );
  }

  curvesOpen = true;

  curvesDlg->setImage (matMid);

  curvesDlg->setModal (false);   // windows modal dialog
  curvesDlg->show ();

}               /*-----  onCurvesDlg  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onCurvesClosed  of  ToolsWidget
 |
 |  Description :
 |
 |   This function  is called  from the  curves dialog  when is  has been
 |   closed. Then the dialog  is deleted and the pointer to  it is set to
 |   nullptr.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::onCurvesClosed ( int result )
{

  mainWnd->fromGui ();   // update values from widget to internal variables

  curvesOpen = false;

  delete curvesDlg;   // dialog has flag delete on close

  curvesDlg = nullptr;

  if ( curves->getChannelGray ()->getCurveType () == cTypeAuto )
    return;

  imageView->deleteLast ();

  matOut = matMid.clone ();   // state before opening the curves dialog

  if ( result == QDialog::Accepted )
  {
    if ( appConfig->runToolChain )
      onRunChain ();
    else
      runTool (cRunCurves);
  }

}               /*-----  onCurvesClosed  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onRunChain  of  ToolsWidget
 |
 |  Description :
 |
 |   This function is  called when the 'Run' button is  pressed.  It runs
 |   the complete  tool chain of  the activated OpenCV methods  and shows
 |   the resulting image in the working tab.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::onRunChain ()
{

  fromGui ();   // update values from widget to internal variables

  if ( not imageWidget->getImageLoaded () )
  {
    QMessageBox::warning ( this, "No image", "First load a valid image !" );
    return;
  }

  runChain (runList);

}               /*-----  onRunChain  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onBtnClicked  of  ToolsWidget
 |
 |  Description :
 |
 |   This function  is called from the  buttons widgets when a  button is
 |   pressed.  It calls the tool bound to the pressed button, the checked
 |   state is not used but it asked for 'active' in fromGui.
 |
 |  Input parameters :
 |   index           : Index of the selected button
 |   checked         : Checked state of the selected button
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::onBtnClicked ( int index, bool /* checked */ )
{

  mainWnd->fromGui ();       // because of zoom values

  runTool ( index, true );   // force action independent of activation state

}               /*-----  onBtnClicked  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onMousePos  of  ToolsWidget
 |
 |  Description :
 |
 |   This function  is called  from the  image scene  when a  mouse press
 |   event happens.  If the curves dialog is open then the mouse position
 |   is forwarded to this dialog.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::onMousePos ( QPointF pos )
{

  if ( curvesDlg != nullptr )
  {
    curvesDlg->evalMousePos (pos);
  }

}               /*-----  onMousePos  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setRotation  of  ToolsWidget
 |
 |  Description :
 |
 |   This  function sets  the  value  of the  edit  field  for the  image
 |   rotation.  This value is then used when running the tool chain. Also
 |   the image rotation is checked by default.
 |
 |  Input parameters :
 |   angle           : Value for image rotation
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::setRotation ( double angle )
{
  QWidget   * widget;
  QLineEdit * txtRotate;

  widget    = parGrid->getWidget ( "rotation" );
  txtRotate = dynamic_cast <QLineEdit *> (widget);

  txtRotate->setText ( QString::number ( angle, 'g', 3 ));

}               /*-----  setRotation  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  runChain  of  setDefault
 |
 |  Description :
 |
 |   This function is  called when the 'Run' button is  pressed.  It runs
 |   the  tool chain  for  all  activated OpenCV  methods  and shows  the
 |   resulting image  in the result  tab.  The  default run chain  can be
 |   modified  with runIdx  list, this  is used  when opening  the curves
 |   dialog or for auto detection the rotation of the image.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::runChain ( L_Int runIdx, bool force )
{
  int index;

  clearHistory ();

  matIn.release ();
  matOut.release ();

  matIn  = matOrg.clone ();   // start with loaded image
  matOut = matOrg.clone ();   // start with loaded image

  for ( int idx = 0; idx < runIdx.count (); idx++ )
  {
    index = runIdx [idx];

    runTool ( index, force );
  }

}               /*-----  runChain  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  runChainTo  of  setDefault
 |
 |  Description :
 |
 |   This  function run  the  chain upto  a defined  run  index. This  is
 |   helpful for automatic detection of image rotation, deskew etc. or to
 |   apply the result and make it visible to the user.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::runChainTo ( int lastIdx )
{
  L_Int runIdx;

  if ( not imageWidget->getImageLoaded () )
  {
    QMessageBox::warning ( this, "No image", "First load a valid image !" );
    return;
  }

  // sliced uses count, not index --> lastIdx + 1
  runIdx = runList.sliced ( 0, lastIdx + 1 );

  runChain (runIdx);

}               /*-----  runChainTo  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  runDeskew  of  ToolsWidget
 |
 |  Description :
 |
 |   This  function applies  a  perspective transformation  to the  input
 |   image.   The function  warpPerspective transforms  the source  image
 |   using  a  transformation matrix.   This  matrix  is calculated  with
 |   getPerspectiveTransform   which  needs   the   coordinates  of   the
 |   quadrangle  vertices  in  the  source image  and  the  corresponding
 |   quadrangle vertices in the destination image.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::runDeskew ()
{
  L_PointsF   points;
  V_CvPoint2f srcPnts;
  V_CvPoint2f dstPnts;
  cv::Mat     transform;
  float       widthA;
  float       widthB;
  float       heightA;
  float       heightB;
  float       maxWidth;
  float       maxHeight;

  points = imageScene->getPath ();

  if ( points.count () != 4 )
    return;

  srcPnts.resize (4);
  dstPnts.resize (4);

  for ( int i = 0; i < 4; i++ )
  {
    srcPnts [i].x = points [i].x ();
    srcPnts [i].y = points [i].y ();
  }

  std::swap ( srcPnts [2], srcPnts [3] );

  // compute the width of the new image, top and bottom
  widthA = graphUtils.getDistance ( points [0], points [1] );
  widthB = graphUtils.getDistance ( points [2], points [3] );
  maxWidth = qMax ( widthA, widthB );

  // compute the height of the new image, left and right
  heightA = graphUtils.getDistance ( points [0], points [2] );
  heightB = graphUtils.getDistance ( points [1], points [3] );
  maxHeight = qMax ( heightA, heightB );

  // set the destination points of the image
  // order of points in OpenCV functions is
  // top-left, top-right, bottom-right, and bottom-left

  dstPnts [0].x = 0;
  dstPnts [0].y = 0;

  dstPnts [1].x = maxWidth - 1;
  dstPnts [1].y = 0;

  dstPnts [2].x = maxWidth  - 1;
  dstPnts [2].y = maxHeight - 1;

  dstPnts [3].x = 0;
  dstPnts [3].y = maxHeight - 1;

  // compute the perspective transform matrix and then apply it
  transform = getPerspectiveTransform ( srcPnts, dstPnts );

  warpPerspective ( matIn, matOut, transform, cv::Size2f ( maxWidth, maxHeight ) );

}               /*-----  runDeskew  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  runFilter  of  ToolsWidget
 |
 |  Description :
 |
 |   This function applies several selectable blur filters like gaussian,
 |   median or simple blur filter.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::runFilter ()
{
  cv::Point point;
  cv::Size  size;

  point.x = -1;
  point.y = -1;

  size.height = appConfig->blurKernel;
  size.width  = appConfig->blurKernel;

  // blurs an image using the normalized box filter
  if ( appConfig->filterType == cSimpleBlur )
    cv::blur ( matIn, matOut, size );

  // replaces each pixel by the median pixel in a rectangular neighborhood
  if ( appConfig->filterType == cMedianBlur )
    cv::medianBlur ( matIn, matOut, appConfig->blurKernel );

  // convolving each point with a Gaussian kernel
  if ( appConfig->filterType == cGaussFilter )
    cv::GaussianBlur ( matIn, matOut, size, 0, 0, cv::BORDER_DEFAULT );

}               /*-----  runFilter  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  runMerge  of  ToolsWidget
 |
 |  Description :
 |
 |   This function  merges  (concatenates)  two  images  horizontally  or
 |   vertically. The 2nd image can be align left (top) or right  (bottom)
 |   or resized to the same rows or columns as the first image.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::runMerge ()
{
  Mat image2;

  image2 = cv::imread ( ToString (appConfig->mergeImage), IMREAD_UNCHANGED );

  if ( image2.empty () )   // cannot be loaded
    return;

  imageMat.removeAlpha (matIn);

  matOut = imageTools.mergeImages ( matIn, image2 );

}               /*-----  runMerge  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  runSharpen  of  ToolsWidget
 |
 |  Description :
 |
 |   This  function  sharpens the  given  image  with several  selectable
 |   methods like highpass, laplace, unsharp mask or others.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::runSharpen ()
{

  // sharpen the image using the gaussian method
  if ( appConfig->sharpType == cSharpGauss )
  {
    matOut = filters.sharpGaussian ( matIn,
                                     appConfig->alphaWeight,
                                     appConfig->betaWeight,
                                     appConfig->gammaAdd );
  }

  // sharpen the image with a highpass filtering
  if ( appConfig->sharpType == cSharpHighpass )
  {
    matOut = filters.sharpHighpass ( matIn,
                                     appConfig->gaussKernel,
                                     appConfig->gaussSigma,
                                     appConfig->alphaWeight,
                                     appConfig->betaWeight,
                                     appConfig->gammaAdd );
  }

  // sharpen the image using the gaussian method
  if ( appConfig->sharpType == cSharpLaplace )
  {
    matOut = filters.sharpLaplace ( matIn,
                                    appConfig->gaussKernel,
                                    appConfig->gaussSigma,
                                    appConfig->laplaceKernel );
  }

  // sharpen the image with the unsharp mask method
  if ( appConfig->sharpType == cUnsharpMask )
  {
    matOut = filters.unsharpMask ( matIn,
                                   appConfig->gaussKernel,
                                   appConfig->unsharpAmount,
                                   appConfig->gaussSigma,
                                   appConfig->unsharpThresh );
  }

}               /*-----  runSharpen  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  runMorphTrans  of  ToolsWidget
 |
 |  Description :
 |
 |
 |   This  function  executes  some  morphological  transformations  like
 |   dilate or closing to improve the shape  of text. This can be used if
 |   the text is rather 'thin' on the scanned document.
 |
 |   Before it calls 'getStructuringElement'  which returns a structuring
 |   element  of   the  specified   size  and  shape   for  morphological
 |   operations.  This element  can be further passed to  erode, dilate or
 |   morphologyEx. But  you can also  construct an arbitrary  binary mask
 |   yourself and use it as the structuring element.
 |
 |   Parameters for getStructuringElement
 |       shape	Element shape that could be one of MorphShapes
 |       ksize	Size of the structuring element.
 |       anchor	Anchor position within the element.
 |              The default value means that the anchor is at the center.
 |              Only the shape of a cross-shaped element depends on the
 |              anchor position.
 |
 |   The definition are in types_c.h and in interface.h.
 |
 |   Definitions for the morphological operations tpye:
 |
 |   MORPH_ERODE      0,
 |   MORPH_DILATE     1,
 |   MORPH_OPEN       2,
 |   MORPH_CLOSE      3,
 |   MORPH_GRADIENT   4,
 |   MORPH_TOPHAT     5,
 |   MORPH_BLACKHAT   6
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::runMorphTrans ()
{
  cv::Size  size;
  cv::Point point;
  Mat       element;
  int       morphShape;
  int       morphSize;
  int       morphType;

  morphShape = MORPH_ELLIPSE;
  morphSize  = appConfig->morphSize;
  morphType  = appConfig->morphTypes.indexOf (appConfig->morphType);

  point.x = -1;
  point.y = -1;

  size.height = morphSize;
  size.width  = morphSize;

  // point is always at center
  element = getStructuringElement ( morphShape, size );

  // morph the image using the structuring element
  morphologyEx ( matIn, matOut, morphType, element,
                 cv::Point ( -1, -1 ), appConfig->morphIter );

}               /*-----  runMorphTrans  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  runTool  of  setDefault
 |
 |  Description :
 |
 |   This function is called from runChain or from a tool button. It runs
 |   the tool with the  given index but only when it  is activated in the
 |   tool buttons or force is true.
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::runTool ( int index, bool force )
{

  if ( not imageWidget->getImageLoaded () )
  {
    QMessageBox::warning ( this, "No image", "First load a valid image !" );
    return;
  }

  switch (index)
  {
    case cRunMorph :
    {  // apply morphological transformations
      if ( appConfig->useMorph or force )
      {
        runMorphTrans ();

        createImgLayer ( "Morphing" );
      }
    }
    break;

    case cRunSharpen :
    {  // sharpen image
      if ( appConfig->useSharpen or force )
      {
        runSharpen ();
        createImgLayer ( "Sharpen Image" );
      }
    }
    break;

    case cRunBlur :
    {  // apply median blur filter with defined kernel size
      if ( appConfig->useBlur or force )
      {
        runFilter ();

        createImgLayer ( "Blur filter" );
      }
    }
    break;

    case cRunCurves :
    {
      if ( appConfig->useCurves or force )
      {
        matMid = matOut.clone ();   // image before curves handling

        appCurves->getCurrent ()->applyCurve ( matMid, matOut );

        createImgLayer ( "Curves filter" );
      }
    }
    break;

    case cRunDeskew :
    {
      if ( appConfig->useDeskew or force )
      {
        runDeskew ();

        // imageView->showLayer ( "hide all", "Deskew" );
        imageScene->removeItems ();  // image size has changed
        imageView->deleteItems ();   // remove all items first

        createImgLayer ( "Deskew" );

        imageWidget->scaleView (matOut);
     }
    }
    break;

    case cRunRotate :
    {
      if ( appConfig->useRotate or force )
      {
        matOut = imageTools.rotateImage ( matIn, appConfig->useAutoRot );

        imageView->showLayer ( "hide all", "Rotate" );

        createImgLayer ( "Image rotated" );
        imageWidget->scaleView (matOut);
      }
    }
    break;

    case cRunCrop :
    {
      if ( appConfig->useCropImg or force )
      {
        matOut = imageTools.cropImage ( matIn, appConfig->useAutoCrop );

        // imageView->showLayer ( "hide all", "Crop" );

        imageScene->removeItems ();  // image size has changed
        imageView->deleteItems ();   // remove all items first

        createImgLayer ( "Image cropped" );

        imageWidget->scaleView (matOut);
      }
    }
    break;

    case cRunMargins :
    {
      if ( appConfig->addMargins or force )
      {
        matOut = imageTools.addMargins (matIn);
        createImgLayer ( "Page Margins" );

        imageView->showLayer ( "hide before", "Page Margins" );

        imageWidget->scaleView (matOut);
      }
    }
    break;

    case cRunMerge :
    {
      runMerge ();

      imageScene->removeItems ();  // image size has changed
      imageView->deleteItems ();   // remove all items first

      imageView->showLayer ( "hide all", "Merge" );

      createImgLayer ( "Merge" );

      imageWidget->scaleView (matOut);
    }
    break;
  }

}               /*-----  runTool  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  showImageInfo  of  ToolsWidget
 |
 |  Description :
 |
 |   This  function extracts  some information  from the  given Mat  like
 |   pixel type, number of channels etc. and returns it as a string list.
 |
 |  Input parameters :
 |   mat             : Reference to matrix of OpenCV
 |
 +-------------------------------------------------------------------------*/

void ToolsWidget::showImageInfo ( Mat & mat )
{
  float size;

  imageMat.evalMatInfo (mat);

  size = 1.0 * imageMat.getImgSize () / 1e+6;

  text = QString ( "%1 MB, %2 x %3, Chan: %4, Bits: %5 Type: %6 ")
           .arg ( size, 4, 'g', 2 )
           .arg ( imageMat.getRows () )
           .arg ( imageMat.getCols () )
           .arg ( imageMat.getChannels () )
           .arg ( imageMat.getDataSize () )
           .arg ( imageMat.getDataType () );

  mainWnd->updateStatusBar ( "Image", text );

}               /*-----  showImageInfo  -----*/

