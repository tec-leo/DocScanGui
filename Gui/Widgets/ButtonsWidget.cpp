/*-------------------------------------------------------------------------
 |
 |  File Name   :  ButtonsWidget.cpp
 |
 |  Description :
 |
 |   This module  contains the  class 'ButtonsWidget' which  implements a
 |   widget with  a list of  IconButtons. Every button  has a name  and a
 |   state which can be toggled between  on and off. The state is visible
 |   with different icons for (power) on and off.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 16.Jul.2023
 |  Modified  : Oskar Leirich                Last changes  : 29.Apr.2026
 |
 |
 |  This unit contains following member functions :
 |   of ButtonsWidget :
 |    ButtonsWidget             ~ButtonsWidget            addButton
 |    clear                     contextMenuEvent          createButton
 |    createLayout              delButton                 getActiveList
 |    getBtnActive              getBtnChecked             onBtnClicked
 |    onIconToggled             setBtnActive              setBtnChecked
 |    swapButtons               updateIndex
 |
 +-------------------------------------------------------------------------*/

#include "ButtonsWidget.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  ButtonsWidget
 |
 |  Description :
 |
 |   This class contains icon buttons with state and name.
 |
 |  Input parameters :
 |   parent          : parent widget
 |
 +-------------------------------------------------------------------------*/

ButtonsWidget::ButtonsWidget ( QWidget * parent ) : QWidget (parent)
{

  setObjectName ( "ButtonsWidget" );

  widgetVBox   = new QVBoxLayout (parent);
  scrollWidget = new QWidget (this);

  curIndex     = -1;
  iconSize     = 18;
  btnFont      = font ();

  createLayout ();

}               /*-----  ButtonsWidget  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  ButtonsWidget
 |
 |  Description :
 |
 |   The destructor does the clean-up.
 |
 +-------------------------------------------------------------------------*/

ButtonsWidget::~ButtonsWidget ()
{

  delete contextMenu;

  qDeleteAll (buttons);

  buttons.clear ();

}                   /*-----  ButtonsWidget  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  clear  of  ButtonsWidget
 |
 |  Description :
 |
 |   This function deletes all buttons.
 |
 +-------------------------------------------------------------------------*/

void ButtonsWidget::clear ()
{

  curIndex = -1;

  qDeleteAll (buttons);

  buttons.clear ();

}               /*-----  clear  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  addButton  of  ButtonsWidget
 |
 |  Description :
 |
 |   This function creates an icon button with the given name and adds it
 |   at the current index or at the end if there are no buttons.
 |
 |  Input parameters :
 |   name            : Text for the button
 |   index           : Index for inserting the button in the list
 |
 +-------------------------------------------------------------------------*/

void ButtonsWidget::addButton ( QString name, QString tooltip )
{

  curIndex++;

  createButton ( name, tooltip );

  //  buttons.append ( iconBtn);
  buttons.insert ( curIndex, iconBtn );

  widgetVBox->insertWidget ( curIndex, iconBtn );

  updateIndex ();

}               /*-----  addButton  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createButton  of  ButtonsWidget
 |
 |  Description :
 |
 |   This function creates an icon button with the given name.
 |
 +-------------------------------------------------------------------------*/

void ButtonsWidget::createButton ( QString name, QString tooltip )
{

  iconBtn = new IconButton (this);

  iconBtn->setIcon      (btnEnabled);
  iconBtn->setIconSize  ( QSize ( iconSize, iconSize ) );
  iconBtn->setText      (name);
  iconBtn->setToolTip   (tooltip);
  iconBtn->setCheckable (true);
  iconBtn->setChecked   (false);

  iconBtn->setProperty ( "Index",  QVariant (curIndex) );
  iconBtn->setProperty ( "Active", QVariant (true) );

  connect ( iconBtn, & IconButton::iconToggled,
            this,    & ButtonsWidget::onIconToggled );

  connect ( iconBtn, & IconButton::clicked,
            this,    & ButtonsWidget::onBtnClicked );

}               /*-----  createButton  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  delButton  of  ButtonsWidget
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

void ButtonsWidget::delButton ( int index )
{
  IconButton * iconBtn;

  if ( ( index < 0  ) or ( index >= buttons.count () ) )
    return;

  iconBtn = buttons.takeAt (index);

  widgetVBox->removeWidget (iconBtn);

  delete iconBtn;

  curIndex = max ( 0, curIndex - 1 );

  updateIndex ();

}               /*-----  delButton  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  swapButtons  of  ButtonsWidget
 |
 |  Description :
 |
 |   This function  swaps the tool  buttons with the given  indexes.  The
 |   index checks are already done by the caller.
 |
 |  Input parameters :
 |   index           :  Index in the tools list
 |
 +-------------------------------------------------------------------------*/

void ButtonsWidget::swapButtons ( int index1, int index2 )
{
  IconButton * iconBtn;

  iconBtn = buttons [index1];

  widgetVBox->removeWidget (iconBtn);
  widgetVBox->insertWidget ( index2, iconBtn );

  buttons.swapItemsAt ( index1, index2 );

  updateIndex ();

}               /*-----  swapButtons  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLayout  of  ButtonsWidget
 |
 |  Description :
 |
 |   This function creates the layout with a scroll area and a widget for
 |   all buttons.
 |
 +-------------------------------------------------------------------------*/

void ButtonsWidget::createLayout ()
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
  scrollArea->setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded );
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
 |  Member function :  contextMenuEvent  of  ButtonsWidget
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

void ButtonsWidget::contextMenuEvent ( QContextMenuEvent * event )
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
 |  Member function :  onBtnClicked  of  ButtonsWidget
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

void ButtonsWidget::onBtnClicked ( bool checked )
{
  int newIdx;

  // QObject::sender() returns the object which sent the signal
  iconBtn = qobject_cast < IconButton * > ( QObject::sender () );

  if (iconBtn)
  {
    newIdx  = iconBtn->property ( "Index" ).toInt ();
    checked = iconBtn->isChecked ();

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
    }

    emit buttonClicked ( curIndex, checked );
  }

}               /*-----  onBtnClicked  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onIconToggled  of  ButtonsWidget
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

void ButtonsWidget::onIconToggled ( int index )
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

  emit buttonToggled ( curIndex, active );

}               /*-----  onIconToggled  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getActiveList  of  ButtonsWidget
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

L_Bool ButtonsWidget::getActiveList ()
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
 |  Member function :  getBtnActive  of  ButtonsWidget
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

bool ButtonsWidget::getBtnActive ( int index )
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
 |  Member function :  setBtnActive  of  ButtonsWidget
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

void ButtonsWidget::setBtnActive ( int index, bool active )
{

  if ( ( index < 0  ) or ( index >= buttons.count () ) )
    return;

  iconBtn = buttons [index];

  iconBtn->setProperty ( "Active",  QVariant (active) );

  if (active)
    iconBtn->setIcon (btnEnabled);
  else
    iconBtn->setIcon (btnDisabled);

}               /*-----  setBtnActive  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getBtnChecked  of  ButtonsWidget
 |
 |  Description :
 |
 |   This function returns the button state 'checked' or 'not checked'.
 |
 |  Input parameters :
 |   index           : Index of the button
 |
 |  Output parameter :
 |   bool            : 'checked' state of the button
 |
 +-------------------------------------------------------------------------*/

bool ButtonsWidget::getBtnChecked ( int index )
{

  if ( ( index < 0  ) or ( index >= buttons.count () ) )
    return false;

  iconBtn = buttons [index];

  return iconBtn->isChecked ();

}               /*-----  getBtnChecked  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setBtnChecked  of  ButtonsWidget
 |
 |  Description :
 |
 |   This  function sets  the button  state to  with the  given index  to
 |   'checked'.
 |
 |  Input parameters :
 |   index           : Index of the button
 |   checked         : New 'checked' state of the button
 |
 +-------------------------------------------------------------------------*/

void ButtonsWidget::setBtnChecked ( int index, bool checked )
{

  if ( ( index < 0  ) or ( index >= buttons.count () ) )
    return;

  curIndex = index;

  iconBtn = buttons [index];

  iconBtn->setChecked (checked);

}               /*-----  setBtnChecked  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  updateIndex  of  ButtonsWidget
 |
 |  Description :
 |
 |   This function updates the indexes in the list of buttons.
 |
 +-------------------------------------------------------------------------*/

void ButtonsWidget::updateIndex ()
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
