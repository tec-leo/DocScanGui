/*-------------------------------------------------------------------------
 |
 |  Include-File :  MainApp.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  MainApp.
 |
 +-------------------------------------------------------------------------*/

#ifndef MainApp_h
#define MainApp_h

#include "MainWnd.h"


/*------------------------------------------*/
/*  C L A S S  -  D e c l a r a t i o n s   */
/*------------------------------------------*/

class   MainApp;
extern  MainApp  * mainApp;

/*-------------------------------------------------------------------------
 |
 |  Class :  MainApp
 |
 |  Description :
 |
 |   This class is derived from  QApplication and creates the main window
 |   of the application.
 |
 +-------------------------------------------------------------------------*/

class MainApp : public QApplication
{

  friend class MainWnd;

   public:
    MainApp ( int & argc, char ** argv );
    virtual ~MainApp ();

    void    exitInstance  ();
    void    initInstance  ();

   public:
    QString decimalPoint;
    QLocale locale;

  protected:

};

#endif
