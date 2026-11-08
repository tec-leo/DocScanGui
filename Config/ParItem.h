/*-------------------------------------------------------------------------
 |
 |  Include-File :  ParItem.h
 |
 |  Description :
 |
 |   This  include   file  contains   the  declarations  of   the  module
 |   'ParItem'.
 |
 +-------------------------------------------------------------------------*/

#ifndef ParItem_h
#define ParItem_h


/*-------------------------------------------------------------------------
 |
 |  Class :  ParItem
 |
 |  Description :
 |
 |   This class implements a parameter item with overloaded functions for
 |   several  data  types. It  holds  the  value,  ranges, pointer  to  a
 |   variable and some additional information.
 |
 +-------------------------------------------------------------------------*/

class ParItem
{
  public:
    ParItem  ();
    virtual ~ParItem ();

  public:

    void    clear       ();

    // overloaded create functions
    void    create      ( QString parName, bool * pVoid );

    void    create      ( QString parName, double * pVoid,
                          double  rangeMin = -DBL_MAX, double rangeMax = DBL_MAX );

    void    create      ( QString parName, float * pVoid,
                          double  rangeMin = -FLT_MAX, double rangeMax = FLT_MAX );

    void    create      ( QString parName, int * pVoid,
                          double  rangeMin = INT_MIN, double rangeMax = INT_MAX );

    void    create      ( QString parName, long * pVoid,
                          double  rangeMin = LONG_MIN, double rangeMax = LONG_MAX );

    void    create      ( QString parName, QString * pVoid  );

    // internal conversion functions
    void    fromString  ();
    void    toString    ();
    double  get         ( bool round );
    void    set         ( double newVal, bool round );
    QString getValue    ();
    void    setValue    ( QString value );
    void    roundLimit  ( bool round );

  public:
    StringUtils strUtils;
    M_StrStr    infos;       // additional information, can be used in a dialog
    QString     section;     // section (storage location) in the parameter file
    QString     name;        // name of the parameter in the parameter file
    QString     varType;     // type of the variable
    QString     valueStr;
    double      value;
    void      * ptrVar;      // cannot be changed to double because of algorithm
    double      rangeMin;    // minimum allowed value
    double      rangeMax;    // maximum allowed value
    double      incr;        // increment for rounding
    bool        limitMin;    // limit to minimum when converting or rounding
    bool        limitMax;    // limit to maximum when converting or rounding
    bool        save;        // save item to parameter file or not
    int         decimals;

};

/*------------------------------------------*/
/*    T Y P E - D e c l a r a t i o n s     */
/*------------------------------------------*/

// type definition of the container to display the parameters

typedef QList < ParItem > L_ParItems;

#endif
