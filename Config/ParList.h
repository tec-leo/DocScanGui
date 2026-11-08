/*-------------------------------------------------------------------------
 |
 |  Include-File :  ParList.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  ParList.
 |
 +-------------------------------------------------------------------------*/

#ifndef ParList_h
#define ParList_h

#include "ParItem.h"

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class  ParFile;

/*-------------------------------------------------------------------------
 |
 |  Class :  ParList
 |
 |  Description :
 |
 |   This class is a container class for parameter items 'PartItem'. They
 |   can be added,  deleted, searched, retrieved, loaded and  stored in a
 |   generic way with the use of QVariant.
 |
 +-------------------------------------------------------------------------*/

class ParList
{

  friend class ParMan;

  public:
    ParList  ();
    virtual ~ParList ();

    void         append          ( ParItem & parItem );
    void         buildParMap     ();
    void         copy            ( QString name );
    int          count           ();
    void         clear           ();
    void         beginLayout     ( QString type );
    void         endLayout       ();
    void         beginGroup      ( QString label );
    void         endGroup        ( int stretch = 0 );
    void         createLabel     ( QString label );
    void         createLabel     ( QString name, QString label );
    void         createStretch   ();
    void         createTable     ( QString name, L_String items );
    void         createCheckbox  ( QString name, QString label = "" ,
                                   bool twoColumns = false );
    void         createCombobox  ( QString name, L_String items,
                                   QString select );
    void         createLineEdit  ( QString name, QString label = "" );
    void         createRadioBtn  ( QString name, QString label = "",
                                   bool twoColumns = false );
    int          findParItem     ( QString name );
    L_ParItems   getParItems     ();
    ParItem    * getParItem     ( QString name );
    bool         getParItem      ( int index, ParItem & item );
    bool         getParItem      ( QString name, ParItem & item );
    bool         setParItem      ( int index, ParItem item );
    bool         setParItem      ( ParItem item );
    bool         getItemValue   ( QString name, QString & value );
    bool         setItemValue   ( QString name, QString value );

    void         load            ();
    void         save            ();

    void         removeLabel     ( QString name );
    void         setDlgParList   ( ParList * parList );
    void         setSrcParList   ( ParList * parList );
    void         setParFile     ( ParFile * parFile );

  protected:
    L_ParItems   parItems;
    M_StrInt     parMap;      // store name / index pair for faster access
    ParItem    * parItem;     // currently selected item
    ParFile    * parFile;
    ParList    * dlgParList;
    ParList    * srcParList;  // used to copy items

};

#endif
