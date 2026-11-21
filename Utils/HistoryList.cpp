/*-------------------------------------------------------------------------
 |
 |  File Name   :  HistoryList.cpp
 |
 |  Description :
 |
 |   This module contains the class  HistoryList which allows to load and
 |   save a list of recently used entries like files and it provides also
 |   'actions' for them.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 13.Apr.2022
 |  Modified  : Oskar Leirich                Last changes  : 19.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of HistoryList :
 |    HistoryList               ~HistoryList              addActions
 |    append                    createActions             deleteItem
 |    deleteItems               getFirst                  getItems
 |    itemTriggered             load                      save
 |    setAppendList             setPrependList            setSortList
 |    updateActions
 |
 +-------------------------------------------------------------------------*/

#include "RegFile.h"
#include "HistoryList.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  HistoryList
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 +-------------------------------------------------------------------------*/

HistoryList::HistoryList ()
{

  maxItems = 0;
  maxSize  = 0;
  withMenu = false;
  sortList = false;

  menuActions.clear ();

}               /*-----  HistoryList  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  of  HistoryList
 |
 |  Description :
 |
 |   The destructor deletes the menu actions.
 |
 +-------------------------------------------------------------------------*/

HistoryList::~HistoryList ()
{

  for ( int i = 0; i < menuActions.size (); i++ )
  {
    delete menuActions [i];
  }

}               /*-----  ~HistoryList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  addActions  of  HistoryList
 |
 |  Description :
 |
 |   This function adds all actions to the given menu.
 |
 |  Input parameters :
 |   menu            : menu action
 |
 +-------------------------------------------------------------------------*/

void HistoryList::addActions ( QMenu * menu )
{

  withMenu = true;

  createActions ();

  // create the actions
  for ( int i = 0; i < maxSize; i++ )
  {
    menu->addAction ( menuActions [i] );
  }

}               /*-----  addActions  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  append  of  HistoryList
 |
 |  Description :
 |
 |   This function appends  the given entry to the item  list and updates
 |   the actions.
 |
 |  Input parameters :
 |   entry            : name of the entry
 |
 +-------------------------------------------------------------------------*/

void HistoryList::append ( QString entry )
{

  entry = QDir::fromNativeSeparators (entry);

  itemList.removeAll (entry);

  if ( ( itemList.size () >= maxItems ) and ( maxItems > 0 ) )
    itemList.removeLast ();

  if ( itemList.size () == 0 )
    itemList.append (entry);
  else
    itemList.prepend (entry);

  updateActions ();

}               /*-----  append  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createActions  of  HistoryList
 |
 |  Description :
 |
 |   This function  creates the  menu actions  for standard,  prepend and
 |   append items.   Every menu entry  is then connected to  the function
 |   'itemTriggered' which  handles all  entries by  using the  'data' of
 |   them.
 |
 +-------------------------------------------------------------------------*/

void HistoryList::createActions ()
{
  QAction * action;

  qDeleteAll (menuActions);

  menuActions.clear ();

  maxSize = appendList.count () + prependList.count () + maxItems;

  // create the actions
  for ( int i = 0; i < maxSize; i++ )
  {
    action = new QAction (this);
    menuActions.append (action);

    action->setVisible (false);

    connect ( action, & QAction::triggered,
              this,   & HistoryList::itemTriggered );
  }

  updateActions ();

}               /*-----  createActions  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  deleteItems  of  HistoryList
 |
 |  Description :
 |
 |   This function  removes all  entries in the  history and  updates the
 |   actions.
 |
 +-------------------------------------------------------------------------*/

void HistoryList::deleteItems ()
{

  itemList.clear ();

  updateActions ();

}               /*-----  deleteItems  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  deleteItem  of  HistoryList
 |
 |  Description :
 |
 |   This function  deletes all occurrences  of the given entry  from the
 |   item list and updates the actions.
 |
 |  Input parameters :
 |   entry           : name of the entry
 |
 +-------------------------------------------------------------------------*/

void HistoryList::deleteItem ( QString entry )
{

  itemList.removeAll (entry);

  updateActions ();

}               /*-----  remove  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getItems  of  HistoryList
 |
 |  Description :
 |
 |   This function returns the item list.
 |
 |  Output parameter :
 |   (L_String)      : file list
 |
 +-------------------------------------------------------------------------*/

L_String HistoryList::getItems ()
{

  return itemList;

}               /*-----  getItems  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getFirst  of  HistoryList
 |
 |  Description :
 |
 |   This function returns the first item of the history list.
 |
 |  Output parameter :
 |   (QString)       : first element of the history list
 |
 +-------------------------------------------------------------------------*/

QString HistoryList::getFirst ()
{
  QString  item;

  if ( itemList.count () > 0 )
    item = itemList [0];

  return item;

}               /*-----  getFirst  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  itemTriggered  of  HistoryList
 |
 |  Description :
 |
 |   This function is called by the  signal slot mechanism. First it asks
 |   for the sender of the signal, retrieves the data (text or index) and
 |   forwards the signal by the signal slot mechanism.
 |
 +-------------------------------------------------------------------------*/

void HistoryList::itemTriggered ()
{
  QAction * action;

  action = qobject_cast < QAction * > ( sender () );

  if (action)
    emit itemSelected ( action->data ().toString () );

}               /*-----  itemTriggered  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  load  of  HistoryList
 |
 |  Description :
 |
 |   This function  reads the given  entry in 'section' from  the current
 |   registry file and converts it to  the given type.  The registry file
 |   is  created with  style and  scope defined  in the  core application
 |   (MainApp).
 |
 |  Input parameters :
 |   section         :  section name.
 |   label           :  entry label in the registry file
 |   size            :  number of entries
 |
 +-------------------------------------------------------------------------*/

void HistoryList::load ( QString section, QString label, int size )
{
  RegFile regFile;
  QString entry;
  QString file;

  this->maxItems = size;
  this->section  = section;
  this->label    = label;

  itemList.clear ();

  regFile.beginGroup (section);

  // load file list
  for ( int i = 1; i <= maxItems; i++ )
  {
    entry.setNum (i);
    entry = label + "-" + entry;

    file = regFile.value (entry).toString ();

    if ( not file.isEmpty () )
      itemList.append (file);
  }

  qDeleteAll (menuActions);

}               /*-----  load  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  save  of  HistoryList
 |
 |  Description :
 |
 |   This function saves all entries in the file list to the registry file
 |   into the defined section.
 |
 +-------------------------------------------------------------------------*/

void HistoryList::save ()
{
  RegFile regFile;
  QString entry;

  regFile.beginGroup (section);

  for ( int i = 0; i < itemList.size (); i++ )
  {
    entry.setNum ( i + 1 );
    entry = label + "-" + entry;

    regFile.setValue ( entry, itemList [i] );
  }

  for ( int i = itemList.size (); i < maxSize; i++ )
  {
    entry.setNum ( i + 1 );
    entry = label + "-" + entry;

    regFile.remove (entry);
  }

}               /*-----  save  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setAppendList  of  HistoryList
 |
 |  Description :
 |
 |   This function sets the  list of items which are added  at the end of
 |   the menu.
 |
 |  Input parameters :
 |   appendList      :  list of items which are added at the end
 |
 +-------------------------------------------------------------------------*/

void  HistoryList::setAppendList ( L_String appendList )
{

  this->appendList = appendList;

}               /*-----  setAppendList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setPrependList  of  HistoryList
 |
 |  Description :
 |
 |   This  function  sets the  list  of  items  which  are added  at  the
 |   beginning of the menu.
 |
 |  Input parameters :
 |   prependList     :  list of items which are added at the begin
 |
 +-------------------------------------------------------------------------*/

void  HistoryList::setPrependList ( L_String prependList )
{

  this->prependList = prependList;

}               /*-----  setPrependList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setSortList  of  HistoryList
 |
 |  Description :
 |
 |   This function stores if the list should be sorted or not.
 |
 |  Input parameters :
 |   sortList        :  list should be sorted (true) or not
 |
 +-------------------------------------------------------------------------*/

void  HistoryList::setSortList ( bool sortList )
{

  this->sortList = sortList;

}               /*-----  setSortList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  updateActions  of  HistoryList
 |
 |  Description :
 |
 |   This  function updates the  text and  data of  the actions,  this is
 |   necessary if the list of files has changed in between.
 |
 +-------------------------------------------------------------------------*/

void HistoryList::updateActions ()
{
  QString entry;
  QString text;
  int     idx;
  int     cnt;

  if ( not withMenu )
    return;

  // make all not visible at the beginning
  for ( int i = 0; i < menuActions.size (); i++ )
  {
    menuActions [i]->setVisible (false);
  }

  idx = 0;

  for ( int i = 0; i < prependList.count (); i++ )
  {
    text = prependList [i];
    menuActions [idx]->setText (text);
    menuActions [idx]->setData (text);
    menuActions [idx]->setVisible (true);
    idx++;
  }

  if (sortList)
    itemList.sort ();

  cnt = qMin ( itemList.size (), maxItems );

  for ( int i = 0; i < cnt; i++ )
  {
    entry = itemList [i];

    text.setNum ( i + 1 );
    text = "&" + text + " " + entry;
    menuActions [idx]->setText (text);
    menuActions [idx]->setData ( itemList [i] );
    menuActions [idx]->setCheckable (false);
    menuActions [idx]->setVisible (true);
    idx++;
  }

  for ( int i = 0; i < appendList.count (); i++ )
  {
    text = appendList [i];
    menuActions [idx]->setText (text);
    menuActions [idx]->setData (text);
    menuActions [idx]->setVisible (true);
    idx++;
  }

}               /*-----  updateActions  -----*/

