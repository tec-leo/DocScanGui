/*-------------------------------------------------------------------------
 |
 |  Include-File :  GraphicUtils.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  GraphicUtils.
 |
 +-------------------------------------------------------------------------*/

#ifndef GraphicUtils_h
#define GraphicUtils_h

/*------------------------------------------*/
/*   T Y P E   -  D e c l a r a t i o n s   */
/*------------------------------------------*/

typedef  QList < QPointF >  L_PointsF;

/*-------------------------------------------------------------------------
 |
 |  Class :   GraphicUtils
 |
 |  Description :
 |
 |   This  class is  a utility  class which  contains some  functions for
 |   clipping, rectangle conversion, point distance and more.
 |
 +-------------------------------------------------------------------------*/

class GraphicUtils
{
  public:
    GraphicUtils ();
    virtual ~GraphicUtils ();

    enum class PointCheck
    {
      Inside, Outside, Changed
    };

    static bool containsPoint ( QWidget * widget, QPoint point );

    QRect      convRectF           ( QRectF rectF );
    bool       clipTestLiangBrasky ( const float & p, const float & q,
                                     double & t0, double & t1 );
    bool       clipLiangBrasky     ( QPointF & p, QPointF & q );
    int        clipTestSutherland  ( const float & x, const float & y );
    PointCheck clipSutherland      ( QPointF & p, QPointF & q );

    bool       findIntersect       ( QLineF line, QRectF rect, QPointF & point );

    float      getDistance         ( QPointF p1, QPointF p2 );
    L_PointsF  getPointsRectF      ( QRectF rect );
    L_PointsF  getRectFMoveBy      ( QRectF rect1, QRectF rect2 );

    void       setWinDimension     ( float xwmin, float xwmax,
                                     float ywmin, float ywmax );
  protected:
    float   xwmin;
    float   xwmax;
    float   ywmin;
    float   ywmax;

};

#endif
