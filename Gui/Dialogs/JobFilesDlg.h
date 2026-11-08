/*-------------------------------------------------------------------------
 |
 |  Include-File :  JobFilesDlg.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   JobFilesDlg.
 |
 +-------------------------------------------------------------------------*/

#ifndef JobFilesDlg_h
#define JobFilesDlg_h

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class  FileSelect;

/*-------------------------------------------------------------------------
 |
 |  Class :  JobFilesDlg
 |
 |  Description :
 |
 |   This class implements  a configuration dialog for  'Job Files', they
 |   can be viewed, edited and selected.
 |
 +-------------------------------------------------------------------------*/

class JobFilesDlg : public QDialog
{

  Q_OBJECT

  public:
    JobFilesDlg ( QWidget * parent );
    virtual ~JobFilesDlg ();

  protected slots:
    void  onJobsChanged  ( QString fileName );

  protected slots:
    void  onCancel       ();
    void  onOk           ();
    void  onEditFile     ();
    void  onSaveFile     ();
    void  onFileChanged  ( const QString & watchName );

  protected:
    void  buildControls  ( int index );
    void  connectSlots   ();
    void  createControls ();
    void  createLayout   ();
    void  initDialog     ();
    void  fromGui        ();
    void  toGui          ();

  protected:
    QFileSystemWatcher * fileWatch;
    QPushButton  * btnCancel;
    QPushButton  * btnOk;
    QPushButton  * btnEdit;
    QPushButton  * btnSave;
    QTextEdit    * jobView;

    QString        jobFile;
    FileSelect   * jobFiles;
    FileUtils      fileUtils;

};

#endif
