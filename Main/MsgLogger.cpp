/*-------------------------------------------------------------------------
 |
 |  File name   :  MsgLogger.cpp
 |
 |  Description :
 |
 |   This  module  contains  the  class 'MsgLogger'  which  implements  a
 |   logging  mechanism  which  stores  information,  warning  and  error
 |   messages.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 04.Oct.2024
 |  Modified  : Oskar Leirich                Last changes  : 23.Sep.2026
 |
 |
 |  This file contains the following functions:
 |   of MsgLogger:
 |    MsgLogger                 ~MsgLogger                clear
 |    getCriticals              getErrors                 getFileName
 |    getInfos                  getLogList                putCritical
 |    putError                  putInfo                   putWarning
 |    setFileName               setGuiApp                 showCritical
 |    showError                 showMessage               writeFile
 |
 +-------------------------------------------------------------------------*/

#include "MsgLogger.h"

MsgLogger * msgLogger;


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  MsgLogger
 |
 |  Description :
 |
 |   The constructor initializes the message window.
 |
 +-------------------------------------------------------------------------*/

MsgLogger::MsgLogger ()
{

  guiApp = true;

  clear ();

}                   /*-----  MsgLogger  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  MsgLogger
 |
 |  Description :
 |
 |   The destructor deletes the edit window if it was created before.  If
 |   the MsgLogger class is used only for opening or closing the log file
 |   then there is no edit window and therefore should not be deleted.
 |
 +-------------------------------------------------------------------------*/

MsgLogger::~MsgLogger ()
{


}                   /*-----  MsgLogger  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  clear  of  MsgLogger
 |
 |  Description :
 |
 |   This function clears all message infos.
 |
 +-------------------------------------------------------------------------*/

void MsgLogger::clear ()
{

  criticals.clear ();
  errors   .clear ();
  infos    .clear ();
  warnings .clear ();

}               /*-----  clear  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getLogList  of  MsgLogger
 |
 |  Description :
 |
 |   This function combines the messages of  the given message infos to a
 |   string list and returns it.
 |
 +-------------------------------------------------------------------------*/

L_String MsgLogger::getLogList ()
{
  L_String msgList;
  MsgInfo  msgInfo;
  QString  msg;

  msgList.clear ();

  if ( errors.count () > 0 )
  {
    msg = "\nErrors found :";
    msgList.append (msg);

    msgList.append ( getLogList (errors) );
  }

  if ( warnings.count () > 0 )
  {
    msg = "\nWarnings found :";
    msgList.append (msg);

    msgList.append ( getLogList (warnings) );
  }

  if ( infos.count () > 0 )
  {
    msg = "\nInfos found :";
    msgList.append (msg);

    msgList.append ( getLogList (infos) );
  }

  return msgList;

}               /*-----  getLogList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getLogList  of  MsgLogger
 |
 |  Description :
 |
 |   This function combines the messages of  the given message infos to a
 |   string list and returns it.
 |
 +-------------------------------------------------------------------------*/

L_String MsgLogger::getLogList ( L_MsgInfos & infos )
{
  L_String msgList;
  MsgInfo  msgInfo;
  QString  msg;

  msgList.clear ();

  for ( int i = 0; i < infos.count (); i++ )
  {
    msgInfo = infos [i];

    msg = msgInfo.title + " : " + msgInfo.text;

    if ( not msgInfo.info.isEmpty () )
      msg = msg + " " + msgInfo.info;

    msgList.append (msg);
  }

  return msgList;

}               /*-----  getLogList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getErrors  of  MsgLogger
 |
 |  Description :
 |
 |   This function combines the messages of the error message infos to
 |   a string list and returns it.
 |
 +-------------------------------------------------------------------------*/

L_String MsgLogger::getErrors ()
{

  return getLogList (errors);

}               /*-----  getErrors  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getInfos  of  MsgLogger
 |
 |  Description :
 |
 |   This function combines the messages of the  message infos to
 |   a string list and returns it.
 |
 +-------------------------------------------------------------------------*/

L_String MsgLogger::getInfos ()
{

  return getLogList (infos);

}               /*-----  getInfos  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getWarnings  of  MsgLogger
 |
 |  Description :
 |
 |   This function combines the messages  of the warning message infos to
 |   a string list and returns it.
 |
 +-------------------------------------------------------------------------*/

L_String MsgLogger::getWarnings ()
{

  return getLogList (warnings);

}               /*-----  getWarnings  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getCriticals  of  MsgLogger
 |
 |  Description :
 |
 |   This function combines the messages of the critical message infos to
 |   a string list and returns it.
 |
 +-------------------------------------------------------------------------*/

L_String MsgLogger::getCriticals ()
{

  return getLogList (criticals);

}               /*-----  getCriticals  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : deleteFile  of  MsgLogger
 |
 |  Description :
 |
 |   This function deletes the log file it exists.
 |
 +-------------------------------------------------------------------------*/

void MsgLogger::deleteFile ()
{
  QDir dir;

  // remove existing log file
  if ( dir.exists (fileName) )
    dir.remove (fileName);

}                   /*-----  deleteFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getFileName  of  MsgLogger
 |
 |  Description :
 |
 |   This function returns the file name to the log file.
 |
 |  Output parameter :
 |   (fileName)      :  File name to the log file.
 |
 +-------------------------------------------------------------------------*/

QString MsgLogger::getFileName ()
{

  return fileName;

}               /*-----  getFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setFileName  of  MsgLogger
 |
 |  Description :
 |
 |   This function sets the file name to the given file.
 |
 +-------------------------------------------------------------------------*/

void MsgLogger::setFileName ( QString name )
{

  fileName = name;

}                   /*-----  setFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  putCritical  of  MsgLogger
 |
 |  Description :
 |
 |   This function  shows the given info  message in the  log window with
 |   error level, title and additional info.
 |
 +-------------------------------------------------------------------------*/

void MsgLogger::putCritical ( QString title, QString text, QString info )
{
  MsgInfo msgInfo;

  msgInfo.setInfo ( MSG_LEVEL_CRIT, title, text, info );

  criticals.append (msgInfo);

}               /*-----  putCritical  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  putError  of  MsgLogger
 |
 |  Description :
 |
 |   This function  shows the given info  message in the  log window with
 |   error level, title and additional info.
 |
 +-------------------------------------------------------------------------*/

void MsgLogger::putError ( QString title, QString text, QString info )
{
  MsgInfo msgInfo;

  msgInfo.setInfo ( MSG_LEVEL_ERROR, title, text, info );

  errors.append (msgInfo);

}               /*-----  putError  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  putWarning  of  MsgLogger
 |
 |  Description :
 |
 |   This function  shows the given info  message in the  log window with
 |   error level, title and additional info.
 |
 +-------------------------------------------------------------------------*/

void MsgLogger::putWarning ( QString title, QString text, QString info )
{
  MsgInfo msgInfo;

  msgInfo.setInfo ( MSG_LEVEL_WARN, title, text, info );

  warnings.append (msgInfo);

}               /*-----  putWarning  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  putInfo  of  MsgLogger
 |
 |  Description :
 |
 |   This function  shows the given info  message in the  log window with
 |   error level, title and additional info.
 |
 +-------------------------------------------------------------------------*/

void MsgLogger::putInfo ( QString title, QString text, QString info )
{
  MsgInfo msgInfo;

  msgInfo.setInfo ( MSG_LEVEL_INFO, title, text, info );

  infos.append (msgInfo);

}               /*-----  putInfo  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setGuiApp  of  MsgLogger
 |
 |  Description :
 |
 |   This function sets the file name to the given file.
 |
 +-------------------------------------------------------------------------*/

void MsgLogger::setGuiApp ( bool gui )
{

  guiApp = gui;

}                   /*-----  setGuiApp  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  showError  of  MsgLogger
 |
 |  Description :
 |
 |   This function is just a shortcut  for issuing a warning message as a
 |   dialog.
 |
 |  Input parameters :
 |    title          :  Title string for QMessageBox and logging
 |    msg            :  Message string for QMessageBox and logging
 |    info           :  Info string for logging
 |
 +-------------------------------------------------------------------------*/

void MsgLogger::showError ( QString title, QString msg, QString info )
{
  QString log;

  if (guiApp)
  {
    QMessageBox::warning ( nullptr, title, msg );
  }
  else
  {
    log =  title + " : " + msg;
    logText.append (log);

    log.append ( "\n" );
    printf ( ToString (log) );
  }

  putError ( title, msg, info );

}               /*-----  showError  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  showMessage  of  MsgLogger
 |
 |  Description :
 |
 |   This function is just a shortcut  for issuing a warning message as a
 |   dialog.
 |
 |  Input parameters :
 |    title          :  Title string for QMessageBox and logging
 |    msg            :  Message string for QMessageBox and logging
 |    info           :  Info string for logging
 |
 +-------------------------------------------------------------------------*/

void MsgLogger::showMessage ( QString title, QString msg, QString info )
{
  QString log;

  if (guiApp)
  {
    QMessageBox::information ( nullptr, title, msg );
  }
  else
  {
    log =  title + " : " + msg;
    logText.append (log);
  }

  putInfo ( title, msg, info );

}               /*-----  showMessage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : writeFile  of  MsgLogger
 |
 |  Description :
 |
 |   This function writes the content  of the message window into the log
 |   file and  closes it.  If the file  already exists then  the original
 |   content will be overwritten.
 |
 +-------------------------------------------------------------------------*/

void MsgLogger::writeFile ()
{
  FileUtils fileUtils;
  L_String  msgList;
  QString   msg;

  fileUtils.setFileName (fileName);

  msgList = getLogList ();

  for ( int i = 0; i < logText.count (); i++ )
  {
    msg = logText [i];
    msgList.append (msg);
  }

  fileUtils.writeFile ( msgList, true );   // append

}                   /*-----  writeFile  -----*/

