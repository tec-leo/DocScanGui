/*-------------------------------------------------------------------------
 |
 |  Include-File :  Spline.h
 |
 |  Description :
 |
 |   This include file contains all  class  definitions  for  the  module
 |   Spline.
 |
 +-------------------------------------------------------------------------*/

#ifndef Spline_h
#define Spline_h


/*-------------------------------------------------------------------------
 |
 |  Class :  Spline
 |
 |  Description :
 |
 |   This class is  used for cubical spline interpolation.   Two types of
 |   splines, natural and periodic, are supported.
 |
 +-------------------------------------------------------------------------*/

class Spline
{

  public:
    Spline ();
    Spline ( const Spline & );
    virtual ~Spline ();

    Spline & operator= ( const Spline & );

  public:
    bool       setPoints        ( QPolygonF points );
    QPolygonF  getPoints        () { return points; }

    void       reset            ();

    bool       isValid          ();
    double     getValueY        ( double x );

    L_Double   getCoefficientsA () { return a; }
    L_Double   getCoefficientsB () { return b; }
    L_Double   getCoefficientsC () { return c; }

  protected:
    bool       buildNatural     ( QPolygonF points );
    int        findPoint        ( double x, QPolygonF values );
    double     sqr              ( double x )  { return x * x; }

  protected:
    QPolygonF  points;    // points are just used as a list of QPointF

    // coefficient vectors
    L_Double   a;
    L_Double   b;
    L_Double   c;

};

#endif
