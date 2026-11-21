/*-------------------------------------------------------------------------
 |
 |  Include-File :  JobManager.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  JobManager.
 |
 +-------------------------------------------------------------------------*/

#ifndef JobManager_h
#define JobManager_h

#include "JobInfo.h"

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class  MsgLogger;
class  ToolsWidget;
class  JobManager;
extern JobManager * jobMan;


/*-------------------------------------------------------------------------
 |
 |  Class :  JobManager
 |
 |  Description :
 |
 |   This  class implements  the  command line  parsing  and handling  of
 |   configuration files and 'job' files. The  commands can be given as a
 |   list of  key=entry entries  in a configuration  file or  directly as
 |   command line arguments.
 |
 |   It has control  over the gui, can  hide the main window  and run all
 |   jobs from the job file or from the main window.
 |
 +-------------------------------------------------------------------------*/

class JobManager : public QObject
{

  friend class MainWnd;

  Q_OBJECT

  public:
    JobManager ();
    virtual ~JobManager ();

    void     evalCmdLine    ();
    void     execJobs       ();
    void     execJobs       ( QString fileName );
    bool     getCmdMode     () { return cmdMode; }
    int      getJobCount    () { return jobQueue.count (); }
    L_String getErrors      () { return errorLog; }
    L_String getLogging     () { return logging; }

  public slots:
    void     onFinished     ();
    void     onBtnClicked   ( QAbstractButton * button );

  protected slots:
    void     onProcFinished (int,
                             QProcess::ExitStatus exitStatus );

  protected:
    void     init           ();
    void     addCmdJob      ();
    bool     checkJobInfo   ();
    void     checkJobs      ();
    void     fillQueue      ();
    void     evalArgs       ();
    void     execSystem     ( JobInfo jobInfo );
    void     execJob        ();
    bool     nextJob        ();
    void     loadConfig     ();
    void     loadJobFiles   ();
    bool     loadJobs       ();
    QString  getStatus      ();
    void     timerEvent     ( QTimerEvent * tEvent );

  protected:
    QProcess    * process;
    ToolsWidget * runTools;
    MsgLogger   * msgLog;
    M_StrStr      argsMap;
    FileUtils     fileUtils;
    ParFile       parFile;
    L_String      jobFiles;
    Q_JobInfo     jobQueue;
    JobInfo       jobInfo;
    QString       jobFile;
    QString       jobDir;
    L_Int         jobList;
    L_String      topics;
    L_String      commands;
    L_String      errorLog;
    L_String      logging;
    QString       topic;
    QString       command;
    QString       cfgFile;
    QString       status;
    QString       text;
    bool          cmdMode;      // run jobs in command mode
    bool          useGui;
    bool          closeAtEnd;
    bool          finished;
    bool          isPaused;
    bool          timerUsed;
    int           timerId;
    int           waitTime;    // in ms

};

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

extern JobManager  * jobMan;

#endif
