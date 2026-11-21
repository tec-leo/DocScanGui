/*-------------------------------------------------------------------------
 |
 |  File Name   :  ListSelectDlg.cpp
 |
 |  Description :
 |
 |   This module implements the dialog class 'ListSelectDlg' which allows
 |   the selection of  an entry in a  list box.  The 'entry'  can also be
 |   deleted or a new one defined.
 |
 |  Developer : Oskar Leirich                Creation date : 18.May.2026
 |  Modified  : Oskar Leirich                Last changes  : 14.Nov.2026
 |
 |
 |  This unit contains following classes :
 |   ListSelectDlg
 |
 |  This unit contains following member functions :
 |   of ListSelectDlg:
 |    ListSelectDlg             ~ListSelectDlg            createLayout
 |    fillOptions               getSelected               initDialog
 |    onOk                      onSelChanged
 |
 +-------------------------------------------------------------------------*/

#include "ListSelectDlg.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  ListSelectDlg
 |
 |  Description :
 |
 |   The constructor  initializes some  variables and sets  window title,
 |   icon and size.
 |
 |  Input parameters :
 |   parent          :  Pointer to the parent widget
 |   optList         :  String list of options
 |   title           :  Title of the dialog
 |   addOptions      :  Allows to add new options (true)
 |
 +-------------------------------------------------------------------------*/

ListSelectDlg::ListSelectDlg ( QWidget * parent, L_String optList,
                               QString title, bool addOptions )
                                 : QDialog (parent)
{

  this->optList    = optList;
  this->addOptions = addOptions;

  setWindowIcon  ( QIcon ( ":/Icons/ListSelect" ) );
  setWindowTitle (title);

  initDialog ();

}               /*----- ListSelectDlg -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  ListSelectDlg
 |
 |  Description :
 |
 |   The destructor deletes all dynamically created objects.
 |
 +-------------------------------------------------------------------------*/

ListSelectDlg::~ListSelectDlg ()
{

  delete  listSelect;
  delete  lineEdit;
  delete  btnCancel;
  delete  btnOk;

}               /*-----  ~ListSelectDlg  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLayout  of  ListSelectDlg
 |
 |  Description :
 |
 |   This function creates all controls and layout of the dialog.
 |
 +-------------------------------------------------------------------------*/

void ListSelectDlg::createLayout ()
{
  QVBoxLayout  * vBox;
  QHBoxLayout  * hBox;

  vBox = new QVBoxLayout ();
  hBox = new QHBoxLayout ();

  btnCancel = new QPushButton ( "Cancel" );
  btnCancel->setIcon ( QIcon ( ":/Icons/Cancel" ) );

  btnOk = new QPushButton ( "Ok" );
  btnOk->setIcon ( QIcon ( ":/Icons/OkClean" ) );

  listSelect = new QListWidget ();
  lineEdit   = new QLineEdit ();

  if (addOptions)
    lineEdit->setReadOnly (false);
  else
    lineEdit->setReadOnly (true);

  hBox->addStretch ();
  hBox->addWidget (btnOk);
  hBox->addStretch ();
  hBox->addWidget (btnCancel);
  hBox->addStretch ();

  vBox->addWidget (lineEdit);
  vBox->addWidget (listSelect);
  vBox->addLayout (hBox);

  this->setLayout (vBox);

}               /*-----  createLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : initDialog  of  ListSelectDlg
 |
 |  Description :
 |
 |   This  function is called  from the  constructor and  initializes the
 |   dialog with the gui (layout), actions and signal / slot connections.
 |
 +-------------------------------------------------------------------------*/

void ListSelectDlg::initDialog ()
{

  createLayout  ();

  connect ( btnCancel,  & QPushButton::pressed,
            this,       & ListSelectDlg::close );

  connect ( btnOk,      & QPushButton::pressed,
            this,       & ListSelectDlg::onOk );

  connect ( listSelect, & QListWidget::currentRowChanged,
            this,       & ListSelectDlg::onSelChanged );

  fillOptions ();

}               /*-----  initDialog  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : fillOptions  of  ListSelectDlg
 |
 |  Description :
 |
 |   This function fills the list box with all available options.
 |
 +-------------------------------------------------------------------------*/

void ListSelectDlg::fillOptions ()
{

  listSelect->clear ();

  listSelect->addItems (optList);

  if ( optList.count () > 0 )
    lineEdit->setText ( optList [0] );

}                   /*-----  fillOptions  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : getSelected  of  ListSelectDlg
 |
 |  Description :
 |
 |   This function returns the selected option.
 |
 +-------------------------------------------------------------------------*/

QString ListSelectDlg::getSelected ()
{

  return lineEdit->text ();

}                   /*-----  getSelected  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onSelChanged  of  ListSelectDlg
 |
 |  Description :
 |
 |   This function  is called when the  selection in the list  widget has
 |   changed.   In this  case the  selected text  is copied  to the  edit
 |   field.
 |
 +-------------------------------------------------------------------------*/

void ListSelectDlg::onSelChanged ( int row )
{
  QString text;

  text = listSelect->item (row)->text ();

  lineEdit->setText (text);

}               /*-----  onSelChanged  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onOk  of  ListSelectDlg
 |
 |  Description :
 |
 |   This function is called when the  'Ok' button is pressed. It updates
 |   the text edit field and closes the dialog.
 |
 +-------------------------------------------------------------------------*/

void ListSelectDlg::onOk ()
{

  getSelected ();

  accept ();

  close ();

}               /*-----  onOk  -----*/

