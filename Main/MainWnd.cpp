/*-------------------------------------------------------------------------
 |
 |  File Name   :  MainWnd.cpp
 |
 |  Description :
 |
 |   This module implements the  class 'MainWnd' which contains functions
 |   for  improving and  optimizing scanned  documents with  the help  of
 |   OpenCV. It  allows to  autorotate, deskew  images and  increases the
 |   contrast with several different methods.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 25.Nov.2024
 |  Modified  : Oskar Leirich                Last changes  : 18.Jun.2026
 |
 |
 |  This class contains following member functions :
 |   MainWnd :
 |    MainWnd                   ~MainWnd                  closeEvent
 |    closeMessageBox           connectSlots              createLayout
 |    createObjects             createSettings            createStatusBar
 |    createToolBar             execConfig                executeJob
 |    fromGui                   init                      loadAppState
 |    loadConfig                onEditJobs                onRunJobs
 |    saveAppState              saveConfig                setPickerCursor
 |    showMessageBox            timerEvent                toGui
 |    updateStatusBar
 |
 +-------------------------------------------------------------------------*/

#include "MainWnd.h"
#include "MainBar.h"
#include "ImageBar.h"
#include "../Gui/Dialogs/CurvesDialog.h"
#include "../Gui/Dialogs/JobFilesDlg.h"
#include "../Gui/ImageWidget.h"
#include "../Gui/ToolsWidget.h"
#include "../Config/ParManager.h"


/*-----------------------------------------*/
/*   C L A S S - D e c l a r a t i o n s   */
/*-----------------------------------------*/

MainWnd    * mainWnd = nullptr;
ParManager * parMan  = nullptr;
JobManager * jobMan  = nullptr;

/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  MainWnd
 |
 |  Description :
 |
 |   The  constructor initializes  some variables,  sets the  main window
 |   icon and title.
 |
 +-------------------------------------------------------------------------*/

MainWnd::MainWnd ( QWidget * parent, Qt::WindowFlags flags )
                                          : QMainWindow ( parent, flags )
{

  flags = windowFlags ();
  flags = flags  | Qt::Window | Qt::WindowTitleHint |
          Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint;

  setWindowFlags (flags);

  setWindowIcon ( QIcon ( ":/Icons/MainApp" ) );

  mainWnd = this;

  setMinimumSize ( 700, 700 );

  init ();

  if ( cmdMode and ( jobMan->useGui == 0 ) )
    hide ();
  else
    show ();

  if (cmdMode)
    jobMan->execJobs ();

}              /*-----  MainWnd  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  MainWnd
 |
 |  Description :
 |
 |   The destructor destroys some objcts.
 |
 +-------------------------------------------------------------------------*/

MainWnd::~MainWnd ()
{

  delete  msgBox;

  delete  regFile;
  delete  jobMan;

  delete  imageWidget;
  delete  toolsWidget;

}              /*----- ~MainWnd -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  closeEvent  of  MainWnd
 |
 |  Description :
 |
 |   This function is called when this  class is going to be closed. Then
 |   the application state is saved and the event is accecpted.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::closeEvent ( QCloseEvent * event )
{
  QDateTime date;

  date = QDateTime::currentDateTime ();
  text = cProgramName + " ended at : " + date.toString ( Qt::TextDate );
  msgLog.putInfo ( cProgramName, "Main Window closed", text );

  msgLog.writeFile ();

  saveAppState ();

  event->accept ();

}               /*----- closeEvent -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : init  of  MainWnd
 |
 |  Description :
 |
 |   This function is called from the constructor.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::init ()
{
  QDateTime date;

  loaded     = false;
  cursor     = nullptr;
  configName = "";
  defaultIni = "DefaultConfig.ini";

  createSettings  ();
  createObjects   ();

  jobMan->evalCmdLine ();
  cmdMode = jobMan->getCmdMode ();

  createToolBar   ();
  createStatusBar ();

  createLayout    ();
  connectSlots    ();

  loadAppState    ();

  // delete Log file before loading parameters
  msgLog.clear ();

  text = regFile->fileName ();
  text = fileUtils.absolutePath (text) + "/" + cProgramName + ".log";

  msgLog.setFileName (text);
  msgLog.deleteFile ();

  parMan->load ();     // calls also loadConfig --> toGui ();

  imgName = imageWidget->getImageName ();

  updateStatusBar ( "Config", configName );

  date = QDateTime::currentDateTime ();
  text = cProgramName + " started at : " + date.toString ( Qt::TextDate );
  msgLog.putInfo ( cProgramName, "Init Main Window", text );

  // command mode, do not open message box
  msgLog.setGuiApp ( not cmdMode );

  timerId = 0;
  startTimer (500);

}               /*-----  init  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  connectSlots  of  MainWnd
 |
 |  Description :
 |
 |   This  function  creates the  signal  slot  connections for  buttons,
 |   widgets, sliders etc.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::connectSlots ()
{

}               /*-----  connectSlots  -----*/


/*-------------------------------------------------------------------------
 |
 |  Memberfunction :  createObjects  of  MainWnd
 |
 |  Description :
 |
 |   This  function  creates  the  gui  objects  like  images  tab,  file
 |   explorer, config pages etc.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::createObjects ()
{
  QFont font;

  font.setFamily    ( "Courier New" );
  font.setPointSize (10);
  font.setBold      (true);

  // imageWidget must be created before the tools window
  imageWidget = new ImageWidget (this);
  imageView   = imageWidget->getImageView ();

  toolsWidget = new ToolsWidget (this);
  jobMan      = new JobManager ();
  msgBox      = new QMessageBox (this);


}               /*-----  createObjects  -----*/


/*-------------------------------------------------------------------------
 |
 |  Memberfunction :  createSettings  of  MainWnd
 |
 |  Description :
 |
 |   This  function  creates  the  gui  objects  like  images  tab,  file
 |   explorer, config pages etc.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::createSettings ()
{
  FileUtils fileUtils;
  QString   fileName;

  regFile    = new RegFile ();  // values are set in MainApp
  fileName   = regFile->fileName ();
  defaultIni = fileUtils.absolutePath (fileName) + "/DefaultConfig.ini";

  // create the parameter manager, must be one of the first steps
  parMan = new ParManager ();

  // in command mode configuration file name could already be set
  if ( configName.isEmpty () )
  {
    // check if another file name is in the ini file
    if ( regFile->contains ( "LastConfig" ) )
    {
      text = regFile->value ( "LastConfig" ).toString ();

      if ( QFile::exists (text) )
        configName = text;
    }
    else
      configName = defaultIni;
  }

  // if file does not exist or control key is pressed open a select dialog
  if ( not QFile::exists (configName) )
  {
    if ( not fileUtils.createFile (configName) )    // create if possible
    {
      text = "Cannot create config file " + configName +
             ",\nusing " + defaultIni + " as default";
      msgLog.showMessage ( "Config file", text, " from Main Window" );

      configName = defaultIni;
      fileUtils.createFile (configName);
    }
  }

  parMan->init (configName);

  appConfig  = parMan->getAppConfig ();
  fileConfig = parMan->getFileConfig ();

}               /*-----  createSettings  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createToolBar  of  MainWnd
 |
 |  Description :
 |
 |   This function initializes all control bars like the button toolbars.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::createToolBar ()
{

  mainBar = new MainBar (this);
  mainBar->setIconSize ( QSize ( 34, 34 ) );

  imageBar = new ImageBar (this);
  imageBar->setIconSize ( QSize ( 34, 34 ) );

  addToolBar (mainBar);
  addToolBar (imageBar);

}               /*-----  createToolBar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createStatusBar  of  MainWnd
 |
 |  Description :
 |
 |   This function creates a statusbar.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::createStatusBar ()
{

  status     = statusBar ();

  imageInfo  = new QLabel ();
  txtInfoMsg = new QLabel ();
  statusMsg  = new QLabel ();
  configMsg  = new QLabel ();

  statusMsg->setText ( "                    " );

  imageInfo->setStyleSheet ( "margin: 1");
  imageInfo->setFrameStyle ( QFrame::Panel | QFrame::Sunken);
  imageInfo->setMinimumWidth (200);
  imageInfo->setAlignment ( Qt::AlignLeft);

  txtInfoMsg->setStyleSheet ( "margin: 1");
  txtInfoMsg->setFrameStyle ( QFrame::Panel | QFrame::Sunken);
  txtInfoMsg->setMinimumWidth (200);
  txtInfoMsg->setAlignment ( Qt::AlignCenter);

  status->addWidget          ( statusMsg,  0 );
  status->addPermanentWidget ( imageInfo,  0 );
  status->addPermanentWidget ( txtInfoMsg, 0 );
  status->addPermanentWidget ( configMsg,  0 );

  status->showMessage ( "Ready", 3000 );

}               /*-----  createStatusBar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLayout  of  MainWnd
 |
 |  Description :
 |
 |   This function creates all widgets and sets up the main window.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::createLayout ()
{
  QSplitter * splitter;
  L_Int       sizes;   // relative sizes for the splitter

  splitter = new QSplitter ( Qt::Horizontal, this );
  splitter->setMouseTracking (true);

  sizes.append (70);
  sizes.append (900);

  splitter->setSizes (sizes);
  splitter->setStretchFactor ( 0, 1 );
  splitter->setStretchFactor ( 1, 3 );

  toolsWidget->setMinimumWidth (320);
  toolsWidget->setMaximumWidth (400);

  splitter->addWidget (toolsWidget);
  splitter->addWidget (imageWidget);

  setCentralWidget (splitter);

}               /*-----  createLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  loadConfig  of  MainWnd
 |
 |  Description :
 |
 |   This function loads all data from the parameter (ini) file.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::loadConfig ()
{

  toGui ();

  configName = parMan->getFileName ();

  regFile->setValue ( "Config", configName );

}               /*-----  loadConfig  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  saveConfig  of  MainWnd
 |
 |  Description :
 |
 |   This function saves all data to the parameter (ini) file.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::saveConfig ()
{

  fromGui ();

  configName = parMan->getFileName ();

  regFile->setValue ( "Config", configName );

}               /*-----  saveConfig  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fromGui  of  MainWnd
 |
 |  Description :
 |
 |   This  function reads  the values  of some  widget from  the gui  and
 |   updates the member variables with this values.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::fromGui ()
{

  imageWidget->fromGui ();
  toolsWidget->fromGui ();

}               /*-----  fromGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  toGui  of  MainWnd
 |
 |  Description :
 |
 |   This  function update  some widget  in the  gui from  values of  the
 |   member variables.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::toGui ()
{

  toolsWidget->toGui ();
  imageWidget->toGui ();

}               /*-----  toGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onEditJobs  of  MainWnd
 |
 |  Description :
 |
 |   This function is called from a toolbar button and opens the job file
 |   dialog.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::onEditJobs ()
{
  JobFilesDlg  jobDlg (mainWnd);

  jobDlg.exec ();

}               /*-----  onEditJobs  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onRunJobs  of  MainWnd
 |
 |  Description :
 |
 |   This function is called from a  toolbar button and checks if the job
 |   file exists and then runs job manager with the file.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::onRunJobs ()
{
  QString  name;

  name = parMan->getFileConfig ()->jobFile;

  if ( not QFile::exists (name) )
  {
    text =  "Job File " + name + " does not exist !!";

    QMessageBox::warning ( this, "Run Task", text );

    return;
  }

  cmdMode = true;

  jobMan->execJobs (name);

  cmdMode = false;

}               /*-----  onRunJobs  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  execConfig  of  MainWnd
 |
 |  Description :
 |
 |   This  function  loads  a  configuration   from  file  or  sets  some
 |   configuration options.   These options  have the  same name  as they
 |   appear in the configuration file.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::execConfig ()
{
  L_String keys;
  QString  name;
  QString  key;
  QString  entry;

  if ( jobInfo.command == "load-curve" )
  {
    if ( infoMap.contains ( "curve-name" ) )
    {
      name = infoMap [ "curve-name" ];

      parMan->getAppCurves ()->selectCurve (name);
    }
  }

  if ( jobInfo.command == "load-config" )
  {
    if ( infoMap.contains ( "config-file" ) )
    {
      name = infoMap [ "config-file" ];

      if ( QFile::exists (name) )
        parMan->load (name);
    }
  }

  if ( jobInfo.command == "save-config" )
  {
    if ( infoMap.contains ( "config-file" ) )
    {
      name = infoMap [ "config-file" ];
      parMan->setFileName (name);
    }
    else
      name = parMan->getFileName ();

    if ( not fileUtils.makeFilePath (name) )
    {
      text  = "Cannot create directory for " + name;
      msgLog.showError ( "Save config", text, " from Main Window" );

      return;
    }

    parMan->save ();
  }

  if ( jobInfo.command == "set-options" )
  {
    keys = infoMap.keys ();

    for ( int i = 0; i < keys.count (); i++ )
    {
      key = keys [i];

      if ( ( key == "topic" ) or( key == "command" ) )
        continue;

      entry = infoMap [key];

      // item exists in appConfig?
      if ( appConfig->haveItem (key) )
      {
        appConfig->setItemValue ( key, entry );
        continue;
      }

      // no, then check fileConfig?
      if ( fileConfig->haveItem (key) )
        fileConfig->setItemValue ( key, entry );
    }

    toGui ();
  }

}               /*-----  execConfig  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  executeJob  of  MainWnd
 |
 |  Description :
 |
 |   This function  is called when a  job was initiated from  the command
 |   line interface or a job file.   It checks what topic is involved and
 |   forwards the jobInfo to  this correspondig widget. The configuration
 |   topics are handled in 'execConfig'.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::executeJob ( JobInfo curJob )
{
  QString  name;

  isRunning = true;

  jobInfo = curJob;
  infoMap = jobInfo.infoMap;

  if ( jobInfo.topic == "config" )
  {
    execConfig ();
  }

  if ( jobInfo.topic == "image" )
  {
    imageWidget->executeJob (curJob);
  }

  if ( jobInfo.topic == "tools" )
  {
    toolsWidget->executeJob (curJob);
  }

  isRunning = false;

}               /*-----  executeJob  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : loadAppState  of  MainWnd
 |
 |  Description :
 |
 |   This function  restores the state  of this toolbars  and dockwidgets
 |   and the  position and size  of the  main window. The  information is
 |   read from the registry as a per user setting.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::loadAppState ()
{
  QByteArray state;
  QPoint     point;
  QSize      size;

  state = regFile->value ( "State" ).toByteArray ();

  if ( not state.isEmpty () )
    restoreState (state);

  point = regFile->value ( "WindowPos" ).toPoint ();
  size  = regFile->value ( "WindowSize" ).toSize ();

  if ( size.width () > 0 )
  {
    move (point);
    resize (size);
  }
  else
  {
    resize ( 1200, 800 );
  }

}               /*-----  loadAppState  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : saveAppState  of  MainWnd
 |
 |  Description :
 |
 |   This function  saves the state  of the toolbars and  dockwidgets and
 |   the position and size of the  main window. The information is stored
 |   to the registry as a per user setting.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::saveAppState ()
{
  QByteArray state;
  QString    section;
  QPoint     point;
  QSize      dim;

  state = saveState (0);

  point = pos ();
  dim   = size ();

  regFile->setValue ( "WindowPos",  point );
  regFile->setValue ( "WindowSize", dim );

  regFile->setValue ( "LastConfig", configName );
  regFile->setValue ( "State",      state );

}               /*-----  saveAppState  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setPickerCursor  of  MainWnd
 |
 |  Description :
 |
 |   This  function sets the cursor to the arrow curser or to the given cursor
 |   when the mouse is over this widget.
 |
 |  Input parameters :
 |   cursor          : Cursor to use in this widget
 |
 +-------------------------------------------------------------------------*/

void MainWnd::setPickerCursor ( QCursor * cursor )
{

  this->cursor = cursor;

  if ( cursor == nullptr )
    setCursor (Qt::ArrowCursor);
  else
    setCursor ( * cursor );

}               /*-----  setPickerCursor  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  closeMessageBox  of  MainWnd
 |
 |  Description :
 |
 |   This function closes the message box.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::closeMessageBox ()
{

  killTimer (3);

  msgBox->close ();

}               /*-----  closeMessageBox  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  showMessageBox  of  MainWnd
 |
 |  Description :
 |
 |   This function shows  a message box with the given  title and message
 |   for the  given time in  seconds. It starts  a timer which  hides the
 |   message box after the sec time.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::showMessageBox ( QString title, QString msg, int sec )
{

  msgBox->setStandardButtons ( QMessageBox::Close );
  msgBox->setWindowTitle (title);
  msgBox->setText (msg);

  timerId = 3;

  startTimer ( sec * 1000 );

  msgBox->show ();

}               /*-----  showMessageBox  -----*/

/*-------------------------------------------------------------------------
 |
 |  Member function :  timerEvent  of  MainWnd
 |
 |  Description :
 |
 |   This function is called when a timer event arrived.  Here timer 0 is
 |   used to load an image after the gui is established.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::timerEvent ( QTimerEvent * tEvent )
{
  int id;

  id = tEvent->timerId ();

  killTimer (id);

  if ( timerId == 0 )   // called from init
  {
    if ( QFile::exists (imgName) )
    {
      imageWidget->loadImage ();
    }

    if ( jobMan->getJobCount () > 0 )
    {
      timerId = 1;
      startTimer (200);    // activates job manager
    }
  }

  if ( timerId == 1 )
  {
    jobMan->execJobs ();  // job queue is already parsed
  }

  if ( timerId == 2 )
  {
    statusMsg->setText ( "" );  // delete status message
  }

  if ( timerId == 3 )
  {
    msgBox->close ();
  }

  timerId = -1;      // nothing selected

}            /*-----  timerEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  updateStatusBar  of  MainWnd
 |
 |  Description :
 |
 |   This function updates a field the  status bar. The 'topic' tells the
 |   field and the second parameter is the message.
 |
 +-------------------------------------------------------------------------*/

void MainWnd::updateStatusBar ( QString topic, QString msg )
{

  if ( topic == "Status" )
  {
    statusMsg->setText (msg);
    timerId = 2;         // delete status message
    startTimer (5000);   // in 5 seconds
  }

  if ( topic == "Image" )
    imageInfo->setText (msg);

  if ( topic == "Info" )
    txtInfoMsg->setText (msg);

  if ( topic == "Config" )
    configMsg->setText (msg);

  status->update ();

}               /*-----  updateStatusBar  -----*/

