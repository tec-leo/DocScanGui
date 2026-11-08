/*-------------------------------------------------------------------------
 |
 |  File Name   :  ImageBar.cpp
 |
 |  Description :
 |
 |   This module contains the class 'ImageBar' which implements a toolbar
 |   for the image editing functionlity.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 28.Apr.2026
 |  Modified  : Oskar Leirich                Last changes  : 05.Aug.2026
 |
 |
 |  This unit contains following member functions :
 |   of ImageBar :
 |    ImageBar                  ~ImageBar                 buildToolBar
 |    connectActions            createActions
 |
 +-------------------------------------------------------------------------*/

#include "../Gui/ToolsWidget.h"
#include "../Gui/Dialogs/HelpAboutDlg.h"
#include "../Gui/Dialogs/ImageFileDlg.h"
#include "../Config/ParManager.h"
#include "../Gui/ImageWidget.h"
#include "../Gui/ImageEdit.h"
#include "MainWnd.h"
#include "ImageBar.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  ImageBar
 |
 |  Description :
 |
 |   The constructor  creates the actions  for the toolbar and  makes the
 |   signal / slot connections.
 |
 |  Input parameters :
 |   mainWnd         : Parent window, typically the main window
 |
 +-------------------------------------------------------------------------*/

ImageBar::ImageBar ( MainWnd * mainWnd ) : QToolBar (mainWnd)
{

  setToolButtonStyle ( Qt::ToolButtonIconOnly );
  setWindowTitle ( "Image Edit Tool Bar" );
  setObjectName  ( "ImageToolBar" );

  createActions ();

  buildToolBar ();

  connectActions ();

}               /*-----  ImageBar  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  of  ImageBar
 |
 |  Description :
 |
 |   The destructor deletes some actions.
 |
 +-------------------------------------------------------------------------*/

ImageBar::~ImageBar ()
{

  delete curves;
  delete crop;
  delete copy;
  delete rotate;
  delete deskew;
  delete merge;

  delete cancel;
  delete apply;

}               /*-----  ~ImageBar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createActions  of  ImageBar
 |
 |  Description :
 |
 |   This function creates the actions for the image toolbar.
 |
 +-------------------------------------------------------------------------*/

void ImageBar::createActions ()
{

  curves = new QAction (this);
  curves->setIcon      ( QIcon ( ":/Icons/CurveLevels" ) );
  curves->setToolTip   ( "Opens the level and curves dialog - L" );
  curves->setStatusTip ( "Opens the level and curves dialog - L" );
  curves->setShortcut  ( QKeySequence ( Qt::Key_L ) );

  deskew = new QAction (this);
  deskew->setIcon      ( QIcon ( ":/Icons/Deskew" ) );
  deskew->setToolTip   ( "Deskew image - D" );
  deskew->setStatusTip ( "Deskew image - D" );
  deskew->setShortcut  ( QKeySequence ( Qt::Key_D ) );

  rotate = new QAction (this);
  rotate->setIcon      ( QIcon ( ":/Icons/Rotate" ) );
  rotate->setToolTip   ( "Rotate image - R" );
  rotate->setStatusTip ( "Rotate image - R" );
  rotate->setShortcut  ( QKeySequence ( Qt::Key_R ) );

  crop   = new QAction (this);
  crop->setIcon        ( QIcon ( ":/Icons/Crop" ) );
  crop->setToolTip     ( "Crop image - C" );
  crop->setStatusTip   ( "Crop image - C" );
  crop->setShortcut    ( QKeySequence ( Qt::Key_C ) );

  copy   = new QAction (this);
  copy->setIcon        ( QIcon ( ":/Icons/CropSave" ) );
  copy->setToolTip     ( "Copy cropped image - K" );
  copy->setStatusTip   ( "Copy cropped image - K" );
  copy->setShortcut    ( QKeySequence ( Qt::Key_K ) );

  merge  = new QAction (this);
  merge->setIcon       ( QIcon ( ":/Icons/MergeImg" ) );
  merge->setToolTip    ( "Merge two images" );
  merge->setStatusTip  ( "Merge two images" );
  merge->setShortcut   ( QKeySequence ( Qt::Key_M ) );

  cancel = new QAction (this);
  cancel->setIcon      ( QIcon ( ":/Icons/Cancel" ) );
  cancel->setToolTip   ( "Cancel current action" );
  cancel->setStatusTip ( "Cancel current action" );
  cancel->setShortcut  ( QKeySequence::Cancel );

  apply  = new QAction ( "Apply", this );
  apply->setIcon       ( QIcon ( ":/Icons/OkClean" ) );
  apply->setToolTip    ( "Apply current action" );
  apply->setStatusTip    ( "Apply current action" );
  apply->setShortcut   ( Qt::Key_Return );

}               /*-----  createActions  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  buildToolBar  of  ImageBar
 |
 |  Description :
 |
 |   This function builds the ToolBar by adding all actions.
 |
 +-------------------------------------------------------------------------*/

void ImageBar::buildToolBar ()
{

  addAction (curves);
  addAction (crop);
  addAction (copy);
  addAction (rotate);
  addAction (deskew);
  addAction (merge);

  addSeparator ();

  addAction (cancel);
  addAction (apply);

}               /*-----  buildToolBar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  connectActions  of  ImageBar
 |
 |  Description :
 |
 |   This function makes the connections for the edit action to the image
 |   widget.
 |
 +-------------------------------------------------------------------------*/

void ImageBar::connectActions ()
{
  ImageWidget * imageWidget;
  ImageEdit   * imageEdit;

  imageWidget = mainWnd->getImageWidget ();
  imageEdit   = imageWidget->getImageEdit ();

  // connections for deskewing, rotating and croping
  connect ( apply,     & QAction::triggered,
            imageEdit, & ImageEdit::onApply );

  connect ( cancel,    & QAction::triggered,
            imageEdit, & ImageEdit::onCancel );

  connect ( curves,    & QAction::triggered,
            imageEdit, & ImageEdit::onCurves );

  connect ( deskew,    & QAction::triggered,
            imageEdit, & ImageEdit::onDeskew );

  connect ( rotate,    & QAction::triggered,
            imageEdit, & ImageEdit::onRotate );

  connect ( crop,      & QAction::triggered,
            imageEdit, & ImageEdit::onCrop );

  connect ( copy,      & QAction::triggered,
            imageEdit, & ImageEdit::onCopy );

  connect ( merge,     & QAction::triggered,
            imageEdit, & ImageEdit::onMerge );

}               /*-----  connectActions  -----*/

