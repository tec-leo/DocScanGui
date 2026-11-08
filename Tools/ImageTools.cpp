/*-------------------------------------------------------------------------
 |
 |  File Name   :  ImageTools.cpp
 |
 |  Description :
 |
 |   This module contains the class 'ImageTools' which implements several
 |   OpenCV functions  like finding  regions, contours,  deskewing images
 |   etc.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 24.Apr.2025
 |  Modified  : Oskar Leirich                Last changes  : 28.Oct.2026
 |
 |
 |  This unit contains following member functions :
 |   of ImageTools :
 |    ImageTools                addMargins                compareAngle
 |    cropImage                 cropToContent             deskewAndCrop
 |    detectAngle               detectContour             findRegions
 |    getBoundingBox            getBoundingRect           getDocPoints
 |    getMarginsBox             getSubRect                mergeImages
 |    rectToGraph               rotateImage
 |
 +-------------------------------------------------------------------------*/

#include "ImageTools.h"
#include "../Config/ParManager.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  ImageTools
 |
 |  Description :
 |
 |   The constructor  initializes some variables  like the lists  for the
 |   image formats.
 |
 +-------------------------------------------------------------------------*/

ImageTools::ImageTools ()
{

  config = nullptr;
  black  = cv::Scalar (   0,   0,   0 );
  white  = cv::Scalar ( 255, 255, 255 );

}               /*-----  ImageTools  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  addMargins  of  ImageTools
 |
 |  Description :
 |
 |   This function adds page margins to the given image with white color.
 |   The page  margins are interpreted  as percentage of the  input image
 |   size not  pixels.  The page  padding is done  with 'copyMakeBorder',
 |   the  border  type   is  set  to  isolated,  which   means  that  the
 |   extrapolation for the padding is  using only pixels inside the input
 |   image (no ROI - region of interest).
 |
 |  Input parameters :
 |   input           : Input image
 |
 |  Output parameter :
 |   (Mat)           : Image with margins
 |
 +-------------------------------------------------------------------------*/

Mat ImageTools::addMargins ( Mat input )
{
  L_Int  margins;
  Scalar color;

  Mat   padded;
  int   width;
  int   height;

  margins = stringUtils.getIntList ( config->pageMargins );

  width  = input.cols;
  height = input.rows;

  margins [0] = margins [0] * height / 100;
  margins [1] = margins [1] * height / 100;
  margins [2] = margins [2] * width  / 100;
  margins [3] = margins [3] * width  / 100;

  if ( input.channels () == 1 )
    color = 255;
  else
    color = white;

  // margins order is top, bottom, left, right
  copyMakeBorder ( input, padded,
                   margins [0], margins [1], margins [2], margins [3],
                   BORDER_ISOLATED, color );

  return padded;

}               /*-----  addMargins  -----*/


/*-------------------------------------------------------------------------
 |
 |  Memberfunction :  compareAngle  of  ImageTools
 |
 |  Description :
 |
 |   This function  is a comparison  function which is used  for sorting.
 |   It returns  true when  the first  double value  is smaller  than the
 |   second value.
 |
 +-------------------------------------------------------------------------*/

bool ImageTools::compareAngle ( double first, double second )
{

  if ( first < second )
    return true;
  else
    return false;

}               /*-----  compareAngle  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  cropImage  of  ImageTools
 |
 |  Description :
 |
 |   This function  crops the  given image to  the given  rectangle.  The
 |   crop values  are interpreted as  percentage of the input  image size
 |   not pixels.
 |   Crop margins have the following order : top, bottom, left, right
 |
 |  Input parameters :
 |   input           : Input image
 |
 |  Output parameter :
 |   (Mat)           : Croped image
 |
 +-------------------------------------------------------------------------*/

Mat ImageTools::cropImage ( Mat input, bool detect )
{
  L_Float margins;
  QRectF  rect;
  Rect2f  rect2f;
  Mat     cropped;
  int     width;
  int     height;

  if (detect)
  {
    cropped = cropToContent (input);

    return cropped;
  }

  margins = stringUtils.getFloatList ( config->cropMargins );

  width  = input.cols;
  height = input.rows;

  // crop order is top, bottom, left, right
  rect.setTop    ( (float)          margins [0] * height / 100 );
  rect.setBottom ( (float) height - margins [1] * height / 100 );
  rect.setLeft   ( (float)          margins [2] * width  / 100 );
  rect.setRight  ( (float) width  - margins [3] * width  / 100 );

  rect2f.x = rect.left ();
  rect2f.y = rect.top ();

  rect2f.width  = rect.width ();
  rect2f.height = rect.height ();

  cropped = getSubRect ( input, rect2f );

  return cropped;

}               /*-----  cropImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  cropToContent  of  ImageTools
 |
 |  Description :
 |
 |   This function  finds the  bounding box around  the text  content and
 |   crops the image to it.
 |
 |  Input parameters :
 |   input           : input image
 |
 |  Output parameter :
 |   (Mat)           : Croped image
 |
 +-------------------------------------------------------------------------*/

Mat ImageTools::cropToContent ( Mat input )
{
  Mat rotated;

  getBoundingBox ( input, box );

  // retrieves a pixel rectangle from an image with sub-pixel accuracy
  getRectSubPix ( input, box.size, box.center, rotated );

  return rotated;

}               /*-----  cropToContent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Memberfunction :  detectAngle  of  ImageTools
 |
 |  Description :
 |
 |   This function searches the bounding  box around the text content and
 |   returns  the 'angle'  of the  box. This  angle can  be used  for the
 |   rotation of  an image.  The  angle is not  taken from the  angle box
 |   function but from the top left and  top right point of the box. This
 |   avoids handling of the orientation of the box.
 |
 +-------------------------------------------------------------------------*/

float ImageTools::detectAngle ( Mat input )
{
  float angle;

  angle = 0.0;

  getBoundingBox ( input, box );

  angle = box.angle;

  if ( fabs (box.angle) > 90.0 )
  {
    angle = remainder ( box.angle, 90.0 );
    angle = angle * 90.0;
  }

  if ( box.angle > 45.0 )
    angle = box.angle - 90.0;

  if ( box.angle < -45.0 )
    angle = 90.0 - box.angle;

  return angle;

}               /*-----  detectAngle  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  detectContour  of  ImageTools
 |
 |  Description :
 |
 |   This  function uses  'findContours' to  detect the  contours on  the
 |   image.   First  it   finds  edges  in  an  image   using  the  Canny
 |   algorithm. Then runs 'findContours' and searches for contours with 4
 |   points.  Then it  stores  the contour  with  largest contour  length
 |   (contour perimeter).
 |
 |  Input parameters :
 |   input           : Input image
 |   points          : Reference to a vector of cv points
 |
 +-------------------------------------------------------------------------*/

void ImageTools::detectContour ( Mat input, V_CvPoint & points )
{
  VV_CvPoint contours;
  V_CvPoint  approx;
  V_CvPoint  contour;
  Mat        blurred;
  Mat        gray;
  Mat        edged;
  double     arcLen;
  double     curArc;
  qint       pntIdx;

  points.clear ();

  // convert to 8 bit gray scale and blur to smooth
  gray = imageMat.convToGray (input);

  cv::GaussianBlur ( gray, blurred, cv::Size ( 0, 0 ), 3, 4, cv::BORDER_DEFAULT );

  // use only lower and upper threshold, apertureSize defaults to 3
  cv::Canny ( blurred, edged, 0, 50, 3 );

  // find all image contours and store in a list
  findContours ( edged, contours, RETR_LIST, CHAIN_APPROX_SIMPLE );

  arcLen = 0;
  pntIdx = -1;

  // for each contour
  for ( qint i = 0; i < contours.size (); i++ )
  {
    contour = contours [i];

    if ( contour.size () < 4 )
      continue;    // skip contours with less than 4 points

    // contour length (perimeter)
    curArc = cv::arcLength ( contour, true);

    // Approximate a closed polygonal curve with precision of 0.02 * arcLen;
    cv::approxPolyDP ( contour, approx, 0.02 * curArc, true );

    if ( ( curArc > arcLen ) and ( approx.size () >= 4 ) )
    {
      arcLen = curArc;
      pntIdx = i;
    }
  }

  if ( pntIdx >= 0 )
  {
    points = approx;

    cvtColor ( edged, blurred, COLOR_GRAY2BGR );

    for ( qint i = 0; i < points.size (); i++ )
    {
      cv::circle ( blurred, points [i], 5, cv::Scalar ( 0, 0, 255 ), 2, LINE_AA );
    }
  }

}               /*-----  detectContour  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  deskewAndCrop  of  ImageTools
 |
 |  Description :
 |
 |   This function rotates the image by the angle of the rotated rect and
 |   makes a border around the rotated box.
 |
 |   For this  it calls  'copyMakeBorder' which  copies the  source image
 |   into the middle of the destination image.  The areas to the left, to
 |   the right,  above and below the  copied source image will  be filled
 |   with extrapolated pixels. This is not what filtering functions based
 |   on it do (they extrapolate pixels on-fly), but at other more complex
 |   functions, including  your own,  may do  to simplify  image boundary
 |   handling.
 |
 |  Input parameters :
 |   input           : Binary input image used for deskewing
 |   box             : Rotated rectangle which defines angle and position
 |
 |  Output parameter :
 |   (Mat)           : Rotated and cropped box
 |
 +-------------------------------------------------------------------------*/

Mat ImageTools::deskewAndCrop ( Mat input, const RotatedRect & box )
{
  Size2f size;
  Mat    cropped;
  Mat    transform;
  Mat    rotated;
  double angle;

  angle = box.angle;
  size  = box.size;

  // adjust the box angle
  if ( angle < -45.0 )
  {
    angle = angle + 90.0;
    std::swap ( size.width, size.height );
  }

  // rotate the text according to the angle
  transform = getRotationMatrix2D ( box.center, angle, 1.0 );
  warpAffine ( input, rotated, transform, input.size (), INTER_CUBIC );

  // retrieves a pixel rectangle from an image with sub-pixel accuracy
  getRectSubPix ( rotated, size, box.center, cropped );

  // form a border around an image.
  copyMakeBorder ( cropped, cropped, 10, 10, 10, 10,
                   BORDER_CONSTANT, Scalar (0) );

  return cropped;

}               /*-----  deskewAndCrop  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findRegions  of  ImageTools
 |
 |  Description :
 |
 |   This  function uses  'findContours' to  detect the  connected pixels
 |   regions  (extremal  regions).  For  each contour  (region)  it  uses
 |   'minAreaRect' to find the rotated rectangle (possibly rotated) for a
 |   identified point set.  The returned RotatedRect can contain negative
 |   indices when data is close to the containing Mat element boundary.
 |
 |   Small regions with width or height below a minimum value like 20 are
 |   ignored, also a width / height ratio below a minimum value like 2.
 |
 |  Input parameters :
 |   input           : Matrix to evaluate for reagions
 |   areas           : Reference to a list with all rotated rectangles
 |
 +-------------------------------------------------------------------------*/

void ImageTools::findRegions ( Mat input, L_RotatedRect & areas )
{
  VV_CvPoint contours;
  Mat        kernel;
  Mat        dilated;
  double     proportion;

  // dilate the image
  kernel = getStructuringElement ( MORPH_CROSS, Size ( 7, 3 ) );
  dilate ( input, dilated, kernel, cv::Point ( -1, -1 ), config->numDilations );

  // find all image contours
  findContours ( dilated, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE );

  // for each contour
  for ( const V_CvPoint & contour : contours )
  {
    // find it's rotated rect
    box = minAreaRect (contour);

    // discard very small boxes
    if ( ( box.size.width  < config->minBoxWidth  ) or
         ( box.size.height < config->minBoxHeight ) )
      continue;

    // discard squares shaped boxes and boxes higher than larger
    if ( box.angle < -45.0 )
      proportion = box.size.height / box.size.width;
    else
      proportion = box.size.width / box.size.height;

    if ( proportion < config->boxProportion )
      continue;

    // add the box
    areas.append (box);
  }

}               /*-----  findRegions  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getMarginsBox  of  ImageTools
 |
 |  Description :
 |
 |   This function checks  if at the margins of the  given image are some
 |   distortions or  artefacts from a  wrong scan frame.  The  image must
 |   already be binarized with 8 bit  and inverted, this means white text
 |   (content) on black.
 |   Then is reduces the margins of the image ( but only to 10 % ) untils
 |   no artefacts at the  margins can be found. If it  has no success the
 |   full image size is returned in the rect.
 |
 +-------------------------------------------------------------------------*/

cv::Rect ImageTools::getMarginsBox ( Mat input )
{
  cv::Rect rectPrev;
  cv::Rect rectTest;
  int64    pntsPrev;
  int64    pntsOrg;
  int64    pntsTest;
  int64    pntsDif;
  int      width;
  int      height;
  int      wDif; // width difference to previous rect
  int      hDif; // height difference to previous rect

  width  = input.cols;
  height = input.rows;

  wDif = int ( 0.005 * width );     // resize image in 0.5 % steps
  hDif = int ( 0.005 * height );

  rectPrev.x = 0;
  rectPrev.y = 0;
  rectPrev.width  = width;
  rectPrev.height = height;

  pntsOrg = imageMat.getWhitePoints (input).size ();

  if ( pntsOrg == 0 )    // completely white page
  {
    rectTest.x = rectTest.x + wDif;
    rectTest.y = rectTest.y + hDif;

    return rectTest;
  }

  pntsPrev = pntsOrg;
  rectTest = rectPrev;

  for ( int i = 0; i < 20; i++ )   // 10 trials until 10 %
  {
    // move towards center
    rectTest.x = rectTest.x + wDif;
    rectTest.y = rectTest.y + hDif;

    // resize to smaller rect
    rectTest.width  = rectTest.width  - 2 * wDif;
    rectTest.height = rectTest.height - 2 * hDif;

    pntsTest = imageMat.getWhitePoints ( input, rectTest ).size ();
    pntsDif  = pntsPrev - pntsTest;    // white points in frame

    if ( ( 1.0 * pntsDif / pntsOrg ) > 0.001)
      break;

    pntsPrev = pntsTest;
    rectPrev = rectTest;
  }

  return rectPrev;

}               /*-----  getMarginsBox  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getBoundingBox  of  ImageTools
 |
 |  Description :
 |
 |   This function tries to detect the angle (rotations) of the text on a
 |   image (scanned  text document).  It assumes  the minAreaRect  has an
 |   angle between 0 and 45 degrees.
 |
 +-------------------------------------------------------------------------*/

void ImageTools::getBoundingBox ( Mat input, RotatedRect & box )
{
  V_CvPoint points;
  cv::Rect  rect;
  Mat       kernel;
  Mat       binImg;
  Mat       gray;
  Mat       morph;
  Point2f   center;
  Size2f    size;

  center.x = input.cols / 2.0;
  center.y = input.rows / 2.0;

  gray = imageMat.convToGray (input);

  // Morph - Erode, thicker font
  kernel = getStructuringElement ( MORPH_RECT, Size ( 3, 3 ),
                                   cv::Point ( -1, -1 ) );
  erode ( gray, morph, kernel, cv::Point ( -1, -1 ), 3 );

  binImg = imageMat.binarize (morph);
  gray   = imageMat.invert (binImg);

  points = imageMat.getWhitePoints (gray );
  box    = minAreaRect (points);

  size = box.size;

  // adjust the box angle
  if ( ( ( size.width > size.height ) and ( input.cols < input.rows  ) ) or
       ( ( size.width < size.height ) and ( input.cols > input.rows  ) ) )
  {
    std::swap ( box.size.width, box.size.height );
    box.center = center;

    if ( box.angle < 0 )
      box.angle = 90 + box.angle;
    else
      box.angle = 90 - box.angle;
  }

}               /*-----  getBoundingBox  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getBoundingBox  of  ImageTools
 |
 |  Description :
 |
 |   This function just returns the bounding box (rotated rectangle) from
 |   the last crop or deskew action.
 |
 +-------------------------------------------------------------------------*/

void ImageTools::getBoundingBox ( RotatedRect & rotRect )
{

  rotRect = box;

}               /*-----  getBoundingBox  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getBoundingRect  of  ImageTools
 |
 |  Description :
 |
 |   This function detects the bounding rectangle around the black points
 |   on an image  (scanned text document). The rectangle  is not rotated,
 |   this means the angle is always 0.
 |
 +-------------------------------------------------------------------------*/

void ImageTools::getBoundingRect ( Mat input, cv::Rect & box )
{
  Mat kernel;
  Mat binImg;
  Mat gray;
  Mat morph;

  gray = imageMat.convToGray (input);

  // Morph - Erode, thicker font
  kernel = getStructuringElement ( MORPH_RECT, Size ( 3, 3 ),
                                   cv::Point ( -1, -1 ) );
  erode ( gray, morph, kernel, cv::Point ( -1, -1 ), 3 );

  binImg = imageMat.binarize (morph);
  gray   = imageMat.invert (binImg);
  box    = getMarginsBox (gray);

}               /*-----  getBoundingRect  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getContourRect  of  ImageTools
 |
 |  Description :
 |
 |   This function  uses 'getContourPts'  to detect  the contours  on the
 |   image.  Then it calculates the  minimum area enclosing the points of
 |   all contours and returns the bounding rect about this.
 |
 |  Input parameters :
 |   input           : Input image
 |
 +-------------------------------------------------------------------------*/

cv::Rect ImageTools::getContourRect ( Mat input )
{
  RotatedRect box;
  V_CvPoint   points;
  cv::Rect    rect;

  getContourPts ( input, points );

  box = minAreaRect (points);
  rect = box.boundingRect ();

  return rect;

}               /*-----  getContourRect  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getContourPts  of  ImageTools
 |
 |  Description :
 |
 |   This  function uses  'findContours' to  detect the  contours on  the
 |   image.   First  it   finds  edges  in  an  image   using  the  Canny
 |   algorithm. Then runs  'findContours' and combines the  points of all
 |   contours.
 |
 |  Input parameters :
 |   input           : Input image
 |   points          : Reference to a vector of cv points
 |
 +-------------------------------------------------------------------------*/

void ImageTools::getContourPts ( Mat input, V_CvPoint & points )
{
  VV_CvPoint contours;
  V_CvPoint  approx;
  V_CvPoint  contour;
  Mat        blurred;
  Mat        gray;
  Mat        edged;
  double     arcLen;

  points.clear ();

  // convert to 8 bit gray scale and blur to smooth
  gray = imageMat.convToGray (input);

  cv::GaussianBlur ( gray, blurred, cv::Size ( 0, 0 ), 3, 4, cv::BORDER_DEFAULT );

  // use only lower and upper threshold, apertureSize defaults to 3
  cv::Canny ( blurred, edged, 0, 50, 3 );

  // find all image contours and store in a list
  findContours ( edged, contours, RETR_LIST, CHAIN_APPROX_SIMPLE );

  arcLen = 0;

  // for each contour
  for ( qint i = 0; i < contours.size (); i++ )
  {
    contour = contours [i];

    if ( contour.size () < 4 )
      continue;    // skip contours with less than 4 points

    // contour length (perimeter)
    arcLen = cv::arcLength ( contour, true);

    // Approximate a closed polygonal curve with precision of 0.02 * arcLen;
    cv::approxPolyDP ( contour, approx, 0.02 * arcLen, true );

    if ( ( approx.size () >= 4 ) and ( arcLen > 20 ) )
    {
      for ( qint k = 0; k < approx.size (); k++ )
      {
        points.push_back ( approx [k] );
      }
    }
  }

}               /*-----  getContourPts  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getDocPoints  of  ImageTools
 |
 |  Description :
 |
 |   This function assumes  the given points are the edges  of a sheet of
 |   paper on a darker background. If there are more points then the four
 |   edges only the  closest points to a default position  are taken. All
 |   points are sorted to the following order:
 |    top left, top right, bottom left, bottom right
 |
 +-------------------------------------------------------------------------*/

V_CvPoint ImageTools::getDocPoints ( int rows, int cols, V_CvPoint & points )
{
  V_CvPoint posPnts;
  cv::Point pnt;
  float     distance;
  float     shortest;
  float     minMargin;
  float     maxMargin;

  posPnts.resize (4);

  minMargin = 0.05f;
  maxMargin = 0.95f;

  // 1. find top left
  pnt.x = qRound ( minMargin * cols );
  pnt.y = qRound ( minMargin * rows );

  shortest = POS_VAL;   // just a high number

  for ( qint i = 0; i < points.size (); i++ )
  {
    distance = getDistance ( pnt, points [i] );

    if ( distance < shortest )
    {
      posPnts [0] = points [i];
      shortest = distance;
    }
  }

  // 2. find top right
  pnt.x = qRound ( maxMargin * cols );
  pnt.y = qRound ( minMargin * rows );

  shortest = POS_VAL;   // just a high number

  for ( qint i = 0; i < points.size (); i++ )
  {
    distance = getDistance ( pnt, points [i] );

    if ( distance < shortest )
    {
      posPnts [1] = points [i];
      shortest = distance;
    }
  }

  // 3. find bottom left
  pnt.x = qRound ( minMargin * cols );
  pnt.y = qRound ( maxMargin * rows );

  shortest = POS_VAL;   // just a high number

  for ( qint i = 0; i < points.size (); i++ )
  {
    distance = getDistance ( pnt, points [i] );

    if ( distance < shortest )
    {
      posPnts [2] = points [i];
      shortest = distance;
    }
  }

  // 3. find bottom right
  pnt.x = qRound ( maxMargin * cols );
  pnt.y = qRound ( maxMargin * rows );

  shortest = POS_VAL;   // just a high number

  for ( qint i = 0; i < points.size (); i++ )
  {
    distance = getDistance ( pnt, points [i] );

    if ( distance < shortest )
    {
      posPnts [3] = points [i];
      shortest = distance;
    }
  }

  return posPnts;

}               /*-----  getDocPoints  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getDistance  of  ImageTools
 |
 |  Description :
 |
 |   This function returns the distance between the two given points.
 |
 +-------------------------------------------------------------------------*/

float ImageTools::getDistance ( cv::Point p1, cv::Point p2 )
{
  float distance;
  int   difx;
  int   dify;

  difx = p1.x - p2.x;
  dify = p1.y - p2.y;

  distance = std::sqrt ( ( difx * difx ) + ( dify * dify ) );

  return distance;

}               /*-----  getDistance  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getSubRect  of  ImageTools
 |
 |  Description :
 |
 |   This function  returns the sub  image from  the input image  and the
 |   given rectangle.  It seems that  getRectSubPix works only with 8 bit
 |   data or  floating point. With  16 bit  unsigned it crashes,  in this
 |   case it must be converted to 8 bit before.
 |
 +-------------------------------------------------------------------------*/

Mat ImageTools::getSubRect ( Mat input, cv::Rect2f rect )
{
  Point2f center;
  Mat     cropped;

  center.x = rect.x + rect.width / 2.0;
  center.y = rect.y + rect.height / 2.0;

  cv::getRectSubPix ( input, rect.size (), center, cropped );

  return cropped;

}               /*-----  getSubRect  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getSubRect  of  ImageTools
 |
 |  Description :
 |
 |   This function calculates the rotated  rectangle from the given image
 |   and rectangle and returns sub image.
 |
 +-------------------------------------------------------------------------*/

Mat ImageTools::getSubRect ( Mat input, cv::Rect2f rect, RotatedRect & box )
{
  Size2f size;
  Mat    cropped;

  cropped = getSubRect ( input, rect );

  getBoundingBox ( cropped, box );

  return cropped;

}               /*-----  getSubRect  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  mergeImages  of  ImageTools
 |
 |  Description :
 |
 |   This function  merges  (concatenates)  two  images  horizontally  or
 |   vertically. The 2nd image can be align left (top) or right  (bottom)
 |   or resized to the same rows or columns as the first image.
 |
 |  Input parameters :
 |   img1            : First image
 |   img2            : Second image
 |
 |  Output parameter :
 |   (Mat)           : Merged image
 |
 +-------------------------------------------------------------------------*/

Mat ImageTools::mergeImages ( Mat img1, Mat img2 )
{
  QString  align;   //  alignment for merging, left, right, center, scale
  QString  msg;
  cv::Size size;
  cv::Rect rect;
  Mat      scaled;
  Mat      merged;
  int      w1, w2;
  int      h1, h2;
  float    scale;
  bool     vertical;  // merge vertical (true) or horizontal
  int      gapPixel;

  align    = config->mergeAlign;
  vertical = config->mergeVertical;
  gapPixel = config->mergeGap;

  // check number of channels
  if ( img1.channels () != img2.channels () )
  {
    msg = "Number of channels is different, cannot merge !!";
    QMessageBox::warning ( 0, "Channels different", msg );
    return merged;
  }

  w1 = img1.cols;
  h1 = img1.rows;

  w2 = img2.cols;
  h2 = img2.rows;

  if ( (vertical) and ( w1 == w2 ) )
  {
    cv::vconcat ( img1, img2, merged ); // easy merge for vertical
    return merged;
  }

  if ( ( not vertical ) and ( h1 == h2 ) )
  {
    cv::hconcat ( img1, img2, merged ); // easy merge for horizontal
    return merged;
  }

  if (vertical)   // merge vertically
  {
    if ( gapPixel > 0 )
    {
      copyMakeBorder ( img1, img1, 0, gapPixel, 0, 0, BORDER_ISOLATED, white );
      h1 = img1.rows;
    }

    if ( ( align == cMergeScaled ) or ( w2 > w1 ) )  // scale img2
    {
      scale = (float) w1 / w2;

      size.width  = w1;
      size.height = h2 * qRound (scale);

      cv::resize ( img2, scaled, size, 0, 0, cv::INTER_AREA );

      cv::vconcat ( img1, scaled, merged ); // easy merge for vertical
    }
    else
    {
      // copy image1 with border, order is top, bottom, left, right
      copyMakeBorder ( img1, merged, 0, h2, 0, 0, BORDER_ISOLATED, white );

      rect.height = h2;
      rect.width  = w2;
      rect.y      = h1;

      if ( align == cMergeLeft )
        rect.x = 0;

      if ( align == cMergeRight )
        rect.x = w1 - w2;

      if ( align == cMergeCenter )
        rect.x = ( w1 - w2 ) / 2;

      imageMat.copyTo ( img2, merged, rect );
    }
  }
  else    // merge horizontally
  {
    if ( gapPixel > 0 )
    {
      copyMakeBorder ( img1, img1, 0, 0, 0, gapPixel, BORDER_ISOLATED, white );
      w1 = img1.cols;
    }

    if ( ( align == cMergeScaled ) or ( h2 > h1 ) )  // scale img2
    {
      scale = (float) h1 / h2;

      size.height = h1;
      size.width  = w2 * qRound (scale);

      cv::resize ( img2, scaled, size, 0, 0, cv::INTER_AREA );

      cv::hconcat ( img1, scaled, merged ); // easy merge for vertical
    }
    else
    {
      // copy image1 with border, order is top, bottom, left, right
      copyMakeBorder ( img1, merged, 0, 0, 0, w2, BORDER_ISOLATED, white );

      rect.height = h2;
      rect.width  = w2;
      rect.x      = w1;

      if ( align == cMergeLeft )
        rect.y = 0;

      if ( align == cMergeRight )
        rect.y = h1 - h2;

      if ( align == cMergeCenter )
        rect.y = ( h1 - h2 ) / 2;

      imageMat.copyTo ( img2, merged, rect );
    }
  }

  return merged;

}               /*-----  mergeImages  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  rectToGraph  of  ImageTools
 |
 |  Description :
 |
 |   This  function converts  the open  cv rotated  rectangle regions  to
 |   graphic rect  items collected  in a item  group. Every  rectangle is
 |   stored in an own rect item.
 |
 +-------------------------------------------------------------------------*/

void ImageTools::rectToGraph ( QGraphicsItemGroup * rootItem )
{
  QGraphicsItemGroup * itemGroup;
  QGraphicsRectItem  * rectItem;
  cv::Rect2f           bRect;
  QString              text;
  QPen                 pen;

  pen.setColor  (Color::red);
  pen.setWidthF (5);

  itemGroup = new QGraphicsItemGroup (rootItem);

  for ( int i = 0; i < regions.size (); i++ )
  {
    rectItem = new QGraphicsRectItem (itemGroup);
    rectItem->setPen (pen);

    text = "Regions Item " + QString::number ( i + 1 );
    rectItem->setToolTip (text);

    box   = regions [i];
    bRect = box.boundingRect2f ();

    rectItem->setRect ( bRect.x, bRect.y, bRect.width, bRect.height );
  }

}               /*-----  rectToGraph  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  rotateImage  of  ImageTools
 |
 |  Description :
 |
 |   This function rotates the image by  multiplies of 90 degrees so that
 |   the remaining angle is between +/- 45 degrees. This allows an easier
 |   padding and cropping of image. The given angle assumes that positive
 |   values mean a clockwise rotation.
 |
 |  Input parameters :
 |   input           : Input image for rotating
 |   angle           : Reference to the input and remaining angle
 |
 |  Output parameter :
 |   (Mat)           : Rotated image
 |
 +-------------------------------------------------------------------------*/

Mat ImageTools::rotateImage ( Mat input, float & angle )
{
  int quadrant;
  Mat rotated;

  if ( angle < 0 )
    angle = 360.0 + angle;

  quadrant = (int) angle / 90.0;

  switch (quadrant)
  {
    case 0:
      cv::rotate ( input, rotated, ROTATE_90_COUNTERCLOCKWISE );
      angle = angle - 90.0;
    break;

    case 1:
      cv::rotate ( input, rotated, ROTATE_90_COUNTERCLOCKWISE );
      angle = angle - 90.0;
    break;

    case 2:
      cv::rotate ( input, rotated, ROTATE_180 );
      angle = angle - 180.0;
    break;

    case 3:
      //      cv::rotate ( input, rotated, ROTATE_90_COUNTERCLOCKWISE );
      cv::rotate ( input, rotated, ROTATE_90_CLOCKWISE );
      angle = angle - 270.0;
    break;
  }

  if ( angle > 45 )
  {
    cv::rotate ( rotated, rotated, ROTATE_90_CLOCKWISE );
    angle = - angle;
  }

  return rotated;

}               /*-----  rotateImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  rotateImage  of  ImageTools
 |
 |  Description :
 |
 |   This function searches  the bounding box around the  text content if
 |   'detect' is true uses the 'angle' of the box for the rotation of the
 |   image. If 'detect' is false then is uses the rotation value from the
 |   config for  the image rotation.  At the end  of the process  it adds
 |   some margin to the image to avoid a croping of text content.
 |
 |  Input parameters :
 |   input           : Input image for rotating
 |   detect          : Automatic detection of rotation
 |
 |  Output parameter :
 |   (Mat)           : Rotated image
 |
 +-------------------------------------------------------------------------*/

Mat ImageTools::rotateImage ( Mat input, bool detect )
{
  Size2f  size;
  Point2f center;
  Mat     transform;
  Mat     boxed;
  Mat     rotated;
  float   angle;
  float   absSin;
  float   absCos;
  float   radian;
  int     width;
  int     height;
  int     inputH;
  int     inputW;

  if (detect)
    angle = - detectAngle (input);
  else
    angle = config->rotation;

  if ( fabs (angle) < cAngleTolerance )  // only very small angle
    return input;

  radian = qDegreesToRadians ( fabs (angle) );

  absSin = fabs ( qSin (radian) );
  absCos = fabs ( qCos (radian) );

  inputH = input.rows;
  inputW = input.cols;

  height = qCeil ( (float) inputH * absCos + inputW * absSin );
  width  = qCeil ( (float) inputH * absSin + inputW * absCos );

  height = fabs ( height - inputH ) / 2;
  width  = fabs ( width  - inputW ) / 2;

  // margins order is top, bottom, left, right
  copyMakeBorder ( input, boxed, height, height, width, width,
                   BORDER_ISOLATED, white );

  center.x = boxed.cols / 2.0;
  center.y = boxed.rows / 2.0;

  // get transformation matrix from box center and angle
  // positive angle values mean counter-clockwise rotation
  transform = getRotationMatrix2D ( center, angle, 1.0 );

  warpAffine ( boxed, rotated, transform, boxed.size (),
               INTER_CUBIC, BORDER_CONSTANT, white );

  size.width  = boxed.cols - 2.0 * width;
  size.height = boxed.rows - 2.0 * height;

  // retrieves a pixel rectangle from an image with sub-pixel accuracy
  // getRectSubPix ( rotated, size, center, boxed );

  return rotated;

}               /*-----  rotateImage  -----*/
