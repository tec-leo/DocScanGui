/*-------------------------------------------------------------------------
 |
 |  Include-File :  ParListGrid.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   ParListGrid.
 |
 +-------------------------------------------------------------------------*/

#ifndef ParListGrid_h
#define ParListGrid_h

#include "../../Config/ParList.h"

/*-------------------------------------------------------------------------
 |
 |  Class :  ParListGrid
 |
 |  Description :
 |
 |   This class  is a  generic widget  with a grid  layout. It  shows all
 |   items of a  'ParList' as controls in a grid  layout.  This avoids to
 |   create the  widgets for  the layout  individually for  all parameter
 |   items.
 |
 +-------------------------------------------------------------------------*/

class ParListGrid : public QWidget
{

  Q_OBJECT

  public:
    ParListGrid ( QWidget * parent );
    ParListGrid ( QWidget * parent, ParList * parList );
    virtual ~ParListGrid ();

    QWidget * getWidget ( QString parName );

    void  fromGui       ();
    void  toGui         ();
    void  setParList    ( ParList * parList );

  protected:
    void  createCheck   ( ParItem & parItem, int & row );
    void  createCombo   ( ParItem & parItem, int & row );
    void  createEdit    ( ParItem & parItem, int & row );
    void  createRadio   ( ParItem & parItem, int & row );
    void  createTable   ( ParItem & parItem, int & row );
    void  createLayout  ();

    void  init          ();

  protected:
    QLayout     * layout;
    QLabel      * label;
    QGridLayout * grid;
    QGroupBox   * group;
    QHBoxLayout * hBox;
    QVBoxLayout * vBox;
    QVBoxLayout * vMain;
    QString       text;
    QString       type;    // type of layout
    quint64       ptrVal;  // for pointer conversion

    ParList     * parList;
    L_ParItems    parItems;
    L_Widgets     widgets;
    M_StrPtr      widgetsMap;

};

#endif
