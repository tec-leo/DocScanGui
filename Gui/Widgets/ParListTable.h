/*-------------------------------------------------------------------------
 |
 |  Include-File :  ParListTable.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   ParListTable.
 |
 +-------------------------------------------------------------------------*/

#ifndef ParListTable_h
#define ParListTable_h

#include "../../Config/ParList.h"

/*-------------------------------------------------------------------------
 |
 |  Class :  ParListTable
 |
 |  Description :
 |
 |   This class is a table widget which shows all items of a 'ParList' in
 |   the table.
 |
 +-------------------------------------------------------------------------*/

class ParListTable : public QTableWidget
{

  friend class ParTableWidget;

  Q_OBJECT

  public:
    ParListTable ( QWidget * parent );
    ParListTable ( QWidget * parent, ParList * parList );
    virtual ~ParListTable ();

    void  adjustGrid  ();
    void  set         ( QString prop, QString value );
    void  fromGui     ();
    void  toGui       ();
    void  setParList  ( ParList * parList );
    void  setParList  ( ParList * fromParList, QString tableName );

  protected:
    void  formatRow   ( int row );
    void  fillTable   ();
    void  getTable    ();
    void  init        ();
    void  resizeEvent ( QResizeEvent * event );

  protected:
    QTableWidgetItem * cell;
    ParList     parListBase;
    ParList   * parList;
    L_ParItems  parItems;
    bool        withComment;

};

#endif
