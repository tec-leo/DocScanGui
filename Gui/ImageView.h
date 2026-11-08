/*-------------------------------------------------------------------------
 |
 |  Include-File :  ImageView.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  ImageView.
 |
 +-------------------------------------------------------------------------*/

#ifndef ImageView_h
#define ImageView_h

#include "Controls/LayersList.h"
#include "ImageScene.h"


/*-------------------------------------------------------------------------
 |
 |  Class :  ImageView
 |
 |  Description :
 |
 |   This  class  is  derived   from  'QGraphicsView'  and  implements  a
 |   'QGraphicsView' which is is derived from QAbstractScrollArea and can
 |   be used  as widget for  example in a  QTabWidget.  It creates  a new
 |   graphic scene and sets it as current 'scene'.
 |
 +-------------------------------------------------------------------------*/

class ImageView : public QGraphicsView
{

  Q_OBJECT

  public:
    ImageView ( QWidget * parent = 0 );
    virtual ~ImageView ();

    void         addLayer       ( QGraphicsItemGroup * layerItem, QString name );
    void         addImage       ( QImage * img, QString name );
    void         deleteItems    ();
    void         deleteLast     ();
    void         deleteItem     ( QString name );
    ImageScene * getImageScene  ()  { return graphScene; }
    double       getScale       ( QString fit );
    QImage     * getImage       ()  { return image; }
    LayersList * getLayerList   ()  { return layersList; }
    L_String     getLayerNames  ()  { return layerNames; }
    QPixmap    * getPixmap      ()  { return & pixMap; }
    void         loadImage      (  QImage * img );
    void         setImage       ( QImage * img, int index, QString name );
    void         updateImage    ( QImage * img );
    void         scaleView      ( double factor );
    void         setSize        ( QSize viewSize );
    void         resetView      ();
    void         showLayer      ( QString action, QString info );

  signals:
    void         textDropped    ( QString text );

  public slots:
    void         onBtnActivate  ( int index, bool active );

  protected:
    void         dragEnterEvent ( QDragEnterEvent * event );
    void         dropEvent      ( QDropEvent * event );
    void         scaleImage     ( double factor );
    void         scaleScene     ( double factor );
    void         init           ();
    void         wheelEvent     ( QWheelEvent * event );

  protected:
    ImageScene     * graphScene;
    QGraphicsItem  * graphItem;
    QImage         * image;
    LayersList     * layersList;
    QWidget        * viewWidget;
    QMenu          * contextMenu;
    QPoint           menuPos;
    L_GraphItems     graphItems;
    L_String         layerNames;
    QImage           tmpImage;
    QPixmap          pixMap;
    QString          fileName;
    QString          text;
    int              lastItem;   // 0 = none, 1 = image, 2 = graphic item

};

#endif
