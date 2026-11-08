/*-------------------------------------------------------------------------
 |
 |  File Name   :  ParItem.cpp
 |
 |  Description :
 |
 |   This module contains the  container class 'ParItem' which implements
 |   a parameter item  with overloaded functions for  several data types.
 |   It  holds  the  value,  ranges,  pointer  to  a  variable  and  some
 |   additional information.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 06.Apr.2022
 |  Modified  : Oskar Leirich                Last changes  : 23.Sep.2026
 |
 |
 |  This unit contains following member functions :
 |    ParItem                   ~ParItem                  clear                     
 |    create                    fromString                get                       
 |    getValue                  roundLimit                set                       
 |    setValue                  toString                  
 |
 +-------------------------------------------------------------------------*/

#include "ParItem.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  ParItem
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 +-------------------------------------------------------------------------*/

ParItem::ParItem ()
{

  section  = "";

  clear ();

}               /*-----  ParItem  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  of  ParItem
 |
 |  Description :
 |
 |   The destructor deletes some objects.
 |
 +-------------------------------------------------------------------------*/

ParItem::~ParItem ()
{

}               /*-----  ~ParItem  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  clear  of  ParItem
 |
 |  Description :
 |
 |   This function initializes the variables of this class.
 |
 +-------------------------------------------------------------------------*/

void ParItem::clear ()
{

  // do not clear section here, only in constructor
  name      = "";
  varType   = "";
  valueStr  = "0.0";
  value     = 0.0;
  ptrVar    = nullptr;
  rangeMin  = 0.0;
  rangeMax  = 1.0;
  incr      = 0.1;
  decimals  = 6;
  limitMin  = true;
  limitMax  = true;
  save      = true;

}               /*-----  clear  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  create  of  ParItem
 |
 |  Description :
 |
 |   This function sets the parameter item to a bool.
 |
 +-------------------------------------------------------------------------*/

void ParItem::create ( QString parName, bool * pVoid )
{

  clear ();

  name    = parName;
  ptrVar  = pVoid;
  varType = "bool";
  incr    = 1.0;

}               /*-----  create  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  create  of  ParItem
 |
 |  Description :
 |
 |   This function sets the parameter item to a double.
 |
 +-------------------------------------------------------------------------*/

void ParItem::create ( QString parName, double * pVoid,
                       double rangeMin, double rangeMax )
{

  clear ();

  name    = parName;
  ptrVar  = pVoid;
  varType = "double";

  this->rangeMin = rangeMin;
  this->rangeMax = rangeMax;

}               /*-----  create  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  create  of  ParItem
 |
 |  Description :
 |
 |   This function sets the parameter item to a float.
 |
 +-------------------------------------------------------------------------*/

void ParItem::create ( QString parName, float * pVoid,
                       double rangeMin, double rangeMax )
{

  clear ();

  name    = parName;
  ptrVar  = pVoid;
  varType = "float";

  this->rangeMin = (double) rangeMin;
  this->rangeMax = (double) rangeMax;

}               /*-----  create  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  create  of  ParItem
 |
 |  Description :
 |
 |   This function sets the parameter item to a int.
 |
 +-------------------------------------------------------------------------*/

void ParItem::create ( QString parName, int * pVoid,
                       double rangeMin, double rangeMax )
{

  clear ();

  name    = parName;
  ptrVar  = pVoid;
  varType = "int";

  this->rangeMin = (double) rangeMin;
  this->rangeMax = (double) rangeMax;

}               /*-----  create  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  create  of  ParItem
 |
 |  Description :
 |
 |   This function sets the parameter item to a long.
 |
 +-------------------------------------------------------------------------*/

void ParItem::create ( QString parName, long * pVoid,
                       double rangeMin, double rangeMax )
{

  clear ();

  name    = parName;
  ptrVar  = pVoid;
  varType = "long";

  this->rangeMin = (double) rangeMin;
  this->rangeMax = (double) rangeMax;

}               /*-----  create  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  create  of  ParItem
 |
 |  Description :
 |
 |   This function sets the parameter item to a string.
 |
 +-------------------------------------------------------------------------*/

void ParItem::create ( QString parName, QString * pVoid )
{

  clear ();

  name    = parName;
  ptrVar  = pVoid;
  varType = "string";

}               /*-----  create  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  roundLimit  of  ParItem
 |
 |  Description :
 |
 |   This  function rounds  the  internal value  with  the increment  and
 |   limits to min / max limits if activated.
 |
 +-------------------------------------------------------------------------*/

void ParItem::roundLimit ( bool round )
{

  // round it to the increment of the parameter
  if (round)
    value = VarUtils::round ( value / incr ) * incr;

  // limit to minimum and maximum values
  if ( ( value > rangeMax ) and limitMax )
    value = rangeMax;

  if ( ( value < rangeMin ) and limitMin )
    value = rangeMin;

}               /*-----  roundLimit  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  get  of  ParItem
 |
 |  Description :
 |
 |   This function dereferences value pointer 'ptrVar' and converts it to
 |   a double.  It does also a rounding of the value to the increments if
 |   round is activated.
 |
 |  Output parameter :
 |   (double)        : Final value, rounded or not
 |
 +-------------------------------------------------------------------------*/

double ParItem::get ( bool round )
{
  double outVal;

  value = * (double *) ptrVar;

  outVal = value;

  if (round)
    outVal = VarUtils::round ( outVal / incr ) * incr;

  return outVal;

}               /*-----  get  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  set  of  ParItem
 |
 |  Description :
 |
 |   This function takes the given value  and converts it to the internal
 |   value.  It does  also a rounding of the value  to the increments and
 |   limits  if round  is activated.  Also  the value  referenced by  the
 |   'ptrVar' is updated.
 |
 |  Input parameters :
 |   newVal          : Input value which should be stored
 |   round           : Round to the increment or not
 |
 +-------------------------------------------------------------------------*/

void ParItem::set ( double newVal, bool round )
{

  value = newVal;

  roundLimit (round);

  * ( (double *) ptrVar ) = value;

}               /*-----  set  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fromString  of  ParItem
 |
 |  Description :
 |
 |   This function converts the parameter value from the internal string.
 |
 +-------------------------------------------------------------------------*/

void ParItem::fromString ()
{

  strUtils.fromString ( valueStr, ptrVar, varType );

}               /*-----  fromString  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getValue  of  ParItem
 |
 |  Description :
 |
 |   This function returns the value of this item as a string.
 |
 +-------------------------------------------------------------------------*/

QString ParItem::getValue ()
{

  strUtils.toString ( valueStr, ptrVar, varType, decimals );

  return valueStr;

}               /*-----  getValue  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setValue  of  ParItem
 |
 |  Description :
 |
 |   This  function  sets  the  internal string  value  to  'newVal'  and
 |   converts it to the type.
 |
 +-------------------------------------------------------------------------*/

void ParItem::setValue ( QString newVal )
{

  valueStr = newVal;

  strUtils.fromString ( valueStr, ptrVar, varType );

}               /*-----  setValue  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  toString  of  ParItem
 |
 |  Description :
 |
 |   This function converts the parameter to the internal string.
 |
 +-------------------------------------------------------------------------*/

void ParItem::toString ()
{

  strUtils.toString ( valueStr, ptrVar, varType, decimals );

}               /*-----  toString  -----*/

