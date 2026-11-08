/*-------------------------------------------------------------------------
 |
 |  Include-File :  ImageEdit.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   ImageEdit.
 |
 +-------------------------------------------------------------------------*/

#ifndef ImageEdit_h
#define ImageEdit_h

#include "ImageView.h"
#include "../Tools/ImageTools.h"

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class  ImageWidget;
class  ToolsWidget;


/*-------------------------------------------------------------------------
 |
 |  Class :  ImageEdit
 |
 |  Description :
 |
 |   This class is derived from QWidget and is contains some functions to
 |   'edit' an image, this means rotating, cropping etc.
 |
 +-------------------------------------------------------------------------*/

class ImageEdit : public QWidget
{

  Q_OBJECT

  public:
    ImageEdit ( ImageWidget * parent );
    virtual ~ImageEdit ();

    QMenu * createMenu     ();
    int     getInsertMode  ()   { return insertMode; }
    void    setToolsWidget ( ToolsWidget * tools )   { toolsWidget = tools; }
    void    init           ();
    void    toGui          ();

  public slots:
    void    onApply        ();
    void    onCurves       ();
    void    onCancel       ();
    void    onDeskew       ();
    void    onRotate       ();
    void    onCrop         ();
    void    onCopy         ();
    void    onMerge        ();
    void    onAutoToggled  ( bool checked );
    void    onToolsToggled ( bool checked );
    void    onSceneMouse   ( QString action );

  protected:
    void    connectSlots   ();
    void    copyImgRect    ();
    void    getVertices    ( Mat inMat, L_PointsF & vertices );
    QString getCropRect    ();
    bool    prepareEdit    ( int runIdx );

  protected:
    AppConfig   * appConfig;
    ImageView   * imageView;
    ImageScene  * imageScene;
    ImageMat      imageMat;
    ImageTools    imageTools;
    ImageWidget * imageWidget;
    ToolsWidget * toolsWidget;
    cv::Mat     * image;      // last image from tools widget

    QAction     * detect;
    QAction     * tools;

    FileUtils     fileUtils;
    StringUtils   strUtils;
    bool          cropCopy;
    int           activeMode;
    int           insertMode;
    QString       text;

};

#endif
