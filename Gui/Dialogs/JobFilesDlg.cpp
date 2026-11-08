/*-------------------------------------------------------------------------
 |
 |  File Name   :  JobFilesDlg.cpp
 |
 |  Description :
 |
 |   This  module contains  the  class 'JobFilesDlg'  which implements  a
 |   configuration dialog for 'Job Files', they can be viewed, edited and
 |   selected.
 |
 |  Developer : Oskar Leirich                Creation date : 31.Oct.2025
 |  Modified  : Oskar Leirich                Last changes  : 13.May.2026
 |
 |
 |  This unit contains following member functions :
 |   of JobFilesDlg :
 |    JobFilesDlg               ~JobFilesDlg              connectSlots
 |    createControls            createLayout              fromGui
 |    initDialog                onCancel                  onEditFile
 |    onFileChanged             onJobsChanged             onOk
 |    onSaveFile                toGui
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "JobFilesDlg.h"
#include "../Controls/FileSelect.h"
#include "../../Config/ParManager.h"
#include "../../Main/MainWnd.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  JobFilesDlg
 |
 |  Description :
 |
 |   The constructor  initializes some  variables and sets  window title,
 |   icon and size.
 |
 |  Input parameters :
 |   parent          :  Pointer to the parent window
 |
 +-------------------------------------------------------------------------*/

JobFilesDlg::JobFilesDlg ( QWidget * parent ) : QDialog (parent)
{

  initDialog ();

  setWindowIcon  ( QIcon ( ":/Icons/NoteText" ) );
  setWindowTitle ( "Job Files" );

  resize ( 400, 600 );

}               /*----- JobFilesDlg -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  JobFilesDlg
 |
 |  Description :
 |
 |   The destructor deletes all dialog control objects.
 |
 +-------------------------------------------------------------------------*/

JobFilesDlg::~JobFilesDlg ()
{

  delete  btnCancel;
  delete  btnOk;

  delete  btnEdit;
  delete  btnSave;

  delete  fileWatch;
  delete  jobFiles;
  delete  jobView;

}               /*-----  ~JobFilesDlg  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  connectSlots  of  JobFilesDlg
 |
 |  Description :
 |
 |   This function  creates the context  menu and connects the  slots for
 |   buttons and actions.
 |
 +-------------------------------------------------------------------------*/

void JobFilesDlg::connectSlots ()
{

  connect ( btnCancel, & QPushButton::pressed,
            this,      & JobFilesDlg::onCancel );

  connect ( btnOk,     & QPushButton::pressed,
            this,      & JobFilesDlg::onOk );

  connect ( btnEdit,   & QPushButton::pressed,
            this,      & JobFilesDlg::onEditFile );

  connect ( btnSave,   & QPushButton::pressed,
            this,      & JobFilesDlg::onSaveFile );

  connect ( jobFiles,  & FileSelect::textChanged,
            this,      & JobFilesDlg::onJobsChanged );

  connect ( fileWatch, & QFileSystemWatcher::fileChanged,
            this,      & JobFilesDlg::onFileChanged );

}               /*-----  connectSlots  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createControls  of JobFilesDlg
 |
 |  Description :
 |
 |   This function creates the layout for the configuration dialog.
 |
 +-------------------------------------------------------------------------*/

void JobFilesDlg::createControls ()
{
  QFont font;

  btnCancel = new QPushButton ( "Cancel" );
  btnCancel->setIcon      ( QIcon ( ":/Icons/Cancel" ) );

  btnOk     = new QPushButton ( "Ok" );
  btnOk->setIcon          ( QIcon ( ":/Icons/OkClean" ) );

  btnEdit   = new QPushButton (this);
  btnEdit->setIcon        ( QIcon ( ":/Icons/NoteText" ) );
  btnEdit->setToolTip     ( "Edit task file" );
  btnEdit->setText        ( "Edit" );
  btnEdit->setIconSize    ( QSize ( 24, 24 ) );

  btnSave   = new QPushButton (this);
  btnSave->setIcon        ( QIcon ( ":/Icons/Save" ) );
  btnSave->setToolTip     ( "Save job file" );
  btnSave->setIconSize    ( QSize ( 24, 24 ) );

  jobFiles  = new FileSelect  (this);
  jobFiles->setHistory    ( "JobsMru", "JobFile" );
  jobFiles->setLabel      ( "Job file :" );
  jobFiles->setFilter     ( "All Files (*.*);Job Manger files (*.txt)" );
  jobFiles->setMustExist  (false);
  jobFiles->setIconSize   (24);
  jobFiles->create        ();
  jobFiles->addWidget     (btnSave);
  jobFiles->addWidget     (btnEdit);

  jobView = new QTextEdit ();
  jobView->setReadOnly    (false);

  font.setFamily    ( "Courier New" );
  font.setPointSize (10);
  jobView->setFont  (font);

}               /*-----  createControls  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLayout  of  JobFilesDlg
 |
 |  Description :
 |
 |   This function creates the layout for  the job files dialog by adding
 |   the controls to the box layouts.
 |
 +-------------------------------------------------------------------------*/

void JobFilesDlg::createLayout ()
{
  QVBoxLayout  * vBox;
  QHBoxLayout  * hBoxBtn;

  // create layout objects
  vBox    = new QVBoxLayout ();
  hBoxBtn = new QHBoxLayout ();

  // create cancel / ok button layout
  hBoxBtn->addStretch ();
  hBoxBtn->addWidget  (btnOk);
  hBoxBtn->addStretch ();
  hBoxBtn->addWidget  (btnCancel);
  hBoxBtn->addStretch ();

  // create number of algo parts and parameter
  vBox->addWidget  ( jobFiles, 0 );
  vBox->addSpacing (10);
  vBox->addWidget  ( jobView, 2 );
  vBox->addSpacing (10);
  vBox->addLayout  (hBoxBtn);

  this->setLayout  (vBox);

}               /*-----  createLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : initDialog  of  JobFilesDlg
 |
 |  Description :
 |
 |   This  member function is called from the constructor.
 |
 +-------------------------------------------------------------------------*/

void JobFilesDlg::initDialog ()
{

  fileWatch = new QFileSystemWatcher (this);

  createControls ();
  createLayout   ();
  connectSlots   ();

  toGui ();

}               /*-----  initDialog  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : fromGui  of  JobFilesDlg
 |
 |  Description :
 |
 |   This  function  reads the  state  of  the  dialog and  converts  the
 |   controls back to internal values.
 |
 +-------------------------------------------------------------------------*/

void JobFilesDlg::fromGui ()
{
  QString  name;

  name = jobFiles->getFileName ();

  // store also in parameter file
  parMan->getFileConfig ()->jobFile = name;

}               /*-----  fromGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : toGui  of  JobFilesDlg
 |
 |  Description :
 |
 |   This  function is  called after  creating the  window,  the internal
 |   values of  the dialog  variables are converted  to the  dialog (Gui)
 |   controls (widgets).
 |
 +-------------------------------------------------------------------------*/

void JobFilesDlg::toGui ()
{
  QString name;

  // read from in parameter file
  name = parMan->getFileConfig ()->jobFile;

  jobFiles->addFile (name);

}               /*-----  toGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onCancel  of  JobFilesDlg
 |
 |  Description :
 |
 |   This function  is called when  the 'onCancel' button is  pressed. In
 |   this case nothig is saved and the dialog is closed.
 |
 +-------------------------------------------------------------------------*/

void JobFilesDlg::onCancel ()
{

  close ();

}               /*-----  onCancel  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onOk  of  JobFilesDlg
 |
 |  Description :
 |
 |   This function is  called when the 'Ok' button is  pressed. It stores
 |   the current job file name in the parameter file.
 |
 +-------------------------------------------------------------------------*/

void JobFilesDlg::onOk ()
{

  fromGui ();

  onSaveFile ();

  jobFiles->save ();

  close ();

}               /*-----  onOk  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onEditFile  of  JobFilesDlg
 |
 |  Description :
 |
 |   This  function allows  to edit  the job  file in  the standard  text
 |   editor.
 |
 +-------------------------------------------------------------------------*/

void JobFilesDlg::onEditFile ()
{
  QString  name;

  name = jobFiles->getFileName ();

  if ( not QFile::exists (name) )
    fileUtils.createFile (name);

  QDesktopServices::openUrl ( QUrl ( "file:///" + name ) );

}               /*-----  onEditFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onSaveFile  of  JobFilesDlg
 |
 |  Description :
 |
 |   This function  saves changes in the  text document to the  file with
 |   the selected path and name.
 |
 +-------------------------------------------------------------------------*/

void JobFilesDlg::onSaveFile ()
{
  QString text;
  bool ok;

  if ( not QFile::exists (jobFile) )
  {
    ok = fileUtils.createFile (jobFile);

    if ( not ok )
      return;
  }

  text = jobView->toPlainText ();

  fileUtils.setFileName (jobFile);
  fileUtils.writeFile   (text);

  text = jobFiles->getFileName ();

  // store also in parameter file
  parMan->getFileConfig ()->jobFile = text;

}               /*-----  onSaveFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onFileChanged  of  JobFilesDlg
 |
 |  Description :
 |
 |   This function is called from the file system watcher when a file has
 |   changed which is in the watch list.
 |
 +-------------------------------------------------------------------------*/

void JobFilesDlg::onFileChanged ( const QString & watchName )
{
  QString name;
  QString text;

  name = jobFiles->getFileName ();

  if ( name != watchName )
    fileWatch->removePath (watchName);
  else
    onJobsChanged (name);

}               /*-----  onFileChanged  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onJobsChanged  of  JobFilesDlg
 |
 |  Description :
 |
 |   This function  is called from the  drop combo box when  the selected
 |   job file has changed.
 |
 +-------------------------------------------------------------------------*/

void JobFilesDlg::onJobsChanged ( QString fileName )
{
  QString name;
  QString text;

  name = fileName;
  name = jobFiles->getFileName ();

  jobView->clear ();    // empty text edit window

  if ( not QFile::exists (name) )
  {
    fileUtils.createFile (name);
    return;
  }
  else
    fileUtils.setFileName (name);

  fileUtils.readFile (text);

  jobView->setText (text);

  if ( name != jobFile )
  {
    fileWatch->removePath (jobFile);

    jobFile = name;   // store name

    fileWatch->addPath (name);
  }

}               /*-----  onJobsChanged  -----*/

