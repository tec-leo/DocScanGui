/*-------------------------------------------------------------------------
 |
 |  File Name   :  PathSelect.cpp
 |
 |  Description :
 |
 |   This module  contains the  class 'PathSelect' which  a path  or file
 |   selection widget as  a reuseable component. It consists  of a label,
 |   an edit field and a browse button to select a file or directory.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 28.Sep.2018
 |  Modified  : Oskar Leirich                Last changes  : 12.Dec.2025
 |
 |
 |  This unit contains following member functions :
 |   of PathSelect :
 |    PathSelect                ~PathSelect               addPath
 |    connectSlots              create                    createLayout
 |    fromGui                   getPath                   getSelection
 |    load                      onSelect                  onTextChanged
 |    onTextChanged             save                      setFilter
 |    setHistory                setLabel                  setOnlyPath
 |    toGui
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "PathSelect.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  PathSelect
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 |  Input parameters :
 |   parent          : parent widget
 |
 +-------------------------------------------------------------------------*/

PathSelect::PathSelect ( QWidget * parent ) : QWidget (parent)
{

  iconName  = ":/Icons/Open";
  filter   = "All Files (*.*)";
  onlyPath = true;
  editable = false;

  // create dropCombo before any settings
  dropCombo = new DropCombo (this);
  dropCombo->setEditable (editable);
  dropCombo->setInsertPolicy ( QComboBox::InsertAtTop );

}               /*-----  PathSelect  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  PathSelect
 |
 |  Description :
 |
 |   The  Destructor  deletes   dynamically  creates  objects.
 |
 +-------------------------------------------------------------------------*/

PathSelect::~PathSelect ()
{

  save ();

  delete  dropCombo;
  delete  lbPath;

}                   /*-----  ~PathSelect  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  addPath  of  PathSelect
 |
 |  Description :
 |
 |   This function checks if the file  exist with the given name and adds
 |   it to the combo box and the history list.
 |
 +-------------------------------------------------------------------------*/

void PathSelect::addPath ( QString pathName )
{
  QDir dir;
  int  index;

  if ( not dir.exists (pathName) )
    dir.mkpath (pathName);

  if ( dir.exists (pathName) )
  {
    pathName = QDir::fromNativeSeparators (pathName);

    index = dropCombo->findText (pathName);

    if ( index < 0 )
    {
      dropCombo->addItem (pathName);
      history.append (pathName);
    }

    index = dropCombo->findText (pathName);
    dropCombo->setCurrentIndex (index);
  }

}               /*-----  addPath  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLayout  of  PathSelect
 |
 |  Description :
 |
 |   This function creates the layout for the file options dialog.
 |
 +-------------------------------------------------------------------------*/

void PathSelect::createLayout ()
{
  QHBoxLayout * hBox;

  hBox     = new QHBoxLayout ();

  btnBrowse = new QPushButton (this);
  btnBrowse->setIcon ( QIcon (iconName) );
  btnBrowse->setToolTip ( "Browse for directory or file" );

  if ( iconSize >= 16 )
    btnBrowse->setIconSize ( QSize ( iconSize, iconSize ) );

  lbPath   = new QLabel ( labelText, this );

  hBox->addWidget  (lbPath);
  hBox->addSpacing (5);
  hBox->addWidget  ( dropCombo, 3 );
  hBox->addSpacing (5);
  hBox->addWidget  (btnBrowse);

  hBox->setContentsMargins ( 1, 1, 1, 1 );

  setLayout (hBox);

}               /*-----  createLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  connectSlots  of  PathSelect
 |
 |  Description :
 |
 |   This function creates the signal slot connections
 |
 +-------------------------------------------------------------------------*/

void PathSelect::connectSlots ()
{

  connect ( btnBrowse,  SIGNAL ( clicked  () ),
            this,       SLOT   ( onSelect () ) );

  connect ( dropCombo,  SIGNAL ( currentIndexChanged (int) ),
            this,       SLOT   ( onTextChanged (int) ) );

  connect ( dropCombo,  SIGNAL ( textDropped   (QString) ),
            this,       SLOT   ( onTextChanged (QString) ) );

  connect ( dropCombo,  SIGNAL ( currentTextChanged (QString) ),
            this,       SLOT   ( onTextChanged      (QString) ) );

}               /*-----  connectSlots  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  create  of  PathSelect
 |
 |  Description :
 |
 |   This function initializes the dialog
 |
 +-------------------------------------------------------------------------*/

void PathSelect::create ()
{

  createLayout ();

  load ();
  toGui ();

  connectSlots ();

}               /*-----  create  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getPath  of  PathSelect
 |
 |  Description :
 |
 |   This function  returns the current  text of the combo  box.  Because
 |   'currentText' of  the combo box works  only if it is  editable, this
 |   function uses the current index and works always.
 |
 +-------------------------------------------------------------------------*/

QString PathSelect::getPath ()
{
  QString text;
  int     index;

  index = dropCombo->currentIndex ();
  text  = dropCombo->itemText (index);

  return text;

}               /*-----  getPath  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getSelection  of  PathSelect
 |
 |  Description :
 |
 |   This  function returns  the selected  files from  a previous  browse
 |   button  action.  The  selection can  contains one  or more  files or
 |   directories.
 |
 +-------------------------------------------------------------------------*/

L_String PathSelect::getSelection ()
{

  return selFiles;

}               /*-----  getSelection  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : fromGui  of  PathSelect
 |
 |  Description :
 |
 |   This  function is  called before  the  dialog should  be closed,  it
 |   stores the gui controls in the files parameter variables.
 |
 +-------------------------------------------------------------------------*/

void PathSelect::fromGui ()
{
  QString  text;

  text = getPath ();

  history.append (text);
  history.save ();

}               /*-----  fromGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  toGui  of  PathSelect
 |
 |  Description :
 |
 |   This function is called after  creating  the  dialog,  the  internal
 |   values of the dialog variables are converted  to  the  dialog  (Gui)
 |   controls (widgets).
 |
 +-------------------------------------------------------------------------*/

void PathSelect::toGui ()
{

  dropCombo->clear ();
  dropCombo->addItems ( history.getItems () );

  if ( dropCombo->count () > 0 )
    dropCombo->setCurrentIndex (0);

}               /*-----  toGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  load  of  PathSelect
 |
 |  Description :
 |
 |   This function loads the path.
 |
 +-------------------------------------------------------------------------*/

void PathSelect::load ()
{

  history.load ( section, key, 10 );

}               /*-----  load  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  save  of  PathSelect
 |
 |  Description :
 |
 |   This function saves some variables.
 |
 +-------------------------------------------------------------------------*/

void PathSelect::save ()
{

  fromGui ();

}               /*-----  save  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setEditable  of  PathSelect
 |
 |  Description :
 |
 |   This function  stores if the  file should be  loaded or save  in the
 |   file selection dialog.
 |
 +-------------------------------------------------------------------------*/

void PathSelect::setEditable ( bool edit )
{

  editable = edit;

  if (dropCombo)
    dropCombo->setEditable (editable);

}               /*-----  setEditable  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setFilter  of  PathSelect
 |
 |  Description :
 |
 |   This  function  stores  the  filter  used  for  selecting  files  or
 |   directories.
 |
 +-------------------------------------------------------------------------*/

void PathSelect::setFilter ( QString filter )
{

  this->filter = filter;

}               /*-----  setFilter  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setOnlyPath  of  PathSelect
 |
 |  Description :
 |
 |   This function  sets the  variable 'onlyPath',  this controls  if the
 |   browse  button allows  to open  only  a directory  or also  existing
 |   files.
 |
 +-------------------------------------------------------------------------*/

void PathSelect::setOnlyPath ( bool onlyPath )
{

  this->onlyPath = onlyPath;

}               /*-----  setOnlyPath  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onSelect  of  PathSelect
 |
 |  Description :
 |
 |   This function creates an file open  dialog to search for an existing
 |   file or directory.
 |
 +-------------------------------------------------------------------------*/

void PathSelect::onSelect ()
{
  QString pathName;
  bool    ok;
  int     index;

  ok = false;

  pathName = getPath ();
  selFiles.clear ();

  if (onlyPath)
  {
    if ( fileUtils.selectDir ( pathName, true ) )
    {
      pathName = QDir::fromNativeSeparators (pathName);
      selFiles.append (pathName);
      ok = true;
    }
  }
  else
  {
    selFiles.append (pathName);

    if ( fileUtils.selectFiles ( selFiles, filter, true ) )
    {
      pathName = selFiles [0];

      if ( QFile::exists (pathName) )
      {
        pathName = QDir::fromNativeSeparators (pathName);
        ok = true;
      }
    }
  }

  if ( not ok )
    return;

  index = dropCombo->findText (pathName);

  if ( index < 0 )
  {
    dropCombo->addItem (pathName);
    history.append (pathName);
  }

  index = dropCombo->findText (pathName);
  dropCombo->setCurrentIndex (index);
  editText = pathName;

}               /*-----  onSelect  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onTextChanged  of  PathSelect
 |
 |  Description :
 |
 |   This function is called when the  combo box selection has changed.
 |   The selection change is send as a signal with the new text.
 |
 +-------------------------------------------------------------------------*/

void PathSelect::onTextChanged ( int index )
{
  QString pathName;
  QDir    dir;

  pathName = dropCombo->itemText (index);

  if ( ( pathName.isEmpty () ) or ( not dir.exists (pathName) ) )
    return;

  emit textChanged (pathName);

}               /*-----  onTextChanged  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onTextChanged  of  PathSelect
 |
 |  Description :
 |
 |   This function adds the given file to the combo box and the history.
 |
 +-------------------------------------------------------------------------*/

void PathSelect::onTextChanged ( QString pathName )
{
  QFileInfo fInfo;
  int       index;
  QDir      dir;

  if ( ( pathName.isEmpty () ) or ( not dir.exists (pathName) ) )
    return;

  pathName = dir.fromNativeSeparators (pathName);

  index = dropCombo->findText (pathName);

  dropCombo->blockSignals (true);

  if ( index < 0 )
  {
    dropCombo->insertItem ( 0, pathName );
    index = dropCombo->findText (pathName);
  }

  history.append (pathName);   // append or move to the beginning

  dropCombo->setCurrentIndex (index);

  dropCombo->blockSignals (false);

  emit textChanged (pathName);

}               /*-----  onTextChanged  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setLabel  of  PathSelect
 |
 |  Description :
 |
 |   This  function sets the label text left to the drop combobox.
 |
 +-------------------------------------------------------------------------*/

void PathSelect::setLabel ( QString label )
{

  labelText = label;

}               /*-----  setLabel  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setHistory  of  PathSelect
 |
 |  Description :
 |
 |   This function  sets the section  name and  key name for  the history
 |   list.
 |
 +-------------------------------------------------------------------------*/

void PathSelect::setHistory ( QString section, QString key )
{

  this->section = section;
  this->key     = key;

}               /*-----  setHistory  -----*/
