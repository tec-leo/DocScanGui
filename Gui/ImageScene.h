/*-------------------------------------------------------------------------
 |
 |  Include-File :  ImageScene.h
 |
 |  Description :
 |
 |   This  include file  contains  all class  definitions  for the  class
 |   ImageScene.
 |
 +-------------------------------------------------------------------------*/

#ifndef ImageScene_h
#define ImageScene_h

/*------------------------------------------*/
/*   T Y P E   -  D e c l a r a t i o n s   */
/*------------------------------------------*/

typedef  QList < QGraphicsItem *        >  L_GraphItems;
typedef  QList < QGraphicsEllipseItem * >  L_GraphEllipse;
typedef  QList < QGraphicsLineItem    * >  L_GraphLine;


/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const int cInsertNone = 0;
const int cInsertLine = 1;
const int cInsertPath = 2;
const int cInsertRect = 3;
const int cMoveLine   = 4;
const int cMovePath   = 5;
const int cMoveRect   = 6;
const int cMoveView   = 7;

/*-------------------------------------------------------------------------
 |
 |  Class :  ImageScene
 |
 |  Description :
 |
 |   This class is derived from QGraphicsScene and implements the graphic
 |   scene for  graphic items  like lines (connections),  circles (nodes)
 |   etc.
 |
 +-------------------------------------------------------------------------*/

class ImageScene : public QGraphicsScene
{

  Q_OBJECT

  public:
    ImageScene ( QWidget * parent = 0 );
    virtual ~ImageScene ();

  public:
    QLineF    getLine           () { return line; }
    QRectF    getRect           () { return rect; }
    L_PointsF getPath           ();
    void      makeLine          ( QPointF p1, QPointF p2, bool withCircle = true );
    void      makeRect          ( QRectF rect );
    void      makePath          ( L_PointsF points );
    void      removeItems       ();
    void      setPenColor       ( QColor color )   { penColor = color; }
    void      setPenWidth       ( float width )    { penWidth = width; }
    void      setPointWidth     ( float width )    { pntWidth = width; }
    void      setInsertMode     ( int mode )       { insertMode = mode; }
    void      setItemGroup      ( QGraphicsItemGroup * layer );

  signals:
    void      mousePressPos     ( QPointF pos );
    void      mouseAction       ( QString action );
    void      moveView          ( QPointF delta );

  protected slots:

  protected:
    void      init              ();
    void      makePen           ();
    void      makeCircle        ( QPointF pnt );
    void      moveLine          ();
    void      moveRect          ();
    void      movePath          ();
    void      moveRectCircles   ();
    void      mousePressEvent   ( QGraphicsSceneMouseEvent * event );
    void      mouseMoveEvent    ( QGraphicsSceneMouseEvent * event );
    void      mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );

  protected:
    QGraphicsItemGroup   * itemGroup;
    QGraphicsLineItem    * lineItem;
    QGraphicsPathItem    * pathItem;
    QGraphicsRectItem    * rectItem;
    QGraphicsEllipseItem * circle;
    QGraphicsItem        * graphItem;
    L_GraphEllipse         circleItems;
    L_GraphLine            lineItems;
    L_GraphItems           graphItems;
    GraphicUtils           graphUtils;

    QLineF    line;
    QRectF    rect;
    QPolygonF path;
    QPen      pen;
    QPen      pntPen;    // pen for marker points - circles
    QBrush    pntBrush;  // brush for marker points - circles

    QPointF   movePos;
    QPointF   begPos;
    QPointF   curPos;
    QPointF   endPos;

    QString   text;
    QColor    penColor;
    float     penWidth;
    float     pntWidth;
    int       insertMode;

};


#endif
