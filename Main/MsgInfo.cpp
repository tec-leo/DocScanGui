/*-------------------------------------------------------------------------
 |
 |  File name   :  MsgInfo.cpp
 |
 |  Description :
 |
 |   This module contains the class  'MsgInfo' which contains the content
 |   of a message  coming. It handles different levels  of messages, like
 |   errors, warnings or just infos.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 24.Jun.2022
 |  Modified  : Oskar Leirich                Last changes  : 18.Nov.2026
 |
 |
 |  This file contains the following functions:
 |   of MsgInfo:
 |    MsgInfo                   ~MsgInfo                  clear
 |    getLevel                  getLevelName              setCritical
 |    setError                  setInfo                   setWarning
 |    toText
 |
 +-------------------------------------------------------------------------*/

#include "MsgInfo.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  MsgInfo
 |
 |  Description :
 |
 |   The constructor initializes the variables.
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
 |   This function  is called  from the  constructor and  initializes the
 |   member variables and maps.
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
 |   This function returns the 'criticality' of the given level as string
 |
 |  Input parameters :
 |   level           : the level
 |
 |  Output parameter :
 |   (QString)       : criticality of the level
 |
 +-------------------------------------------------------------------------*/

QString MsgInfo::getLevelName ( int level )
{
  int  index;

  index = level / 10;

  if ( ( index < 0 ) or ( index >= msgLevels.size () ) )
  {
    return "";
  }

  return msgLevels.at (index);

}               /*-----  getLevelName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getLevel  of  MsgInfo
 |
 |  Description :
 |
 |   This  function  returns the  'criticality'  of  the given  level  as
 |   number.
 |
 |  Input parameters :
 |   name            : the name of the level
 |
 |  Output parameter :
 |   (int)           : the level
 |
 +-------------------------------------------------------------------------*/

int MsgInfo::getLevel ( QString name )
{

  return levelMap [name];

}               /*-----  getLevel  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setInfo  of  MsgInfo
 |
 |  Description :
 |
 |   This function  sets this message  info to  the given level  with the
 |   given title, info text and additional info.
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
 |   This function  sets this message  info to 'critical' with  the given
 |   title, info text and additional info.
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
 |   This  function sets  this message  info to  'error' with  the given
 |   title, info text and additional info.
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
 |   This function  sets this message  info to 'warning' with  the given
 |   title, info text and additional info.
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
 |   This function  sets this message  info to 'information' with  the given
 |   title, info text and additional info.
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
 |    Failed Loading File: Stick_1.png
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

  help = "level: " + getLevelName (level);
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

