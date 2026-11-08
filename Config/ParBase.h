/*-------------------------------------------------------------------------
 |
 |  Include-File :  ParBase.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  ParBase.
 |
 +-------------------------------------------------------------------------*/

#ifndef ParBase_h
#define ParBase_h

#include "ParList.h"

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class  ParFile;

/*-------------------------------------------------------------------------
 |
 |  Class :  ParBase
 |
 |  Description :
 |
 |   This class is  a base class for loading and  storing parameters to a
 |   file in config-ini format.
 |
 +-------------------------------------------------------------------------*/

class ParBase
{

  friend class ParMan;

  public:
    ParBase  ();
    virtual ~ParBase ();

    virtual void load         ();
    virtual void save         ();

    virtual void fillParList  ();
    virtual void setDefaults  ();

  public:
    void         updateList   ();
    ParList    * getParList   ();
    void         setSection   ( QString section );
    void         setParFile   ( ParFile * parFile );
    bool         setItemValue ( QString name, QString value );
    bool         haveItem     ( QString name );
    bool         isSaved      ();

  public slots:
    void         setModified  ( bool modified = true );

  protected:
    StringUtils strUtils;
    ParList     parList;
    ParFile   * parFile;
    QString     dummyLabel;
    QString     section;
    QString     name;
    QString     text;
    int         maxTextWidth;
    bool        saved;

};

#endif
