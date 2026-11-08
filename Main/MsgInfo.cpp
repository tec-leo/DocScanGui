/*-------------------------------------------------------------------------
 |
 |  File name   :  MsgInfo.cpp
 |
 |  Description :
 |
 |   This module contains the class  'MsgInfo' which contains the content
 |   of  a  message coming  from  algorithm  or internally.   It  handles
 |   different levels of messages, like errors, warnings or just infos.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 24.Jun.2022
 |  Modified  : Oskar Leirich                Last changes  : 19.Aug.2024
 |
 |
 |  This file contains the following functions:
 |   of MsgInfo:
 |    MsgInfo                   ~MsgInfo                  clear
 |    getLevelName              levelToName               nameToLevel
 |    setInfo                   toText
 |
 +-------------------------------------------------------------------------*/

#include "MsgInfo.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  MsgInfo
 |
 |  Description :
 |
 |   The constructor initializes the variabels.
 |
 +-------------------------------------------------------------------------*/

MsgInfo::MsgInfo ()
{

  clear ();

}                   /*-----  MsgInfo  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  MsgInfo
 |
 |  Description :
 |
 |   The destructor deletes deletes some objects.
 |
 +-------------------------------------------------------------------------*/

MsgInfo::~MsgInfo ()
{

  clear ();

}                   /*-----  MsgInfo  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  clear  of  MsgInfo
 |
 |  Description :
 |
 |   This  function  is  called  from   the  constructor  and  clear  the
 |   variables.
 |
 +-------------------------------------------------------------------------*/

void MsgInfo::clear ()
{

  date.clear  ();
  title.clear ();
  text.clear  ();
  info.clear  ();

  level = MSG_LEVEL_INFO;

  msgLevels.append ( "CRITICAL" );
  msgLevels.append ( "ERROR" );
  msgLevels.append ( "WARNING" );
  msgLevels.append ( "INFORMATION" );
  msgLevels.append ( "NOTIFICATION" );

  levelMap [ "CRITICAL"     ] = MSG_LEVEL_CRIT;
  levelMap [ "ERROR"        ] = MSG_LEVEL_ERROR;
  levelMap [ "WARNING"      ] = MSG_LEVEL_WARN;
  levelMap [ "INFORMATION"  ] = MSG_LEVEL_INFO;
  levelMap [ "NOTIFICATION" ] = MSG_LEVEL_NOTIFY;

}               /*-----  clear  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getLevelName  of  MsgInfo
 |
 |  Description :
 |
 |   This function returns the name of the level.
 |
 |  Input parameters :
 |   level           : the level
 |
 |  Output parameter :
 |   (QString)       : the name of the level
 |
 +-------------------------------------------------------------------------*/

QString MsgInfo::getLevelName ( int level )
{

  return levelToName (level);

}               /*-----  getLevelName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  levelToName  of  MsgInfo
 |
 |  Description :
 |
 |   This function returns the name of the level
 |
 |  Input parameters :
 |   level           : the level
 |
 |  Output parameter :
 |   (QString)       : the name of the level
 |
 +-------------------------------------------------------------------------*/

QString MsgInfo::levelToName ( int level )
{
  int  index;

  index = level / 10;

  if ( ( index < 0 ) or ( index >= msgLevels.size () ) )
  {
    return "";
  }

  return msgLevels.at (index);

}               /*-----  levelToName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  nameToLevel  of  MsgInfo
 |
 |  Description :
 |
 |   This function returns the level  coresponding to a name. Because the
 |   name coresponds  to a group of  levels, the smallest level  from the
 |   group will be returned
 |
 |  Input parameters :
 |   name            : the name of the level
 |
 |  Output parameter :
 |   (int)           : the level
 |
 +-------------------------------------------------------------------------*/

int MsgInfo::nameToLevel ( QString name )
{

  return levelMap [name];

}               /*-----  nameToLevel  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setInfo  of  MsgInfo
 |
 |  Description :
 |
 |   This  function  is  called  from   the  constructor  and  clear  the
 |   variables.
 |
 +-------------------------------------------------------------------------*/

void MsgInfo::setInfo ( int level, QString title, QString text, QString info )
{

  date = VarUtils::getDate () + " " +  VarUtils::getTime ();

  this->title = title;
  this->text  = text;
  this->info  = info;
  this->level = level;

}               /*-----  setInfo  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setCritical  of  MsgInfo
 |
 |  Description :
 |
 |   This function  shows the given info  message in the  log window with
 |   error level, title and additional info.
 |
 +-------------------------------------------------------------------------*/

void MsgInfo::setCritical ( QString title, QString text, QString info )
{

  setInfo ( MSG_LEVEL_CRIT, title, text, info );

}               /*-----  setCritical  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setError  of  MsgInfo
 |
 |  Description :
 |
 |   This function  shows the given info  message in the  log window with
 |   error level, title and additional info.
 |
 +-------------------------------------------------------------------------*/

void MsgInfo::setError ( QString title, QString text, QString info )
{

  setInfo ( MSG_LEVEL_ERROR, title, text, info );

}               /*-----  setError  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setWarning  of  MsgInfo
 |
 |  Description :
 |
 |   This function  shows the given info  message in the  log window with
 |   error level, title and additional info.
 |
 +-------------------------------------------------------------------------*/

void MsgInfo::setWarning ( QString title, QString text, QString info )
{

  setInfo ( MSG_LEVEL_WARN, title, text, info );

}               /*-----  setWarning  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setInfo  of  MsgInfo
 |
 |  Description :
 |
 |   This function  shows the given info  message in the  log window with
 |   error level, title and additional info.
 |
 +-------------------------------------------------------------------------*/

void MsgInfo::setInfo ( QString title, QString text, QString info )
{

  setInfo ( MSG_LEVEL_INFO, title, text, info );

}               /*-----  setInfo  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  toText  of  MsgInfo
 |
 |  Description :
 |
 |   This function returns the message information in a stringlist.
 |
 |   Example:
 |    level:Error
 |    time:Fr Jun 22 18:11:13 2018
 |    title:Error
 |    text:
 |    Failed Loading Filter collection: FilterModels_117_x64.dll
 |    extra info:
 |
 |  Output parameter :
 |   (L_String)      : the name of the level
 |
 +-------------------------------------------------------------------------*/

L_String MsgInfo::toText ()
{
  L_String msgText;
  QString  help;

  help = "level: " + levelToName (level);
  msgText.append (help);

  help = "time: " + date;
  msgText.append (help);

  help = "title: " + title;
  msgText.append (help);

  msgText.append ("text:");
  msgText.append (text);

  msgText.append ("extra info:");
  msgText.append (info);

  return msgText;

}               /*-----  toText  -----*/

