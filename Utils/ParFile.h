/*-------------------------------------------------------------------------
 |
 |  Include-File :  ParFile.h
 |
 |  Description :
 |
 |   This include  file contains all external declarations  of the module
 |   'ParFile'.
 |
 +-------------------------------------------------------------------------*/

#ifndef ParFile_h
#define ParFile_h


/*------------------------------------------*/
/*    T Y P E  -  D e c l a r a t i o n s   */
/*------------------------------------------*/

typedef  unsigned  char  uchar;
typedef  QHash < QString, QString >  H_StrStr;

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class SectionMap
{

  public:
    H_StrStr keyHash;
    L_String keyList;

};

/*------------------------------------------*/
/*    T Y P E  -  D e c l a r a t i o n s   */
/*------------------------------------------*/

typedef  QHash  < QString, SectionMap * >  H_StrSectHash;


/*-------------------------------------------------------------------------
 |
 |  Class :  ParFile
 |
 |  Description :
 |
 |   This class  is derived from  QSettings which provides  functions for
 |   accessing the parameters from a parameter file.
 |
 +-------------------------------------------------------------------------*/

class ParFile
{

  public:
    ParFile  ();
    virtual ~ParFile ();

    void     clear           ();
    bool     load            ();
    bool     save            ();

    QString  getAsString     ();
    L_String getAsStringList ();
    QString  getFileName     ();
    void     setFileName     ( QString fileName );

    void     setSection      ( QString section );
    QString  getSection      ();
    L_String getSectionText  ();
    H_StrStr getSectionHash  ();
    M_StrStr getSectionMap   ();
    L_String getSectionKeys  ();
    L_String getSectionNames ();

    void     deleteSection   ( QString section,
                               bool incSubSections = false );
    void     emptySection    ( QString section,
                               bool incSubSections = false );
    bool     entryExists     ( QString key );
    bool     sectionExists   ( QString section );

    bool     findPar         ( QString key, bool    & parameter );
    bool     findPar         ( QString key, char    & parameter );
    bool     findPar         ( QString key, uchar   & parameter );
    bool     findPar         ( QString key, short   & parameter );
    bool     findPar         ( QString key, ushort  & parameter );
    bool     findPar         ( QString key, int     & parameter );
    bool     findPar         ( QString key, uint    & parameter );
    bool     findPar         ( QString key, long    & parameter );
    bool     findPar         ( QString key, ulong   & parameter );
    bool     findPar         ( QString key, float   & parameter );
    bool     findPar         ( QString key, double  & parameter );
    bool     findPar         ( QString key, QString & parameter );

    bool     writePar        ( QString key, bool      parameter );
    bool     writePar        ( QString key, char      parameter );
    bool     writePar        ( QString key, uchar     parameter );
    bool     writePar        ( QString key, short     parameter );
    bool     writePar        ( QString key, ushort    parameter );
    bool     writePar        ( QString key, int       parameter );
    bool     writePar        ( QString key, uint      parameter );
    bool     writePar        ( QString key, long      parameter );
    bool     writePar        ( QString key, ulong     parameter );
    bool     writePar        ( QString key, float     parameter,
                               int decimals = 4 );
    bool     writePar        ( QString key, double    parameter,
                               int decimals = 4 );
    bool     writePar        ( QString key, QString   parameter );

  protected:
    bool     findPar         ( QString key );
    bool     writePar        ( QString key );

  protected:
    H_StrStr    * sectionPtr;
    SectionMap  * sectionMap;
    H_StrSectHash parHash;
    L_String      sectionNames;
    QString       section;
    QString       fileName;
    QString       entryValue;
    bool          ok;

};

#endif
