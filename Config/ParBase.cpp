/*-------------------------------------------------------------------------
 |
 |  File Name   :  ParBase.cpp
 |
 |  Description :
 |
 |   This module contains the  class ParBase which implements the loading
 |   and storing of parameters from or to a file in config-ini format.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 14.Jul.2023
 |  Modified  : Oskar Leirich                Last changes  : 09.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of ParBase :
 |    ParBase                   ~ParBase                  fillParList
 |    getParList                haveItem                  isSaved
 |    load                      save                      setDefaults
 |    setItemValue              setModified               setParFile
 |    setSection                updateList
 |
 +-------------------------------------------------------------------------*/

#include "ParBase.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  ParBase
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 +-------------------------------------------------------------------------*/

ParBase::ParBase ()
{

  parFile = nullptr;

  maxTextWidth = 50;

  setDefaults ();

}               /*-----  ParBase  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  of  ParBase
 |
 |  Description :
 |
 |   The destructor is empty and just placeholder for the future.
 |
 +-------------------------------------------------------------------------*/

ParBase::~ParBase ()
{

}               /*-----  ~ParBase  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : fillParList  of  ParBase
 |
 |  Description :
 |
 |   This  function   fills  the   parameter  container  list   with  all
 |   parameter.  It  is a  placeholder  and  should be  overwritten  from
 |   derived classes if they support the par list concept.
 |
 +-------------------------------------------------------------------------*/

void ParBase::fillParList ()
{

}                   /*-----  fillParList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getParList  of  ParBase
 |
 |  Description :
 |
 |   This function returns a pointer to the parameter list 'parList'.
 |
 |  Output parameter :
 |   (parList *)     :  Pointer to the parameter list 'parList'.
 |
 +-------------------------------------------------------------------------*/

ParList * ParBase::getParList ()
{

  return & parList;

}               /*-----  getParList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  haveItem  of  ParList
 |
 |  Description :
 |
 |   This function checks  if an item with the given  'name' exist in the
 |   parLlist and returns the result.
 |
 +-------------------------------------------------------------------------*/

bool ParBase::haveItem ( QString name )
{
  int  idx;

  idx = parList.findParItem (name);

  if ( idx < 0 )
    return false;
  else
    return true;

}               /*-----  haveItem  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  isSaved  of  ParBase
 |
 |  Description :
 |
 |   This function returns if the parameter are already saved.
 |
 |  Output parameter :
 |   (bool)          : true - saved
 |
 +-------------------------------------------------------------------------*/

bool ParBase::isSaved ()
{

  return saved;

}               /*-----  isSaved  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  load  of  ParBase
 |
 |  Description :
 |
 |   This function loads all parameter from the parameter file.
 |
 +-------------------------------------------------------------------------*/

void ParBase::load ()
{

  setDefaults ();

  saved = true;

}               /*-----  load  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  save  of  ParBase
 |
 |  Description :
 |
 |   This function stores all parameter into the parameter file.
 |
 +-------------------------------------------------------------------------*/

void ParBase::save ()
{

  saved = true;

}               /*-----  save  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setDefaults  of  ParBase
 |
 |  Description :
 |
 |   This function  sets all  parameter to  the default  value.  It  is a
 |   placeholder and should  be overwritten from derived  classes if they
 |   support the par list concept.
 |
 +-------------------------------------------------------------------------*/

void ParBase::setDefaults ()
{

  setModified (true);

  dummyLabel = "";

}               /*-----  setDefaults  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setModified  of  ParBase
 |
 |  Description :
 |
 |   This function is called if any of the parameter has changed and this
 |   should be rememberd. It can be  used derived classes if they need to
 |   know if the parameter are already saved.
 |
 |  Input parameters :
 |   modified        : true or false
 |
 +-------------------------------------------------------------------------*/

void ParBase::setModified ( bool modified /*= true*/ )
{

  saved = not modified;

}               /*-----  setModified  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setSection  of  ParBase
 |
 |  Description :
 |
 |   This function stores the name of the section in the parameter file.
 |
 +-------------------------------------------------------------------------*/

void ParBase::setSection ( QString section )
{

  this->section = section;

  if (parFile)
    parFile->setSection (section);

}              /*-----  setSection  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setParFile  of  ParBase
 |
 |  Description :
 |
 |   This function updates the pointer to the ParFile class, this must be
 |   called if the name of the parameter file has changed.
 |
 +-------------------------------------------------------------------------*/

void ParBase::setParFile ( ParFile * parFile )
{

  this->parFile = parFile;    // assume we work with pointers

  parList.setParFile (parFile);

}               /*-----  setParFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setItemValue  of  ParList
 |
 |  Description :
 |
 |   This function sets the item value with 'name' to the given 'value'.
 |
 +-------------------------------------------------------------------------*/

bool ParBase::setItemValue ( QString name, QString value )
{

  return parList.setItemValue ( name, value );

}               /*-----  setItemValue  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : updateList  of  ParBase
 |
 |  Description :
 |
 |   This function  sets the default label  for all items and  limits the
 |   text with to 'maxTextWidth'.
 |
 +-------------------------------------------------------------------------*/

void ParBase::updateList ()
{
  ParItem  parItem;

  // set default label
  for ( int i = 0; i < parList.count (); i++ )
  {
    parList.getParItem ( i, parItem );
    parItem.infos ["label"] = parItem.infos ["info"];

    if ( parItem.varType == "string" )
    {
      if ( parItem.infos.contains ("options") )
      {
        parItem.toString ();
      }
    }
    else
    {
      parItem.infos ["maxWidth"] = QString::number (maxTextWidth);
    }

    parList.setParItem ( i, parItem );
  }

}                   /*-----  updateList  -----*/

