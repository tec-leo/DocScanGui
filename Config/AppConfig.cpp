/*-------------------------------------------------------------------------
 |
 |  File Name   :  AppConfig.cpp
 |
 |  Description :
 |
 |   This module  contains the  parameter (settings)  of the  opencv tool
 |   options.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 16.Feb.2025
 |  Modified  : Oskar Leirich                Last changes  : 24.Sep.2026
 |
 |
 |  This unit contains following member functions :
 |   of AppConfig :
 |    AppConfig                 ~AppConfig                fillCropPar
 |    fillFilterPar             fillParList               fillRegionsPar
 |    fillToolsPar              load                      save
 |    setDefaults
 |
 +-------------------------------------------------------------------------*/

#include "AppConfig.h"

/*------------------------------------------*/
/*  C O N S T -  D e c l a r a t i o n s    */
/*------------------------------------------*/

const char * c_ParMain = "Tools";

/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  AppConfig
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 +-------------------------------------------------------------------------*/

AppConfig::AppConfig () : ParBase ()
{

  section = c_ParMain;

  setDefaults ();

  fillParList ();

}               /*-----  AppConfig  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  of  AppConfig
 |
 |  Description :
 |
 |   The destructor does the clean-up.
 |
 +-------------------------------------------------------------------------*/

AppConfig::~AppConfig ()
{

}               /*-----  ~AppConfig  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fillCropPar  of  AppConfig
 |
 |  Description :
 |
 |   This function fills the parameter container list with the  parameter
 |   for crop, rotate and add margins.
 |
 +-------------------------------------------------------------------------*/

void AppConfig::fillCropPar ()
{
  ParItem  parItem;
  QString  info;
  QString  tooltip;

  parItem.section = section;

  tooltip = "Fixed value for rotation angle";
  parItem.create ( "rotation", & rotation );
  parItem.infos ["info"]     = "Rotation angle";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Try to detect the rotation angle of a text document";
  parItem.create ( "useAutoRot", & useAutoRot );
  parItem.infos ["info"]     = "Automatic rotation";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Crop margins : top, bottom, left, right";
  parItem.create ( "cropMargins", & cropMargins );
  parItem.infos ["info"]     = "Crop content to margins";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Crop a text document to the content automatically";
  parItem.create ( "useAutoCrop", & useAutoCrop );
  parItem.infos ["info"]     = "Automatic cropping";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Page margins : top, bottom, left, right";
  parItem.create ( "pageMargins", & pageMargins );
  parItem.infos ["info"]     = "Add margins to content";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  info = mergeTypes.join ( "," );
  tooltip = "Alignment for merging : left, right, centered, scaled etc.";
  parItem.create ( "mergeAlign", & mergeAlign );
  parItem.infos ["info"]     = "Merge alignment";
  parItem.infos ["options"]  = info;
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Merge second image vertical or horizontal";
  parItem.create ( "mergeVertical", & mergeVertical );
  parItem.infos ["info"]     = "Merge vertical";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Additional gap in pixels between merge images";
  parItem.create ( "mergeGap", & mergeGap );
  parItem.infos ["info"]     = "Merge gap in pixels";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "File path to second merge image";
  parItem.create ( "mergeImage", & mergeImage );
  parItem.infos ["info"]     = "Merge image";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

}               /*-----  fillCropPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fillFilterPar  of  AppConfig
 |
 |  Description :
 |
 |   This function fills the parameter container list with the  parameter
 |   for filtering and sharpening.
 |
 +-------------------------------------------------------------------------*/

void AppConfig::fillFilterPar ()
{
  ParItem  parItem;
  QString  info;
  QString  tooltip;
  bool     sharpenAll;

  parItem.section = section;

  sharpenAll = false;

  // parameter for sharping
  tooltip = "Kernel size for gaussian blur in sharping";
  parItem.create ( "gaussKernel", & gaussKernel );
  parItem.infos ["info"]     = "Gaussian blur kernel size";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Weight of the first array in addWeighted";
  parItem.create ( "alphaWeight", & alphaWeight );
  parItem.infos ["info"]     = "First weight (alpha)";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Weight of the second array in addWeighted";
  parItem.create ( "betaWeight", & betaWeight );
  parItem.infos ["info"]     = "Second weight (beta)";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Scalar added to each sum in addWeighted";
  parItem.create ( "gammaAdd", & gammaAdd );
  parItem.infos ["info"]     = "Scalar added to each sum (gamma)";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  if (sharpenAll)
  {
    info = sharpTypes.join ( "," );
    tooltip = "Sharpening methods like gaussian, laplace, etc.";
    parItem.create ( "sharpType", & sharpType );
    parItem.infos ["info"]     = "Sharpening";
    parItem.infos ["options"]  = info;
    parItem.infos ["tooltip"]  = tooltip;
    parList.append (parItem);

    tooltip = "Kernel size for the laplace sharpen filter";
    parItem.create ( "laplaceKernel", & laplaceKernel );
    parItem.infos ["info"]     = "Kernel size for laplace";
    parItem.infos ["tooltip"]  = tooltip;
    parList.append (parItem);

    tooltip = "Gaussian kernel standard deviation in x and y";
    parItem.create ( "gaussSigma", & gaussSigma );
    parItem.infos ["info"]     = "Gaussian blur sigma";
    parItem.infos ["tooltip"]  = tooltip;
    parList.append (parItem);

    tooltip = "Percentage of the magnitude of each overshoot";
    parItem.create ( "unsharpAmount", & unsharpAmount );
    parItem.infos ["info"]     = "Amount for unsharp mask";
    parItem.infos ["tooltip"]  = tooltip;
    parList.append (parItem);

    tooltip = "Minimal brightness change that be sharpened";
    parItem.create ( "unsharpThresh", & unsharpThresh );
    parItem.infos ["info"]     = "Threshold for unsharp mask";
    parItem.infos ["tooltip"]  = tooltip;
    parList.append (parItem);
  }

  // parameter for blur filter
  info = filterTypes.join ( "," );
  tooltip = "Type of filter like median blur, gaussian";
  parItem.create ( "filterType", & filterType );
  parItem.infos ["info"]     = "Filter type";
  parItem.infos ["options"]  = info;
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Kernel size for the blur filter";
  parItem.create ( "blurKernel", & blurKernel );
  parItem.infos ["info"]     = "Kernel size for filter";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  // parameter for morphing
  info = morphTypes.join ( "," );
  tooltip = "Type of morphological transformation";
  parItem.create ( "morphType", & morphType );
  parItem.infos ["info"]     = "Morph. Operation";
  parItem.infos ["options"]  = info;
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Element size for morphing";
  parItem.create ( "morphSize", & morphSize );
  parItem.infos ["info"]     = "Morph element size";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Number of iterations for morphing";
  parItem.create ( "morphIter", & morphIter );
  parItem.infos ["info"]     = "Iterations for morphing";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

}               /*-----  fillFilterPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fillToolsPar  of  AppConfig
 |
 |  Description :
 |
 |   This function fills the parameter  container  list  with  the  usage
 |   parameters for  sharpen,  crop,  rotate,  merge,  add  margins  etc.
 |   Normally it would be enough to just load and save  this  parameters,
 |   but then they could not be controlled easily by the job manager.
 |
 +-------------------------------------------------------------------------*/

void AppConfig::fillToolsPar ()
{
  ParItem  parItem;
  QString  info;
  QString  tooltip;

  parItem.section = section;

  tooltip = "Radius for filtering picker color (black or white point)";
  parItem.create ( "pickRadius", & pickRadius );
  parItem.infos ["info"]     = "Radius for color picker";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Sharpen image with gaussian method";
  parItem.create ( "useSharpen", & useSharpen );
  parItem.infos ["info"]     = "Sharpen image";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Erode or dilate image";
  parItem.create ( "useMorph", & useMorph );
  parItem.infos ["info"]     = "Erode or dilate";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Blur image with simple median or gaussian filter";
  parItem.create ( "useBlur", & useBlur );
  parItem.infos ["info"]     = "Blur filtering";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Levels and curves to increase contrast";
  parItem.create ( "useCurves", & useCurves );
  parItem.infos ["info"]     = "Levels and Curves";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Rotate image with defined values or automatic detection";
  parItem.create ( "useRotate", & useRotate );
  parItem.infos ["info"]     = "Rotate image";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Crop image with defined values or automatic detection";
  parItem.create ( "useCropImg", & useCropImg );
  parItem.infos ["info"]     = "Crop image";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Add margins to the image with defined values";
  parItem.create ( "addMargins", & addMargins );
  parItem.infos ["info"]     = "Add margins";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

}               /*-----  fillToolsPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fillRegionsPar  of  AppConfig
 |
 |  Description :
 |
 |   This function fills the parameter container list with the  parameter
 |   for text regions (currently not used).
 |
 +-------------------------------------------------------------------------*/

void AppConfig::fillRegionsPar ()
{
  ParItem  parItem;
  QString  info;
  QString  tooltip;

  parItem.section = section;

  tooltip = "Percentage of the lowest angles to ignore";
  parItem.create ( "anglePercLow", & anglePercLow );
  parItem.infos ["info"]     = "Low perc. of angles";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Percentage of the highest angles to ignore";
  parItem.create ( "anglePercHigh", & anglePercHigh );
  parItem.infos ["info"]     = "High perc. of angles";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Minimal box width for region detection";
  parItem.create ( "minBoxWidth", & minBoxWidth );
  parItem.infos ["info"]     = "Min. rectangle width";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Minimal box height for region detection";
  parItem.create ( "minBoxHeight", & minBoxHeight );
  parItem.infos ["info"]     = "Min. rectangle height";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

  tooltip = "Proportion of a box --> width / height";
  parItem.create ( "boxProportion", & boxProportion );
  parItem.infos ["info"]     = "Min. rectangle proportion";
  parItem.infos ["tooltip"]  = tooltip;
  parList.append (parItem);

}               /*-----  fillRegionsPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fillParList  of  AppConfig
 |
 |  Description :
 |
 |   This function fills the parameter container list with all parameter.
 |
 +-------------------------------------------------------------------------*/

void AppConfig::fillParList ()
{
  ParItem  parItem;
  QString  info;
  QString  tooltip;

  parList.clear ();

  ParBase::fillParList ();

  parItem.section = section;

  // parameter run tools
  fillToolsPar ();

  // parameter for filter and sharpen
  fillFilterPar ();

  // parameter for rotation, crop and add margins
  fillCropPar ();

  // parameter for text regions
  // fillRegionsPar ();

  maxTextWidth = 40;

  ParBase::updateList ();

}               /*-----  fillParList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  load  of  AppConfig
 |
 |  Description :
 |
 |   This function loads the global options which are not  under  control
 |   from parList from the parameter file.
 |
 +-------------------------------------------------------------------------*/

void AppConfig::load ()
{

  parFile->setSection  (c_ParMain);

  ParBase::load ();

  parList.load ();

  parFile->findPar ( "autoDetect",   autoDetect   );
  parFile->findPar ( "runToolChain", runToolChain );
  parFile->findPar ( "zoomImage",    zoomImage    );

}               /*-----  load  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  save  of  AppConfig
 |
 |  Description :
 |
 |   This function saves the global options to the parameter file.
 |
 +-------------------------------------------------------------------------*/

void AppConfig::save ()
{

  parFile->setSection  (c_ParMain);

  ParBase::save ();

  parList.save ();

  parFile->writePar ( "autoDetect",   autoDetect   );
  parFile->writePar ( "runToolChain", runToolChain );
  parFile->writePar ( "zoomImage",    zoomImage    );

}               /*-----  save  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setDefaults  of  AppConfig
 |
 |  Description :
 |
 |   This function sets all parameters to the default value.
 |
 +-------------------------------------------------------------------------*/

void AppConfig::setDefaults ()
{

  ParBase::setDefaults ();

  useSharpen    = false;
  useMorph      = false;
  useBlur       = false;
  useCurves     = false;
  useDeskew     = false;
  useRotate     = false;
  useCropImg    = false;
  addMargins    = false;

  useAutoRot    = false;
  useAutoCrop   = false;

  runToolChain  = false;

  pickRadius    = 3;
  blurKernel    = 5;
  rotation      = 0;

  // parameter for filtering
  filterTypes.clear ();
  filterTypes.append (cSimpleBlur);
  filterTypes.append (cMedianBlur);
  filterTypes.append (cGaussFilter);

  filterType = cGaussFilter;

  // parameter for merging
  mergeTypes.clear ();
  mergeTypes.append (cMergeScaled);
  mergeTypes.append (cMergeLeft);
  mergeTypes.append (cMergeCenter);
  mergeTypes.append (cMergeRight);

  mergeAlign = cMergeScaled;
  mergeImage = "";
  mergeGap   = 0;

  mergeVertical = true;

  // parameter for sharpen
  sharpTypes.clear ();
  sharpTypes.append (cSharpGauss);
  sharpTypes.append (cUnsharpMask);
  sharpTypes.append (cSharpHighpass);
  sharpTypes.append (cSharpLaplace);

  sharpType = cSharpGauss;

  laplaceKernel = 3;       // kernel size for sharpen laplace filter

  gaussKernel   = 3;       // gaussian blur kernel size
  gaussSigma    = 0.0;     // sigma value for the gaussian in unsharp mask
  unsharpAmount = 1.0;     // percentage of the magnitude of each overshoot
  unsharpThresh = 5.0;     // minimal brightness change that be sharpened

  alphaWeight   =  1.5;    // defaults for Gaussian sharpening
  betaWeight    = -0.5;
  gammaAdd      =  0.0;

  // parameter for morphing
  morphType     = "Erode";
  morphSize     = 3;
  morphIter     = 1;

  morphTypes.clear ();
  morphTypes.append ( "Erode" );
  morphTypes.append ( "Dilate" );
  morphTypes.append ( "Open" );
  morphTypes.append ( "Close" );

  anglePercLow  = 0.2;
  anglePercHigh = 0.7;
  boxProportion = 2.5;
  numDilations  = 5;
  minBoxWidth   = 40;
  minBoxHeight  = 25;

  cropMargins   = "0,0,0,0";
  pageMargins   = "0,0,0,0";
  zoomImage     = "Fit Page";

}               /*-----  setDefaults  -----*/
