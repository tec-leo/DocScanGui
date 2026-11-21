/*-------------------------------------------------------------------------
 |
 |  Include-File :  VarUtils.h
 |
 |  Description :
 |
 |   This  include file  contains all class   definitions for  the module
 |   'VarUtils'.
 |
 +-------------------------------------------------------------------------*/

#ifndef VarUtils_h
#define VarUtils_h


/*-------------------------------------------------------------------------
 |
 |  Class :  VarUtils
 |
 |  Description :
 |
 |   This  class contains  some  utility functions  for rounding,  random
 |   values and getting environment information from the operation system
 |   like date, time or any environment key.
 |
 +-------------------------------------------------------------------------*/

class VarUtils
{

  public:
    VarUtils ();
    virtual  ~VarUtils () {}

    static  QString  getEnv       ( QString key );
    static  QString  getDate      ();
    static  QString  getTime      ();
    static  L_Int    getRange     ( int first, int last, int incr = 1 );

    static  void     seedRandom   ( qint32 seed = -1 );
    static  double   random       ();
    static  double   random       ( int range );
    static  double   random       ( double lowest, double highest );
    static  int      random       ( int lowest, int highest );
    static  double   randomNormal ( double average, double variation );

    static  double   roundRes     ( double inValue, double resolution );
    static  int      round        ( double inValue );
    static  double   roundPercent ( double inValue, double percent );
    static  int64    roundLong    ( double inValue );

    static  QRandomGenerator * randGen;

};

#endif
