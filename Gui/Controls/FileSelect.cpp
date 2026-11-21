/*-------------------------------------------------------------------------
 |
 |  File Name   :  FileSelect.cpp
 |
 |  Description :
 |
 |   This module contains the class  'FileSelect' which implements a file
 |   selection widget as a reuseable  component.  It consists of a label,
 |   a drop  combo box  with a history  and a browse  button to  select a
 |   file.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 30.Oct.2025
 |  Modified  : Oskar Leirich                Last changes  : 09.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of FileSelect :
 |    FileSelect                ~FileSelect               addFile
 |    addWidget                 connectSlots              create
 |    createLayout              fileChanged               fromGui
 |    getFileName               load                      onSelectFile
 |    onTextChanged             onTextDropped             save
 |    setEditable               setFilter                 setHistory
 |    setLabel                  setLoadFile               setMustExist
 |    toGui
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "FileSelect.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  FileSelect
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 |  Input parameters :
 |   parent          : parent widget
 |
 +-------------------------------------------------------------------------*/

FileSelect::FileSelect ( QWidget * parent ) : QWidget (parent)
{

  filter      = "All Files (*.*)";
  iconName    = ":/Icons/Open";
  mustExist   = true;
  withHistory = false;
  loadFile    = true;
  editable    = false;
  iconSize    = -1;    // use default

  dropCombo = nullptr;
  btnBrowse = nullptr;
  lbCombo   = nullptr;

}               /*-----  FileSelect  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  FileSelect
 |
 |  Description :
 |
 |   The destructor saves the history and deletes dropCombo and label.
 |
 +-------------------------------------------------------------------------*/

FileSelect::~FileSelect ()
{

  save ();

  delete  dropCombo;
  delete  lbCombo;

}                   /*-----  ~FileSelect  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  addFile  of  FileSelect
 |
 |  Description :
 |
 |   This function call 'fileChanged' to check if the file exist with the
 |   given name and adds it to the combo box and the history list.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::addFile ( QString fileName )
{

  fileChanged (fileName);


}               /*-----  addFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  addWidget  of  FileSelect
 |
 |  Description :
 |
 |   This function is a convenient function to easily add a widget at the
 |   end without dealing with the layout manager.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::addWidget ( QWidget * widget )
{

  hLayout->addSpacing (5);
  hLayout->addWidget  (widget);

}               /*-----  addWidget  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLayout  of  FileSelect
 |
 |  Description :
 |
 |   This function creates the layout for the file select widget.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::createLayout ()
{

  hLayout   = new QHBoxLayout ();

  dropCombo = new DropCombo (this);
  dropCombo->setEditable (editable);
  dropCombo->setInsertPolicy ( QComboBox::InsertAtTop );

  btnBrowse = new QPushButton (this);
  btnBrowse->setIcon ( QIcon (iconName) );
  btnBrowse->setToolTip ( "Browse for file" );

  if ( iconSize >= 16 )
    btnBrowse->setIconSize ( QSize ( iconSize, iconSize ) );

  lbCombo   = new QLabel ( labelText, this );

  hLayout->addWidget  (lbCombo);
  hLayout->addSpacing (5);
  hLayout->addWidget  ( dropCombo, 3 );
  hLayout->addSpacing (5);
  hLayout->addWidget  (btnBrowse);

  hLayout->setContentsMargins ( 1, 1, 1, 1 );

  setLayout (hLayout);

}               /*-----  createLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  connectSlots  of  FileSelect
 |
 |  Description :
 |
 |   This function creates the signal slot connections.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::connectSlots ()
{

  connect ( btnBrowse,  SIGNAL ( clicked () ),
            this,       SLOT   ( onSelectFile () ) );

  connect ( dropCombo,  SIGNAL ( currentIndexChanged (int) ),
            this,       SLOT   ( onIndexChanged      (int) ) );

  connect ( dropCombo,  SIGNAL ( textDropped   (QString) ),
            this,       SLOT   ( onTextDropped (QString) ) );

}               /*-----  connectSlots  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  create  of  FileSelect
 |
 |  Description :
 |
 |   This function  creates the  layout, loads the  history and  adds the
 |   values to the dropCombo.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::create ()
{

  createLayout ();

  load ();
  toGui ();

  connectSlots ();

}               /*-----  create  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  clearFiles  of  FileSelect
 |
 |  Description :
 |
 |   This function removes  all files from the history list  which do not
 |   exit.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::clearFiles ()
{
  L_String files;
  QString  entry;

  files = history.getItems ();

  for ( int i = 0; i < files.count (); i++ )
  {
    entry = files [i];

    if ( not QFile::exists (entry) )
      history.deleteItem (entry);
  }

  dropCombo->blockSignals (true);

  dropCombo->clear ();
  dropCombo->addItems ( history.getItems () );

  dropCombo->blockSignals (false);

}               /*-----  clearFiles  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fileChanged  of  FileSelect
 |
 |  Description :
 |
 |   This function  is called  when a  new file is  dropped on  the combo
 |   box. Then the  new file is added  to the combo box  and the history.
 |   The signals from dropCombo are blocked during the operation.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::fileChanged ( QString fileName )
{
  int index;

  fileName = QDir::fromNativeSeparators (fileName);

  if ( fileName.isEmpty () )
    return;

  if ( not QFile::exists (fileName) and mustExist )
    return;

  if ( not QFile::exists (fileName) and not mustExist )
  {
    fileUtils.createFile (fileName);
  }

  dropCombo->blockSignals (true);

  index = dropCombo->findText (fileName);

  if ( not QFile::exists (fileName) )
  {
    history.deleteItem (fileName);

    if ( index >= 0 )
      dropCombo->removeItem (index);

    dropCombo->blockSignals (false);

    return;
  }

  if ( index < 0 )
  {
    dropCombo->insertItem ( 0, fileName );
    index = dropCombo->findText (fileName);
  }

  history.append (fileName);   // append or move to the beginning

  dropCombo->setCurrentIndex (index);

  dropCombo->blockSignals (false);

  emit textChanged (fileName);

}               /*-----  fileChanged  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getFileName  of  FileSelect
 |
 |  Description :
 |
 |   This function returns the current text of the combo box.
 |
 +-------------------------------------------------------------------------*/

QString FileSelect::getFileName ()
{
  QString fileName;

  fileName = dropCombo->currentText ();
  history.append (fileName);   // append or move to the beginning

  return fileName;

}               /*-----  getFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : fromGui  of  FileSelect
 |
 |  Description :
 |
 |   This function is called from 'save', it adds the current text of the
 |   combobox to the history and saves it.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::fromGui ()
{
  QString  text;

  text = getFileName ();

  history.append (text);

  if (withHistory)
    history.save ();

}               /*-----  fromGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  toGui  of  FileSelect
 |
 |  Description :
 |
 |   This function  is called  after creating the  widget. It  clears the
 |   dropCombo and adds the values from the history to it.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::toGui ()
{

  dropCombo->clear ();
  dropCombo->addItems ( history.getItems () );

  if ( dropCombo->count () > 0 )
    dropCombo->setCurrentIndex (0);

}               /*-----  toGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  load  of  FileSelect
 |
 |  Description :
 |
 |   This function loads the recent file list (history).
 |
 +-------------------------------------------------------------------------*/

void FileSelect::load ()
{

  if (withHistory)
    history.load ( section, key, 10 );

}               /*-----  load  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  save  of  FileSelect
 |
 |  Description :
 |
 |   This  function  calls 'fromGui'  to  add  the  current text  of  the
 |   combobox to the history and saving it.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::save ()
{

  fromGui ();

}               /*-----  save  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setEditable  of  FileSelect
 |
 |  Description :
 |
 |   This function stores if the dropCombo should be editable or not.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::setEditable ( bool edit )
{

  editable = edit;

  if (dropCombo)
    dropCombo->setEditable (editable);

}               /*-----  setEditable  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setFilter  of  FileSelect
 |
 |  Description :
 |
 |   This function stores the filter used for the file selection dialog.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::setFilter ( QString filter )
{

  this->filter = filter;

}               /*-----  setFilter  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setLoadFile  of  FileSelect
 |
 |  Description :
 |
 |   This function  stores if the file  should be loaded or  saved in the
 |   file selection dialog.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::setLoadFile ( bool load )
{

  loadFile = load;

}               /*-----  setLoadFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setMustExist  of  FileSelect
 |
 |  Description :
 |
 |   This function  stores if the file  must exist in the  file selection
 |   dialog (load) or not (save).
 |
 +-------------------------------------------------------------------------*/

void FileSelect::setMustExist ( bool mustExist )
{

  this->mustExist = mustExist;

}               /*-----  setMustExist  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onSelectFile  of  FileSelect
 |
 |  Description :
 |
 |   This function creates an file open  dialog to search for an existing
 |   file. It uses the utility function of fileUtils.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::onSelectFile ()
{
  QString fileName;

  fileName = getFileName ();

  if ( fileUtils.selectFile ( fileName, filter, loadFile, mustExist ) )
  {
    if ( QFile::exists (fileName) or ( not mustExist ) )
    {
      dropCombo->blockSignals (true);

      onTextChanged (fileName);

      dropCombo->blockSignals (false);
    }
  }

}               /*-----  onSelectFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onIndexChanged  of  FileSelect
 |
 |  Description :
 |
 |   This function is called when the combo box selection has changed.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::onIndexChanged ( int /* index */ )
{
  QString text;

  text = dropCombo->currentText ();

  onTextChanged (text);

}               /*-----  onIndexChanged  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onTextChanged  of  FileSelect
 |
 |  Description :
 |
 |   This function is called when the  combo box selection has changed or
 |   when the  user edits  the text or  pastes a new  text into  the edit
 |   field.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::onTextChanged ( const QString & text )
{

  fileChanged (text);

}               /*-----  onTextChanged  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onTextDropped  of  FileSelect
 |
 |  Description :
 |
 |   This function  is called when  the user drops  a text (file)  to the
 |   dropCombo. It is invoked by the signal 'textDropped' from dropCombo.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::onTextDropped ( QString fileName )
{

  fileChanged (fileName);

}               /*-----  onTextChanged  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setHistory  of  FileSelect
 |
 |  Description :
 |
 |   This function  sets the section  name and  key name for  the history
 |   list.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::setHistory ( QString section, QString key )
{

  withHistory = true;

  this->section = section;
  this->key     = key;

}               /*-----  setHistory  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setLabel  of  FileSelect
 |
 |  Description :
 |
 |   This function sets the label text left to the drop combobox.
 |
 +-------------------------------------------------------------------------*/

void FileSelect::setLabel ( QString label )
{

  labelText = label;

}               /*-----  setLabel  -----*/
