/*-------------------------------------------------------------------------
 |
 |  File Name   :  HelpAboutDlg.cpp
 |
 |  Description :
 |
 |   This module implements  a dialog class which shows  the About dialog
 |   of this program..
 |
 |
 |  Developer : Oskar Leirich                Creation date : 24.Jul.2023
 |  Modified  : Oskar Leirich                Last changes  : 12.Dec.2024
 |
 |
 |  This unit contains following member functions :
 |   of HelpAboutDlg:
 |    HelpAboutDlg              ~HelpAboutDlg             initDialog
 |
 +-------------------------------------------------------------------------*/


#include "HelpAboutDlg.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  HelpAboutDlg
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 |  Input-Parameter  :
 |   parent          :  Pointer to the parent window
 |   maxBest         :  Maximum number of results to show
 |
 +-------------------------------------------------------------------------*/

HelpAboutDlg::HelpAboutDlg ( QWidget * parent ) : QDialog (parent )
{

  setWindowTitle ( "Help About" );

  setWindowIcon ( QIcon (":/Icons/MainApp.svg") );

  initDialog ();

  resize ( 350, 300 );

}               /*----- HelpAboutDlg -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  HelpAboutDlg
 |
 |  Description :
 |
 |   The destructor deletes all  dynamically created objects if there are
 |   any.
 |
 +-------------------------------------------------------------------------*/

HelpAboutDlg::~HelpAboutDlg ()
{

  delete  btnClose;

}               /*-----  ~HelpAboutDlg  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : initDialog  of  HelpAboutDlg
 |
 |  Description :
 |
 |   This  function is called  from the  constructor and  initializes the
 |   dialog with the gui (layout), actions and signal / slot connections.
 |
 +-------------------------------------------------------------------------*/

void HelpAboutDlg::initDialog ()
{
  QGridLayout * grid;
  QVBoxLayout * vBox1;
  QVBoxLayout * vBox2;
  QVBoxLayout * vBox3;
  QHBoxLayout * hBox1;
  QHBoxLayout * hBox2;
  QHBoxLayout * hBox3;
  QTextEdit   * copy;
  QLabel      * logo;
  QLabel      * title;
  QPixmap       pixMap;
  QString       edit;
  QFont         font;

  grid  = new QGridLayout ();
  vBox1 = new QVBoxLayout ();
  vBox2 = new QVBoxLayout ();
  vBox3 = new QVBoxLayout ();
  hBox1 = new QHBoxLayout ();
  hBox2 = new QHBoxLayout ();
  hBox3 = new QHBoxLayout ();

  logo     = new QLabel ();
  title    = new QLabel ();

  btnClose = new QPushButton ( "Ok" );

  edit.append ( "This program is licensed under the\n" );
  edit.append ( "GNU GENERAL PUBLIC LICENSE, Version 3.\n" );
  edit.append ( "\nThe license text can be found in 'LICENSE.txt'.\n" );
  edit.append ( "\nCopyright(C) Leirich Oskar 2024" );

  copy = new QTextEdit (this);
  copy->setPlainText (edit);
  copy->setReadOnly (true);

  pixMap.load ( ":/Icons/MainApp.svg" );
  logo->setPixmap  (pixMap);

  title->setText   ( "Circuit Draw" );
  font.setPointSize (16);
  font.setWeight (QFont::Bold);
  title->setFont (font);

  hBox1->addWidget ( title, 0, Qt::AlignCenter );

  grid->addWidget ( new QLabel ( "Version :" ), 0, 0 );
  grid->addWidget ( new QLabel ( "Build :" ),   1, 0 );
  grid->addWidget ( new QLabel ( versionStr ),  0, 1 );
  grid->addWidget ( new QLabel ( buildStr ),    1, 1 );

  vBox1->addStretch ();
  vBox1->addLayout (grid);
  vBox1->addStretch ();

  hBox2->addWidget ( logo, 0, Qt::AlignCenter );
  hBox2->addLayout (vBox1);

  vBox2->addWidget (copy);

  hBox3->addStretch ();
  hBox3->addWidget (btnClose);
  hBox3->addStretch ();

  vBox3->addLayout ( hBox1, 0 );
  vBox3->addLayout ( hBox2, 2 );
  vBox3->addLayout ( vBox2, 0 );
  vBox3->addLayout ( hBox3, 0 );

  setLayout (vBox3);

  connect( btnClose, & QPushButton::clicked,
           this,     & HelpAboutDlg::close );

}               /*-----  initDialog  -----*/

