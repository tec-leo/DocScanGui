/*-------------------------------------------------------------------------
 |
 |  Include-File :  ToolsWidget.h
 |
 |  Description :
 |
 |   This include  file contains all external declarations  of the module
 |   'ToolsWidget'.
 |
 +-------------------------------------------------------------------------*/

#ifndef ToolsWidget_h
#define ToolsWidget_h

#include "../Tools/ImageTools.h"
#include "../Tools/Filters.h"
#include "../Main/JobInfo.h"
#include "Widgets/ParListGrid.h"

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const int cRunMorph   = 0;
const int cRunSharpen = 1;
const int cRunBlur    = 2;
const int cRunCurves  = 3;
const int cRunRotate  = 4;
const int cRunCrop    = 5;
const int cRunMargins = 6;
const int cRunMerge   = 7;
const int cRunDeskew  = 8;

const QString cToolMorph   = "Erode or dilate";
const QString cToolSharp   = "Sharpen image";
const QString cToolBlur    = "Blur filtering";
const QString cToolCurves  = "Color Curves";
const QString cToolDeskew  = "Deskew and crop";
const QString cToolRotate  = "Rotate image";
const QString cToolCrop    = "Crop image";
const QString cToolMargins = "Add margins";
const QString cToolMerge   = "Merge images";

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class  AppCurves;
class  AppConfig;
class  FileConfig;
class  ButtonsWidget;
class  ImageWidget;
class  ImageView;
class  ImageScene;
class  CurvesDialog;
class  ColorCurves;
class  MsgLogger;


/*-------------------------------------------------------------------------
 |
 |  Class :  ToolsWidget
 |
 |  Description :
 |
 |   This class  contains functions for improving  and optimizing scanned
 |   documents with the  help of OpenCV. It allows  to autorotate, deskew
 |   images and increases the contrast with several different methods.
 |
 +-------------------------------------------------------------------------*/

class ToolsWidget : public QWidget
{

  Q_OBJECT

  public:
    ToolsWidget ( QWidget * parent );
    virtual ~ToolsWidget ();

    void     executeJob       ( JobInfo curJob );
    QImage * getConvImage     ();
    Mat    * getLastImage     ()  { return & matOut; }

    void     runChainTo       ( int lastIdx );
    void     runTool          ( int index, bool force = false );
    void     setRotation      ( double angle );
    void     fromGui          ();
    void     toGui            ();
    bool     loadImage        ();
    void     saveImage        ( QString newName );
    void     saveImage        ( QString newName, Mat imgOut );

  public slots:
    void     onApplyCurves    ();
    void     onCurvesDlg      ();
    void     onMousePos       ( QPointF pos );
    void     onCurvesClosed   ( int result );
    void     onRunChain       ();
    void     onBtnClicked     ( int index, bool checked );

  protected:
    void     clearHistory     ();
    void     createObjects    ();
    QLabel * createTitleLabel ( QString title );
    void     createToolsGrid  ();
    void     createConfig     ();
    void     createLayout     ();
    void     createImgLayer   ( QString name );
    void     connectSlots     ();
    void     init             ();

    L_Int    getToolsList     ( QString tools );
    void     normInputImg     ( Mat & mat );
    Mat      normOutputImg    ( Mat mat );

    void     runDeskew        ();
    void     runFilter        ();
    void     runMerge         ();
    void     runSharpen       ();
    void     runMorphTrans    ();
    void     runChain         ( L_Int runIdx, bool force = false );
    void     showImageInfo    ( Mat & mat );

  protected:
    ImageWidget   * imageWidget;
    ImageView     * imageView;
    ImageScene    * imageScene;
    CurvesDialog  * curvesDlg;
    ColorCurves   * curves;
    ButtonsWidget * toolButtons;
    MsgLogger     * msgLog;

    AppCurves     * appCurves;
    AppConfig     * appConfig;
    FileConfig    * fileConfig;
    FileUtils       fileUtils;
    GraphicUtils    graphUtils;
    StringUtils     strUtils;
    L_String        jobList;
    L_Int           runList;

    ParListGrid   * parGrid;
    ParList       * parList;
    ParList         dlgParList;

    QListWidget   * outList;    // history list of all run tool outputs
    ImageMat        imageMat;
    ImageTools      imageTools;
    QString         imageName;
    Filters         filters;
    JobInfo         jobInfo;
    M_StrStr        infoMap;

    L_CvMat         imgStack;
    L_String        imgNames;
    Mat             matOrg;
    Mat             matIn;
    Mat             matMid;
    Mat             matOut;

    bool            imgLoaded;
    bool            cmdMode;      // run jobs in command mode
    bool            curvesOpen;

    // parameter and history;
    QString         text;

};

#endif
