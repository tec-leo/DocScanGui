/*-------------------------------------------------------------------------
 |
 |  File Name   :  FileConfig.cpp
 |
 |  Description :
 |
 |   This  module contains  class  'FileConfig' with  (settings) for  job
 |   files, load and save configuration options.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 13.Mar.2026
 |  Modified  : Oskar Leirich                Last changes  : 14.May.2026
 |
 |
 |  This unit contains following member functions :
 |   of FileConfig:
 |    FileConfig                ~FileConfig               fillParList
 |    load                      save                      setDefaults
 |
 +-------------------------------------------------------------------------*/

#include "FileConfig.h"

/*------------------------------------------*/
/*  C O N S T -  D e c l a r a t i o n s    */
/*------------------------------------------*/

const char * c_ParImage = "FilesConfig";

/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  FileConfig
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 +-------------------------------------------------------------------------*/

FileConfig::FileConfig () : ParBase ()
{

  section = c_ParImage;

  setDefaults ();

  fillParList ();

}                   /*-----  FileConfig  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  FileConfig
 |
 |  Description :
 |
 |   The destructor does the clean-up.
 |
 +-------------------------------------------------------------------------*/

FileConfig::~FileConfig ()
{

}                   /*-----  FileConfig  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : fillParList  of  FileConfig
 |
 |  Description :
 |
 |   This function fills the parameter container list with all parameter.
 |
 +-------------------------------------------------------------------------*/

void FileConfig::fillParList ()
{
  ParItem  parItem;
  QString  info;
  QString  tooltip;

  parList.clear ();

  ParBase::fillParList ();

  parItem.section = section;

  parItem.section = section;

  tooltip = "Load image as gray scale image";
  parItem.create ( "loadGray", & loadGray );
  parItem.infos ["info"]     = "Load and convert to Gray";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Load image in 8 bit unsigned";
  parItem.create ( "load8bit", & load8bit );
  parItem.infos ["info"]     = "Load in 8 bit";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Save image without changing format or type";
  parItem.create ( "saveAsIs", & saveAsIs );
  parItem.infos ["info"]     = "Save image as it is";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Save image as gray scale image";
  parItem.create ( "saveGray", & saveGray );
  parItem.infos ["info"]     = "Save image as grayscale";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Save image in 8 bit unsigned";
  parItem.create ( "save8bit", & save8bit );
  parItem.infos ["info"]     = "Save in 8 bit";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Save image in this type like jpg or png";
  parItem.create ( "saveType", & saveType );
  parItem.infos ["info"]     = "Save as jpg or png";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Compression level for jpg files, typically 80 - 95";
  parItem.create ( "saveJpgComp", & saveJpgComp );
  parItem.infos ["info"]     = "Jpg compression level";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Path to the ocr to pdf executable";
  parItem.create ( "ocrFile", & ocrFile );
  parItem.infos ["info"]     = "Executable file path";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Parameters for the ocr to pdf executable";
  parItem.create ( "ocrConfig", & ocrConfig );
  parItem.infos ["info"]     = "Ocr parameters";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Language for the ocr";
  parItem.create ( "language", & language );
  parItem.infos ["info"]     = "Ocr language";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Setting image dpi for the ocr";
  parItem.create ( "imageDpi", & imageDpi );
  parItem.infos ["info"]     = "Image dpi for the ocr";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Run ocr program and generate pdf";
  parItem.create ( "createPdf", & createPdf );
  parItem.infos ["info"]     = "Generate pdf";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Open pdf viewer after pdf generation";
  parItem.create ( "openPdf", & openPdf );
  parItem.infos ["info"]     = "Open pdf viewer";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  maxTextWidth = 40;

  ParBase::updateList ();

}                   /*-----  fillParList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  load  of  FileConfig
 |
 |  Description :
 |
 |   This function loads the global options from the parameter file.
 |
 +-------------------------------------------------------------------------*/

void FileConfig::load ()
{

  parFile->setSection (c_ParImage);

  ParBase::load ();

  parList.load ();

  load8bit = true;

  parFile->findPar ( "jobFile", jobFile );

}                   /*-----  load  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  save  of  FileConfig
 |
 |  Description :
 |
 |   This function saves the global options to the parameter file.
 |
 +-------------------------------------------------------------------------*/

void FileConfig::save ()
{

  parFile->setSection (c_ParImage);

  ParBase::save ();

  parList.save ();

  parFile->writePar ( "jobFile", jobFile );

}                   /*-----  save  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setDefaults  of  FileConfig
 |
 |  Description :
 |
 |   This function sets all parameter to the default value.
 |
 +-------------------------------------------------------------------------*/

void FileConfig::setDefaults ()
{

  ParBase::setDefaults ();

  jobFile     = "E:/Image/Scan/Jobs/DocScanRun.txt";

  // parameter load and save
  loadGray    = true;
  load8bit    = true;

  saveAsIs    = true;
  saveGray    = false;
  save8bit    = true;
  saveType    = "jpg";
  saveJpgComp = 90;

  ocrFile     = "ocrmypdf";
  ocrConfig   = "--language <lang> --image-dpi <dpi>";
  language    = "en";
  imageDpi    = 300;
  createPdf   = false;
  openPdf     = false;

}                   /*-----  setDefaults  -----*/
