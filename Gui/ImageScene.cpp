/*-------------------------------------------------------------------------
 |
 |  File Name   :  ImageScene.cpp
 |
 |  Description :
 |
 |   This  module contains  the class  'ImageScene' which  implements the
 |   graphic scene  for graphic  items like lines  (connections), circles
 |   (nodes) etc.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 28.Nov.2024
 |  Modified  : Oskar Leirich                Last changes  : 22.Feb.2026
 |
 |
 |  This unit contains following member functions :
 |   of ImageScene:
 |    ImageScene                ~ImageScene               getPath
 |    init                      makeCircle                makeLine
 |    makePath                  makePen                   makeRect
 |    mouseMoveEvent            mousePressEvent           mouseReleaseEvent
 |    moveLine                  movePath                  moveRect
 |    removeItems               setItemGroup
 |
 +-------------------------------------------------------------------------*/

#include "ImageScene.h"
#include "../Main/MainWnd.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  ImageScene
 |
 |  Description :
 |
 |   The constructor  initializes some variables  and the basic  setup of
 |   the graphics scene is done here.
 |
 |  Input parameters :
 |   parent          :  Pointer to the parent widget
 |
 +-------------------------------------------------------------------------*/

ImageScene::ImageScene ( QWidget * parent ) : QGraphicsScene (parent)
{

  init ();

}               /*----- ImageScene -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  ImageScene
 |
 |  Description :
 |
 |   The destructor destroys the graphics view.
 |
 +-------------------------------------------------------------------------*/

ImageScene::~ImageScene ()
{


}               /*-----  ~ImageScene  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : getPath  of  ImageScene
 |
 |  Description :
 |
 |   This function returns the 4 points of the path from the deskew mode.
 |   The order of the points is top left, right and bottom left, right.
 |
 +-------------------------------------------------------------------------*/

L_PointsF ImageScene::getPath ()
{
  L_PointsF points;
  QPointF   pnt;

  if ( lineItems.count () != 4 )
    return points;

  line = lineItems [0]->line ();

  points.append ( line.p1 () );
  points.append ( line.p2 () );

  line = lineItems [1]->line ();

  points.append ( line.p1 () );
  points.append ( line.p2 () );

  return points;

}               /*-----  getPath  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : init  of  ImageScene
 |
 |  Description :
 |
 |   This member function is called  from the constructor and initializes
 |   the graphic items and creates the pen and context menu.
 |
 +-------------------------------------------------------------------------*/

void ImageScene::init ()
{

  lineItem  = nullptr;
  pathItem  = nullptr;
  rectItem  = nullptr;
  circle    = nullptr;
  graphItem = nullptr;

  penColor  = Color::blue;
  penWidth  = 5.0;
  pntWidth  = 18.0;   // for the circles (points)

  pntPen.setColor  (Color::green);
  pntPen.setWidthF (5.0);

  pntBrush.setColor (Color::red);
  pntBrush.setStyle (Qt::Dense6Pattern);

  makePen ();

  insertMode = cInsertNone;

}               /*-----  init  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : makePen  of  ImageScene
 |
 |  Description :
 |
 |   This function creates a pen with the defined color and width.
 |
 +-------------------------------------------------------------------------*/

void ImageScene::makePen ()
{

  pen.setColor  (penColor);
  pen.setWidthF (penWidth);

}               /*-----  makePen  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : makeCircle  of  ImageScene
 |
 |  Description :
 |
 |   This function creates a ellipse (circle)  but does not add it to the
 |   graphics scene. The points are selectable and moveable.
 |
 +-------------------------------------------------------------------------*/

void ImageScene::makeCircle ( QPointF pnt )
{
  QRectF rect;

  rect.setX ( pnt.x () - pntWidth / 2.0 );
  rect.setY ( pnt.y () - pntWidth / 2.0 );
  rect.setWidth  (pntWidth);
  rect.setHeight (pntWidth);

  rect.moveCenter (pnt);

  // create first point
  circle = new QGraphicsEllipseItem ();
  circle->setRect  (rect);
  circle->setPen   (pntPen);
  circle->setBrush (pntBrush);
  circle->setFlag  ( QGraphicsItem::ItemIsMovable, true );
  circle->setFlag  ( QGraphicsItem::ItemIsSelectable, true );

}               /*-----  makeCircle  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : makeLine  of  ImageScene
 |
 |  Description :
 |
 |   This  function creates  a line  with  two points  (circles) at  both
 |   end. The points are selectable and  moveable, the line has a tooltip
 |   which shows the  angle of it. The  index of the points  is stored in
 |   the custom data of the circles (QGraphicsItem).
 |
 +-------------------------------------------------------------------------*/

void ImageScene::makeLine ( QPointF p1, QPointF p2, bool withCircle )
{
  QRectF rect;
  int    idx;

  line.setPoints ( p1, p2 );

  makePen ();

  idx = circleItems.count ();

  lineItem = new QGraphicsLineItem ();
  lineItem->setLine (line);
  lineItem->setPen  (pen);

  lineItems.append (lineItem);
  addItem (lineItem);

  if ( not withCircle )
    return;

  pen.setColor  (Color::red);

  makeCircle (p1);

  // data is a container for key - value pairs of QGraphicsItem
  circle->setData ( 0, QVariant (idx) );
  circleItems.append (circle);
  addItem (circle);

  idx++;

  makeCircle (p2);

  // data is a container for key - value pairs of QGraphicsItem
  circle->setData ( 0, QVariant (idx) );
  circleItems.append (circle);
  addItem (circle);

}               /*-----  makeLine  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : moveLine  of  ImageScene
 |
 |  Description :
 |
 |   This function moves the line with two points (circles) at both end.
 |
 +-------------------------------------------------------------------------*/

void ImageScene::moveLine ()
{
  float angle;
  int   pntIndex;

  graphItems = selectedItems ();

  if ( graphItems.count () > 0 )
  {
    pntIndex = graphItems [0]->data (0).toInt ();

    if ( pntIndex == 0 )
      line.setP1 (curPos);
    else
      line.setP2 (curPos);
  }

  lineItem->setLine (line);

  angle = line.angle ();

  text.setNum ( angle, 'g', 3 );
  lineItem->setToolTip (text);

  mainWnd->updateStatusBar ( "Info", "Angle: "  + text + " deg" );

}               /*-----  moveLine  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : makeRect  of  ImageScene
 |
 |  Description :
 |
 |   This function  creates a rectangle  with 4 points (circles)  at each
 |   edge of the  rectangle. The points are selectable  and moveable, the
 |   index of  the points  is stored  in the custom  data of  the circles
 |   (QGraphicsItem).
 |
 |   Note the order of the points top left, right and bottom left, right.
 |   BottomRight should be the last for stretch a rectangle.
 |
 +-------------------------------------------------------------------------*/

void ImageScene::makeRect ( QRectF rect )
{
  L_PointsF points;
  QPointF   pnt;

  makePen ();

  this->rect = rect;

  rectItem = new QGraphicsRectItem ();
  rectItem->setRect (rect);
  rectItem->setPen  (pen);

  addItem (rectItem);

  points = graphUtils.getPointsRectF (rect);

  pntPen.setColor  (Color::red);

  for ( int idx = 0; idx < 4; idx++ )
  {
    pnt = points [idx];

    makeCircle (pnt);

    // data is a container for key - value pairs of QGraphicsItem
    circle->setData ( 0, QVariant (idx) );

    circleItems.append (circle);
    addItem (circle);
  }

}               /*-----  makeRect  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : makePath  of  ImageScene
 |
 |  Description :
 |
 |   This function creates a polygon with 4 points (circles) at each edge
 |   of the polygon. The points are selectable and moveable, the index of
 |   the  points   is  stored   in  the  custom   data  of   the  circles
 |   (QGraphicsItem).
 |
 |   Note the order of the points top left, right and bottom left, right.
 |   BottomRight should be the last for stretch a rectangle.
 |
 +-------------------------------------------------------------------------*/

void ImageScene::makePath ( L_PointsF points )
{

  makePen ();

  pntPen.setColor  (Color::red);

  makeLine ( points [0], points [1], true  );  // top left    --> top right
  makeLine ( points [2], points [3], true  );  // bottom left --> bottom right
  makeLine ( points [0], points [2], false );  // top left    --> bottom left
  makeLine ( points [1], points [3], false );  // top right   --> bottom right

}               /*-----  makePath  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : moveRect  of  ImageScene
 |
 |  Description :
 |
 |   This function is called when a point of the rectangle has moved. The
 |   index of the selected point is  stored in the first custom data. The
 |   rectangle  is updated  with the  selected  point and  then also  the
 |   'QGraphicsRectItem' in the graphic scene.
 |
 +-------------------------------------------------------------------------*/

void ImageScene::moveRect ()
{
  L_PointsF points;
  QPointF   pnt;
  QRectF    rectPrev;
  QRectF    rectPnt;
  int       pntIndex;

  graphItem = mouseGrabberItem ();

  if (not graphItem)    // could be a line or rect
    return;

  pntIndex = graphItem->data (0).toInt ();

  rectPrev = rect;    // rectangle before the move

  if ( pntIndex == 0 )      // first point
    rect.setTopLeft (curPos);

  if ( pntIndex == 1 )      // second point
    rect.setTopRight (curPos);

  if ( pntIndex == 2 )      // third point
    rect.setBottomLeft (curPos);

  if ( pntIndex == 3 )      // fourth point
    rect.setBottomRight (curPos);

  rectItem->setRect (rect);

  points = graphUtils.getRectFMoveBy ( rect, rectPrev );

  for ( int i = 0; i < 4; i++ )
  {
    pnt = points [i];

    circleItems [i]->moveBy ( pnt.x (), pnt.y () );
  }

}               /*-----  moveRect  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : movePath  of  ImageScene
 |
 |  Description :
 |
 |   This function is called when a point of the rectangle has moved. The
 |   index of the selected point is  stored in the first custom data. The
 |   rectangle  is updated  with the  selected  point and  then also  the
 |   'QGraphicsRectItem' in the graphic scene.
 |
 +-------------------------------------------------------------------------*/

void ImageScene::movePath ()
{
  L_PointsF points;
  QPointF   pnt;
  QRectF    rectPrev;
  QRectF    rectPnt;
  int       pntIndex;

  graphItem = mouseGrabberItem ();

  if (not graphItem)    // could be a line or rect
    return;

  pntIndex = graphItem->data (0).toInt ();

  rectPrev = rect;    // rectangle before the move

  if ( pntIndex == 0 )      // first point
  {
    line = lineItems [0]->line ();
    line.setP1 (curPos);
    lineItems [0]->setLine (line);

    line = lineItems [2]->line ();
    line.setP1 (curPos);
    lineItems [2]->setLine (line);
  }

  if ( pntIndex == 1 )      // second point
  {
    line = lineItems [0]->line ();
    line.setP2 (curPos);
    lineItems [0]->setLine (line);

    line = lineItems [3]->line ();
    line.setP1 (curPos);
    lineItems [3]->setLine (line);
  }

  if ( pntIndex == 2 )      // third point
  {
    line = lineItems [2]->line ();
    line.setP2 (curPos);
    lineItems [2]->setLine (line);

    line = lineItems [1]->line ();
    line.setP1 (curPos);
    lineItems [1]->setLine (line);
  }

  if ( pntIndex == 3 )      // fourth point
  {
    line = lineItems [1]->line ();
    line.setP2 (curPos);
    lineItems [1]->setLine (line);

    line = lineItems [3]->line ();
    line.setP2 (curPos);
    lineItems [3]->setLine (line);
  }

}               /*-----  movePath  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  mousePressEvent  of  ImageScene
 |
 |  Description :
 |
 |   This function is the event handler for the mouse press event.
 |
 |  Input parameters :
 |   event           : Graphics scene mouse event
 |
 +-------------------------------------------------------------------------*/

void ImageScene::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{

  if ( event->button () != Qt::LeftButton )
    return;

  begPos  = event->scenePos ();

  curPos  = begPos;
  endPos  = begPos;
  movePos = begPos;

  switch (insertMode)
  {
    case cInsertNone:
      emit mousePressPos (begPos);

      insertMode = cMoveView;

      QApplication::setOverrideCursor ( Qt::OpenHandCursor );
    break;

    case cInsertLine:
    {
      circleItems.clear ();

      makeLine ( begPos, endPos );
    }
    break;

    case cInsertRect:
    {
      rect.setTopLeft (begPos);
      rect.setBottomRight (endPos);

      makeRect (rect);
    }
    break;

    case cInsertPath:
    {
    }
    break;
  }

  QGraphicsScene::mousePressEvent (event);

}               /*-----  mousePressEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  mouseMoveEvent  of  ImageScene
 |
 |  Description :
 |
 |   This function  is the event  handler for  the mouse move  event.  It
 |   contains  a switch  for the  different  insert or  move actions  for
 |   rotate, crop or deskew.
 |
 |  Input parameters :
 |   event           : Graphics scene mouse event
 |
 +-------------------------------------------------------------------------*/

void ImageScene::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
  QPointF delta;

  curPos = event->scenePos ();

  switch (insertMode)
  {
    case cInsertNone:
    break;

    case cInsertLine:
    {
      line.setP2 (curPos);
      moveLine ();
    }
    break;

    case cInsertRect:
    {
      moveRect ();
    }
    break;

    case cInsertPath:
    {
    }
    break;

    case cMoveLine:
    {
      moveLine ();
    }
    break;

    case cMoveRect:
    {
      moveRect ();
    }
    break;

    case cMovePath:
    {
      movePath ();
    }
    break;

    case cMoveView:
    {
      delta = curPos - movePos;

      if ( ( fabs ( delta.x () ) > 10 ) or ( fabs ( delta.y () ) > 10 ) )
      {
        movePos = curPos;
        emit moveView (delta);
      }
    }
    break;
  }

  QGraphicsScene::mouseMoveEvent (event);

}               /*-----  mouseMoveEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  mouseReleaseEvent  of  ImageScene
 |
 |  Description :
 |
 |   This function is  the event handler for the mouse  release event. It
 |   contains  a switch  for the  different  insert or  move actions  for
 |   rotate, crop or deskew.
 |
 |  Input parameters :
 |   event           : Graphics scene mouse event
 |
 +-------------------------------------------------------------------------*/

void ImageScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
  QPointF delta;

  endPos = event->scenePos ();

  curPos = endPos;

  switch (insertMode)
  {
    case cInsertNone:
      emit mousePressPos (begPos);
    break;

    case cInsertLine:
    {
      line.setP2 (curPos);
      lineItem->setLine (line);

      emit mouseAction ( "release line" );  // set insert action to move
    }
    break;

    case cInsertRect:
    {
      moveRect ();

      emit mouseAction ( "release rect" );  // set insert action to move
    }
    break;

    case cInsertPath:
    {
    }
    break;

    case cMoveRect:
    {
      emit mouseAction ( "move rect" );  // set insert action to move
    }
    break;

    case cMoveLine:
    {
      emit mouseAction ( "move line" );  // set insert action to move
    }
    break;

    case cMoveView:
    {
      QApplication::restoreOverrideCursor ();

      delta = curPos - movePos;

      emit moveView (delta);
      emit mouseAction ( "view moved" );  // set insert action to view moved
    }
    break;
  }

  QGraphicsScene::mouseReleaseEvent (event);

}               /*-----  mouseReleaseEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  removeItems  of  ImageScene
 |
 |  Description :
 |
 |   This function deletes the graphic  items created for rotate, crop or
 |   deskew.
 |
 +-------------------------------------------------------------------------*/

void ImageScene::removeItems ()
{

  for ( int i = 0; i < circleItems.count (); i++ )
  {
    removeItem ( circleItems [i] );
  }

  for ( int i = 0; i < lineItems.count (); i++ )
  {
    removeItem ( lineItems [i] );
  }

  if (rectItem)
    removeItem (rectItem);

  circleItems.clear ();
  lineItems.clear ();

  lineItem  = nullptr;
  pathItem  = nullptr;
  rectItem  = nullptr;
  circle    = nullptr;
  graphItem = nullptr;

}               /*-----  removeItems  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setItemGroup  of  ImageScene
 |
 |  Description :
 |
 |   This function sets the 'itemGroup' to  the given group item. This is
 |   the root  item for lines,  marker points, rectangles,  polygons etc.
 |   This makes  is easy to  handle (hide, show,  delete) all items  in a
 |   group like a layer.
 |
 |   Note: not used, seems not to work, bug in Qt ???
 |
 |  Input parameters :
 |   layer           :  New value for variable itemGroup
 |
 +-------------------------------------------------------------------------*/

void ImageScene::setItemGroup ( QGraphicsItemGroup * layer )
{

  itemGroup = layer;

}               /*-----  setItemGroup  -----*/

