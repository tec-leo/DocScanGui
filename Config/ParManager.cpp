/*-------------------------------------------------------------------------
 |
 |  File Name   :  ParManager.cpp
 |
 |  Description :
 |
 |   This module implements the  class ParManager which provides functions
 |   for accessing the parameters from a settings file.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 20.Apr.2025
 |  Modified  : Oskar Leirich                Last changes  : 06.Nov.2026
 |
 |
 |  This unit contains the following global functions :
 |   of ParManager :
 |    ParManager                ~ParManager               getAbsolutePath
 |    getBaseDir                getFileName               getParFile
 |    getRelativePath           init                      load
 |    loadBase                  onLoadConfig              onLoadMRU
 |    onSaveConfig              onSaveConfigAs            save
 |    setFileName
 |
 +-------------------------------------------------------------------------*/

#include "ParManager.h"
#include "../Main/MainWnd.h"

/*------------------------------------------*/
/*  C O N S T -  D e c l a r a t i o n s    */
/*------------------------------------------*/

const QString cParSection = "DocScanGui";
const QString cParMain    = "Main";
const QString cConfigMRU  = "ConfigMRU";
const QString cDocCurves  = "Curves.ini";


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  ParManager
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 +-------------------------------------------------------------------------*/

ParManager::ParManager ()
{

  parMan     = this;
  fileName   = "DocScanGui.par";

  parFile    = nullptr;
  appConfig  = nullptr;
  fileConfig = nullptr;

}               /*-----  ParManager  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  of  ParManager
 |
 |  Description :
 |
 |   The destructor deletes some objects.
 |
 +-------------------------------------------------------------------------*/

ParManager::~ParManager ()
{

  delete  appCurves;

  qDeleteAll (parData);

  if (parFile)
  {
    delete parFile;
  }

}               /*-----  ~ParManager  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  init  of  ParManager
 |
 |  Description :
 |
 |   This function is  called from the main window after  the name of the
 |   parameter file is  read from the registry. It  creates all parameter
 |   classes and adds them to the list.
 |
 +-------------------------------------------------------------------------*/

bool ParManager::init ( QString name )
{
  RegFile * regFile;
  FileUtils fileUtils;

  regFile = mainWnd->getRegFile ();

  text = regFile->fileName ();
  text = fileUtils.absolutePath (text) + "/" + cDocCurves;

  appCurves  = new AppCurves ();
  appCurves->setIniName (text);

  fileName   = name;
  workDir    = QDir::currentPath ();

  appConfig  = new AppConfig ();
  fileConfig = new FileConfig ();

  parData.append (appConfig);
  parData.append (fileConfig);

  parFile = nullptr;   // will be checked in setFileName

  // update all parameter classes with the new parameter file
  setFileName (fileName);

  configMRU.load ( cConfigMRU, "MRUFile", 10 );

  connect ( & configMRU, & HistoryList::itemSelected,
            this,        & ParManager::onLoadMRU );

  if ( QFile::exists (fileName) )
    return true;
  else
    return false;

}               /*-----  init  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getParFile  of  ParManager
 |
 |  Description :
 |
 |   This function returns a pointer to the parameter file.
 |
 +-------------------------------------------------------------------------*/

ParFile * ParManager::getParFile ()
{

  return parFile;

}              /*-----  getParFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getAbsolutePath  of  ParManager
 |
 |  Description :
 |
 |   This function transforms a path relative to parameter file directory
 |   to an absolute one.
 |
 +-------------------------------------------------------------------------*/

QString ParManager::getAbsolutePath ( QString parName )
{
  QString cleanPath;
  QString tmpPath;

  tmpPath = QDir::fromNativeSeparators (parName);

  if ( parDir.isRelativePath (tmpPath) )
  {
    tmpPath = parDir.absoluteFilePath (tmpPath);
    tmpPath = parDir.cleanPath (tmpPath);
  }

  return tmpPath;

}              /*-----  getAbsolutePath  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getRelativePath  of  ParManager
 |
 |  Description :
 |
 |   This function returns the path of a file relative to parameter dir.
 |
 +-------------------------------------------------------------------------*/

QString ParManager::getRelativePath ( QString parName )
{
  QString cleanPath;
  QString tmpPath;

  tmpPath = QDir::fromNativeSeparators (parName);

  if ( parDir.isRelativePath (tmpPath) )
    tmpPath = parDir.relativeFilePath (tmpPath);

  return tmpPath;

}              /*-----  getRelativePath  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getBaseDir  of  ParManager
 |
 |  Description :
 |
 |   This function returns the base directory of the parameter file.
 |
 +-------------------------------------------------------------------------*/

QDir ParManager::getBaseDir ()
{

  return parDir;

}              /*-----  getBaseDir  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getFileName  of  ParManager
 |
 |  Description :
 |
 |   This function returns the name of the parameter file.
 |
 +-------------------------------------------------------------------------*/

QString ParManager::getFileName ()
{

  return fileName;

}              /*-----  getFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setFileName  of  ParManager
 |
 |  Description :
 |
 |   This function  sets the name  of the  parameter file. This  means it
 |   deletes the old  one and creates a  new one with the  new name. Also
 |   parDir is  updated and  the new  parameter file is  set in  all base
 |   parameter classes.
 |
 +-------------------------------------------------------------------------*/

void ParManager::setFileName ( QString name )
{
  ParBase * parBase;
  QString   parPath;
  QDir      cwd;

  fileName = QDir::fromNativeSeparators (name);

  cwd.setPath ( QDir::currentPath () );

  parPath = cwd.absoluteFilePath (fileName);
  parPath = parPath.left ( parPath.lastIndexOf ( '/' ) );
  parDir  = QDir (parPath);

  if (parFile)
    delete parFile;

  parFile = new ParFile ();
  parFile->setFileName (fileName);

  for ( int i = 0; i < parData.count (); i++ )
  {
    parBase = parData.at (i);
    parBase->setParFile (parFile);
  }

}              /*-----  setFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  loadBase  of  ParManager
 |
 |  Description :
 |
 |   This function loads the parameter from the parameter file.
 |
 +-------------------------------------------------------------------------*/

void ParManager::loadBase ()
{
  ParBase * parBase;

  if ( not QFile::exists (fileName) )
  {
    return;
  }

  parFile->setSection (cParSection);

  for ( int i = 0; i < parData.count (); i++ )
  {
    parBase = parData.at (i);
    parBase->load ();
  }

}              /*-----  loadBase  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  load  of  ParManager
 |
 |  Description :
 |
 |   This function loads the parameter from the file with the given name.
 |
 +-------------------------------------------------------------------------*/

void ParManager::load ( QString name )
{

  setFileName (name);

  load ();

}              /*-----  load  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  load  of  ParManager
 |
 |  Description :
 |
 |   This function loads the parameter from the parameter file.
 |
 +-------------------------------------------------------------------------*/

void ParManager::load ()
{

  appCurves->load ();

  parFile->load ();

  configMRU.append (fileName);
  configMRU.save ();

  loadBase ();

  mainWnd->loadConfig ();

  mainWnd->updateStatusBar ( "Config", fileName );

}              /*-----  load  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  save  of  ParManager
 |
 |  Description :
 |
 |   This function saves the parameter in the parameter file.
 |
 +-------------------------------------------------------------------------*/

bool ParManager::save ( bool createBackup )
{
  ParBase * parBase;
  QString   help;
  QString   tmp;
  QString   suffix;
  QString   oldName;
  QString   baseName;
  int       pos;

  if (createBackup)
  {
    pos = fileName.lastIndexOf ( '.' );
    baseName = fileName.left (pos);
    suffix   = fileName.mid (pos);

    // store a backup file
    for ( int i = 0; i < 10000; i++ )
    {
      help = QString ( "(%1)" ).arg ( i + 1, 2, 10, QChar ('0') );
      tmp = baseName + help + suffix;

      if ( not QFile::exists (tmp) )
      {
        QFile::copy ( fileName, tmp );
        break;
      }
    }
  }

  appCurves->save ();

  configMRU.append (fileName);
  configMRU.save ();

  mainWnd->saveConfig ();

  parFile->setFileName (fileName);

  parFile->setSection (cParSection);

  for ( int i = 0; i < parData.count (); i++ )
  {
    parBase = parData.at (i);
    parBase->save ();
  }

  return parFile->save ();    // write to file

}              /*-----  save  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onLoadMRU  of  ParManager
 |
 |  Description :
 |
 |   This function is called from the main window frame if a new parameter
 |   file should be loaded from the recent list.
 |
 |  Input parameters :
 |   parName         : name and path of new parameter file
 |
 +-------------------------------------------------------------------------*/

void ParManager::onLoadMRU ( QString parName )
{
  QString msg;

  if ( not QFile::exists (parName) )
  {
    msg = "The configuration file you selected does not exist :\n" + parName;

    QMessageBox::warning ( 0, "Load Config", msg );

    return;
  }

  fileName = parName;
  parFile->setFileName (parName);

  load ();

}               /*-----  onLoadMRU  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onLoadConfig  of  ParManager
 |
 |  Description :
 |
 |   This  function is  called  from  the main  toolbar  frame  if a  new
 |   parameter file should be selected from a dialog and loaded.
 |
 +-------------------------------------------------------------------------*/

bool ParManager::onLoadConfig ()
{
  QString tmpName;
  QString filter;
  QString title;

  filter = "Config (*.ini);;All files (*.*)";
  title  = "Select configuration file";

  tmpName = QFileDialog::getOpenFileName ( mainWnd, title, fileName, filter );

  if ( ( tmpName.isEmpty () ) or ( not QFile::exists (tmpName) ) )
    return false;

  if ( fileName.compare ( tmpName, Qt::CaseInsensitive ) != 0 )
  {
    setFileName (fileName);   // creates and updates also parFile
  }

  load ();   // load base parameter classes

  return true;

}                   /*-----  onLoadConfig  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onSaveConfig  of  ParManager
 |
 |  Description :
 |
 |   This function is called from the  main window frame if all parameter
 |   should be saved into a parameter file.
 |
 +-------------------------------------------------------------------------*/

void ParManager::onSaveConfig ()
{

  if ( fileName.isEmpty () )
    return;

  save ();

  mainWnd->statusBar ()->showMessage ( "Configuration file saved !", 5000 );

}               /*-----  onSaveConfig  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onSaveConfigAs  of  ParManager
 |
 |  Description :
 |
 |   This function saves  the configuration options into a  file with the
 |   name of fileName'.
 |
 +-------------------------------------------------------------------------*/

void ParManager::onSaveConfigAs ()
{
  QString parName;
  QString msg;

  parName = fileName;
  parName = QFileDialog::getSaveFileName ( mainWnd,
                                           "Select configuration file",
                                           parName,
                                           "Config (*.ini);;All files (*.*)" );

  if ( not parName.isEmpty () )
  {
    fileName = parName;

    if ( QFile::exists (fileName) )
      QFile::remove (fileName);

    parFile->clear ();

    save ();

    msg = "Config file saved as : " + fileName;
    mainWnd->statusBar ()->showMessage ( msg, 5000 );
    mainWnd->updateStatusBar ( "Config", fileName );
  }

}                   /*-----  onSaveConfigAs  -----*/
