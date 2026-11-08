/*-------------------------------------------------------------------------
 |
 |  Include-File :  DropCombo.h
 |
 |  Description :
 |
 |   This include file  contains all   class  definitions for  the  class
 |   DropComb.
 |
 +-------------------------------------------------------------------------*/

#ifndef DropCombo_h
#define DropCombo_h


/*-------------------------------------------------------------------------
 |
 |  Class :  DropCombo
 |
 |  Description :
 |
 |   This class  is derived  from QComboBox and  implements the  drag and
 |   drop mechanism for the combo box.
 |
 +-------------------------------------------------------------------------*/

class DropCombo : public QComboBox
{

  Q_OBJECT

  public:
    DropCombo ( QWidget * parent = nullptr );
    virtual ~DropCombo ();

    void      moveItem       ( int src, int dst );

  protected:
    void      dragEnterEvent ( QDragEnterEvent * event );
    void      dropEvent      ( QDropEvent * event );

  signals:
    void      textDropped    ( QString text );
    
};

#endif
