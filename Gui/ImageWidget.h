/*-------------------------------------------------------------------------
 |
 |  Include-File :  ImageWidget.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   ImageWidget.
 |
 +-------------------------------------------------------------------------*/

#ifndef ImageWidget_h
#define ImageWidget_h

#include "ImageView.h"
#include "../Tools/ImageTools.h"
#include "../Main/JobInfo.h"

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class  FileConfig;
class  FileSelect;
class  PathSelect;
class  ImageEdit;
class  ToolsWidget;
class  MsgLogger;


/*-------------------------------------------------------------------------
 |
 |  Class :  ImageWidget
 |
 |  Description :
 |
 |   This class is derived from QWidget and is contains an 'ImageView' to
 |   show a  QGraphicsView, zoom  controls for the  image and  image file
 |   loading and saving or exporting controls.
 |
 +-------------------------------------------------------------------------*/

class ImageWidget : public QWidget
{

  Q_OBJECT

  public:
    ImageWidget ( QWidget * parent );
    virtual ~ImageWidget ();

  public:
    void         executeJob        ( JobInfo curJob );
    void         addImage          ( QImage * img, QString name );
    QString      getExportDir      ();
    ImageEdit  * getImageEdit      ()  { return imageEdit;   }
    ImageView  * getImageView      ()  { return imageView;   }
    ImageScene * getImageScene     ()  { return imageScene;  }
    QString      getImageName      ()  { return imageName;   }
    void         setImageName      ( QString name )  { imageName = name; }
    bool         getImageLoaded    ()  { return imageLoaded; }
    void         fromGui           ();
    void         toGui             ();
    void         loadImage         ();
    void         saveImage         ( QString newName );
    void         scaleView         ( Mat newMat );
    void         scaleView         ( bool forceScale = false );

  public slots:
    void         onExportImage     ();
    void         onViewMoved       ( QPointF delta );
    void         onZoomIn          ();
    void         onZoomOut         ();

  protected slots:
    void         onFileChanged     ( QString newFile );
    void         onReloadImage     ();
    void         onSaveImage       ();
    void         onSaveImageAs     ();
    void         onImageToPdf      ();
    void         onProcFinished    ( int exitCode,
                                     QProcess::ExitStatus exitStatus );

    void         onSliderChanged   ( int value );
    void         onZoomChanged     ( int index );

  protected:
    void         createMenu        ();
    void         contextMenuEvent  ( QContextMenuEvent * event );
    void         createObjects     ();
    void         createLayout      ();
    void         connectSlots      ();
    void         init              ();
    void         addZoomValue      ( QString value );
    bool         evalZoomFactor    ( bool forceScale = false );
    QString      getCropRect       ();
    void         wheelEvent        ( QWheelEvent * event );

  protected:
    QProcess     * process;
    QMenu        * contextMenu;
    ImageEdit    * imageEdit;
    ImageView    * imageView;
    ImageScene   * imageScene;
    ImageMat       imageMat;
    ImageTools     imageTools;
    ToolsWidget  * toolsWidget;
    QString        imageName;
    QString        pdfName;
    RegFile      * regFile;    // contains general settings
    AppConfig    * appConfig;
    FileConfig   * fileConfig;
    MsgLogger    * msgLog;

    FileSelect   * cbImage;
    PathSelect   * cbExport;

    QComboBox    * comboZoom;
    QSlider      * sliderZoom;
    QToolButton  * btnZoomIn;
    QToolButton  * btnZoomOut;

    QPushButton  * btnImgReload;
    QPushButton  * btnImgSave;
    QPushButton  * btnImgSaveAs;
    QPushButton  * btnImgExport;
    QPushButton  * btnImgToPdf;

    FileUtils      fileUtils;
    StringUtils    strUtils;

    bool           imageLoaded;
    bool           cmdMode;

    L_String       zoomValues;
    QString        zoomImage;
    double         zoomFactor;
    int            zoomIndex;
    QString        procTitle;
    QString        procMsg;
    QString        text;

};

#endif
