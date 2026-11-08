/*-------------------------------------------------------------------------
 |
 |  File Name   :  ParListGrid.cpp
 |
 |  Description :
 |
 |   This module implements a the  class 'ParListGrid' which implements a
 |   generic widget with a grid layout. It shows all items of a 'ParList'
 |   as controls  in a  grid layout.   This avoids  to define  the layout
 |   individually for all parameter items.
 |
 |  Developer : Oskar Leirich                Creation date : 18.Jul.2023
 |  Modified  : Oskar Leirich                Last changes  : 19.Aug.2024
 |
 |
 |  This unit contains following classes :
 |   ParListGrid
 |
 |  This unit contains following member functions :
 |   of ParListGrid:
 |    ParListGrid               ~ParListGrid              createCheck
 |    createCombo               createEdit                createLayout
 |    createRadio               fromGui                   getWidget
 |    init                      setParList                toGui
 |
 +-------------------------------------------------------------------------*/

#include "ParListGrid.h"
#include "ParListTable.h"

/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  ParListGrid
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

ParListGrid::ParListGrid ( QWidget * parent ) : QWidget (parent)
{

  parList = nullptr;

}               /*----- ParListGrid -----*/


ParListGrid::ParListGrid ( QWidget * parent, ParList * parList )
                           : QWidget (parent)
{

  this->parList = parList;

  init ();

}               /*----- ParListGrid -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  ParListGrid
 |
 |  Description :
 |
 |   The destructor deletes all  dynamically created objects if there are
 |   any.
 |
 +-------------------------------------------------------------------------*/

ParListGrid::~ParListGrid ()
{

  qDeleteAll (widgets);

}               /*-----  ~ParListGrid  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getWidget  of  ParListGrid
 |
 |  Description :
 |
 |   This function returns a pointer to a widget if there is a widget box
 |   which was created from a parItem with the given name.
 |
 +-------------------------------------------------------------------------*/

QWidget * ParListGrid::getWidget ( QString parName )
{
  QWidget * widget;

  widget = nullptr;

  if ( widgetsMap.contains (parName) )
    widget = (QWidget *) widgetsMap [parName];

  return widget;

}               /*-----  getWidget  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createCheck  of  ParListGrid
 |
 |  Description :
 |
 |   This function creates a check box and adds it to the current layout.
 |
 +-------------------------------------------------------------------------*/

void ParListGrid::createCheck ( ParItem & parItem, int & row )
{
  QCheckBox * check;
  bool  twoColumns;

  twoColumns = false;

  text = parItem.infos ["twoCol"];
  twoColumns = StringUtils::stringToBool (text);

  text = parItem.infos ["label"];

  if (grid)
  {
    if (twoColumns)
    {
      label = new QLabel (text);
      check = new QCheckBox ();

      grid->addWidget ( label, row, 0 );
      grid->addWidget ( check, row, 1 );
    }
    else
    {
      check = new QCheckBox (text);
      grid->addWidget ( check, row, 0, 1, 2 );
    }

    row++;
  }
  else
  {
    check = new QCheckBox (text);

    if ( type == "vMain" )
      vMain->addWidget (check);
    else
      layout->addWidget (check);
  }

  if ( parItem.infos.contains ("tooltip") )
    check->setToolTip ( parItem.infos ["tooltip"] );

  ptrVal = (qulonglong) check;
  text.setNum (ptrVal);
  parItem.infos ["widget"] = text;

}               /*-----  createCheck  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createCombo  of  ParListGrid
 |
 |  Description :
 |
 |   This function creates a combo box edit and adds it to the current layout.
 |
 +-------------------------------------------------------------------------*/

void ParListGrid::createCombo ( ParItem & parItem, int & row )
{
  QComboBox * combo;
  L_String    items;

  text  = parItem.infos ["items"];
  items = text.split ( "," );

  if ( parItem.infos.contains ("label") )
  {
    text  = parItem.infos ["label"];
    label = new QLabel (text);
  }
  else
    label = nullptr;

  combo = new QComboBox ();

  if (grid)
  {
    if (label)
    {
      grid->addWidget ( label, row, 0 );
      grid->addWidget ( combo, row, 1 );
    }
    else
      grid->addWidget ( combo, row, 0, 1, 2 );

    row++;
  }
  else
  {
    if ( type == "vMain" )
      vMain->addWidget (combo);
    else
      layout->addWidget (combo);
  }

  combo->addItems (items);

  if ( parItem.infos.contains ("tooltip") )
    combo->setToolTip ( parItem.infos ["tooltip"] );

  ptrVal = (qulonglong) combo;
  text.setNum (ptrVal);
  parItem.infos ["widget"] = text;

}               /*-----  createCombo  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createEdit  of  ParListGrid
 |
 |  Description :
 |
 |   This function creates a line edit and adds it to the current layout.
 |
 +-------------------------------------------------------------------------*/

void ParListGrid::createEdit ( ParItem & parItem, int & row )
{
  QLineEdit * edit;
  int         width;

  text  = parItem.infos ["label"];

  label = new QLabel (text);
  edit  = new QLineEdit ();

  if ( parItem.infos.contains ("tooltip") )
    label->setToolTip ( parItem.infos ["tooltip"] );

  if ( parItem.infos.contains ( "maxWidth" ) )
  {
    text  = parItem.infos ["maxWidth"];
    width = text.toInt ();
    edit->setMaximumWidth (width);
  }

  if (grid)
  {
    grid->addWidget ( label, row, 0 );
    grid->addWidget ( edit,  row, 1, Qt::AlignRight );
    row++;
  }
  else
  {
    hBox = new QHBoxLayout ();
    hBox->addWidget (label);
    hBox->addWidget (edit);

    if ( type == "vMain" )
      vMain->addLayout (hBox);
  }

  ptrVal = (qulonglong) edit;
  text.setNum (ptrVal);
  parItem.infos ["widget"] = text;

}               /*-----  createEdit  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createRadio  of  ParListGrid
 |
 |  Description :
 |
 |   This function  creates a  radio button  and adds  it to  the current
 |   layout.
 |
 +-------------------------------------------------------------------------*/

void ParListGrid::createRadio ( ParItem & parItem, int & row )
{
  QRadioButton * radio;
  bool  twoColumns;

  twoColumns = false;

  text = parItem.infos ["twoCol"];
  twoColumns = StringUtils::stringToBool (text);

  text = parItem.infos ["label"];

  if (grid)
  {
    if (twoColumns)
    {
      label = new QLabel (text);
      radio = new QRadioButton ();

      grid->addWidget ( label, row, 0 );
      grid->addWidget ( radio, row, 1 );
    }
    else
    {
      radio = new QRadioButton (text);
      grid->addWidget ( radio, row, 0 );
    }

    row++;
  }
  else
  {
    radio = new QRadioButton (text);

    if ( type == "vMain" )
      vMain->addWidget (radio);
    else
      layout->addWidget (radio);
  }

  if ( parItem.infos.contains ("tooltip") )
    radio->setToolTip ( parItem.infos ["tooltip"] );

  ptrVal = (qulonglong) radio;
  text.setNum (ptrVal);
  parItem.infos ["widget"] = text;

}               /*-----  createRadio  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createTable  of  ParListGrid
 |
 |  Description :
 |
 |   This function creates  a table widget (ParListTable) and  adds it to
 |   the current layout.
 |
 +-------------------------------------------------------------------------*/

void ParListGrid::createTable ( ParItem & parItem, int & row )
{
  ParListTable * table;

  text = parItem.infos ["label"];

  table = new ParListTable (this);

  if ( type == "grid" )
  {
    grid->addWidget ( table, row, 0 );
    row++;
  }

  if ( type == "hBox" )
    hBox->addWidget (table);

  if ( type == "vMain" )
    vMain->addWidget (table);

  ptrVal = (qulonglong) table;
  text.setNum (ptrVal);
  parItem.infos ["widget"] = text;

}               /*-----  createTable  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLayout  of  ParListGrid
 |
 |  Description :
 |
 |   This function creates the widgets for the grid layout.
 |
 +-------------------------------------------------------------------------*/

void ParListGrid::createLayout ()
{
  QString control;
  int     row;
  int     stretch;

  grid   = nullptr;
  group  = nullptr;
  layout = nullptr;

  hBox   = nullptr;
  vBox   = nullptr;
  vMain  = new QVBoxLayout ();

  type   = "vMain";
  row    = 0;

  for ( int idx = 0; idx < parItems.count (); idx++ )
  {
    // use a reference to avoid storing back

    ParItem & parItem = parItems [idx];
    parItem.infos.remove ( "widget" );

    control  = parItem.infos ["control"];

    if ( control == "table-item" )
      continue;

    if ( control == "begGroup" )
    {
      text  = parItem.infos ["label"];
      group = new QGroupBox (text);

      continue;
    }

    if ( control == "endGroup" )
    {
      stretch = parItem.infos ["stretch"].toInt ();

      group->setLayout (layout);
      vMain->addWidget ( group, stretch );

      hBox   = nullptr;
      grid   = nullptr;
      group  = nullptr;
      layout = nullptr;
      row    = 0;

      continue;
    }

    if ( control == "begLayout" )
    {
      type = parItem.name;

      if ( type == "grid" )
      {
        grid = new QGridLayout ();
        grid->setColumnStretch ( 0, 2 );

        layout = grid;
      }

      if ( type == "hBox" )
      {
        hBox = new QHBoxLayout ();

        layout = hBox;
      }

      continue;
    }

    if ( control == "endLayout" )
    {
      vMain->addLayout (layout);

      hBox   = nullptr;
      grid   = nullptr;
      group  = nullptr;
      layout = nullptr;
      row    = 0;

      continue;
    }

    if ( control == "stretch" )
    {
      label = new QLabel ( "" );

      if (grid)
        grid->addWidget ( label, row++, 0 );
      else
        vMain->addStretch ();

      continue;
    }

    if ( control == "label" )
    {
      text = parItem.infos ["label"];

      label = new QLabel (text);

      if (grid)
      {
        grid->addWidget ( label, row, 0, 1, 2 );
        row++;
      }
      else
        vMain->addWidget (label);

      continue;
    }

    if ( control == "edit" )
    {
      createEdit ( parItem, row );
      continue;
    }

    if ( control == "check" )
    {
      createCheck ( parItem, row );
      continue;
    }

    if ( control == "combo" )
    {
      createCombo ( parItem, row );
      continue;
    }

    if ( control == "radio" )
    {
      createRadio ( parItem, row );
      continue;
    }

    if ( control == "table" )
    {
      createTable ( parItem, row );
      continue;
    }
  }

  vMain->addStretch ();

  setLayout (vMain);

}               /*-----  createLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : fromGui  of  ParListGrid
 |
 |  Description :
 |
 |   This  function reads  the values  of  controls from  the layout  and
 |   converts the controls back to the parameter item values.
 |
 +-------------------------------------------------------------------------*/

void ParListGrid::fromGui ()
{
  ParListTable * table;
  QCheckBox    * check;
  QComboBox    * combo;
  QLineEdit    * edit;
  QWidget      * widget;
  QString        control;
  ParItem        parItem;

  for ( int i = 0; i < parItems.count (); i++ )
  {
    parItem = parItems [i];

    if ( not parItem.infos.contains ("widget") )
      continue;

    text = parItem.infos ["widget"];
    widget = (QWidget *) text.toULongLong ();

    control  = parItem.infos ["control"];

    if ( control == "edit" )
    {
      edit = dynamic_cast <QLineEdit *> (widget);

      if ( not edit )
        continue;

      parItem.valueStr = edit->text ();
      parItem.fromString ();        // convert back

      continue;
    }

    if ( control == "check" )
    {
      check = dynamic_cast <QCheckBox *> (widget);

      if ( not check )
        continue;

      if (check)
      {
        if ( check->checkState () == Qt::Checked )
          parItem.valueStr = "true";
        else
          parItem.valueStr = "false";

        parItem.fromString ();        // convert back
      }

      continue;
    }

    if ( control == "combo" )
    {
      combo = dynamic_cast <QComboBox *> (widget);

      if ( not combo )
        continue;

      parItem.valueStr = combo->currentText ();
      parItem.fromString ();        // convert back

      continue;
    }

    if ( control == "table" )
    {
      table = dynamic_cast <ParListTable *> (widget);

      if ( not table )
        continue;

      table->fromGui ();

      continue;
    }

  }

}               /*-----  fromGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : toGui  of  ParListGrid
 |
 |  Description :
 |
 |   This function fills  the controls of the layout with  the content of
 |   the parameter items.
 |
 +-------------------------------------------------------------------------*/

void ParListGrid::toGui ()
{
  ParListTable * table;
  QCheckBox    * check;
  QComboBox    * combo;
  QLineEdit    * edit;
  QWidget      * widget;
  QString        control;
  ParItem        parItem;

  widgets.clear ();

  for ( int i = 0; i < parItems.count (); i++ )
  {
    parItem = parItems [i];

    if ( not parItem.infos.contains ("widget") )
      continue;

    text = parItem.infos ["widget"];
    widget = (QWidget *) text.toULongLong ();

    parItem.toString ();

    control  = parItem.infos ["control"];

    if ( control == "edit" )
    {
      edit = dynamic_cast <QLineEdit *> (widget);

      if ( not edit )
        continue;

      edit->setText (parItem.valueStr);

      widgets.append (edit);
      widgetsMap [parItem.name] = widget;

      continue;
    }

    if ( control == "check" )
    {
      check = dynamic_cast <QCheckBox *> (widget);

      if ( not check )
        continue;

      if ( parItem.valueStr == "true" )
        check->setCheckState (Qt::Checked);
      else
        check->setCheckState (Qt::Unchecked);

      widgets.append (check);
      widgetsMap [parItem.name] = widget;

      continue;
    }

    if ( control == "combo" )
    {
      combo = dynamic_cast <QComboBox *> (widget);

      if ( not combo )
        continue;

      text  = parItem.valueStr;

      combo->setCurrentText (text);

      widgets.append (combo);
      widgetsMap [parItem.name] = widget;

      continue;
    }

    if ( control == "table" )
    {
      table = dynamic_cast <ParListTable *> (widget);

      if ( not table )
        continue;

      text = parItem.infos ["label"];

      table->setParList ( parList, text );

      widgets.append (table);
      widgetsMap [parItem.name] = widget;

      continue;
    }
  }

}               /*-----  toGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : init  of  ParListGrid
 |
 |  Description :
 |
 |   This function is called from the constructor.
 |
 +-------------------------------------------------------------------------*/

void ParListGrid::init ()
{

  widgets.clear ();
  widgetsMap.clear ();

  parItems = parList->getParItems ();

  createLayout  ();

}               /*-----  init  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setParList  of  ParListGrid
 |
 |  Description :
 |
 |   This function receives a pointer to the ParList and stores it.
 |
 |  Input parameters :
 |   parList         :  Pointer to the list of parameter items.
 |
 +-------------------------------------------------------------------------*/

void ParListGrid::setParList ( ParList * parList )
{

  this->parList = parList;

  init ();

}               /*-----  setParList  -----*/
