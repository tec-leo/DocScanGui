/*-------------------------------------------------------------------------
 |
 |  Include-File :  ButtonsWidget.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  ButtonsWidget.
 |
 +-------------------------------------------------------------------------*/

#ifndef ButtonsWidget_h
#define ButtonsWidget_h

#include "../Controls/IconButton.h"

/*-------------------------------------------------------------------------
 |
 |  Class :  ButtonsWidget
 |
 |  Description :
 |
 |   This class  implements a  widget with a  list of  IconButtons. Every
 |   button has a  name and a state  which can be toggled  between on and
 |   off. The  state is visible with  different icons for (power)  on and
 |   off.
 |
 +-------------------------------------------------------------------------*/

class ButtonsWidget : public QWidget
{

  Q_OBJECT

  public:
    ButtonsWidget ( QWidget * parent = 0 );
    virtual ~ButtonsWidget ();

    void   clear            ();

    void   addButton        ( QString name, QString tooltip );
    void   delButton        ( int index );
    void   swapButtons      ( int index1, int index2 );

    L_Bool getActiveList    ();
    int    getButtonCount   () { return buttons.count (); }

    bool   getBtnActive     ( int index );
    void   setBtnActive     ( int index, bool active );

    int    getCurrentIdx    () { return curIndex; }
    bool   getBtnChecked    ( int index );
    void   setBtnChecked    ( int index, bool checked );
    void   setFont          ( QFont font )  { btnFont = font; }
    void   setIconSize      ( int size )    { iconSize = size; }

  signals:
    void   buttonToggled    ( int index, bool active );
    void   buttonClicked    ( int index, bool clicked );

  protected slots:
    void   onBtnClicked     ( bool state );
    void   onIconToggled    ( int index );

  protected:
    void   contextMenuEvent ( QContextMenuEvent * event );
    void   createButton     ( QString name, QString tooltip );
    void   createLayout     ();
    void   updateIndex      ();

  protected:
    L_IconButton   buttons;
    IconButton   * iconBtn;
    QMenu        * contextMenu;
    QScrollArea  * scrollArea;
    QVBoxLayout  * widgetVBox;
    QWidget      * scrollWidget;
    QFont          btnFont;
    QIcon          btnEnabled;
    QIcon          btnDisabled;
    int            curIndex;
    int            iconSize;

};

#endif
