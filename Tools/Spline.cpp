/*-------------------------------------------------------------------------
 |
 |  File Name   :  Spline.cpp
 |
 |  Description :
 |
 |
 |   This module  contains the class 'Spline'  which implements functions
 |   for cubical spline interpolation.  Two types of splines, natural and
 |   periodic, are supported.
 |
 |   The usage is simple, first call setPoints () to determine the spline
 |   coefficients for a tabulated  function y(x).  After the coefficients
 |   have been set up, the interpolated  function value for an argument x
 |   can be determined by calling Spline::getValueY ().
 |
 |  Developer : Oskar Leirich                Creation date : 26.Mar.2025
 |  Modified  : Oskar Leirich                Last changes  : 28.Mar.2025
 |
 |
 |  This unit contains following member functions :
 |   of Spline :
 |    Spline                    ~Spline                   buildNatural
 |    findPoint                 getValueY                 isValid
 |    reset                     setPoints
 |
 +-------------------------------------------------------------------------*/


#include "AppInc.h"
#include "Spline.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  Spline
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 +-------------------------------------------------------------------------*/

Spline::Spline ()
{

}               /*-----  Spline  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  of  Spline
 |
 |  Description :
 |
 |   The destructor deletes some objects.
 |
 +-------------------------------------------------------------------------*/

Spline::~Spline ()
{

}               /*-----  ~Spline  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  isValid  of  Spline
 |
 |  Description :
 |
 |   This function  return if the  coefficients are valid, this  means if
 |   there are calculated coefficients.
 |
 |  Output parameter :
 |   (bool)          : true - coefficients are valid
 |
 +-------------------------------------------------------------------------*/

bool Spline::isValid ()
{

  if ( a.isEmpty () )
    return false;

  return true;

}               /*-----  isValid  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  buildNatural  of  Spline
 |
 |  Description :
 |
 |   This function determines the coefficients for a natural spline.
 |
 |  Input parameters :
 |   points          : Points
 |
 |  Output parameter :
 |   (bool)          : if successful
 |
 +-------------------------------------------------------------------------*/

bool Spline::buildNatural ( QPolygonF points )
{
  QPointF  * p;
  L_Double d;
  L_Double h;
  L_Double s;
  int      i;
  int      size;
  double   dy1;
  double   dy2;

  p    = points.data ();
  size = points.size ();

  // set up tridiagonal equation system
  // use coefficient vectors as temporary buffers
  h.resize ( size - 1 );
  d.resize ( size - 1 );

  for ( i = 0; i < size - 1; i++ )
  {
    h [i] = p [ i + 1 ].x () - p [i].x ();

    if ( h [i] <= 0 )
      return false;
  }

  dy1 = ( p [1].y () - p [0].y () ) / h [0];

  for ( i = 1; i < size - 1; i++ )
  {
    b [i] = c [i] = h [i];
    a [i] = 2.0 * ( h [ i - 1 ] + h [i] );

    dy2 = ( p [ i + 1 ].y () - p [i].y () ) / h [i];
    d [i] = 6.0 * ( dy1 - dy2 );
    dy1 = dy2;
  }

  // solve tridiagonal equation system

  // L-U Factorization
  for ( i = 1; i < size - 2; i++ )
  {
    c [i] /= a [i];
    a [ i + 1 ] -= b [i] * c [i];
  }

  // forward elimination
  s.resize (size);

  s [1] = d [1];

  for ( i = 2; i < size - 1; i++ )
    s [i] = d [i] - c [ i - 1 ] * s [ i - 1 ];

  // backward elimination
  s [ size - 2 ] = - s [ size - 2 ] / a [ size - 2 ];

  for ( i = size - 3; i > 0; i-- )
    s [i] = - ( s [i] + b [i] * s [ i + 1 ] ) / a [i];
  s [ size - 1 ] = s [0] = 0.0;

  // finally, determine the spline coefficients
  for ( i = 0; i < size - 1; i++ )
  {
    a [i] = ( s [ i + 1 ] - s [i] ) / ( 6.0 * h [i] );
    b [i] = 0.5 * s [i];
    c [i] = ( p [ i + 1 ].y () - p [i].y () ) / h [i]
           - ( s [ i + 1 ] + 2.0 * s [i] ) * h [i] / 6.0;
  }

  return true;

}               /*-----  buildNatural  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPoint  of  Spline
 |
 |  Description :
 |
 |   This function searches  for the point just before the  given x value
 |   and returns it's index.
 |
 |  Input parameters :
 |   x               : x value for searching closest point
 |   points          : list of points
 |
 |  Output parameter :
 |   (int)           : index of the found point
 |
 +-------------------------------------------------------------------------*/

int Spline::findPoint ( double x, QPolygonF points )
{
  int idx1;
  int idx2;
  int idx3;
  int size;

  size = points.size ();

  if ( x <= points [0].x () )
  {
    idx1 = 0;
  }
  else
  {
    if ( x >= points [ size - 2 ].x () )
    {
      idx1 = size - 2;
    }
    else
    {
      idx1 = 0;
      idx2 = size - 2;
      idx3 = 0;

      while ( idx2 - idx1 > 1 )
      {
        idx3 = idx1 + ( ( idx2 - idx1 ) >> 1 );

        if ( points [idx3].x () > x )
          idx2 = idx3;
        else
          idx1 = idx3;
      }
    }
  }

  return idx1;

}               /*-----  findPoint  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getValueY  of  Spline
 |
 |  Description :
 |
 |   This  function calculates  the  interpolated y-value  for the  given
 |   x-value.
 |
 |  Input parameters :
 |   x               : x-coordinate
 |
 |  Output parameter :
 |   (double)        : y-value for given x-value
 |
 +-------------------------------------------------------------------------*/

double Spline::getValueY ( double x )
{
  double delta;
  double valueY;

  int idx;

  if ( a.size () == 0 )
    return 0.0;

  idx = findPoint ( x, points );

  delta = x - points [idx].x ();

  valueY =  ( ( ( ( a [idx] * delta ) + b [idx] ) * delta + c [idx] ) *
              delta + points [idx].y () );

  return valueY;

}               /*-----  getValueY  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  reset  of  Spline
 |
 |  Description :
 |
 |   This function clears all coeficients and points.
 |
 +-------------------------------------------------------------------------*/

void Spline::reset ()
{

  a.clear ();
  b.clear ();
  c.clear ();

  points.clear ();

}               /*-----  reset  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setPoints  of  Spline
 |
 |  Description :
 |
 |   This  function stores  the given  points and  calculates the  spline
 |   coefficients  depending  on  the  spline type.  This  function  will
 |   determine the  coefficients for a  natural or a periodic  spline and
 |   store  them internally.   The sequence  of the  x-coordinate of  the
 |   points  has to  be  strictly monotone  increasing,  or the  function
 |   returns with false.
 |
 |  Input parameters :
 |   points          : Points
 |
 |  Output parameter :
 |   (bool)          : true - x points are strictly monotone increasing,
 |
 +-------------------------------------------------------------------------*/

bool Spline::setPoints ( QPolygonF points )
{
  int size;
  bool ok;

  size = points.size ();

  if ( size <= 2 )
  {
    reset ();
    return false;
  }

  this->points = points;

  a.resize ( size - 1 );
  b.resize ( size - 1 );
  c.resize ( size - 1 );

  ok = buildNatural (points);

  if ( not ok )
    reset ();

  return ok;

}               /*-----  setPoints  -----*/

