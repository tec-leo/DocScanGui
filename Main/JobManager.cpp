/*-------------------------------------------------------------------------
 |
 |  File Name   :  JobManager.cpp
 |
 |  Description :
 |
 |   This  module contains  the class  'JobManager' which  implements the
 |   command line parsing and handling  of 'job' files.  The commands can
 |   be given as a list of key / entry pairs in a configuration file.  in
 |   the job file.
 |
 |   It has control  over the gui, can  hide the main window  and run all
 |   jobs from the job file or from the main window.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 23.Sep.2025
 |  Modified  : Oskar Leirich                Last changes  : 18.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of JobManager :
 |    JobManager                ~JobManager               addCmdJob
 |    checkJobs                 evalArgs                  evalCmdLine
 |    execJob                   execJobs                  execSystem
 |    fillQueue                 getStatus                 init
 |    loadConfig                loadJobFiles              loadJobs
 |    nextJob                   onFinished                timerEvent
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "JobManager.h"
#include "MainWnd.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  JobManager
 |
 |  Description :
 |
 |   The constructor call init to evaluate the command line options.
 |
 +-------------------------------------------------------------------------*/

JobManager::JobManager ()
{

  init ();

}               /*-----  JobManager  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  of  JobManager
 |
 |  Description :
 |
 |   The destructor deletes the process if it exists.
 |
 +-------------------------------------------------------------------------*/

JobManager::~JobManager ()
{

  if ( process != nullptr )
    delete process;

}               /*-----  ~JobManager  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  init  of  JobManager
 |
 |  Description :
 |
 |   This function  is called  from the  constructor and  initializes the
 |   member  variables  and maps.   It  creates  a process  instance  and
 |   connects the finish function.
 |
 +-------------------------------------------------------------------------*/

void JobManager::init ()
{

  command    = "";
  cfgFile    = "";
  jobFile    = "";
  topic      = "";
  status     = "ready";
  cmdMode    = false;
  useGui     = false;
  closeAtEnd = true;
  finished   = true;
  timerId    = 0;
  timerUsed  = true;
  isPaused   = false;
  waitTime   = 1000;

  process    = nullptr;
  runTools   = mainWnd->getToolsWidget ();
  msgLog     = mainWnd->getMsgLogger ();

  process = new QProcess ();

  connect ( process, & QProcess::finished,
            this,    & JobManager::onProcFinished );

  topics.append ( "system" );
  topics.append ( "config" );
  topics.append ( "image" );
  topics.append ( "tools" );

  commands.append ( "config-load-config" );
  commands.append ( "config-save-config" );
  commands.append ( "config-save-as"     );
  commands.append ( "config-set-options" );
  commands.append ( "config-load-curve"  );

  commands.append ( "image-load-image"   );
  commands.append ( "image-save-image"   );
  commands.append ( "image-export-dir"   );
  commands.append ( "image-export-image" );

  commands.append ( "tools-run-chain"    );
  commands.append ( "tools-run-tools"     );

  jobQueue.clear ();
  errorLog.clear ();
  logging.clear ();

}               /*-----  init  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  addCmdJob  of  JobManager
 |
 |  Description :
 |
 |   This function  checks if there are  is a job defined  on the command
 |   line and if so then it is directly added to the job queue.
 |
 +-------------------------------------------------------------------------*/

void JobManager::addCmdJob ()
{

  jobQueue.clear ();

  // default for only command line job
  if ( not topic.isEmpty () )
  {
    jobInfo.topic   = topic;
    jobInfo.command = command;

    jobInfo.infoMap [ "command" ] = command;
    jobInfo.infoMap [ "topic"   ] = topic;

    if ( checkJobInfo () )
      jobQueue.enqueue (jobInfo);
  }

}               /*-----  addCmdJob  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  checkJobInfo  of  JobManager
 |
 |  Description :
 |
 |   This function checks the topics and commands in current 'jobInfo' if
 |   are valid.  If they don't exist  then an error log  entry is created
 |   and the function returns false.
 |
 +-------------------------------------------------------------------------*/

bool JobManager::checkJobInfo ()
{
  QString msg;

  if ( not topics.contains (jobInfo.topic) )
  {
    msg = "Topic " + jobInfo.topic + " is not valid";
    errorLog.append (msg);

    return false;
  }

  if ( jobInfo.topic == "system")   // not command check necessary
    return true;

  text = jobInfo.topic + "-" + jobInfo.command;

  if ( not commands.contains (text) )
  {
    msg = "Command" + jobInfo.command + " is not valid";
    errorLog.append (msg);

    return false;
  }

  return true;

}               /*-----  checkJobInfo  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  checkJobs  of  JobManager
 |
 |  Description :
 |
 |   This function  checks the 'jobFile' if  there are job files  or jobs
 |   defined, stores them in  the job list and adds the  first job to the
 |   job queue.
 |
 +-------------------------------------------------------------------------*/

void JobManager::checkJobs ()
{

  jobQueue.clear ();

  if ( not loadJobs () )  // checks also if file exists
    return;

  loadJobFiles ();        // load also other job files

  if ( jobList.isEmpty () ) // master job file has no jobs
  {
    // check for additional job files
    for ( int i = 0; i < jobFiles.count (); i++ )
    {
      jobFile = jobFiles.takeFirst ();

      if ( loadJobs () )  // checks also if file exists
        break;
    }
  }

  if ( not jobList.isEmpty () )   // jobs exist
  {
    fillQueue ();
  }
  else   // default for only command line job
  {
    if ( not topic.isEmpty () )
    {
      jobInfo.topic  = topic;
      jobInfo.command = command;

      jobInfo.infoMap [ "command" ] = command;
      jobInfo.infoMap [ "topic"   ] = topic;

      jobQueue.enqueue (jobInfo);
    }
  }

}               /*-----  checkJobs  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  execJob  of  JobManager
 |
 |  Description :
 |
 |   This function dequeues  the next job from the job  list and forwards
 |   it  to  the main  window  which  dispatches  it depending  from  the
 |   topic. Only the topic 'system' is handled directly in this module.
 |
 +-------------------------------------------------------------------------*/

void JobManager::execJob ()
{

  jobInfo = jobQueue.dequeue ();

  if ( jobInfo.topic == "system" )
  {
    execSystem (jobInfo);

    return;
  }

  text = "\tcommand : " + jobInfo.command + " topic " + jobInfo.topic;
  msgLog->putInfo ( "Job manager", "execJobs", text );

  // default always goes to the main windows as job distributor
  mainWnd->executeJob (jobInfo);

}               /*-----  execJob  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  execJobs  of  JobManager
 |
 |  Description :
 |
 |   This function executes all jobs in the job queue. For this it starts
 |   a timer to decouple the execution from the caller of the file.  This
 |   is the  place to implement  logging, reporting etc.  between several
 |   jobs.
 |
 +-------------------------------------------------------------------------*/

void JobManager::execJobs ()
{

  if ( not jobQueue.isEmpty () )
  {
    timerId = 0;

    startTimer (200);
  }

}               /*-----  execJobs  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  execJobs  of  JobManager
 |
 |  Description :
 |
 |   This function checks if the file with the given 'fileName' exists and
 |   checks the jobs and fills the queue in this case.
 |
 +-------------------------------------------------------------------------*/

void JobManager::execJobs ( QString fileName )
{

  if ( not fileName.isEmpty () and QFile::exists (fileName) )
  {
    jobFile = fileName;

    checkJobs ();
  }
  else
  {
    text = "Cannot open jobfile : " + fileName + " !!";
    mainWnd->show ();

    msgLog->showError ( "JobManager", "execJobs", text );
    return;
  }

  execJobs ();

}               /*-----  execJobs  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  execSystem  of  JobManager
 |
 |  Description :
 |
 |   This function executes the system  commands given in the JobInfo. It
 |   can  contain any  call for  an executable,  script, batch  or system
 |   command.
 |
 |   Example:
 |    [Job-1]
 |    topic=system
 |    command=notepad
 |    args=1
 |    arg-1=d:\Test\runjob.txt
 |
 +-------------------------------------------------------------------------*/

void JobManager::execSystem ( JobInfo jobInfo )
{
  L_String args;
  QString  arg;
  QString  text;
  int      cnt;

  timerId  = 1;
  waitTime = 0;

  if ( jobInfo.infoMap.contains ( "waitTime" ) )
  {
    text     = jobInfo.infoMap ["waitTime"];   // time in sec.
    waitTime = qRound ( 1000.0 * text.toInt () );
  }

  if ( jobInfo.command == "message-box" )
  {
    arg  = jobInfo.infoMap ["title"];
    text = jobInfo.infoMap ["message"];

    timerUsed = false;

    QMessageBox::information ( mainWnd, arg, text );

    timerUsed = true;

    return;
  }

  if ( jobInfo.command == "pause" )
  {
    text = jobInfo.infoMap ["waitTime"];
    timerId = 1;

    isPaused = true;
    startTimer (waitTime);

    return;
  }

  args.clear ();
  args.append (jobInfo.command);

  text = jobInfo.infoMap ["args"];
  cnt  = text.toInt ();

  for ( int i = 0; i < cnt; i++ )
  {
    text = QString ( "arg-%1" ).arg ( i + 1 );
    arg  = jobInfo.infoMap [text];
    args.append (arg);
  }

  arg = args.join ( " " );

  finished = false;

  process->startCommand (arg);

}               /*-----  execSystem  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  evalArgs  of  JobManager
 |
 |  Description :
 |
 |   This function  parses the arguments from  a config file or  from the
 |   command line. The arguments from the command line have priority over
 |   options in the config file.  They  are already converted to a string
 |   map for easier parsing.
 |
 +-------------------------------------------------------------------------*/

void JobManager::evalArgs ()
{
  L_String keys;
  QString  key;
  QString  entry;
  QDir     dir;

  cmdMode = false;
  command = "";
  topic   = "";

  jobQueue.clear ();

  keys = argsMap.keys ();

  for ( int i = 0; i < keys.count (); i++ )
  {
    key   = keys [i];
    entry = argsMap [key];

    if ( key == "cwd" )
    {
      QDir::setCurrent (entry);
      continue;
    }

    if ( key == "useGui" )
    {
      if ( entry == "1" )
        useGui = true;
      else
        useGui = false;
    }

    if ( key == "topic" )
    {
      topic = entry;
      continue;
    }

    if ( key == "command" )
    {
      command = entry;
      continue;
    }

    if ( key == "jobFile" )
    {
      jobFile = entry;
      continue;
    }
  }

  if ( not jobFile.isEmpty () and QFile::exists (jobFile) )
  {
    checkJobs ();
  }
  else
  {
    if ( not topic.isEmpty () )
      addCmdJob ();
  }

}                   /*-----  evalArgs  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  evalCmdLine  of  JobManager
 |
 |  Description :
 |
 |   This  function parses  the command  line for  options. It  split the
 |   options into key value pairs and stores them in a map. It there is a
 |   key with the name 'arg_file' then  it loads also the key value pairs
 |   from it. The name of the job file is handled in eval Args.
 |
 +-------------------------------------------------------------------------*/

void JobManager::evalCmdLine ()
{
  L_String args;
  L_String curArgs;
  M_StrStr newArgs;
  L_String keys;
  QString  option;
  QString  key;
  QString  entry;

  newArgs.clear ();
  argsMap.clear ();

  args = QCoreApplication::arguments ();

  // skip args [0] - it is the file path to exe
  for ( int i = 1; i < args.count (); i++ )
  {
    option = args [i];

    if ( option.startsWith ( "--" ) )
      option = option.mid (2);
    else
      continue;    // no valid option

    curArgs = option.split ( "=" );

    if ( curArgs.count () != 2 )
      continue;    // no valid option

    key   = curArgs [0];
    entry = curArgs [1];

    newArgs [key] = entry;
  }

  key = "arg_file";

  if ( newArgs.contains (key) )
  {
    cfgFile = newArgs [key];

    loadConfig ();

    // overwrite config file arguments with command line arguments
    keys = newArgs.keys ();

    for ( int i = 0; i < keys.count (); i++ )
    {
      key = keys [i];
      entry = newArgs [key];

      // copy to argsMap
      argsMap [key] = entry;
    }
  }
  else
    argsMap = newArgs;

  evalArgs ();

  if ( jobQueue.isEmpty () and ( topic.isEmpty () ) )
  {
    cmdMode = false;
    argsMap.clear ();
  }
  else
    cmdMode = true;

}            /*-----  evalCmdLine  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fillQueue  of  JobManager
 |
 |  Description :
 |
 |   This function fills the job queue with all jobs in the job list.
 |
 +-------------------------------------------------------------------------*/

void JobManager::fillQueue ()
{
  QString text;
  int     jobNo;

  jobQueue.clear ();

  for ( int i = 0; i < jobList.count (); i++ )
  {
    jobNo = jobList [i];

    text.setNum (jobNo);
    parFile.setSection ( "Job-" + text );

    jobInfo.init ();
    jobInfo.jobNo = jobNo;
    jobInfo.jobFile = jobFile;

    parFile.findPar ( "topic",   jobInfo.topic );
    parFile.findPar ( "command", jobInfo.command );

    jobInfo.infoMap = parFile.getSectionMap ();

    if ( checkJobInfo () )
      jobQueue.enqueue (jobInfo);
  }

}               /*-----  fillQueue  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getStatus  of  JobManager
 |
 |  Description :
 |
 |   This function returns in a string if all jobs are finished, working,
 |   waiting, pending etc.
 |
 +-------------------------------------------------------------------------*/

QString JobManager::getStatus ()
{

  if ( jobQueue.isEmpty () )
    status = "ready";
  else
    status = "running";

  return status;

}               /*-----  getStatus  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  nextJob  of  JobManager
 |
 |  Description :
 |
 |   This function executes the next job  if the queue is not empty. When
 |   the queue is  empty then is checks if there  are other job (include)
 |   files available.  If  so then it fills the queue  with next job file
 |   and returns if the queue is empty and all jobs are finished.
 |
 +-------------------------------------------------------------------------*/

bool JobManager::nextJob ()
{

  if ( not jobQueue.isEmpty () )
  {
    execJob ();
  }

  if ( jobQueue.isEmpty () )   // current queue has finished
  {
    // check for additional job files
    for ( int i = 0; i < jobFiles.count (); i++ )
    {
      jobFile = jobFiles.takeFirst ();

      if ( loadJobs () )  // checks also if file exists
      {
        fillQueue ();
        break;
      }
    }
  }

  if ( not jobQueue.isEmpty () )
    return true;
  else
    return false;

}               /*-----  nextJob  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  loadConfig  of  JobManager
 |
 |  Description :
 |
 |   This function  is called from  'evalCmdLine' and loads and  parses a
 |   config file  for options.   This config  file contains  command line
 |   options in ini format, this means it contains key / entry pairs like
 |   useGui=true.  The arguments from the command line have priority over
 |   options in the config file.
 |
 |   The config file can be set  on the command line with name args_file,
 |   like this: --arg_file=D:/DocScanGui/Scan/DocGray-1.cfg
 |
 +-------------------------------------------------------------------------*/

void JobManager::loadConfig ()
{
  L_String lines;
  L_String curArgs;
  QString  option;
  QString  key;
  QString  entry;

  if ( cfgFile.isEmpty () or ( not QFile::exists (cfgFile) ) )
    return;

  fileUtils.setFileName (cfgFile);
  fileUtils.readFile (lines);

  // skip args [0] - it is the file path to .exe
  for ( int i = 0; i < lines.count (); i++ )
  {
    option = lines [i];

    curArgs = option.split ( "=" );

    if ( curArgs.count () != 2 )
      continue;    // no valid option

    key   = curArgs [0];
    entry = curArgs [1];

    argsMap [key] = entry;
  }

}                   /*-----  loadConfig  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  loadJobs  of  JobManager
 |
 |  Description :
 |
 |   This function loads and parses the config file for options. They are
 |   organized in config-ini style and can contain several different jobs.
 |
 +-------------------------------------------------------------------------*/

bool JobManager::loadJobs ()
{
  L_String jobs;
  L_String values;
  QString  info;
  QString  text;
  QString  jobCount;
  int      minIdx;
  int      maxIdx;
  int      curIdx;

  jobList.clear ();

  if ( jobFile.isEmpty () or ( not QFile::exists (jobFile) ) )
    return false;

  jobCount.clear ();

  useGui     = false;
  closeAtEnd = true;
  cmdMode    = true;

  text = "Load job file " + jobFile;
  msgLog->putInfo ( "Job manager", "loadJobs", text );

  parFile.setFileName (jobFile);
  parFile.load ();

  parFile.setSection ("Jobs" );

  parFile.findPar ( "useGui",     useGui );
  parFile.findPar ( "closeAtEnd", closeAtEnd );

  // do we have jobs in the master file?
  if ( not parFile.findPar ( "JobCount", jobCount ) )
    return true;

  jobs = jobCount.split ( "," );

  for ( int i = 0; i < jobs.count (); i++ )
  {
    info = jobs [i];

    if ( info.contains ( "-" ) )
    {
      values = info.split ( "-" );

      minIdx = values [0].toInt ();
      maxIdx = values [1].toInt ();

      for ( int idx = minIdx; idx <= maxIdx; idx++ )
      {
        curIdx = idx;   // count starts from 1

        jobList.append (curIdx);
      }
    }
    else
    {
      values = info.split ( " " );

      for ( int idx = 0; idx < values.count (); idx++ )
      {
        text = values [idx];
        curIdx = text.toInt ();   // count starts from 1

        jobList.append (curIdx);
      }
    }
  }

  return true;

}                   /*-----  loadJobs  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  loadJobFiles  of  JobManager
 |
 |  Description :
 |
 |   This function loads  and parses the job file for  options. It checks
 |   if there are references to other job files which should be loaded.
 |
 |   The entry  'JobFiles' contains just  numbers of job files,  the file
 |   names are  stored in seperate entries,  the path can be  absolute or
 |   relative. If the name contains no path then they are found when they
 |   are in the same directoy as the main job file.
 |
 |  Example:
 |
 |   [Jobs]
 |   JobCount=1,2-6
 |   JobFiles=2,3,5
 |   JobFile-1=RunJob-1.txt
 |   JobFile-2=RunJob-2.txt
 |   JobFile-3=RunJob-3.txt
 |   JobFile-4=RunJob-4.txt
 |   JobFile-5=RunJob-5.txt
 |
 |   Here the job files 2, 3 and  5 are loaded and executed after the the
 |   jobs 1 and 2-6 are executed and finished.
 |
 +-------------------------------------------------------------------------*/

void JobManager::loadJobFiles ()
{
  L_String jobs;
  L_String values;
  QString  jobCount;
  QString  info;
  QString  text;
  QString  name;
  QString  curIdx;
  int      minIdx;
  int      maxIdx;

  jobFiles.clear ();

  if ( jobFile.isEmpty () or ( not QFile::exists (jobFile) ) )
    return;

  jobFile = QDir::fromNativeSeparators (jobFile);

  jobDir = fileUtils.absolutePath (jobFile);

  parFile.setFileName (jobFile);
  parFile.load ();

  parFile.setSection ("Jobs" );

  // check if it contains JobFiles
  if ( not parFile.findPar ( "JobFiles", jobCount ) )
    return;

  jobs = jobCount.split ( "," );

  for ( int i = 0; i < jobs.count (); i++ )
  {
    info = jobs [i];

    if ( info.contains ( "-" ) )
    {
      values = info.split ( "-" );

      minIdx = values [0].toInt ();
      maxIdx = values [1].toInt ();

      for ( int idx = minIdx; idx <= maxIdx; idx++ )
      {
        curIdx.setNum (idx);

        text = "JobFile-" + curIdx;

        parFile.findPar ( text, name );

        if ( not QFile::exists (name) )
          name = jobDir + "/" + name;

        if ( QFile::exists (name) )
          jobFiles.append (name);
      }
    }
    else
    {
      values = info.split ( " " );

      for ( int idx = 0; idx < values.count (); idx++ )
      {
        text = "JobFile-" + values [idx];

        parFile.findPar ( text, name );

        if ( not QFile::exists (name) )
          name = jobDir + "/" + name;

        if ( QFile::exists (name) )
          jobFiles.append (name);
      }
    }
  }

}                   /*-----  loadJobFiles  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onFinished  of  JobManager
 |
 |  Description :
 |
 |   This function is called from the  timer when the job queue is empty.
 |   It shows  the main  windows if  it was hidden  to show  a 'finished'
 |   message box  for a short  time and then  starts a timer.   When this
 |   timer triggers it  checks the options for  'closeAtEnd' and 'useGui'
 |   to hide and/or closing the main window.
 |
 +-------------------------------------------------------------------------*/

void JobManager::onFinished ()
{

  finished = true;

  if ( not cmdMode )
    return;

  text = "Job file " + jobFile + " finished";
  msgLog->putInfo ( "Job manager", "finished", text );

  if ( useGui == 0 )
    mainWnd->show ();

  // show for 2 seconds
  mainWnd->showMessageBox ( "Job Manager", text, 4 );

  timerId = 3;     // wait for end message
  startTimer (5000);

}               /*-----  onFinished  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onBtnClicked  of  JobManager
 |
 |  Description :
 |
 |   This  function is  called  from  a sQMessageBox  when  a button  was
 |   clicked.
 |
 +-------------------------------------------------------------------------*/

void JobManager::onBtnClicked ( QAbstractButton * )
{

  isPaused = false;

}               /*-----  onBtnClicked  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onProcFinished  of  JobManager
 |
 |  Description :
 |
 |   This function  is called  (signaled) when  the process  finishes and
 |   updates the status bar with the exit status.
 |
 |  Input parameters :
 |   exitCode        : Exit code of the process
 |   exitStatus      : Exit status - Normal or crash
 |
 +-------------------------------------------------------------------------*/

void JobManager::onProcFinished ( int, QProcess::ExitStatus exitStatus )
{

  finished = true;

  if ( exitStatus == QProcess::CrashExit )
    text = "Process crashed";
  else
    text = "Process finished normally";

  mainWnd->updateStatusBar ( "Info", text );

  process->close ();

}               /*-----  onProcFinished  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  timerEvent  of  JobManager
 |
 |  Description :
 |
 |   This function  is called when  a timer event  arrived. . If  the job
 |   queue is not empty then timer 0 is used to start the next task, this
 |   is done also from 'execJobs'.
 |
 +-------------------------------------------------------------------------*/

void JobManager::timerEvent ( QTimerEvent * tEvent )
{
  int  id;

  id = tEvent->timerId ();

  killTimer (id);

  if ( not timerUsed )
    return;

  switch (timerId)
  {
    case 0:
    {
      nextJob ();

      if (isPaused)
        return;
    }
    break;

    case 1:    // pause timer, or tools chain running
    {
      isPaused = false;
    }
    break;

    case 2:    // end case, close main application
    {
      mainWnd->close ();
      return;
    }
    break;

    case 3:    // close  end message box
    {
      if ( useGui == 0 )
        mainWnd->hide ();

      if (closeAtEnd)
      {
        timerId = 2;
        startTimer (1000);
      }
      return;
    }
    break;
  }

  timerId = 0;

  if ( not jobQueue.isEmpty () )
    startTimer (100);
  else
  {
    onFinished ();
    cmdMode = false;
  }

}            /*-----  timerEvent  -----*/

