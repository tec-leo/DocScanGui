/*-------------------------------------------------------------------------
 |
 |  Include-File :  DropLineEdit.h
 |
 |  Description :
 |
 |   This include file  contains all   class  definitions for  the  class
 |   DropComb.
 |
 +-------------------------------------------------------------------------*/

#ifndef DropLineEdit_h
#define DropLineEdit_h


/*-------------------------------------------------------------------------
 |
 |  Class :  DropLineEdit
 |
 |  Description :
 |
 |   This class  is derived  from QLineEdit and  implements the  drag and
 |   drop mechanism for the line edit field.
 |
 +-------------------------------------------------------------------------*/

class DropLineEdit : public QLineEdit
{

  Q_OBJECT

  public:
    DropLineEdit ( QWidget * parent = nullptr );
    virtual ~DropLineEdit ();

  protected:
    void      dragEnterEvent ( QDragEnterEvent * event );
    void      dropEvent      ( QDropEvent * event );

  signals:
    void      textDropped    ( QString text );

};

#endif
