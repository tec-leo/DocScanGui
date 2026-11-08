/*-------------------------------------------------------------------------
 |
 |  File Name   :  ImageEdit.cpp
 |
 |  Description :
 |
 |   This  module  contains  the  class  'ImageEdit'  whichcontains  some
 |   functions to 'edit' an image, this means rotating, cropping etc.
 |
 |  Developer : Oskar Leirich                Creation date : 26.Apr.2026
 |  Modified  : Oskar Leirich                Last changes  : 10.Sep.2026
 |
 |
 |  This unit contains following member functions :
 |   of ImageEdit:
 |    ImageEdit                 ~ImageEdit                connectSlots
 |    getCropRect               init                      onApply
 |    onAutoToggled             onBtnReload               onCancel
 |    onCopyCrop                onCrop                    onDeskew
 |    onExportImage             onFileChanged             onMerge
 |    onRotate                  onSaveImage               onSaveImageAs
 |    onSceneMouse              onSliderChanged           onToolsToggled
 |    onViewMoved               onZoomChanged             onZoomIn
 |    onZoomOut                 scaleView                 wheelEvent
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "ImageEdit.h"
#include "ImageWidget.h"
#include "ToolsWidget.h"
#include "../Config/ParManager.h"
#include "../Main/MainWnd.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  ImageEdit
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 |  Input parameters :
 |   parent          :  Pointer to the parent window
 |
 +-------------------------------------------------------------------------*/

ImageEdit::ImageEdit ( ImageWidget * parent ) : QWidget (parent)
{

  imageWidget = parent;

  init ();

}               /*----- ImageEdit -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  ImageEdit
 |
 |  Description :
 |
 |   The destructor deletes all  dynamically created objects if there are
 |   any.
 |
 +-------------------------------------------------------------------------*/

ImageEdit::~ImageEdit ()
{

}               /*-----  ~ImageEdit  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  connectSlots  of  ImageEdit
 |
 |  Description :
 |
 |   This  function  creates the  signal  slot  connections for  buttons,
 |   widgets, sliders etc.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::connectSlots ()
{

  connect ( imageScene, & ImageScene::mouseAction,
            this,       & ImageEdit::onSceneMouse );

}               /*-----  connectSlots  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createMenu  of  ImageEdit
 |
 |  Description :
 |
 |   This function creates the contextMenu for all edit actions.
 |
 +-------------------------------------------------------------------------*/

QMenu * ImageEdit::createMenu ()
{
  QMenu   * contextMenu;
  QAction * levels;
  QAction * deskew;
  QAction * rotate;
  QAction * crop;
  QAction * merge;

  QAction * cancel;
  QAction * apply;
  QAction * copy;

  contextMenu = new QMenu ();

  levels = contextMenu->addAction ( "Curve levels" );
  crop   = contextMenu->addAction ( "Crop image" );
  copy   = contextMenu->addAction ( "Copy image" );
  rotate = contextMenu->addAction ( "Rotate image" );
  deskew = contextMenu->addAction ( "Deskew image" );
  merge  = contextMenu->addAction ( "Merge image" );

  contextMenu->addSeparator ();

  apply  = contextMenu->addAction ( "Apply" );
  cancel = contextMenu->addAction ( "Cancel" );

  contextMenu->addSeparator ();
  detect = contextMenu->addAction ( "Auto detect" );
  tools  = contextMenu->addAction ( "First run tools" );

  detect->setCheckable (true);
  tools ->setCheckable (true);

  detect->setChecked (appConfig->autoDetect);
  tools ->setChecked (appConfig->runToolChain);

  // edit actions
  connect ( levels, & QAction::triggered,
            this,   & ImageEdit::onCurves );

  connect ( crop,   & QAction::triggered,
            this,   & ImageEdit::onCrop );

  connect ( copy,   & QAction::triggered,
            this,   & ImageEdit::onCopy );

  connect ( rotate, & QAction::triggered,
            this,   & ImageEdit::onRotate );

  connect ( deskew, & QAction::triggered,
            this,   & ImageEdit::onDeskew );

  connect ( merge,  & QAction::triggered,
            this,   & ImageEdit::onMerge );

  // apply and cancel actions
  connect ( cancel, & QAction::triggered,
            this,   & ImageEdit::onCancel );

  connect ( apply,  & QAction::triggered,
            this,   & ImageEdit::onApply );

  // apply and cancel actions
  connect ( detect, & QAction::toggled,
            this,   & ImageEdit::onAutoToggled );

  connect ( tools,  & QAction::toggled,
            this,   & ImageEdit::onToolsToggled );

  return contextMenu;

}               /*-----  createMenu  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : init  of  ImageEdit
 |
 |  Description :
 |
 |   This member function is called  from the constructor, it initializes
 |   some variables, acquires the  pointers to the application parameter.
 |   scene.  Then it  creates  the objects,  layout  and established  the
 |   signal slot connections.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::init ()
{

  appConfig  = parMan->getAppConfig ();
  imageTools.setConfig (appConfig);

  imageView   = imageWidget->getImageView  ();
  imageScene  = imageWidget->getImageScene ();

  activeMode  = cInsertNone;
  insertMode  = cInsertNone;
  cropCopy    = false;

  // not in init, because of creation order
  toolsWidget = nullptr;

  connectSlots ();

}               /*-----  init  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : copyImgRect  of  ImageEdit
 |
 |  Description :
 |
 |   This  function  is  called  after the  'Apply'  button  is  pressed,
 |   copyCrop is active and a valid cropping rectangle is selected.  Then
 |   it copies  the selected region  into a  new image with  the original
 |   name and an increasing number like (1), (2).
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::copyImgRect ()
{
  Mat     cropped;
  Mat   * lastImg;
  QString imgName;
  QString newName;
  QString help;
  QString ext;
  QString path;
  int     pos;

  lastImg = toolsWidget->getLastImage ();

  cropped = imageTools.cropImage ( * lastImg, false );

  imgName = imageWidget->getImageName ();

  pos  = imgName.lastIndexOf ( '.' );
  path = imgName.left (pos);
  ext  = imgName.mid (pos);

  // create new unique file name
  for ( int i = 0; i < 50; i++ )
  {
    help = QString ( "_(%1)" ).arg ( i + 1, 2, 10, QChar ('0') );
    newName = path + help + ext;

    if ( not QFile::exists (newName) )
      break;
  }

  toolsWidget->saveImage ( newName, cropped );

  text = "Cropped image saved as " + newName;
  mainWnd->updateStatusBar ( "Info", text );

}               /*-----  copyImgRect  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getVertices  of  ImageEdit
 |
 |  Description :
 |
 |   This function calls 'getContourPts' to  detect the outer vertices of
 |   all  contours found  in  the image.  It returns  the  points of  the
 |   rotated rectangle  of the minimum  area enclosing the points  of all
 |   contours.
 |   The vertices are then sorted so that have the order :
 |      top left, top right, bottom left, bottom right
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::getVertices ( Mat inMat, L_PointsF & vertices )
{
  V_CvPoint points;
  V_CvPoint vertPtns;
  QPointF   point;

  imageTools.getContourPts ( inMat, points );

  // sort and reduce to 4 points
  vertPtns = imageTools.getDocPoints ( image->rows, image->cols, points );

  vertices.clear ();

  for ( qint i = 0; i < vertPtns.size (); i++ )
  {
    point.setX ( vertPtns [i].x );
    point.setY ( vertPtns [i].y );

    vertices.append (point);
  }

}               /*-----  getVertices  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getCropRect  of  ImageEdit
 |
 |  Description :
 |
 |   This function  returns the  export directory, it  is taken  from the
 |   path select combobox.
 |
 +-------------------------------------------------------------------------*/

QString ImageEdit::getCropRect ()
{
  L_String margins;
  QRectF   rect;
  int      cols;
  int      rows;
  float    margin;

  rect = imageScene->getRect ();

  margins.clear ();

  rows = image->rows;
  cols = image->cols;

  margin = 1.0 - ( rows - rect.top () ) / rows;
  text.setNum ( 100.0 * margin, 'g', 3 );
  margins.append (text);

  margin = 1.0 * ( rows - rect.bottom () ) / rows;
  text.setNum ( 100.0 * margin, 'g', 3 );
  margins.append (text);

  margin = 1.0 - ( cols - rect.left () ) / cols;
  text.setNum ( 100.0 * margin, 'g', 3 );
  margins.append (text);

  margin = 1.0 * ( cols - rect.right () ) / cols;
  text.setNum ( 100.0 * margin, 'g', 3 );
  margins.append (text);

  text = margins.join ( "," );

  return text;

}               /*-----  getCropRect  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onApply  of  ImageEdit
 |
 |  Description :
 |
 |   This function  is called when the  'Ok' button is pressed.   It does
 |   not reset the points in the level curve before it closes the dialog.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::onApply ()
{
  AppConfig configBak;
  QLineF    line;
  double    angle;

  mainWnd->fromGui ();   // calls fromGui also from toolsWidget and self

  configBak = * appConfig;     // make a copy

  switch (activeMode)
  {
    case cInsertNone:
      return;
    break;

    case cInsertLine:
    {
      line  = imageScene->getLine ();
      angle = line.angle ();    // angle is measured counter-clockwise

      if ( angle > 180.0 )
        angle = angle - 360.0;

      toolsWidget->setRotation ( - angle ); // rotate in opposite direction

      appConfig->rotation   = -angle;
      appConfig->useRotate  = true;         // use angle from gui
      appConfig->useAutoRot = false;        // no autorotation

      if ( appConfig->runToolChain )
        toolsWidget->runChainTo (cRunRotate); // apply including rotate
      else
        toolsWidget->runTool (cRunRotate);    // run only rotate
    }
    break;

    case cInsertRect:
    {
      text = getCropRect ();

      appConfig->cropMargins = text;
      appConfig->useCropImg  = true;         // use page margins from gui
      appConfig->useAutoCrop = false;        // no autorotation

      configBak.cropMargins = appConfig->cropMargins;    // store also in backup

      if (cropCopy)
      {
        if ( appConfig->runToolChain )
          toolsWidget->runChainTo ( cRunCrop - 1 ); // apply without crop

        copyImgRect ();
      }
      else
      {
        if ( appConfig->runToolChain )
          toolsWidget->runChainTo (cRunCrop); // apply including crop
        else
          toolsWidget->runTool (cRunCrop);    // run only crop
      }
    }
    break;

    case cInsertPath:
    {
      appConfig->useDeskew  = true;

      if ( appConfig->runToolChain )
        toolsWidget->runChainTo (cRunDeskew); // apply including deskew
      else
        toolsWidget->runTool (cRunDeskew);    // run only deskew
    }
    break;
  }

  * appConfig = configBak;     // copy back original settings

  onCancel ();    // finish current action

}               /*-----  onApply  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onCancel  of  ImageEdit
 |
 |  Description :
 |
 |   This function  is called  when the 'Cancel'  button is  pressed.  It
 |   removes all items in the image scene clear all layers.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::onCancel ()
{

  cropCopy = false;

  if ( activeMode == cInsertLine )
    toolsWidget->setRotation (0);   // reset rotation angle

  mainWnd->fromGui ();   // calls fromGui also from toolsWidget

  activeMode = cInsertNone;
  insertMode = cInsertNone;

  imageScene->removeItems ();

  imageScene->setInsertMode (insertMode);

}               /*-----  onCancel  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onAutoToggled  of  ImageEdit
 |
 |  Description :
 |
 |   This function is called when the  'Auto detect' action is toggled in
 |   the context menu. It just stores the new state.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::onAutoToggled ( bool checked )
{

  appConfig->autoDetect = checked;

}               /*-----  onAutoToggled  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onToolsToggled  of  ImageEdit
 |
 |  Description :
 |
 |   This function is called when the 'Run Tools first' action is toggled
 |   in the context menu. It just stores the new state.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::onToolsToggled ( bool checked )
{

  appConfig->runToolChain = checked;

}               /*-----  onToolsToggled  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onDeskew  of  ImageEdit
 |
 |  Description :
 |
 |   This function  is called  when the 'Deskew'  button is  pressed.  It
 |   starts a path selection with 4  points. After applying the action it
 |   applies a perspective transformation of image to the selected path.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::onDeskew ()
{
  L_PointsF points;
  QRectF    rect;

  if ( not prepareEdit ( cRunDeskew - 1 ) )
    return;

  activeMode = cInsertPath;

  points.clear ();

  if ( appConfig->autoDetect )
  {
    getVertices ( * image, points );
  }

  if ( ( not appConfig->autoDetect ) or ( points.size () != 4 ) )
  {
    rect.setX      ( 0.1 * image->cols );
    rect.setY      ( 0.1 * image->rows );
    rect.setWidth  ( 0.8 * image->cols );
    rect.setHeight ( 0.8 * image->rows );

    points.clear ();
    points.append ( rect.topLeft     () );
    points.append ( rect.topRight    () );
    points.append ( rect.bottomLeft  () );
    points.append ( rect.bottomRight () );
  }

  insertMode = cMovePath;    // and start with moving the lines

  imageScene->makePath (points);   // make the line in image scene

  imageScene->setInsertMode (insertMode);

}               /*-----  onDeskew  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onCurves  of  ImageEdit
 |
 |  Description :
 |
 |   This function  is called  when the 'Curves'  button is  pressed.  It
 |   forwards  the function  call  to  the tool  widget  which creates  a
 |   'CurvesDialog'.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::onCurves ()
{

  if ( toolsWidget == nullptr )
    toolsWidget = mainWnd->getToolsWidget ();

  toolsWidget->onCurvesDlg ();

}               /*-----  onCurves  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onCopy  of  ImageEdit
 |
 |  Description :
 |
 |   This  function is  called when  the  'Copy' button  is pressed.   It
 |   starts a  rectangle selection. After  applying the action  it copies
 |   the selected image portion to a new file.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::onCopy ()
{

  onCrop ();

  cropCopy = true;

}               /*-----  onCopy  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onCrop  of  ImageEdit
 |
 |  Description :
 |
 |   This  function is  called when  the  'Crop' button  is pressed.   It
 |   starts a rectangle selection. After applying the action it crops the
 |   selected image portion to the selected rectangle.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::onCrop ()
{
  QRectF   rect;
  cv::Rect box;

  if ( not prepareEdit ( cRunCrop - 1 ) )
    return;

  if (cropCopy)
    imageScene->setPenColor ( Color::darkgreen );

  activeMode = cInsertRect;

  if ( appConfig->autoDetect )
  {
    box = imageTools.getContourRect ( * image );

    if ( ( box.width > 100 ) and ( box.height > 100 ) )
    {
      rect.setX      ( box.x - 3 );   // add some pixel margin
      rect.setY      ( box.y - 3 );
      rect.setWidth  ( box.width  + 6 );
      rect.setHeight ( box.height + 6 );
    }
    else
    {
      rect.setX      (5);   // add some pixel margin
      rect.setY      (5);
      rect.setWidth  ( image->cols - 10 );
      rect.setHeight ( image->rows - 10 );
    }

    insertMode = cMoveRect;    // and start with moving the lines

    imageScene->makeRect (rect);   // make the line in image scene

  }
  else
  {
    insertMode = cInsertRect;
  }

  imageScene->setInsertMode (insertMode);

}               /*-----  onCrop  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onMerge  of  ImageEdit
 |
 |  Description :
 |
 |   This function  is called  when the 'Cancel'  button is  pressed.  It
 |   resets the points in the level curve before it closes the dialog.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::onMerge ()
{
  QString imgName;
  QString filter;

  filter = "*.png;*.jpg;*.tiff";

  if ( insertMode != cInsertNone )
    onCancel ();    // cancel last action

  mainWnd->fromGui ();

  imgName = imageWidget->getImageName ();

  if ( not fileUtils.selectFile ( imgName, filter, true, true ) )
    return;

  appConfig->mergeImage = imgName;

  toolsWidget->runTool (cRunMerge);    // run merge images

}               /*-----  onMerge  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onRotate  of  ImageEdit
 |
 |  Description :
 |
 |   This function  is called  when the 'Rotate'  button is  pressed.  It
 |   allows to visually  draw a line with with two  end points. The angle
 |   of  the line  can then  be applied  to the  image. If  autodetect is
 |   checked then  an automatic  rotation analysis of  the image  is done
 |   first and a default line with the found rotation is created.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::onRotate ()
{
  QLineF  line;
  QPointF p1;
  QPointF p2;
  float   angle;

  if ( insertMode != cInsertNone )
    onCancel ();    // cancel last action

  mainWnd->fromGui ();

  if (appConfig->runToolChain)
    toolsWidget->runChainTo (cRunDeskew);    // apply including deskew

  image = toolsWidget->getLastImage ();

  activeMode = cInsertLine;

  imageScene->removeItems ();

  imageScene->setPenColor ( Color::blue );
  imageScene->setPenWidth (5.0);

  if ( appConfig->autoDetect )
  {
    // OpenCV counts angle clockwise, change to counter-clockwise
    angle = - imageTools.detectAngle ( * image );

    text.setNum ( angle, 'g', 3 );
    mainWnd->updateStatusBar ( "Info", "Angle: "  + text + " deg" );

    toolsWidget->setRotation (-angle);   // rotate in opposite direction

    // create a default line with the given angle

    p1.setX ( image->cols * 0.2 );  // point 1 at 20 % of the image width
    p1.setY ( image->rows * 0.2 );  // point 1 at 20 % of the image height

    p2.setX ( image->cols * 0.8 );  // point 2 at 80 % of the image width
    p2.setY ( image->rows * 0.2 );  // point 2 at 20 % of the image height

    line.setP1 (p1);
    line.setP2 (p2);

    line.setAngle (angle);       // counts angle counter-clockwise

    p2 = line.p2 ();

    imageScene->makeLine ( p1, p2 );   // make the line in image scene
    insertMode = cMoveLine;            // and start with moving the line
  }
  else
  {
    insertMode = cInsertLine;
  }

  imageScene->setInsertMode (insertMode);

}               /*-----  onRotate  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onSceneMouse  of  ImageEdit
 |
 |  Description :
 |
 |   This function is called from the image scene when a mouse action has
 |   finished  during  an  insert  mode   action  like  rotate,  crop  or
 |   deskew. The mode is then changed  to a move item action which allows
 |   to modify the drawn line, rectangle or path.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::onSceneMouse ( QString action )
{
  QLineF line;
  double angle;

  switch (insertMode)
  {
    case cInsertNone:
    break;

    case cInsertLine:
    {
      if ( action == "release line" )
      {
        insertMode = cMoveLine;

        line = imageScene->getLine ();
        angle = line.angle ();

        toolsWidget->setRotation ( - angle ); // rotate in opposite direction
      }
    }
    break;

    case cInsertRect:
    {
      if ( action == "release rect" )
        insertMode = cMoveRect;
    }
    break;

    case cMoveLine:
    {
      line = imageScene->getLine ();
      angle = line.angle ();

      toolsWidget->setRotation ( - angle ); // rotate in opposite direction
    }
    break;

    case cInsertPath:
    {
    }
    break;

    case cMoveView:
    {
      if ( action == "view moved" )
        insertMode = cInsertNone;
    }
    break;

  }

  imageScene->setInsertMode (insertMode);

}               /*-----  onSceneMouse  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : prepareEdit  of  ImageEdit
 |
 |  Description :
 |
 |   This function is called when the  one of the edit buttons is pressed
 |   as a preparation for the edit.   It updates the pointers to the tool
 |   widget, image etc.  and runs the  tool chain if configured.  Then it
 |   removede all items from the scene and sets pen color and width.
 |
 +-------------------------------------------------------------------------*/

bool ImageEdit::prepareEdit ( int runIdx )
{

  if ( insertMode != cInsertNone )
  {
    onCancel ();    // cancel last action
    return false;
  }

  // not in init, because of creation order
  if ( toolsWidget == nullptr )
    toolsWidget = mainWnd->getToolsWidget ();

  image = toolsWidget->getLastImage ();

  if ( ( image->cols < 30 ) or ( image->rows < 30 ) )
  {
    mainWnd->showMessageBox ( "Image Edit", "Image is too small", 5 );
    return false;
  }

  mainWnd->fromGui ();

  if (appConfig->runToolChain)
    toolsWidget->runChainTo (runIdx);    // apply including deskew

  imageScene->removeItems ();

  imageScene->setPenColor ( Color::blue );
  imageScene->setPenWidth (6.0);

  return true;

}               /*-----  prepareEdit  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  toGui  of  ImageEdit
 |
 |  Description :
 |
 |   This  function updates  the  actions which  are  depending from  the
 |   configuration after the configuration is loaded.
 |
 +-------------------------------------------------------------------------*/

void ImageEdit::toGui ()
{

  detect->setChecked (appConfig->autoDetect);
  tools ->setChecked (appConfig->runToolChain);

}               /*-----  toGui  -----*/

