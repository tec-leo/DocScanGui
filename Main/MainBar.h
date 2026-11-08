/*-------------------------------------------------------------------------
 |
 |  Include-File :  MainBar.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  MainBar.
 |
 +-------------------------------------------------------------------------*/

#ifndef MainBar_h
#define MainBar_h


/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class MainWnd;


/*-------------------------------------------------------------------------
 |
 |  Class :  MainBar
 |
 |  Description :
 |
 |   This class implements the main toolbar of DocScanGui.
 |
 +-------------------------------------------------------------------------*/

class MainBar : public QToolBar
{

  friend class MainWnd;

  Q_OBJECT

  public:
    MainBar ( MainWnd * mainWnd );
    virtual ~MainBar ();

  protected slots:
    void   helpAbout      ();
    void   helpContents   ();
    void   onBtnConfig    ();

  protected:
    void   createActions  ();
    void   connectActions ();
    void   buildToolBar   ();

  protected:
    QAction * load;
    QAction * save;
    QAction * saveAs;
    QAction * runTools;
    QAction * exit;
    QAction * editJobs;
    QAction * runJobs;
    QAction * config;
    QAction * about;
    QAction * help;

};

#endif
