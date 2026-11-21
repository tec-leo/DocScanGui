/*-------------------------------------------------------------------------
 |
 |  File Name   :  MainBar.cpp
 |
 |  Description :
 |
 |   This module contains the class 'MainBar' which implements the main
 |   toolbar of DocScanGui.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 24.Jul.2023
 |  Modified  : Oskar Leirich                Last changes  : 18.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of MainBar :
 |    MainBar                   ~MainBar                  buildToolBar
 |    connectActions            createActions             helpAbout
 |    helpContents              onBtnConfig
 |
 +-------------------------------------------------------------------------*/

#include "../Gui/ToolsWidget.h"
#include "../Gui/Dialogs/HelpAboutDlg.h"
#include "../Gui/Dialogs/ImageFileDlg.h"
#include "../Config/ParManager.h"
#include "MainWnd.h"
#include "MainBar.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  MainBar
 |
 |  Description :
 |
 |   The constructor sets  the toolbar title and creates  the actions for
 |   the toolbar and makes the signal / slot connections.
 |
 |  Input parameters :
 |   mainWnd         : Parent window, typically the main window
 |
 +-------------------------------------------------------------------------*/

MainBar::MainBar ( MainWnd * mainWnd ) : QToolBar (mainWnd)
{

  setToolButtonStyle ( Qt::ToolButtonIconOnly );
  setWindowTitle ( "Main Tool Bar" );
  setObjectName  ( "MainToolBar" );

  createActions ();

  buildToolBar ();

  connectActions ();

}               /*-----  MainBar  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  of  MainBar
 |
 |  Description :
 |
 |   The destructor deletes the actions.
 |
 +-------------------------------------------------------------------------*/

MainBar::~MainBar ()
{

  delete  load;
  delete  save;
  delete  saveAs;
  delete  runTools;
  delete  exit;

  delete  editJobs;
  delete  runJobs;

  delete  about;
  delete  help;

}               /*-----  ~MainBar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createActions  of  MainBar
 |
 |  Description :
 |
 |   This function creates the actions for toolbar of the main window. To
 |   add 'showStatusText' is normally not necessary as main window is the
 |   parent of this bar.
 |
 +-------------------------------------------------------------------------*/

void MainBar::createActions ()
{
  QMenu * mruLoadMenu;

  mruLoadMenu = new QMenu (this);

  // actions for parameter
  load = new QAction       (this);
  load->setIcon            ( QIcon ( ":/Icons/Open" ) );
  load->setToolTip         ( "Load configuration" );
  load->setStatusTip       ( "Load the configuration (parameter)" );
  load->showStatusText     (mainWnd);

  parMan->getConfigMRU ()->addActions (mruLoadMenu);
  load->setMenu (mruLoadMenu);

  save = new QAction       (this);
  save->setIcon            ( QIcon ( ":/Icons/Save" ) );
  save->setToolTip         ( "Save configuration - Alt-S" );
  save->setStatusTip       ( "Save the configuration (parameter)" );
  save->showStatusText     (mainWnd);
  save->setShortcut        ( QKeySequence ( Qt::ALT | Qt::Key_S ) );

  saveAs = new QAction     (this);
  saveAs->setIcon          ( QIcon ( ":/Icons/SaveAs" ) );
  saveAs->setToolTip       ( "Save configuration as" );
  saveAs->setStatusTip     ( "Save the configuration with a new name" );
  saveAs->showStatusText   (mainWnd);

  // actions for tools and jobs
  runTools = new QAction   (this);
  runTools->setIcon        ( QIcon ( ":/Icons/RunTools" ) );
  runTools->setToolTip     ( "Run the selected tools - Alt 1" );
  runTools->setStatusTip   ( "Run the selected tools" );
  runTools->setShortcut    ( QKeySequence ( Qt::ALT | Qt::Key_1 ) );
  runTools->showStatusText (mainWnd);

  editJobs = new QAction   (this);
  editJobs->setIcon        ( QIcon ( ":/Icons/NoteText" ) );
  editJobs->setToolTip     ( "Edit the job file - Alt 2" );
  editJobs->setStatusTip   ( "Edit the job file" );
  editJobs->setShortcut     ( QKeySequence ( Qt::ALT | Qt::Key_2 ) );
  editJobs->showStatusText (mainWnd);

  runJobs = new QAction    (this);
  runJobs->setIcon         ( QIcon ( ":/Icons/RunJobs" ) );
  runJobs->setToolTip      ( "Execute the job file - Alt - 3" );
  runJobs->setStatusTip    ( "Execute the jobs  from the job file" );
  runJobs->setShortcut     ( QKeySequence ( Qt::ALT | Qt::Key_3 ) );
  runJobs->showStatusText  (mainWnd);

  config = new QAction    (this);
  config->setIcon         ( QIcon ( ":/Icons/Configure" ) );
  config->setToolTip      ( "Image and OCR settings - Alt 4" );
  config->setStatusTip    ( "Image and OCR settings" );
  config->setShortcut     ( QKeySequence ( Qt::ALT | Qt::Key_4 ) );
  config->showStatusText  (mainWnd);

  // actions for help and exit
  about = new QAction      (this);
  about->setIcon           ( QIcon ( ":/Icons/HelpAbout" ) );
  about->setToolTip        ( "About" );
  about->setStatusTip      ( "Show the application's About box" );
  about->showStatusText    (mainWnd);

  help = new QAction       (this);
  help->setIcon            ( QIcon ( ":/Icons/HelpBook" ) );
  help->setToolTip         ( "Help Contents" );
  help->setStatusTip       ( "Show the help contents" );
  help->showStatusText     (mainWnd);

  exit = new QAction       (this);
  exit->setIcon            ( QIcon ( ":/Icons/Exit" ) );
  exit->setToolTip         ( "Exit the application" );
  exit->setStatusTip       ( "Exit the application" );
  exit->showStatusText     (mainWnd);

}               /*-----  createActions  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  buildToolBar  of  MainBar
 |
 |  Description :
 |
 |   This function builds the toolBar by adding all actions.
 |
 +-------------------------------------------------------------------------*/

void MainBar::buildToolBar ()
{

  addAction (load);
  addAction (save);
  addAction (saveAs);

  addSeparator ();

  addAction (runTools);
  addAction (editJobs);
  addAction (runJobs);

  addSeparator ();

  addAction (config);
  addAction (about);
  addAction (help);

  addSeparator ();

  addAction (exit);

}               /*-----  buildToolBar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  connectActions  of  MainBar
 |
 |  Description :
 |
 |   This function makes the connections for the main toolbar.
 |
 +-------------------------------------------------------------------------*/

void MainBar::connectActions ()
{
  ToolsWidget * toolsWnd;

  toolsWnd = mainWnd->getToolsWidget ();

  // connections for parameter
  connect ( load,     & QAction::triggered,
            parMan,   & ParManager::onLoadConfig );

  connect ( save,     & QAction::triggered,
            parMan,   & ParManager::onSaveConfig );

  connect ( saveAs,   & QAction::triggered,
            parMan,   & ParManager::onSaveConfigAs );

  connect ( runTools, & QAction::triggered,
            toolsWnd, & ToolsWidget::onRunChain );

  connect ( editJobs, & QAction::triggered,
            mainWnd,  & MainWnd::onEditJobs );

  connect ( runJobs,  & QAction::triggered,
            mainWnd,  & MainWnd::onRunJobs );

  connect ( config,   & QAction::triggered,
            this,     & MainBar::onBtnConfig );

  connect ( about,    & QAction::triggered,
            this,     & MainBar::helpAbout );

  connect ( help,     & QAction::triggered,
            this,     & MainBar::helpContents );

  connect ( exit,     & QAction::triggered,
            mainWnd,  & MainWnd::close );

}               /*-----  connectActions  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  helpAbout  of  MainBar
 |
 |  Description :
 |
 |   This function shows the 'about' dialog.
 |
 +-------------------------------------------------------------------------*/

void MainBar::helpAbout ()
{
  HelpAboutDlg  aboutDlg (this);

  aboutDlg.exec ();

}               /*-----  helpAbout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  helpContents  of  MainBar
 |
 |  Description :
 |
 |   This function shows  the 'content' of the user  manual. Therefore it
 |   locates the pdf file and opens  it or extracts is from the resources
 |   it it does not exist.
 |
 +-------------------------------------------------------------------------*/

void MainBar::helpContents ()
{
  QString cwd;
  QString qrcName;
  QString name;
  QFile   file;

  cwd  = QDir::currentPath ();
  name = cwd + "/" + cProgramName + ".pdf";

  if ( not QFile::exists (name) )
  {
    qrcName = ":/Doc/" +  cProgramName + ".pdf";
    file.setFileName (qrcName);

    if ( not file.open ( QIODevice::ReadOnly ) )
    {
      QMessageBox::warning ( this, "Cannot read manual file", name );
      return;
    }

    file.copy (name);

    file.close ();
  }

  QDesktopServices::openUrl ( QUrl ( "file:///" + name ) );

}               /*-----  helpContents  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onBtnConfig  of  MainBar
 |
 |  Description :
 |
 |   This function  is called  when the 'Config'  button is  pressed.  It
 |   opens a dialog with the configuration options for loading and saving
 |   the image files and the OCR options.
 |
 +-------------------------------------------------------------------------*/

void MainBar::onBtnConfig ()
{
  ImageFileDlg  imgDlg (this);

  imgDlg.exec ();

}               /*-----  MainBar  -----*/

