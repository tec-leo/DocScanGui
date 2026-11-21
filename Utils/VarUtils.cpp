/*-------------------------------------------------------------------------
 |
 |  File Name   :  VarUtils.cpp
 |
 |  Description :
 |
 |   This  module  contains the  class  'VarUtils'  which implement  some
 |   utility   functions  for   rounding,  random   values  and   getting
 |   environment information from the operation system like date, time or
 |   any environment key.
 |
 |  Developer : Oskar Leirich                Creation date : 23.Nov.2022
 |  Modified  : Oskar Leirich                Last changes  : 20.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of VarUtils :
 |    VarUtils                  getDate                   getEnv
 |    getRange                  getTime                   randGen
 |    random                    randomNormal              round
 |    roundLong                 roundPercent              roundRes
 |    seedRandom
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "VarUtils.h"
#include <ctime>
#include <limits>

QRandomGenerator * VarUtils::randGen = QRandomGenerator::global ();

const quint32  cMaxRand  = UINT_MAX;


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  VarUtils
 |
 |  Description :
 |
 |   The constructor initializes the seed of the random generator.
 |
 +-------------------------------------------------------------------------*/

VarUtils::VarUtils ()
{

  std::srand ( std::time (nullptr) );

}               /*-----  VarUtils  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getEnv  of  VarUtils
 |
 |  Description :
 |
 |   This function returns the value for the  given  key  in  the  system
 |   environment.
 |
 |  Input parameters :
 |   key             : Key in the system environment.
 |
 |  Output parameter :
 |   (QString)       : Result as string
 |
 +-------------------------------------------------------------------------*/

QString VarUtils::getEnv ( QString key )
{
  StringUtils strUtils;
  L_String    env;
  QString     entry;
  int         pos;

  env = QProcess::systemEnvironment ();
  entry = key + "=.*";

  pos = strUtils.indexOf ( env, entry );

  if ( pos >= 0 )
  {
    entry = env [pos];
    env   = entry.split ( "=" );
    entry = env [1];
  }

  return  entry;

}               /*-----  getEnv  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getDate  of  VarUtils
 |
 |  Description :
 |
 |   This function returns the current date as a string in ISO format.
 |
 |  Output parameter :
 |   (QString)       : Date string in ISO format
 |
 +-------------------------------------------------------------------------*/

QString VarUtils::getDate ()
{
  QDate    date;
  QString  dateStr;

  date = QDate::currentDate ();

  dateStr = date.toString ( Qt::ISODate );

  return  dateStr;

}               /*-----  getDate  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getTime  of  VarUtils
 |
 |  Description :
 |
 |   This function returns the current time as a string in ISO format.
 |
 |  Output parameter :
 |   (QString)       : Time string in ISO format
 |
 +-------------------------------------------------------------------------*/

QString VarUtils::getTime ()
{
  QTime    time;
  QString  timeStr;

  time = QTime::currentTime ();

  timeStr = time.toString ( Qt::TextDate );

  return  timeStr;

}               /*-----  getTime  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getRange  of  VarUtils
 |
 |  Description :
 |
 |   This function creates a list of integer values in the range between first
 |   and last (not included) with the given increment (defaults to 1);
 |
 |  Input parameters :
 |   first           : First integer value
 |   last            : Last integer value (not included)
 |   incr            : Integer increment
 |
 |  Output parameter :
 |   (L_Int)         : List of integers
 |
 +-------------------------------------------------------------------------*/

L_Int VarUtils::getRange ( int first, int last, int incr )
{
  L_Int intList;
  int   iVal;

  iVal = first;

  while ( iVal < last )
  {
    intList.append (iVal);

    iVal = iVal + incr;
  }

  return intList;

}               /*-----  getRange  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  seedRandom  of  VarUtils
 |
 |  Description :
 |
 |   This function creates  a new 32 bit random generator  with the given
 |   seed. If the seed is -1 then it is intialized with the global object
 |   that was securely seeded with QRandomGenerator::system.  This is the
 |   recommended way for creating a new QRandomGenerator object that will
 |   be kept for some time.
 |
 |   Note : for compatibility with the stl use the QRandomGenerator64
 |
 |  Input parameters :
 |   seed            : new seed for the random generator
 |
 +-------------------------------------------------------------------------*/

void VarUtils::seedRandom ( qint32 seed )
{

  if (randGen)
    delete randGen;

  if ( seed == - 1 )
    randGen = QRandomGenerator::global ();
  else
  {
    randGen = new QRandomGenerator (seed);
  }

}               /*-----  seedRandom  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  random  of  VarUtils
 |
 |  Description :
 |
 |   This function creates a random number  in the given range. The means
 |   the random  number in the range  0..1.0 will be multiplied  with the
 |   given rande.
 |
 |  Input parameters :
 |   range           : input range
 |
 |  Output parameter :
 |   (double)        : random number in input range -  0 .. range
 |
 +-------------------------------------------------------------------------*/

double VarUtils::random ( int range )
{
  double rndValue;

  rndValue = random () * range;

  return rndValue;

}               /*-----  random  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  random  of  VarUtils
 |
 |  Description :
 |
 |   This function creates  a random double value in the  range between 0
 |   and 1.
 |
 |  Output parameter :
 |   (int)           : random number
 |
 +-------------------------------------------------------------------------*/

double VarUtils::random ()
{
  double  rndValue;
  quint32 bound;

  bound = randGen->bounded (cMaxRand);
  rndValue = (double) bound / ( cMaxRand - 1 );

  return rndValue;

}               /*-----  random  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  random  of  VarUtils
 |
 |  Description :
 |
 |   This function calculates an equal distributed random number which is
 |   in the range between lowest and highest.
 |
 |  Input parameters :
 |   lowest          : Minimum value for the random number
 |   highest         : Maximum value for the random number
 |
 |  Output parameter :
 |   (double)        : Random double value
 |
 +-------------------------------------------------------------------------*/

double VarUtils::random ( double lowest, double highest )
{
  double d_random;

  d_random = lowest + ( highest - lowest ) * random ();

  return  d_random;

}               /*-----  random  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  random  of  VarUtils
 |
 |  Description :
 |
 |   This function  generates an equal distributed  random number (32-bit
 |   quantity)  in  the  range  between lowest  (inclusive)  and  highest
 |   (exclusive),  both of  which may  be negative,  but highest  must be
 |   greater than lowest.
 |
 |  Input parameters :
 |   lowest          : Minimum value for the random number
 |   highest         : Maximum value for the random number
 |
 |  Output parameter :
 |   (int)           : Random integer value
 |
 +-------------------------------------------------------------------------*/

int VarUtils::random ( int lowest, int highest )
{
  int rndValue;

  // bounded generates lowest inclusive, but highest is exclusive
  rndValue = randGen->bounded ( lowest, highest + 1 );

  return  rndValue;

}               /*-----  random  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  randomNormal  of  VarUtils
 |
 |  Description :
 |
 |   This function calculates a normal distributed random number with the
 |   given average and the given variance. The calculation is  done  with
 |   the dozen rule of Zielinski.
 |
 |  Input parameters :
 |   average         : Average value for the random number
 |   variation       : Variance for the random number
 |
 |  Output parameter :
 |   (double)        :
 |
 +-------------------------------------------------------------------------*/

double VarUtils::randomNormal ( double average, double variation )
{
  int     i;
  double  t;
  double  d_random;

  t = 0;

  for ( i = 0; i < 12; i++ )
  {
    t = t + random ();
  }

  t = t - 6.0;

  d_random = average + variation * t;

  return d_random;

}               /*-----  randomNormal  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  roundRes  of  VarUtils
 |
 |  Description :
 |
 |   This  function  rounds  the  given  'real'  variable  to  the  given
 |   resolution.
 |
 |  Input parameters :
 |   inValue         : input variable
 |   resolution      : resolution of the rounding
 |
 |  Output parameter :
 |   (double)        : rounded result
 |
 +-------------------------------------------------------------------------*/

double VarUtils::roundRes ( double inValue, double resolution )
{
  double rndValue;
  int    intVal;

  intVal = round ( inValue / resolution );

  rndValue = intVal * resolution;

  return rndValue;

}               /*-----  roundRes  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  round  of  VarUtils
 |
 |  Description :
 |
 |   This function rounds the given 'real' variables and returns it as an
 |   integer variable. It rounds to  the closest absolute integer number,
 |   this means -0.7 will be rounded to -1 and not to 0.
 |
 |  Input parameters :
 |   inValue         : input variable
 |
 |  Output parameter :
 |   (int)           : rounded result
 |
 +-------------------------------------------------------------------------*/

int VarUtils::round ( double inValue )
{

  if ( inValue > 0 )
    return  (int) ( inValue + 0.5 );
  else
    return  (int) ( inValue - 0.5 );

}               /*-----  round  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  roundPercent  of  VarUtils
 |
 |  Description :
 |
 |   This function  rounds the  given 'double' variable  up to  the given
 |   percent for negative and positive values.
 |
 |  Input parameters :
 |   inValue         : input variable
 |   percent         : percentage ( from 0 ... 1.0 )
 |
 |  Output parameter :
 |   (double)        : rounded result
 |
 +-------------------------------------------------------------------------*/

double VarUtils::roundPercent ( double inValue, double percent )
{
  double outValue;
  double logx;
  double powx;
  double perc;

  if ( inValue == 0 )
    return inValue;

  outValue = fabs (inValue);

  logx = log10 (outValue);    // log 10 for value
  powx = floor (logx);        // skip decimals
  powx = pow ( 10.0, powx );  // power of 10

  perc = powx * percent;

  while ( powx < outValue )
  {
    powx = powx + perc;
  }

  outValue = powx;

  if ( inValue < 0 )
    outValue = - outValue;

  return outValue;

}               /*-----  round  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  roundLong  of  VarUtils
 |
 |  Description :
 |
 |   This function rounds the given 'real'  variables and returns it as a
 |   64 bit integer variable.
 |
 |  Input parameters :
 |   inValue         : input variable
 |
 |  Output parameter :
 |   (int64)         : rounded results
 |
 +-------------------------------------------------------------------------*/

int64 VarUtils::roundLong ( double inValue )
{

  if ( inValue > 0 )
    return  (int64) ( inValue + 0.5 );
  else
    return  (int64) ( inValue - 0.5 );

}               /*-----  roundLong  -----*/

