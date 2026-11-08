/*-------------------------------------------------------------------------
 |
 |  File Name   :  ParListTable.cpp
 |
 |  Description :
 |
 |   This module  implements a the  class 'ParListTable' which  shows all
 |   items of a 'ParList' in a table widget.
 |
 |  Developer : Oskar Leirich                Creation date : 18.Oct.2023
 |  Modified  : Oskar Leirich                Last changes  : 19.Aug.2024
 |
 |
 |  This unit contains following classes :
 |   ParListTable
 |
 |  This unit contains following member functions :
 |   of ParListTable:
 |    ParListTable              ~ParListTable             fillTable
 |    fromGui                   getTable                  init
 |    setParList                toGui
 |
 +-------------------------------------------------------------------------*/

#include "ParListTable.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  ParListTable
 |
 |  Description :
 |
 |   There  are two  constructors,  one is  the  default constructor,  to
 |   really  use  it  'setParList'  must be  called  before.  The  second
 |   receives a pointer to the ParList directly.
 |
 |  Input parameters :
 |   parent          :  Pointer to the parent window
 |   parList         :  Pointer to the list of parameter items.
 |
 +-------------------------------------------------------------------------*/

ParListTable::ParListTable ( QWidget * parent ) : QTableWidget (parent)
{

  parList = nullptr;

}               /*----- ParListTable -----*/


ParListTable::ParListTable ( QWidget * parent, ParList * parList )
                           : QTableWidget (parent)
{

  this->parList = parList;

  init ();

}               /*----- ParListTable -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  ParListTable
 |
 |  Description :
 |
 |   The destructor deletes all  dynamically created objects if there are
 |   any.
 |
 +-------------------------------------------------------------------------*/

ParListTable::~ParListTable ()
{


}               /*-----  ~ParListTable  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : adjustGrid  of  ParListTable
 |
 |  Description :
 |
 |   This function adjusts the layout of the grid, more precise the width
 |   of the columns  so that the layout is adjusted as much as possible.
 |
 +-------------------------------------------------------------------------*/

void  ParListTable::adjustGrid ()
{
  QHeaderView * header;

  resizeColumnsToContents ();

  setColumnWidth ( 1, 40 );

  header = horizontalHeader ();

  header->setSectionResizeMode ( 0, QHeaderView::Fixed );
  header->setSectionResizeMode ( 1, QHeaderView::Interactive );

  if (withComment)
    header->setSectionResizeMode ( 2, QHeaderView::Stretch );

  resizeRowsToContents ();

}                   /*-----  adjustGrid  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  formatRow  of  ParListTable
 |
 |  Description :
 |
 |   This  function   fills  the   row  of   the  table   with  'special'
 |   cell-formats.
 |
 +-------------------------------------------------------------------------*/

void ParListTable::formatRow ( int row )
{
  Qt::ItemFlags flags;
  ParItem parItem;
  QColor  color;
  QString text;
  QFont   font;

  parItem = parItems [row];
  parItem.toString ();

  text = parItem.name;
  cell = new QTableWidgetItem ();
  cell->setText (text);
  flags = cell->flags () & ( ~Qt::ItemIsEditable );
  cell->setFlags (flags);

  if ( parItem.infos.contains ( "fgColor" ) )
  {
    text = parItem.infos.value ( "fgColor");
    color = QColor::fromString (text);

    cell->setForeground ( QBrush (color) );
  }

  if ( parItem.infos.contains ( "setFont") )
  {
    font = cell->font ();

    if ( parItem.infos.contains ( "fontFamily" ) )
    {
      text = parItem.infos.value ( "fontFamily");
      font.setFamily (text);
    }

    if ( parItem.infos.contains ( "fontSize" ) )
    {
      text = parItem.infos.value ( "fontSize" );
      font.setPointSize ( text.toInt () );
    }

    if ( parItem.infos.contains ( "fontWeight" ) )
    {
      text = parItem.infos.value ( "fontWeight" );
      font.setWeight ( (QFont::Weight) text.toInt () );
    }

    cell->setFont (font);
  }

  setItem ( row, 0, cell );

  cell = new QTableWidgetItem ();
  cell->setFlags (flags);
  setItem ( row, 1, cell );

  if (withComment)
  {
    text = parItem.infos ["info"];
    cell = new QTableWidgetItem ();
    cell->setFlags (flags);
    cell->setText (text);
    setItem ( row, 2, cell );
  }

}               /*-----  formatRow  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fillTable  of  ParListTable
 |
 |  Description :
 |
 |   This function fills the table with content of the par List.
 |
 +-------------------------------------------------------------------------*/

void ParListTable::fillTable ()
{
  Qt::ItemFlags flags;
  QHeaderView * header;
  L_String      labels;
  ParItem       parItem;
  QString       text;
  int           rows;
  int           cols;

  clear ();

  rows = parItems.count ();

  // text for header row (title)
  labels.append ( "Name" );
  labels.append ( "Value" );

  if (withComment)
  {
    cols = 3;
    labels.append ( "Comment" );
  }
  else
    cols = 2;

  // table dimensions
  setRowCount (rows);
  setColumnCount (cols);

  setHorizontalHeaderLabels (labels);

  for ( int i = 0; i < rows; i++ )
  {
    parItem = parItems [i];
    parItem.toString ();

    if ( parItem.infos ["control"] == "cell-format" )
    {
      formatRow (i);
    }
    else
    {
      text = parItem.name;
      cell = new QTableWidgetItem ();
      flags = cell->flags () & ( ~Qt::ItemIsEditable );
      cell->setFlags (flags);
      cell->setText (text);

      text = parItem.infos ["info"];
      cell->setToolTip (text);
      setItem ( i, 0, cell );

      text = parItem.valueStr;
      cell = new QTableWidgetItem ();
      cell->setText (text);
      setItem ( i, 1, cell );

      if (withComment)
      {
        text = parItem.infos ["info"];
        cell = new QTableWidgetItem ();
        cell->setFlags (flags);
        cell->setText (text);
        setItem ( i, 2, cell );
      }
    }
  }

  header = new QHeaderView ( Qt::Horizontal, this );
  setHorizontalHeader (header);

  adjustGrid ();

}               /*-----  fillTable  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : getTable  of  ParListTable
 |
 |  Description :
 |
 |   This function  is called when  the values  should be copied  back to
 |   ISA. It reads the table and stores it in the arrays.
 |
 +-------------------------------------------------------------------------*/

void ParListTable::getTable ()
{
  ParItem parItem;
  QString text;
  int     rows;

  rows = rowCount ();

  // fill table with arrays
  for ( int i = 0; i < rows; i++ )
  {
    parItem = parItems [i];

    if ( parItem.infos ["control"] == "cell-format" )
      continue;

    cell = item ( i, 1 );
    parItem.valueStr = cell->text ();
    parItem.fromString ();

    parItems [i] = parItem;
  }

}               /*-----  getTable  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : fromGui  of  ParListTable
 |
 |  Description :
 |
 |   This  function reads  the values  of  controls from  the layout  and
 |   converts the controls back to the parameter item values.
 |
 +-------------------------------------------------------------------------*/

void ParListTable::fromGui ()
{

  getTable ();

}               /*-----  fromGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : toGui  of  ParListTable
 |
 |  Description :
 |
 |   This function fills  the controls of the layout with  the content of
 |   the parameter items.
 |
 +-------------------------------------------------------------------------*/

void ParListTable::toGui ()
{

  fillTable ();

}               /*-----  toGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : init  of  ParListTable
 |
 |  Description :
 |
 |   This function is called from the constructor.
 |
 +-------------------------------------------------------------------------*/

void ParListTable::init ()
{

  parItems = parList->getParItems ();

  fillTable  ();

}               /*-----  init  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setParList  of  ParListTable
 |
 |  Description :
 |
 |   This function receives a pointer to the ParList and stores it.
 |
 |  Input parameters :
 |   parList         :  Pointer to the list of parameter items.
 |
 +-------------------------------------------------------------------------*/

void ParListTable::setParList ( ParList * parList )
{

  this->parList = parList;

  init ();

}               /*-----  setParList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  set  of  ParListTable
 |
 |  Description :
 |
 |   This function is  a generic setter which sets the  given property to
 |   the given value.
 |
 +-------------------------------------------------------------------------*/

void ParListTable::set ( QString prop, QString value )
{

  if ( prop == "withComment" )
    withComment = StringUtils::stringToBool (value);

}               /*-----  set  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setParList  of  ParListTable
 |
 |  Description :
 |
 |   This function copies  the given ParList and stores it.   Also a name
 |   with the table name is given, the  table is filled with all items in
 |   infos [items].
 |
 |  Input parameters :
 |   parList         :  Pointer to the list of parameter items.
 |   tableName       :  Item name item which contains the table definition
 |
 +-------------------------------------------------------------------------*/

void ParListTable::setParList ( ParList * fromParList, QString tableName )
{
  L_String items;
  QString  text;
  ParItem  parItem;

  parList = & parListBase;

  parList->clear ();

  parItems = fromParList->getParItems ();

  if ( not fromParList->getParItem ( tableName, parItem ) )
    return;

  text = parItem.infos ["items"];

  items = text.split ( "," );

  for ( int i = 0; i < items.count (); i++ )
  {
    text = items [i];

    if ( fromParList->getParItem ( text, parItem ) )
    {
      parList->append (parItem);
    }
  }

  init ();

}               /*-----  setParList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : resizeEvent  of  ParListTable
 |
 |  Description :
 |
 |   This function adjusts the layout of the grid, more precise the width
 |   of the columns.
 |
 +-------------------------------------------------------------------------*/

void ParListTable::resizeEvent ( QResizeEvent * )
{

  adjustGrid ();

}                   /*-----  resizeEvent  -----*/
