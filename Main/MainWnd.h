/*-------------------------------------------------------------------------
 |
 |  Include-File :  MainWnd.h
 |
 |  Description :
 |
 |   This include  file contains all external declarations  of the module
 |   'MainWnd'.
 |
 +-------------------------------------------------------------------------*/

#ifndef MainWnd_h
#define MainWnd_h

#include "JobManager.h"
#include "MsgLogger.h"

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class  AppCurves;
class  AppConfig;
class  FileConfig;
class  MainWnd;
class  MainBar;
class  ImageBar;
class  ImageWidget;
class  ImageView;
class  CurvesDialog;
class  ToolsWidget;

extern MainWnd  * mainWnd;

/*-------------------------------------------------------------------------
 |
 |  Class :  MainWnd
 |
 |  Description :
 |
 |   This class  contains functions for improving  and optimizing scanned
 |   documents with the  help of OpenCV. It allows  to autorotate, deskew
 |   images and increases the contrast with several different methods.
 |
 +-------------------------------------------------------------------------*/

class MainWnd : public QMainWindow
{

  Q_OBJECT

  public:
    MainWnd  ( QWidget * parent = 0, Qt::WindowFlags flags = Qt::WindowFlags () );
    virtual ~MainWnd ();

    void          executeJob       ( JobInfo curJob );
    bool          getCmdMode       () { return cmdMode; }

    MsgLogger   * getMsgLogger     ()     { return & msgLog; }
    RegFile     * getRegFile       ()     { return regFile; }
    ImageWidget * getImageWidget   ()     { return imageWidget; }
    ToolsWidget * getToolsWidget   ()     { return toolsWidget; }
    void          insertText       ( QString text );
    void          setPickerCursor  ( QCursor * cursor );
    void          updateStatusBar  ( QString topic, QString msg );
    void          closeMessageBox  ();
    void          showMessageBox   ( QString title, QString msg, int sec );
    void          fromGui          ();
    void          loadConfig       ();
    void          saveConfig       ();

  public slots:
    void          onEditJobs       ();    // called from Toolbar
    void          onRunJobs        ();    // called from Toolbar

  protected:
    void          closeEvent       ( QCloseEvent * event );
    void          clearWindows     ();
    void          createObjects    ();
    void          createSettings   ();
    void          createToolBar    ();
    void          createStatusBar  ();
    void          createLayout     ();
    void          connectSlots     ();
    void          init             ();
    void          toGui            ();

    void          loadAppState     ();
    void          saveAppState     ();

    void          execConfig       ();
    void          timerEvent       ( QTimerEvent * tEvent );

  protected:
    MainBar       * mainBar;    // toolbar of the main application
    ImageBar      * imageBar;
    ImageWidget   * imageWidget;
    ImageView     * imageView;
    ToolsWidget   * toolsWidget;

    QCursor       * cursor;

    QStatusBar    * status;
    QLabel        * statusMsg;
    QLabel        * imageInfo;
    QLabel        * configMsg;
    QLabel        * txtInfoMsg;
    RegFile       * regFile;    // contains general settings
    AppConfig     * appConfig;
    FileConfig    * fileConfig;
    QMessageBox   * msgBox;
    MsgLogger       msgLog;
    FileUtils       fileUtils;
    StringUtils     strUtils;

    QString         imgName;

    // parameter and history;
    bool            cmdMode;      // run jobs from file or command line
    bool            loaded;
    bool            isRunning;
    int             timerId;
    JobInfo         jobInfo;
    M_StrStr        infoMap;

    QString         configName;
    QString         defaultIni;
    QString         text;

};

#endif
