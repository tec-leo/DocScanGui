/*-------------------------------------------------------------------------
 |
 |  File name   :  ImageView.cpp
 |
 |  Description :
 |
 |   This module  contains the  class 'ImageView'  which is  derived from
 |   'QGraphicsView' which is is derived from QAbstractScrollArea and can
 |   be used  as widget for  example in a  QTabWidget.  It creates  a new
 |   graphic scene and sets it as current 'scene'.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 28.Nov.2024
 |  Modified  : Oskar Leirich                Last changes  : 21.Oct.2025
 |
 |
 |  This file contains the following functions:
 |   of ImageView :
 |    ImageView                 ~ImageView                addImage
 |    addLayer                  deleteItems               getScale
 |    init                      loadImage                 onBtnActive
 |    resetView                 scaleView                 setImage
 |    setSize
 |
 +-------------------------------------------------------------------------*/

#include "ImageView.h"
#include "../Main/MainWnd.h"

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const int   cNone  = 0;
const int   cImage = 1;
const int   cLayer = 2;

const float zoomFactor = 0.8f;

/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  ImageView
 |
 |  Description :
 |
 |   The constructor  initializes some variables and  creates the graphic
 |   scene and viewport.
 |
 |  Input parameters :
 |   parent          : parent widget
 |
 +-------------------------------------------------------------------------*/

ImageView::ImageView ( QWidget * parent ) : QGraphicsView (parent)
{

  graphScene = new ImageScene (parent);

  init ();

  setHorizontalScrollBarPolicy (Qt::ScrollBarAsNeeded);
  setVerticalScrollBarPolicy   (Qt::ScrollBarAsNeeded);

  setTransformationAnchor (QGraphicsView::NoAnchor);

  setAcceptDrops (true);

  show ();

}               /*-----  ImageView  -----*/


/*-------------------------------------------------------------------------
 |
 |   D E S T R U C T O R  of  ImageView
 |
 |  Description :
 |
 |   The destructor deletes some objects.
 |
 +-------------------------------------------------------------------------*/

ImageView::~ImageView ()
{

  delete layersList;
  delete graphScene;
  //  delete contextMenu;

}               /*-----  ~ImageView  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : init  of  ImageView
 |
 |  Description :
 |
 |   This member function is called  from the constructor and creates the
 |   context menu.
 |
 +-------------------------------------------------------------------------*/

void ImageView::init ()
{

  setScene (graphScene);

  graphScene->setBackgroundBrush (Qt::NoBrush);
  graphScene->setForegroundBrush (Qt::NoBrush);

  viewWidget = viewport ();

  lastItem   = cNone;

  image      = & tmpImage;

  layersList = new LayersList ();
  layerNames.clear ();

  connect ( layersList, & LayersList::btnIconToggled,
            this,       & ImageView::onBtnActivate );

}               /*-----  init  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  addImage  of  ImageView
 |
 |  Description :
 |
 |   This function takes the given image and  makes a copy of it. It does
 |   not take ownership of the given  image. Then the pointer to the copy
 |   is updated,  a pixmap is created  from it and added  to the graphics
 |   scene.
 |
 |  Input parameters :
 |   layerItem       : Pointer to an image
 |   name            : Name of the layer
 |
 +-------------------------------------------------------------------------*/

void ImageView::addImage ( QImage * img, QString name )
{
  QGraphicsPixmapItem * item;
  int cnt;

  cnt = graphItems.count ();

  if ( img != image )
  {
    tmpImage = img->copy ();
    image    = & tmpImage;
  }

  pixMap = QPixmap::fromImage ( * image );

  item = graphScene->addPixmap (pixMap);

  text.setNum ( cnt + 1 );
  text = "Layer " + text + " : " + name;
  item->setData ( 0, text);
  //  item->setZValue (cnt);

  graphItems.append (item);

  layersList->addButton (name);

  layerNames.append (name);

  lastItem = cImage;   // image

}               /*-----  addImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  addLayer  of  ImageView
 |
 |  Description :
 |
 |   This function takes the given graphics item group and adds it to the
 |   graphics scene  which takes the  ownership of  it. The item  is also
 |   stored in the own list of graphic items.
 |
 |  Input parameters :
 |   layerItem       : Layer with graphics items
 |   name            : Name of the layer
 |
 +-------------------------------------------------------------------------*/

void ImageView::addLayer ( QGraphicsItemGroup * layerItem, QString name )
{
  int cnt;

  cnt = graphItems.count ();
  layerItem->setZValue (cnt);

  text.setNum ( cnt + 1 );
  text = "Layer " + text + " : " + name;
  layerItem->setData ( 0, QVariant (text) );

  layerItem->setVisible (true);

  graphScene->addItem (layerItem);

  graphItems.append (layerItem);

  layersList->addButton (name);

  layerNames.append (name);

  lastItem = cLayer;   // graphic item

}               /*-----  addLayer  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  deleteItem  of  ImageView
 |
 |  Description :
 |
 |   This function deletes all graphic items.
 |
 +-------------------------------------------------------------------------*/

void ImageView::deleteItem ( QString name )
{
  int index;

  index = layerNames.indexOf (name);

  if ( index < 0 )
    return;

  graphItem = graphItems.takeAt (index);
  graphScene->removeItem (graphItem);

  layersList->delButton (index);
  layerNames.removeAt (index);

}               /*-----  deleteItem  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  deleteItems  of  ImageView
 |
 |  Description :
 |
 |   This function deletes all graphic items.
 |
 +-------------------------------------------------------------------------*/

void ImageView::deleteItems ()
{

  graphScene->clear ();

  graphItems.clear ();

  layersList->clear ();
  layerNames.clear ();

}               /*-----  deleteItems  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  deleteLast  of  ImageView
 |
 |  Description :
 |
 |   This function deletes the last graphic item,
 |
 +-------------------------------------------------------------------------*/

void ImageView::deleteLast ()
{

  graphItem = graphItems.takeLast ();
  graphScene->removeItem (graphItem);

  delete graphItem;

  layersList->delLastButton ();
  layerNames.removeLast ();

}               /*-----  deleteLast  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  dragEnterEvent  of  ImageView
 |
 |  Description :
 |
 |   This function initiates the drag process.
 |
 +-------------------------------------------------------------------------*/

void ImageView::dragEnterEvent ( QDragEnterEvent * event )
{

  event->setDropAction ( Qt::CopyAction );

  if ( event->mimeData ()->hasText () )
  {
    event->acceptProposedAction ();
  }

}               /*-----  dragEnterEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  dropEvent  of  ImageView
 |
 |  Description :
 |
 |   This function implements  the drop event. It will  move the selected
 |   items to the  index marked by the drop location  and will signal the
 |   moved indexes.
 |
 +-------------------------------------------------------------------------*/

void ImageView::dropEvent ( QDropEvent * event )
{
  QString fileName;

  fileName = event->mimeData ()->text ();
  fileName = fileName.remove ( "file:///" );

  emit textDropped (fileName);

  event->acceptProposedAction ();

}               /*-----  dropEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getScale  of  ImageView
 |
 |  Description :
 |
 |   This function calculates  the scaling factor so that  the image fits
 |   into the  screen area. It  can calculate  the scaling for  fitting a
 |   page, width or height.
 |
 |  Input parameters :
 |   fit             : Tells if fitting means page, width or height
 |
 |  Output parameter :
 |   (double)        : scaling factor (not in percentage)
 |
 +-------------------------------------------------------------------------*/

double ImageView::getScale ( QString fit )
{
  QSize  areaSize;
  QSize  imgSize;
  double scale;
  double scaleH;
  double scaleW;
  int    scrollH;
  int    scrollV;

  if ( image->sizeInBytes () < 10 )
    return 1.0;

  areaSize = viewport ()->size ();
  imgSize  = image->size ();

  scrollH = horizontalScrollBar ()->height ();
  scrollV = verticalScrollBar ()->height ();

  scaleH = (double) ( areaSize.height () - scrollH ) / imgSize.height ();
  scaleW = (double) ( areaSize.width  () - scrollV ) / imgSize.width ();

  scale = 1.0;

  if ( fit.contains ( "page", Qt::CaseInsensitive ) )
  {
    scale = qMin ( scaleW, scaleH );
  }

  if ( fit.contains ( "height", Qt::CaseInsensitive ) )
  {
    scale = scaleH;
  }

  if ( fit.contains ( "width", Qt::CaseInsensitive ) )
  {
    scale = scaleW;
  }

  return scale;

}               /*-----  getScale  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  loadImage  of  ImageView
 |
 |  Description :
 |
 |   This function  makes a copy  of the  given QImage, stores  width and
 |   height, sets the scene rect and adds a new layer.
 |
 +-------------------------------------------------------------------------*/

void ImageView::loadImage ( QImage * img )
{
  int width;
  int height;

  deleteItems ();

  resetTransform ();

  // make a copy
  tmpImage = img->copy ();
  image    = & tmpImage;

  width  = image->width ();
  height = image->height ();

  setSceneRect ( 0, 0, width, height );

  addImage ( image, "Original" );

}               /*-----  loadImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  onBtnActivate  of  ImageView
 |
 |  Description :
 |
 |   This function is  called from the layers list when  a button icon in
 |   the widget  is toggled.   Depending on  the state  the corresponding
 |   graphic layer is shown or hidden.
 |
 |  Input parameters :
 |   index           : Checked state of the selected button
 |
 +-------------------------------------------------------------------------*/

void ImageView::onBtnActivate ( int index, bool active )
{

  graphItem = graphItems [index];

  graphItem->setVisible (active);

  graphScene->invalidate ( graphScene->sceneRect () );

}               /*-----  onBtnActivate  -----*/

/*-------------------------------------------------------------------------
 |
 |  Member function :  setImage  of  ImageView
 |
 |  Description :
 |
 |   This function takes the given image and  makes a copy of it. It does
 |   not take ownership of the given  imaes. Then the pointer to the copy
 |   is updated, a pixmap is created from it and given to the label.
 |
 +-------------------------------------------------------------------------*/

void ImageView::setImage ( QImage * img, int index, QString name )
{
  QGraphicsPixmapItem * item;

  if ( ( index < 0 ) or ( index >= graphItems.count () ) )
    return;

  // make a copy
  tmpImage = img->copy ();
  image    = & tmpImage;

  // remove graphItem at index
  graphItem = graphItems [index];
  graphScene->removeItem (graphItem);

  delete graphItem;   // got ownership, so it must be deleted.

  // create a pixmap and add it to the scene
  pixMap = QPixmap::fromImage ( * image );

  item = graphScene->addPixmap (pixMap);
  graphItems [index] = item;

  // need to change the stack order ?

  if ( index < ( graphItems.count () - 2 ) )
  {
    graphItem = graphItems [ index + 1 ];
    item->stackBefore (graphItem);
  }

  layersList->setButton ( index, name );

}               /*-----  setImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  resetView  of  ImageView
 |
 |  Description :
 |
 |   This function resets the transformation matrix because every scaling
 |   is multiplied to the matrix.
 |
 +-------------------------------------------------------------------------*/

void ImageView::resetView ()
{
  QTransform transformMat;

  transformMat = transform ();   // current transformation matrix

  // reset to identity matrix, no scaling or translating
  resetTransform ();

  // apply original translation matrix, e.g because of sliders
  translate ( transformMat.dx (), transformMat.dy () );

}               /*-----  resetView  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  scaleView  of  ImageView
 |
 |  Description :
 |
 |   This function  first resets the graphic  view and then it  scales it
 |   with the given scaling factor.
 |
 +-------------------------------------------------------------------------*/

void ImageView::scaleView ( double factor )
{

  if ( ( lastItem == cImage ) and ( image->sizeInBytes () < 10 ) )
    return;

  resetView ();

  scale ( factor, factor );

}               /*-----  scaleView  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setSize  of  ImageView
 |
 |  Description :
 |
 |   This function sets the viewport size to the given size.
 |
 +-------------------------------------------------------------------------*/

void ImageView::setSize ( QSize viewSize )
{

  viewWidget = viewport ();

  viewWidget->resize (viewSize);

}               /*-----  setSize  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  showLayer  of  ImageView
 |
 |  Description :
 |
 |   This function is called to show  or hide specific layers. The action
 |   describes  some show  or hide  actions.  The  info is  typically the
 |   detailed info (layer name) for the action.
 |
 |   Implemented actions:
 |    - show all
 |    - hide all
 |    - hide before
 |
 +-------------------------------------------------------------------------*/

void ImageView::showLayer ( QString action, QString info )
{
  int index;
  int cnt;

  cnt = graphItems.count ();

  if ( action == "show all" )
  {
    for ( int i = 0; i < cnt; i++ )
    {
      graphItem = graphItems [i];
      graphItem->setVisible (true);
    }
  }

  if ( action == "hide all" )
  {
    for ( int i = 0; i < cnt; i++ )
    {
      graphItem = graphItems [i];
      graphItem->setVisible (false);
    }
  }

  if ( action == "hide before" )
  {
    index = layerNames.indexOf (info);

    for ( int i = 0; i < index; i++ )
    {
      graphItem = graphItems [i];
      graphItem->setVisible (false);
    }

    graphScene->setBackgroundBrush ( QBrush () );

  }

  // invalidateScene ( graphScene->sceneRect () );
  graphScene->invalidate ( graphScene->sceneRect () );

}               /*-----  showLayer  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  updateImage  of  ImageView
 |
 |  Description :
 |
 |   This function updates  the and scene rectangle for  the given OpenCV
 |   image.
 |
 +-------------------------------------------------------------------------*/

void ImageView::updateImage ( QImage * img )
{
  int width;
  int height;

  tmpImage = img->copy ();
  image    = & tmpImage;

  resetTransform ();

  width  = image->width ();
  height = image->height ();

  setSceneRect ( 0, 0, width, height );

  viewWidget = viewport ();

}               /*-----  updateImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  wheelEvent  of  ImageView
 |
 |  Description :
 |
 |   This function  is the event  handler for  the mouse wheel  event. It
 |   just accepts the wheel event so  that the parent does not handle it.
 |   The image  widget implements  also a wheelEvent  for zooming  in and
 |   out.
 |
 |  Input parameters :
 |   event           : Graphics view mouse wheel event
 |
 +-------------------------------------------------------------------------*/

void ImageView::wheelEvent ( QWheelEvent * event )
{

  event->ignore ();   // default is zoom
  //  event->accept ();

}               /*-----  wheelEvent  -----*/

