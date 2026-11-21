/*-------------------------------------------------------------------------
 |
 |  Include-File :  MsgLogger.h
 |
 |  Description :
 |
 |   This include file contains the class definitions for the module
 |   'MsgLogger'.
 |
 +-------------------------------------------------------------------------*/

#ifndef MsgLogger_h
#define MsgLogger_h

#include "MsgInfo.h"

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class   MsgLogger;
extern  MsgLogger  * msgLogger;

/*------------------------------------------*/
/*   T Y P E   -  D e c l a r a t i o n s   */
/*------------------------------------------*/

typedef  QList < MsgInfo > L_MsgInfos;

/*-------------------------------------------------------------------------
 |
 |  Class :  MsgLogger
 |
 |  Description :
 |
 |   This class implements a  logging mechanism which stores information,
 |   warning  and error  messages. When  the logger  is used  from a  gui
 |   program then the  show* function show the message  in a QMessageBox,
 |   if not they are stored in a log list.
 |
 +-------------------------------------------------------------------------*/

class MsgLogger
{

  public:
    MsgLogger ();
    virtual ~MsgLogger ();

    void     clear          ();
    void     setGuiApp      ( bool gui );

    void     deleteFile     ();
    QString  getFileName    ();
    void     setFileName    ( QString name );

    int      getCirticalCnt ()    { return criticals.count (); }
    int      getErrorsCnt   ()    { return errors.count ();    }
    int      getInfoCnt     ()    { return infos.count ();     }
    int      getWarningsCnt ()    { return warnings.count ();  }

    L_String getLogList     ();
    L_String getCriticals   ();
    L_String getErrors      ();
    L_String getInfos       ();
    L_String getWarnings    ();
    L_String getLogList     ( L_MsgInfos & infos );

    void     putCritical    ( QString title, QString text, QString info = "" );
    void     putError       ( QString title, QString text, QString info = "" );
    void     putWarning     ( QString title, QString text, QString info = "" );
    void     putInfo        ( QString title, QString text, QString info = "" );
    void     showError      ( QString title, QString msg,  QString info = "" );
    void     showMessage    ( QString title, QString msg,  QString info = "" );

    void     writeFile      ();

  protected:
    L_MsgInfos criticals;
    L_MsgInfos errors;
    L_MsgInfos infos;
    L_MsgInfos warnings;
    L_String   logText;
    QString    fileName;
    bool       guiApp;

};

#endif


