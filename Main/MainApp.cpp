/*-------------------------------------------------------------------------
 |
 |  File Name   :  MainApp.cpp
 |
 |  Description :
 |
 |   This module  provides a  class which  is derived  from QApplication.
 |   The main window of the application is a MainWindow.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 29.Nov.2024
 |  Modified  : Oskar Leirich                Last changes  : 19.May.2026
 |
 |
 |  This unit contains following member functions :
 |   MainApp :
 |    MainApp                   ~MainApp                  exitInstance
 |    initInstance              main
 |
 +-------------------------------------------------------------------------*/

#include "MainApp.h"


/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

MainApp  * mainApp   = nullptr;


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  MainApp
 |
 |  Description :
 |
 |   The constructor is empty.
 |
 +-------------------------------------------------------------------------*/

MainApp::MainApp ( int & argc, char ** argv ) : QApplication ( argc, argv )
{

  mainApp = this;

  initInstance ();

}               /*----------   MainApp   ----------*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  MainApp
 |
 |  Description :
 |
 |   The destructor of the main application.
 |
 +-------------------------------------------------------------------------*/

MainApp::~MainApp ()
{

  exitInstance ();

  mainApp = nullptr;

}               /*----------   MainApp   ----------*/


/*-------------------------------------------------------------------------
 |
 |  Member function : exitInstance  of  MainApp
 |
 |  Description :
 |
 |   This function is called by the framework when the main application
 |   return from the exit function.
 |
 +-------------------------------------------------------------------------*/

void MainApp::exitInstance ()
{

  if (mainWnd)
  {
    delete mainWnd;
    mainWnd = nullptr;
  }

}               /*-----  exitInstance  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : initInstance  of  MainApp
 |
 |  Description :
 |
 |   This function creates a 'MainWnd' and  defines it as the main window
 |   of this 'application'.
 |
 +-------------------------------------------------------------------------*/

void  MainApp::initInstance ()
{

  QCoreApplication::setOrganizationName   (cCompanyName);
  QCoreApplication::setOrganizationDomain (cCompanyDomain);
  QCoreApplication::setApplicationName    (cProgramConfig);

  decimalPoint = locale.decimalPoint ();

  // force number formats etc. to system settings
  QLocale::setDefault ( QLocale::c () );

  mainWnd = new MainWnd ();
  mainWnd->show ();

}               /*-----  initInstance  -----*/


/*-------------------------------------------------------------------------
 |
 |  Function :  main
 |
 |  Description :
 |
 |    This is the main entry point of the application.
 |
 +-------------------------------------------------------------------------*/

int main ( int argc, char * argv [] )
{
  int status;

  MainApp mainApp ( argc, argv );

  QApplication::setStyle ( QStyleFactory::create ("Fusion") );

  mainApp.setWindowIcon ( QIcon ( ":/Icons/MainApp.png" ) );

  try
  {
    status = mainApp.exec ();
  }
  catch (...)
  {
    status = 1;
  }

  return status;

}
