/*-------------------------------------------------------------------------
 |
 |  Include-File :  LayersList.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  LayersList.
 |
 +-------------------------------------------------------------------------*/

#ifndef LayersList_h
#define LayersList_h

#include "IconButton.h"

/*-------------------------------------------------------------------------
 |
 |  Class :  LayersList
 |
 |  Description :
 |
 |   This class  implements a  widget with a  list of  IconButtons. Every
 |   button represents a layer in the graphic  scene and has a name and a
 |   visibility  state. The  state can  be toggled  to indicate  that the
 |   layer is visible  or hidden. The visibility is  shown with different
 |   icons for (power) on and off.
 |
 +-------------------------------------------------------------------------*/

class LayersList : public QWidget
{

  Q_OBJECT

  public:
    LayersList ( QWidget * parent = 0 );
    virtual ~LayersList ();

    void   clear            ();

    void   addButton        ( QString name );
    void   delButton        ( int index = -1 );
    void   delLastButton    ();
    void   setButton        ( int indexs, QString name );

    L_Bool getActiveList    ();

    bool   getBtnActive     ( int index );
    void   setBtnActive     ( int index, bool active );

    int    getCurrentIdx    () { return curIndex; }

  signals:
    void   btnIconToggled   ( int index, bool checked );
    void   buttonClicked    ( int index, bool clicked );

  protected slots:
    void   onBtnClicked     ( bool state );
    void   onIconToggled    ( int index );

  protected:
    void   contextMenuEvent ( QContextMenuEvent * event );
    void   createButton     ( QString name );
    void   createLayout     ();
    void   updateIndex      ();

  protected:
    L_IconButton   buttons;
    IconButton   * iconBtn;
    QMenu        * contextMenu;
    QScrollArea  * scrollArea;
    QVBoxLayout  * widgetVBox;
    QWidget      * scrollWidget;
    QIcon          btnEnabled;
    QIcon          btnDisabled;
    int            curIndex;

};

#endif
