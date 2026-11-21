/*-------------------------------------------------------------------------
 |
 |  Include-File :  ParManager.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  ParManager.
 |
 +-------------------------------------------------------------------------*/

#ifndef ParManager_h
#define ParManager_h

#include "AppConfig.h"
#include "FileConfig.h"
#include "AppCurves.h"


/*------------------------------------------*/
/*    T Y P E - D e c l a r a t i o n s     */
/*------------------------------------------*/

typedef QList < ParBase * > L_ParData;


/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class ParManager;

extern ParManager * parMan;


/*-------------------------------------------------------------------------
 |
 |  Class :  ParManager
 |
 |  Description :
 |
 |   This class provides functions for  loading and saving the parameters
 |   from a settings file.
 |
 +-------------------------------------------------------------------------*/

class ParManager: public QObject
{

  friend class MainWnd;

  Q_OBJECT

  public:
    ParManager ();
    virtual ~ParManager ();

    bool          init            ( QString name );
    void          load            ( QString name );
    void          load            ();
    bool          save            ( bool createBackup = true );

    QString       getAbsolutePath ( QString parName );
    QString       getRelativePath ( QString parName );
    QDir          getBaseDir      ();
    ParFile     * getParFile      ();
    QString       getFileName     ();
    void          setFileName     ( QString name );
    HistoryList * getConfigMRU    () { return & configMRU; }

    AppConfig   * getAppConfig    () { return appConfig; }
    FileConfig  * getFileConfig   () { return fileConfig; }
    AppCurves   * getAppCurves    () { return appCurves; }

  public slots:
    void          onLoadMRU       ( QString parName );
    bool          onLoadConfig    ();
    void          onSaveConfig    ();
    void          onSaveConfigAs  ();

  protected:
    void          loadBase        ();

  protected:
    ParFile     * parFile;
    AppConfig   * appConfig;
    FileConfig  * fileConfig;
    AppCurves   * appCurves;
    FileUtils     fileUtils;
    HistoryList   configMRU;
    L_ParData     parData;
    QDir          parDir;
    QString       workDir;
    QString       fileName;
    QString       text;

};

#endif
