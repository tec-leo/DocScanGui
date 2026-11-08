/*-------------------------------------------------------------------------
 |
 |  Include-File :  HelpAboutDlg.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  HelpAboutDlg.
 |
 +-------------------------------------------------------------------------*/

#ifndef HelpAboutDlg_h
#define HelpAboutDlg_h


/*-------------------------------------------------------------------------
 |
 |  Class :  HelpAboutDlg
 |
 |  Description :
 |
 |   This object  is derived from QDialog  and shows the  About dialog of
 |   this program.
 |
 +-------------------------------------------------------------------------*/

class HelpAboutDlg : public QDialog
{

  Q_OBJECT

  public:
    HelpAboutDlg ( QWidget * parent );
    virtual ~HelpAboutDlg ();

  protected:
    QPushButton * btnClose;

  protected:
    void  createActions ();
    void  initDialog    ();

};

#endif
