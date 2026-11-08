/*-------------------------------------------------------------------------
 |
 |  File Name   :  ImageFileDlg.cpp
 |
 |  Description :
 |
 |   This  module contains  the class  'ImageFileDlg' which  implements a
 |   configuration dialog for  loading and saving the  image file formats
 |   and options.
 |
 |  Developer : Oskar Leirich                Creation date : 13.Mar.2026
 |  Modified  : Oskar Leirich                Last changes  : 08.Sep.2026
 |
 |
 |  This unit contains following member functions :
 |   of ImageFileDlg :
 |    ImageFileDlg              ~ImageFileDlg             connectSlots
 |    createControls            createLayout              fromGui
 |    initDialog                onCancel                  onOk
 |    toGui
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "ImageFileDlg.h"
#include "../Controls/FileSelect.h"
#include "../../Config/ParManager.h"
#include "../../Main/MainWnd.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  ImageFileDlg
 |
 |  Description :
 |
 |   The constructor  initializes some  variables and sets  window title,
 |   icon and size.
 |
 |  Input parameters :
 |   parent          :  Pointer to the parent window
 |
 +-------------------------------------------------------------------------*/

ImageFileDlg::ImageFileDlg ( QWidget * parent ) : QDialog (parent)
{

  initDialog ();

  setWindowIcon  ( QIcon ( ":/Icons/RunTools" ) );
  setWindowTitle ( "Image and OCR" );

  //  resize ( 400, 600 );

}               /*----- ImageFileDlg -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  ImageFileDlg
 |
 |  Description :
 |
 |   The destructor deletes all dialog control objects.
 |
 +-------------------------------------------------------------------------*/

ImageFileDlg::~ImageFileDlg ()
{

  delete  btnCancel;
  delete  btnOk;

  delete  txtOcrPar;

  delete  grid_1;
  delete  grid_2;

}               /*-----  ~ImageFileDlg  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  connectSlots  of  ImageFileDlg
 |
 |  Description :
 |
 |   This function  creates the context  menu and connects the  slots for
 |   buttons and actions.
 |
 +-------------------------------------------------------------------------*/

void ImageFileDlg::connectSlots ()
{

  connect ( btnCancel, & QPushButton::pressed,
            this,      & ImageFileDlg::onCancel );

  connect ( btnOk,     & QPushButton::pressed,
            this,      & ImageFileDlg::onOk );

}               /*-----  connectSlots  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createControls  of ImageFileDlg
 |
 |  Description :
 |
 |   This  function creates  a  grid  layout for  the  image options  for
 |   loading and saving.
 |
 +-------------------------------------------------------------------------*/

void ImageFileDlg::createControls ()
{
  ParItem * itemPtr;
  ParItem   item;
  QString   text;

  cbOcrProg = new FileSelect (this);
  cbOcrProg->setLabel        ( "Ocr Program : " );
  cbOcrProg->setIconName     ( ":/Icons/Open" );
  cbOcrProg->setEditable     (true);
  cbOcrProg->setMustExist    (false);
  cbOcrProg->create          ();

  txtOcrPar = new QLineEdit (this);

  // set new dialog parList for the load image and ocr
  parList->setDlgParList ( & list_1 );

  parList->beginGroup  ( "Load Image Options" );
  parList->beginLayout ( "grid" );

  parList->createCheckbox ( "loadGray", "Convert to Gray" );
  parList->createCheckbox ( "load8bit" );

  parList->endGroup ();

  parList->beginGroup  ( "Ocr Program Options" );
  parList->beginLayout ( "grid" );

  itemPtr = parList->getParItem ( "language" );
  itemPtr->infos ["maxWidth"] = "40";

  parList->createLineEdit ( "language" );
  parList->createLineEdit ( "imageDpi" );
  parList->createCheckbox ( "openPdf" );

  parList->endGroup ();

  parList->createStretch ();

  // add parList for the dialog controls
  grid_1->setParList ( & list_1 );

  // set new dialog parList for save image
  parList->setDlgParList ( & list_2 );

  itemPtr = parList->getParItem ( "saveType" );
  itemPtr->infos ["maxWidth"] = "50";

  parList->beginGroup  ( "Save Image Options" );
  parList->beginLayout ( "grid" );

  parList->createCheckbox ( "saveAsIs" );
  parList->createCheckbox ( "saveGray" );
  parList->createLineEdit ( "saveType" );
  parList->createLineEdit ( "saveJpgComp" );

  parList->createStretch ();

  parList->endGroup ();

  // add parList for the dialog controls
  grid_2->setParList ( & list_2 );

}               /*-----  createControls  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLayout  of  ImageFileDlg
 |
 |  Description :
 |
 |   This function creates the layout for the image file dialog by adding
 |   the controls to the box layouts.
 |
 +-------------------------------------------------------------------------*/

void ImageFileDlg::createLayout ()
{
  QVBoxLayout  * vBox;
  QHBoxLayout  * hBox1;
  QHBoxLayout  * hBox2;
  QHBoxLayout  * hBox3;

  btnCancel = new QPushButton ( "Cancel" );
  btnCancel->setIcon ( QIcon ( ":/Icons/Cancel" ) );

  btnOk = new QPushButton ( "Ok" );
  btnOk->setIcon ( QIcon ( ":/Icons/OkClean" ) );

  // create layout objects
  vBox  = new QVBoxLayout ();
  hBox1 = new QHBoxLayout ();
  hBox2 = new QHBoxLayout ();
  hBox3 = new QHBoxLayout ();

  hBox1->addWidget  (grid_1);
  hBox1->addWidget  (grid_2);

  hBox2->addWidget  ( new QLabel ( "Ocr Parameters :" ) );
  hBox2->addWidget  (txtOcrPar);

  // create cancel / ok button layout
  hBox3->addStretch ();
  hBox3->addWidget  (btnOk);
  hBox3->addStretch ();
  hBox3->addWidget  (btnCancel);
  hBox3->addStretch ();

  // order groups in one vertical box
  vBox->addLayout  (hBox1);
  vBox->addWidget  (cbOcrProg);
  vBox->addLayout  (hBox2);
  vBox->addSpacing (10);
  vBox->addLayout  (hBox3);

  this->setLayout  (vBox);

}               /*-----  createLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : initDialog  of  ImageFileDlg
 |
 |  Description :
 |
 |   This  member function is called from the constructor.
 |
 +-------------------------------------------------------------------------*/

void ImageFileDlg::initDialog ()
{

  fileConfig = parMan->getFileConfig ();
  parList    = fileConfig->getParList ();
  grid_1    = new ParListGrid  (this);
  grid_2    = new ParListGrid  (this);

  createControls ();
  createLayout   ();
  connectSlots   ();

  toGui ();

}               /*-----  initDialog  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : fromGui  of  ImageFileDlg
 |
 |  Description :
 |
 |   This  function  reads the  state  of  the  dialog and  converts  the
 |   controls back to internal values.
 |
 +-------------------------------------------------------------------------*/

void ImageFileDlg::fromGui ()
{

  fileConfig->ocrFile   = cbOcrProg->getFileName ();
  fileConfig->ocrConfig = txtOcrPar ->text ();

  grid_1 ->fromGui ();
  grid_2 ->fromGui ();

}               /*-----  fromGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : toGui  of  ImageFileDlg
 |
 |  Description :
 |
 |   This  function is  called after  creating the  window,  the internal
 |   values of  the dialog  variables are converted  to the  dialog (Gui)
 |   controls (widgets).
 |
 +-------------------------------------------------------------------------*/

void ImageFileDlg::toGui ()
{

  cbOcrProg->addFile ( fileConfig->ocrFile );

  grid_2->toGui ();
  grid_1->toGui ();

  grid_1->getWidget ("load8bit")->hide ();

  txtOcrPar->setText ( fileConfig->ocrConfig );

}               /*-----  toGui  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onCancel  of  ImageFileDlg
 |
 |  Description :
 |
 |   This function  is called when  the 'onCancel' button is  pressed. In
 |   this case nothig is saved and the dialog is closed.
 |
 +-------------------------------------------------------------------------*/

void ImageFileDlg::onCancel ()
{

  close ();

}               /*-----  onCancel  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : onOk  of  ImageFileDlg
 |
 |  Description :
 |
 |   This function is  called when the 'Ok' button is  pressed. It stores
 |   the current job file name in the parameter file.
 |
 +-------------------------------------------------------------------------*/

void ImageFileDlg::onOk ()
{

  fromGui ();

  close ();

}               /*-----  onOk  -----*/

