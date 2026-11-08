/*-------------------------------------------------------------------------
 |
 |  Include-File :  AppConfig.h
 |
 |  Description :
 |
 |   This  include file  contains all class   definitions for  the module
 |   'AppConfig'.
 |
 +-------------------------------------------------------------------------*/

#ifndef AppConfig_h
#define AppConfig_h

#include "ParBase.h"

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const QString cSimpleBlur    = "Simple blur";
const QString cMedianBlur    = "Median blur";
const QString cGaussFilter   = "Gaussian filter";

const QString cSharpGauss    = "Sharpen Gaussian";
const QString cUnsharpMask   = "Unsharp Mask";
const QString cSharpHighpass = "Sharpen Highpass";
const QString cSharpLaplace  = "Sharpen Laplace";

const QString cMergeLeft     = "Left (top)";
const QString cMergeCenter   = "Centered";
const QString cMergeRight    = "Right (bottom)";
const QString cMergeScaled   = "Scaled";


/*-------------------------------------------------------------------------
 |
 |  Class :  AppConfig
 |
 |  Description :
 |
 |   This  class contains  the parameter  (settings) of  the opencv  tool
 |   options. It is derived from ParBase.
 |
 +-------------------------------------------------------------------------*/

class AppConfig : public ParBase
{

  public:
    AppConfig ();
    virtual ~AppConfig ();

    virtual void load        ();
    virtual void save        ();
    virtual void setDefaults ();
    virtual void fillParList ();

  protected:
    void     fillCropPar     ();
    void     fillFilterPar   ();
    void     fillRegionsPar  ();
    void     fillToolsPar    ();

  public:
    QString  zoomImage;

    bool     runToolChain;   // run tool chain before curves, crop, rotate
    bool     autoDetect;     // auto detect values for deskew, crop, rotate

    bool     useSharpen;
    bool     useMorph;
    bool     useBlur;
    bool     useCurves;
    bool     useDeskew;

    bool     useRotate;
    bool     useAutoRot;

    bool     useCropImg;
    bool     useAutoCrop;

    bool     addMargins;

    int      pickRadius;       // radius (half kernel size) for picker color

    // parameter for filtering
    L_String filterTypes;
    QString  filterType;       // type median blur, gaussian
    int      blurKernel;       // kernel size for filter

    // parameter for sharpen
    L_String sharpTypes;
    QString  sharpType;        // type median blur, gaussian

    int      gaussKernel;      // kernel size for gaussian blur
    int      gaussSigma;       // Gaussian kernel standard deviation
    int      unsharpRadius;    // sigma value for the gaussian in unsharp mask

    int      laplaceKernel;    // kernel size for sharpen laplace filter
    int      unsharpAmount;    // percentage of the magnitude of each overshoot
    int      unsharpThresh;    // minimal brightness change that be sharpened

    // common parameter for sharpen for addWeighted
    double   alphaWeight;      // weight of the first array elements
    double   betaWeight;       // weight of the second array elements
    double   gammaAdd;         // scalar added to each sum

    // parameter for cropping, rotating, adding margins
    QString  cropMargins;      // page crop margins top, bottom, left, right
    QString  pageMargins;      // page margins top, bottom, left, right
    double   rotation;         // rotation (angle) of the image

    // parameter for merging
    L_String mergeTypes;
    QString  mergeAlign;       // alignment for merging, left, right, center
    QString  mergeImage;       // file name for 2nd image to merge
    bool     mergeVertical;    // merge vertical (true) or horizontal
    int      mergeGap;         // additional gap in pixels between merge images

    // parameter for morphing
    L_String morphTypes;
    QString  morphType;        // type like erode, dilate, open, close
    int      morphSize;        // element size, typically 3
    int      morphIter;        // number of iterations

    // upto now without gui
    int      numDilations;     // interations a dilation is applied
    double   anglePercLow;     // percentage of the lowest angles to ignore
    double   anglePercHigh;    // percentage of the highest angles to ignore
    double   boxProportion;    // proportion of a box --> width / height
    double   minBoxWidth;      // minimal box width for region detection
    double   minBoxHeight;     // minimal box height for region detection

};

#endif
