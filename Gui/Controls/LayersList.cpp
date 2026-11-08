/*-------------------------------------------------------------------------
 |
 |  File Name   :  LayersList.cpp
 |
 |  Description :
 |
 |   This  module  contains the  class  'LayersList'  which implements  a
 |   widget with a  list of IconButtons. Every button  represents a layer
 |   in the  graphic scene and  has a name  and a visibility  state.  The
 |   state  can be  toggled  to indicate  that the  layer  is visible  or
 |   hidden. The visibility is shown  with different icons for (power) on
 |   and off.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 04.Mar.2024
 |  Modified  : Oskar Leirich                Last changes  : 26.May.2025
 |
 |
 |  This unit contains following member functions :
 |   of LayersList :
 |    LayersList                ~LayersList               addButton
 |    clear                     contextMenuEvent          createButton
 |    createLayout              delButton                 delLastButton
 |    getActiveList             getBtnActive              onBtnClicked
 |    onIconToggled             setBtnActive              setButton
 |    updateIndex
 |
 +-------------------------------------------------------------------------*/

#include "LayersList.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  LayersList
 |
 |  Description :
 |
 |   This class contains icon buttons with state and name.
 |
 |  Input parameters :
 |   parent          : parent widget
 |
 +-------------------------------------------------------------------------*/

LayersList::LayersList ( QWidget * parent ) : QWidget (parent)
{

  setObjectName ( "LayersList" );

  widgetVBox   = new QVBoxLayout (parent);
  scrollWidget = new QWidget (this);

  curIndex     = -1;

  createLayout ();

}               /*-----  LayersList  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  LayersList
 |
 |  Description :
 |
 |   The destructor does the clean-up.
 |
 +-------------------------------------------------------------------------*/

LayersList::~LayersList ()
{

  delete contextMenu;

  clear ();

}                   /*-----  LayersList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  clear  of  LayersList
 |
 |  Description :
 |
 |   This function deletes all buttons.
 |
 +-------------------------------------------------------------------------*/

void LayersList::clear ()
{

  curIndex = buttons.count () - 1;

  for ( int idx = curIndex; idx >= 0; idx-- )
  {
    widgetVBox->takeAt (idx);
  }

  qDeleteAll (buttons);
  buttons.clear ();

  curIndex = -1;

}               /*-----  clear  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  addButton  of  LayersList
 |
 |  Description :
 |
 |   This function creates an icon button with the given name and adds it
 |   at the current index or at the end if there are no buttons.
 |
 |  Input parameters :
 |   name            : Text for the button
 |   index           : Index for inserting the buttion in the list
 |
 +-------------------------------------------------------------------------*/

void LayersList::addButton ( QString name )
{

  curIndex++;

  createButton (name);

  buttons.insert  ( curIndex, iconBtn );

  widgetVBox->insertWidget ( curIndex, iconBtn );

  updateIndex ();

}               /*-----  addButton  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createButton  of  LayersList
 |
 |  Description :
 |
 |   This function creates an icon button with the given name.
 |
 +-------------------------------------------------------------------------*/

void LayersList::createButton ( QString name )
{

  iconBtn = new IconButton (this);

  iconBtn->setIcon (btnEnabled);
  iconBtn->setText      (name);
  iconBtn->setCheckable (true);
  iconBtn->setChecked   (true);

  iconBtn->setProperty ( "Index",   QVariant (curIndex) );
  iconBtn->setProperty ( "Active",  QVariant (true) );

  connect ( iconBtn, & IconButton::iconToggled,
            this,    & LayersList::onIconToggled );

  connect ( iconBtn, & IconButton::clicked,
            this,    & LayersList::onBtnClicked );

}               /*-----  createButton  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  delButton  of  LayersList
 |
 |  Description :
 |
 |   This function deletes the an icon button with the given index from the
 |   button list.
 |
 |  Input parameters :
 |   index           :  Index in the tools list
 |
 +-------------------------------------------------------------------------*/

void LayersList::delButton ( int index )
{
  IconButton * iconBtn;

  if ( ( index < 0  ) or ( index >= buttons.count () ) )
    return;

  iconBtn = buttons.takeAt (index);

  delete iconBtn;

  curIndex = qMax ( 0, curIndex - 1 );

  updateIndex ();

}               /*-----  delButton  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  delLastButton  of  LayersList
 |
 |  Description :
 |
 |   This function deletes the the last icon button.
 |
 +-------------------------------------------------------------------------*/

void LayersList::delLastButton ()
{

  iconBtn = buttons.takeLast ();

  delete iconBtn;

  curIndex--;

  updateIndex ();

}               /*-----  delLastButton  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLayout  of  LayersList
 |
 |  Description :
 |
 |   This function creates the layout with a scroll area and a widget for
 |   all buttons.
 |
 +-------------------------------------------------------------------------*/

void LayersList::createLayout ()
{
  QHBoxLayout * hBox;

  hBox = new QHBoxLayout  ();

  widgetVBox->setContentsMargins ( 0, 0, 0, 0 );
  widgetVBox->setSpacing (0);
  widgetVBox->addStretch ();

  btnEnabled.addFile  ( ":/Icons/RedOn" );
  btnDisabled.addFile ( ":/Icons/WhiteOn" );

  scrollArea = new QScrollArea (this);

  scrollArea->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  scrollArea->setVerticalScrollBarPolicy ( Qt::ScrollBarAsNeeded );
  scrollArea->setWidgetResizable (true);

  scrollWidget->setLayout (widgetVBox);
  scrollArea->setWidget (scrollWidget);

  hBox->addWidget (scrollArea);

  setLayout (hBox);

  contextMenu = new QMenu ();
  contextMenu->clear ();

}               /*-----  createLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  contextMenuEvent  of  LayersList
 |
 |  Description :
 |
 |   This function is the context menu  handler and displays a popup menu
 |   if the mouse was right-clicked inside this widget.
 |
 |  Input parameters :
 |   event           :  context menu event
 |
 +-------------------------------------------------------------------------*/

void LayersList::contextMenuEvent ( QContextMenuEvent * event )
{
  QPoint pos;

  event->setAccepted (true);

  pos = event->globalPos ();

  // test to see if we can find out the buttons
  for ( int i = 0; i < buttons.count (); i++ )
  {
//    if ( containsPoint ( buttons.at (i), pos ) )
//      break;

  }

  contextMenu->exec (pos);

}               /*-----  contextMenuEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onBtnClicked  of  LayersList
 |
 |  Description :
 |
 |   This function is called when a  button in the widget is pressed.  It
 |   is checked if  the sender is a  QPushButton and if so  a signal sent
 |   out which informs about label and pressed state of this button.
 |   It  is  not necessary  to  block  signals when  calling  setChecked,
 |   because it does not signal 'clicked', but 'toggled'.
 |
 |  Input parameters :
 |   checked         : Checked state of the selected button
 |
 +-------------------------------------------------------------------------*/

void LayersList::onBtnClicked ( bool checked )
{
  int newIdx;

  // QObject::sender() returns the object which sent the signal
  iconBtn = qobject_cast < IconButton * > ( QObject::sender () );

  if (iconBtn)
  {
    newIdx   = iconBtn->property ( "Index" ).toInt ();
    checked  = iconBtn->isChecked ();

    // always select current button (pressed and on)
    if ( not checked )
    {
      iconBtn->setChecked (true);
    }

    if ( newIdx != curIndex )
    {
      iconBtn = buttons [curIndex];

      iconBtn->setChecked (false);  // deselect previous button

      curIndex = newIdx;

      emit buttonClicked ( curIndex, checked );
    }
  }

}               /*-----  onBtnClicked  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onIconToggled  of  LayersList
 |
 |  Description :
 |
 |   This function is called when a button in the widget is  pressed.  It
 |   is checked if the sender is a QPushButton and if so  a  signal  sent
 |   out which informs about label and pressed state of this button.
 |
 |  Input parameters :
 |   index           : Checked state of the selected button
 |
 +-------------------------------------------------------------------------*/

void LayersList::onIconToggled ( int index )
{
  bool active;

  iconBtn = buttons [index];

  if (iconBtn)
  {
    active = not iconBtn->property ( "Active" ).toBool ();
    iconBtn->setProperty ( "Active",  QVariant (active) );

    if (active)
      iconBtn->setIcon (btnEnabled);
    else
      iconBtn->setIcon (btnDisabled);
  }

  emit btnIconToggled ( index, active );

}               /*-----  onIconToggled  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getActiveList  of  LayersList
 |
 |  Description :
 |
 |   This  function returns  a list  with booleans  which indicat  if the
 |   button is  'active' or not. Active  is indicated with a  red icon, a
 |   grey icon for the not active state.
 |
 |  Output parameter :
 |   L_Bool          : The 'active' state of all buttons
 |
 +-------------------------------------------------------------------------*/

L_Bool LayersList::getActiveList ()
{
  L_Bool states;
  bool   active;

  for ( int index = 0; index < buttons.count (); index++ )
  {
    iconBtn = buttons [index];
    active = iconBtn->property ( "Active" ).toBool ();

    states.append (active);
  }

  return states;

}               /*-----  getActiveList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getBtnActive  of  LayersList
 |
 |  Description :
 |
 |   This function returns  if the button with the given  index is active
 |   or not.
 |
 |  Input parameters :
 |   index           : Index of the button
 |
 |  Output parameter :
 |   active          : The 'active' state of the button
 |
 +-------------------------------------------------------------------------*/

bool LayersList::getBtnActive ( int index )
{
  bool active;

  if ( ( index < 0  ) or ( index >= buttons.count () ) )
    return false;

  iconBtn = buttons [index];

  active = iconBtn->property ( "Active" ).toBool ();

  return active;

}               /*-----  getBtnActive  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setBtnActive  of  LayersList
 |
 |  Description :
 |
 |   This function sets the button with the given  index to 'active'.
 |
 |  Input parameters :
 |   index           : Index of the button
 |   active          : New 'active' state of the button
 |
 +-------------------------------------------------------------------------*/

void LayersList::setBtnActive ( int index, bool active )
{

  if ( ( index < 0  ) or ( index >= buttons.count () ) )
    return;

  iconBtn = buttons [index];

  iconBtn->setProperty ( "Active",  QVariant (active) );

}               /*-----  setBtnActive  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setButton  of  LayersList
 |
 |  Description :
 |
 |   This function sets the button with the given  index to 'active'.
 |
 |  Input parameters :
 |   index           : Index of the button
 |   active          : New 'active' state of the button
 |
 +-------------------------------------------------------------------------*/

void LayersList::setButton ( int index, QString name )
{

  if ( ( index < 0  ) or ( index >= buttons.count () ) )
    return;

  iconBtn = buttons [index];

  iconBtn->setText (name);

}               /*-----  setButton  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  updateIndex  of  LayersList
 |
 |  Description :
 |
 |   This function updates the indexes in the list of buttons.
 |
 +-------------------------------------------------------------------------*/

void LayersList::updateIndex ()
{
  int index;

  index = curIndex;

  for ( int idx = 0; idx < buttons.count (); idx++ )
  {
    iconBtn = buttons [idx];

    iconBtn->setProperty ( "Index", QVariant (idx) );

    if ( idx == index )
      iconBtn->setChecked (true);
    else
      iconBtn->setChecked (false);
  }

}               /*-----  updateIndex  -----*/
