/*-------------------------------------------------------------------------
 |
 |  File Name   :  ParList.cpp
 |
 |  Description :
 |
 |   This  module contains  the container  class 'ParList'  which manages
 |   parameter items of the class 'PartItem'. They can be added, deleted,
 |   searched, retrieved,  loaded and stored  in a generich way  with the
 |   overloaded functions 'findPar' and 'writePar' of ParFile for several
 |   data types.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 06.Apr.2022
 |  Modified  : Oskar Leirich                Last changes  : 09.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of ParList :
 |    ParList                   ~ParList                  append
 |    beginGroup                beginLayout               buildParMap
 |    clear                     copy                      count
 |    createCheckbox            createCombobox            createLabel
 |    createLineEdit            createRadioBtn            createStretch
 |    createTable               endGroup                  endLayout
 |    findParItem               getParItem                getParItems
 |    load                      removeLabel               save
 |    setDlgParList             setParItem                setSrcParList
 |    setParFile
 |
 +-------------------------------------------------------------------------*/

#include "ParList.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  ParList
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 +-------------------------------------------------------------------------*/

ParList::ParList ()
{

  parFile    = nullptr;
  parItem    = nullptr;
  dlgParList = nullptr;
  srcParList = nullptr;

}               /*-----  ParList  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  of  ParList
 |
 |  Description :
 |
 |   The destructor deletes some objects.
 |
 +-------------------------------------------------------------------------*/

ParList::~ParList ()
{

  clear ();

}               /*-----  ~ParList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  append  of  ParList
 |
 |  Description :
 |
 |   This function appends a new parItem to the par list and map.
 |
 +-------------------------------------------------------------------------*/

void ParList::append ( ParItem & parItem )
{
  int  lastIndex;

  lastIndex = parItems.count ();

  parItems.append (parItem);
  parMap [parItem.name] = lastIndex;

  parItem.infos.clear ();

}               /*-----  append  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  buildParMap  of  ParList
 |
 |  Description :
 |
 |   This function  builds the par  map completely new from  the existing
 |   items. This may be necessary after creating a new dialog par list.
 |
 +-------------------------------------------------------------------------*/

void ParList::buildParMap ()
{
  ParItem item;

  parMap.clear ();

  for ( int idx = 0; idx < parItems.count (); idx++ )
  {
    item = parItems [idx];

    parMap [item.name] = idx;
  }

}               /*-----  buildParMap  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  copy  of  ParList
 |
 |  Description :
 |
 |   This function copies a new parItem to list if the name exists
 |
 +-------------------------------------------------------------------------*/

void ParList::copy ( QString name )
{
  ParItem parItem;

  if ( srcParList->getParItem ( name, parItem ) )
    append ( parItem );

}               /*-----  copy  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  clear  of  ParList
 |
 |  Description :
 |
 |   This function clears all container objects (list and map).
 |
 +-------------------------------------------------------------------------*/

void ParList::clear ()
{

  parItems.clear ();
  parMap.clear ();

}               /*-----  clear  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  count  of  ParList
 |
 |  Description :
 |
 |   This function returns the number of parameter items.
 |
 +-------------------------------------------------------------------------*/

int ParList::count ()
{

  return parItems.count ();

}               /*-----  count  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  beginLayout  of  ParList
 |
 |  Description :
 |
 |   This function starts a new layout.
 |
 +-------------------------------------------------------------------------*/

void ParList::beginLayout ( QString type )
{
  ParItem  parItem;

  parItem.name = type;
  parItem.infos ["control"] = "begLayout";

  dlgParList->append (parItem);

}               /*-----  beginLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  endLayout  of  ParList
 |
 |  Description :
 |
 |   This function ends the last layout.
 |
 +-------------------------------------------------------------------------*/

void ParList::endLayout ()
{
  ParItem  parItem;

  parItem.name = "end layout";
  parItem.infos ["control"] = "endLayout";
  dlgParList->append (parItem);

}               /*-----  endLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  beginGroup  of  ParList
 |
 |  Description :
 |
 |   This function starts a new group.
 |
 +-------------------------------------------------------------------------*/

void ParList::beginGroup ( QString label )
{
  ParItem  parItem;

  parItem.name = "begin group";
  parItem.infos ["control"] = "begGroup";
  parItem.infos ["label"]   = label;

  dlgParList->append (parItem);

}               /*-----  beginGroup  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  endGroup  of  ParList
 |
 |  Description :
 |
 |   This function ends the last group.
 |
 +-------------------------------------------------------------------------*/

void ParList::endGroup ( int stretch )
{
  ParItem  parItem;

  parItem.name = "end group";
  parItem.infos ["control"] = "endGroup";
  parItem.infos ["stretch"] = QString::number (stretch);

  dlgParList->append (parItem);

}               /*-----  endGroup  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLineEdit  of  ParList
 |
 |  Description :
 |
 |   This function  creates a line edit from the parameter with the
 |   given name.
 |
 +-------------------------------------------------------------------------*/

void ParList::createLineEdit ( QString name, QString label )
{
  ParItem  parItem;

  if ( getParItem ( name, parItem ) )
  {
    if ( not label.isEmpty () )
      parItem.infos ["label"] = label;

    parItem.infos ["control"] = "edit";
    dlgParList->append (parItem);
  }

}               /*-----  createLineEdit  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLabel    of  ParList
 |
 |  Description :
 |
 |   This function creates a new label with the given text and adds it to
 |   the dialog parlist.
 |
 +-------------------------------------------------------------------------*/

void ParList::createLabel ( QString label )
{
  ParItem  parItem;

  parItem.infos ["control"] = "label";
  parItem.infos ["label"]   = label;

  dlgParList->append (parItem);

}               /*-----  createStretch  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLabel    of  ParList
 |
 |  Description :
 |
 |   This function searches a par item  with the given name. If it exists
 |   it creates a label from it and adds it to the dialog parlist.
 |
 +-------------------------------------------------------------------------*/

void ParList::createLabel ( QString name, QString label )
{
  ParItem  parItem;

  if ( getParItem ( name, parItem ) )
  {
    parItem.infos ["control"] = "label";
    parItem.infos ["label"]   = label;

    dlgParList->append (parItem);
  }

}               /*-----  createStretch  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createStretch  of  ParList
 |
 |  Description :
 |
 |   This function creates an empty entry in the grid like a 'stretch' in
 |   the layout.
 |
 +-------------------------------------------------------------------------*/

void ParList::createStretch ()
{
  ParItem  parItem;

  parItem.infos ["control"] = "stretch";
  dlgParList->append (parItem);

}               /*-----  createStretch  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createCheckbox  of  ParList
 |
 |  Description :
 |
 |   This function creates  a check box from the par  item with the given
 |   name.
 |
 +-------------------------------------------------------------------------*/

void ParList::createCheckbox ( QString name, QString label, bool twoColumns )
{
  ParItem  parItem;

  if ( getParItem ( name, parItem ) )
  {
    if ( not label.isEmpty () )
      parItem.infos ["label"] = label;

    parItem.infos ["twoCol"] = StringUtils::boolToString (twoColumns);

    parItem.infos ["control"] = "check";
    dlgParList->append (parItem);
  }

}               /*-----  createCheckbox  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createCombobox  of  ParList
 |
 |  Description :
 |
 |   This function creates  a combo box from the par  item with the given
 |   name.
 |
 +-------------------------------------------------------------------------*/

void ParList::createCombobox ( QString name, L_String items, QString select )
{
  ParItem  parItem;
  QString  text;

  if ( getParItem ( name, parItem ) )
  {
    text = items.join ( "," );

    parItem.infos ["items"]   = text;
    parItem.infos ["control"] = "combo";

    parItem.valueStr = select;

    dlgParList->append (parItem);
  }

}               /*-----  createCombobox  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createRadioBtn  of  ParList
 |
 |  Description :
 |
 |   This function  creates a  radio button  from the  par item  with the
 |   given name.
 |
 +-------------------------------------------------------------------------*/

void ParList::createRadioBtn ( QString name, QString label, bool twoColumns )
{
  ParItem  parItem;

  if ( getParItem ( name, parItem ) )
  {
    parItem.infos ["label" ] = label;
    parItem.infos ["twoCol"] = StringUtils::boolToString (twoColumns);

    parItem.infos ["control"] = "radio";
    dlgParList->append (parItem);
  }

}               /*-----  createRadioBtn  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createTable  of  ParList
 |
 |  Description :
 |
 |   This function  creates a table  widget which contains  all parameter
 |   items with the names given by 'items'.
 |
 +-------------------------------------------------------------------------*/

void ParList::createTable ( QString name, L_String items )
{
  ParItem parItem;
  QString text;

  parItem.name = name;

  text = items.join ( "," );

  parItem.infos ["label"]   = name;
  parItem.infos ["items"]   = text;
  parItem.infos ["control"] = "table";

  dlgParList->append (parItem);

  for ( int i = 0; i < items.count (); i++ )
  {
    text = items [i];

    if ( getParItem ( text, parItem ) )
    {
      parItem.infos ["name"]    = text;
      parItem.infos ["control"] = "table-item";
      parItem.infos ["index"]   = QString::number (i);

      dlgParList->append (parItem);
    }
  }

}               /*-----  createTable  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findParItem  of  ParList
 |
 |  Description :
 |
 |   This function returns the index of the parameter item with the given
 |   name.
 |
 +-------------------------------------------------------------------------*/

int ParList::findParItem ( QString name )
{
  int  idx;

  idx = -1;

  if ( parMap.contains (name) )
    idx = parMap [name];

  return idx;

}               /*-----  findParItem  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getItemValue  of  ParList
 |
 |  Description :
 |
 |   This function sets the item value with 'name' to the given 'value'.
 |
 +-------------------------------------------------------------------------*/

bool ParList::getItemValue ( QString name, QString & value )
{
  bool ok;

  parItem = getParItem (name);

  ok = false;

  if (parItem)    // found, can work with it
  {
    value = parItem->getValue ();

    ok = true;
  }

  return ok;

}               /*-----  getItemValue  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getParItem  of  ParList
 |
 |  Description :
 |
 |   This function returns the parameter item at the given index.
 |
 +-------------------------------------------------------------------------*/

bool ParList::getParItem ( int index, ParItem & item )
{
  bool ok;

  if ( ( index < parItems.count () ) and ( index >= 0 ) )
  {
    item = parItems [index];
    ok = true;
  }
  else
  {
    item.clear ();

    ok = false;
  }

  return ok;

}               /*-----  getParItem  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getParItem  of  ParList
 |
 |  Description :
 |
 |   This function returns the parameter item with the given name.
 |
 +-------------------------------------------------------------------------*/

bool ParList::getParItem ( QString name, ParItem & item )
{
  bool ok;
  int  idx;

  if ( parMap.contains (name) )
  {
    idx  = parMap [name];
    item = parItems [idx];

    ok = true;
  }
  else
  {
    item.clear ();

    ok = false;
  }

  return ok;

}               /*-----  getParItem  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setItemValue  of  ParList
 |
 |  Description :
 |
 |   This function sets the item value with 'name' to the given 'value'.
 |
 +-------------------------------------------------------------------------*/

bool ParList::setItemValue ( QString name, QString value )
{
  bool ok;

  parItem = getParItem (name);

  ok = false;

  if (parItem)    // found, can work with it
  {
    parItem->setValue (value);

    ok = true;
  }

  return ok;

}               /*-----  setParItem  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getParItem  of  ParList
 |
 |  Description :
 |
 |   This function returns the parameter item with the given name.
 |
 +-------------------------------------------------------------------------*/

ParItem * ParList::getParItem ( QString name )
{
  int idx;

  parItem = nullptr;

  if ( parMap.contains (name) )
  {
    idx     = parMap [name];
    parItem = & parItems [idx];
  }

  return parItem;

}               /*-----  getParItem  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getParItems  of  ParList
 |
 |  Description :
 |
 |   This function returns the list of all parameter items.
 |
 +-------------------------------------------------------------------------*/

L_ParItems ParList::getParItems ()
{

  return parItems;

}               /*-----  getParItems  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setParItem  of  ParList
 |
 |  Description :
 |
 |   This function sets (replaces) the  parameter item at the given index
 |   with the given 'item'.
 |
 +-------------------------------------------------------------------------*/

bool ParList::setParItem ( int index, ParItem item )
{
  bool ok;

  if ( ( index < parItems.count () ) and ( index >= 0 ) )
  {
    parItems.replace ( index, item );
    ok = true;
  }
  else
  {
    ok = false;
  }

  return ok;

}               /*-----  setParItem  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setParItem  of  ParList
 |
 |  Description :
 |
 |   This function sets (replaces) the parameter item if it already exists.
 |
 +-------------------------------------------------------------------------*/

bool ParList::setParItem ( ParItem item )
{
  int  idx;
  bool ok;

  idx = findParItem ( item.name );

  if ( idx >= 0 )
  {
    parItems.replace ( idx, item );
    ok = true;
  }
  else
  {
    ok = false;
  }

  return ok;

}               /*-----  setParItem  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  load  of  ParList
 |
 |  Description :
 |
 |   This function loads all parameter from the parameter file.
 |
 +-------------------------------------------------------------------------*/

void ParList::load ()
{
  ParItem  parItem;
  QString  section;

  section = parFile->getSection ();

  for ( int i = 0; i < parItems.count (); i++ )
  {
    parItem = parItems [i];

    if ( ( section != parItem.section ) and ( not parItem.section.isEmpty () ) )
    {
      section = parItem.section;
      parFile->setSection (section);
    }

    if ( parFile->findPar ( parItem.name, parItem.valueStr ) )
      parItem.fromString ();
  }

}               /*-----  load  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  save  of  ParList
 |
 |  Description :
 |
 |   This function stores all parameter into the parameter file.
 |
 +-------------------------------------------------------------------------*/

void ParList::save ()
{
  ParItem  parItem;
  QString  section;

  section = parFile->getSection ();

  for ( int i = 0; i < parItems.count (); i++ )
  {
    parItem = parItems [i];

    if ( not parItem.save )
      continue;

    parItem.toString ();

    if ( ( section != parItem.section ) and ( not parItem.section.isEmpty () ) )
    {
      section = parItem.section;
      parFile->setSection (section);
    }

    parFile->writePar ( parItem.name, parItem.valueStr );
  }

}               /*-----  save  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setParFile  of  ParList
 |
 |  Description :
 |
 |   This function updates the pointer to the ParFile class, this must be
 |   called if the name of the parameter file has changed.
 |
 +-------------------------------------------------------------------------*/

void ParList::setParFile ( ParFile * parFile )
{

  this->parFile = parFile;    // assume we work with pointers

}               /*-----  setParFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setDlgParList  of  ParList
 |
 |  Description :
 |
 |   This function stores  a pointer to new parameter list  which is used
 |   to feed it into a generic dialog.
 |
 |  Input parameters :
 |   parList         :  Pointer to new parameter list for adding controls
 |
 +-------------------------------------------------------------------------*/

void  ParList::setDlgParList ( ParList * parList )
{

  dlgParList = parList;

}               /*-----  setDlgParList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setSrcParList  of  ParList
 |
 |  Description :
 |
 |   This function stores  a pointer to new parameter list  which is used
 |   to copy items from this list.
 |
 |  Input parameters :
 |   parList         :  Pointer to parameter list for copying
 |
 +-------------------------------------------------------------------------*/

void  ParList::setSrcParList ( ParList * parList )
{

  srcParList = parList;

}               /*-----  setSrcParList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  removeLabel  of  ParList
 |
 |  Description :
 |
 |   This function removes the label from  the info list of the parameter
 |   item with the given name.
 |
 +-------------------------------------------------------------------------*/

void ParList::removeLabel ( QString name )
{
  ParItem  parItem;

  if ( getParItem ( name, parItem ) )
  {
    parItem.infos.remove ("label");
    setParItem (parItem);
  }

}               /*-----  removeLabel  -----*/

