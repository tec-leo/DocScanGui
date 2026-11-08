/*-------------------------------------------------------------------------
 |
 |  Include-File :  MsgInfo.h
 |
 |  Description :
 |
 |   This include file contains the class definitions for the module
 |   'MsgInfo'.
 |
 +-------------------------------------------------------------------------*/

#ifndef MsgInfo_h
#define MsgInfo_h


/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

#define  MSG_LEVEL_CRIT     0  // failure probable and maybe imminent
#define  MSG_LEVEL_ERROR   10  // bad behavior, failure may occur
#define  MSG_LEVEL_WARN    20  // something is wrong or strange
#define  MSG_LEVEL_INFO    30  // informing the user or storing in log
#define  MSG_LEVEL_NOTIFY  40  // notification the user or storing in log


/*-------------------------------------------------------------------------
 |
 |  Class :  MsgInfo
 |
 |  Description :
 |
 |   This class contains  the content of a message  coming from algorithm
 |   or internally. It handles different levels of messages, like errors,
 |   warnings or just infos.
 |
 +-------------------------------------------------------------------------*/

class MsgInfo
{
  public:
    MsgInfo ();
    virtual ~MsgInfo ();

    void     clear        ();
    QString  getLevelName ( int level );
    QString  levelToName  ( int level );
    int      nameToLevel  ( QString name );
    void     setInfo      ( int level, QString title,
                            QString text, QString info );
    void     setCritical  ( QString title, QString text, QString info = "" );
    void     setError     ( QString title, QString text, QString info = "" );
    void     setWarning   ( QString title, QString text, QString info = "" );
    void     setInfo      ( QString title, QString text, QString info = "" );
    L_String toText ();

  public:
    QString  date;
    QString  title;
    QString  text;
    QString  info;
    int      level;

  protected:
    L_String msgLevels;
    M_StrInt levelMap;

};

#endif
