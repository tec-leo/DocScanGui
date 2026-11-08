/*-------------------------------------------------------------------------
 |
 |  Include-File :  ListSelectDlg.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   ListSelectDlg.
 |
 +-------------------------------------------------------------------------*/

#ifndef ListSelectDlg_h
#define ListSelectDlg_h


/*-------------------------------------------------------------------------
 |
 |  Class :  ListSelectDlg
 |
 |  Description :
 |
 |   This dialog  class implements the  selection of  an entry in  a list
 |   box.  The 'entry' can also be deleted or a new one defined.
 |
 +-------------------------------------------------------------------------*/

class ListSelectDlg : public QDialog
{

  Q_OBJECT

  public:
    ListSelectDlg ( QWidget * parent, L_String optList,
                    QString title, bool addOptions );
    virtual ~ListSelectDlg ();

    QString  getSelected  ();

  protected slots:
    void     onSelChanged ( int row );
    void     onOk         ();

  protected:
    void    fillOptions   ();
    void    createLayout  ();
    void    initDialog    ();

  protected:
    QListWidget  * listSelect;
    QLineEdit    * lineEdit;
    QPushButton  * btnCancel;
    QPushButton  * btnOk;
    L_String       optList;
    bool           addOptions;

};


#endif
