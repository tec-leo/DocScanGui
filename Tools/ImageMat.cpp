/*-------------------------------------------------------------------------
 |
 |  File Name   :  ImageMat.cpp
 |
 |  Description :
 |
 |   This module contains the class 'ImageMat' which implements functions
 |   to convert  between a QImage  and a  Mat from OpenCV.  All functions
 |   assume the that the color order is RGB.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 14.Jun.2023
 |  Modified  : Oskar Leirich                Last changes  : 16.Apr.2026
 |
 |
 |  This unit contains following member functions :
 |   of ImageMat :
 |    ImageMat                  binarize                  blackOnWhite
 |    convMatImage              convToGray                copyTo
 |    evalMatInfo               getBlackRatio             getImageFormat
 |    getMatInfo                getMatInfoStr             getMatType
 |    getWhitePoints            invert
 |
 +-------------------------------------------------------------------------*/

#include "ImageMat.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  ImageMat
 |
 |  Description :
 |
 |   The constructor  initializes some variables  like the lists  for the
 |   image formats.
 |
 +-------------------------------------------------------------------------*/

ImageMat::ImageMat ()
{

  matDepthStr.append ( "8-bit unsigned int"   );
  matDepthStr.append ( "8-bit signed int"     );
  matDepthStr.append ( "16-bit unsigned int"   );
  matDepthStr.append ( "16-bit signed int"     );
  matDepthStr.append ( "32-bit signed int"     );
  matDepthStr.append ( "32-bit floating point" );
  matDepthStr.append ( "64-bit floating point" );

  matTypeStr.append ( "8U"  );
  matTypeStr.append ( "8S"  );
  matTypeStr.append ( "16U" );
  matTypeStr.append ( "16S" );
  matTypeStr.append ( "32S" );
  matTypeStr.append ( "32F" );
  matTypeStr.append ( "64F" );

  matTypeLen.append (1);   // CV_8U
  matTypeLen.append (1);   // CV_8S
  matTypeLen.append (2);   // CV_16U
  matTypeLen.append (2);   // CV_16S
  matTypeLen.append (4);   // CV_32S
  matTypeLen.append (4);   // CV_32F
  matTypeLen.append (8);   // CV_64F
  matTypeLen.append (2);   // CV_16F

}               /*-----  ImageMat  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  binarize  of  ImageMat
 |
 |  Description :
 |
 |   This  function converts  the  image  first to  gray  scale and  then
 |   applies 'threshold' to  the simple threshold function  of OpenCV. It
 |   the  given threshold  is  below 0  the it  uses  the otsu  algorithm
 |   (automatic threshold  finding) to make it  binary. At the end  it is
 |   inverted if it contains more white than black pixel.
 |
 |  Input parameters :
 |   input           : Matrix to binarize
 |
 |  Output parameter :
 |   (Mat)           : Resulting matrix, binarized and white on black
 |
 +-------------------------------------------------------------------------*/

Mat ImageMat::binarize ( Mat input, int threshold )
{
  Mat binImg;

  input = convToGray (input);

  if ( threshold < 0 )
    cv::threshold ( input, binImg, 0, 255, THRESH_OTSU );
  else
    cv::threshold ( input, binImg, threshold, 255, THRESH_BINARY );

  return binImg;

}               /*-----  binarize  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  copyTo  of  ImageMat
 |
 |  Description :
 |
 |   This  function copies  from  the source  matrix  to the  destination
 |   matrix at  the position of rect.  It uses a roi  matrix which shares
 |   the data with the source matrix.
 |
 |  Input parameters :
 |   src             : Source matrix
 |   dst             : Destination matrix
 |   rect            : Position and size in the Destination matrix
 |
 +-------------------------------------------------------------------------*/

void ImageMat::copyTo ( Mat src, Mat dst, cv::Rect rect )
{
  Mat roiMat;
  Mat mask;
  int hDif;
  int wDif;

  rect.width  = src.cols;
  rect.height = src.rows;

  // check if copy is possible, change rect position if necessary
  wDif = dst.cols - ( rect.x + rect.width );

  if ( wDif < 0 )
    rect.x = rect.x + wDif;

  hDif = dst.rows - ( rect.y + rect.height );

  if ( hDif < 0 )
    rect.y = rect.y + hDif;

  if ( ( rect.x < 0 ) or ( rect.y < 0 ) )
  {
    // change size of src and rect?
    return;
  }

  // address region of interest in destination matrix
  roiMat = cv::Mat ( dst, rect );

  mask = src.clone (); // make a copy
  mask = 1;            // and set mask to 1 - copy every pixel

  cv::copyTo ( src, roiMat, mask );

}               /*-----  copyTo  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  blackOnWhite  of  ImageMat
 |
 |  Description :
 |
 |   This  function takes a binary image inverts it if it contains more white
 |   pixels than black ones.
 |
 |  Input parameters :
 |   input           : Input matrix
 |
 |  Output parameter :
 |   (Mat)           : Resulting matrix black on white
 |
 +-------------------------------------------------------------------------*/

Mat ImageMat::blackOnWhite ( Mat input )
{
  int white;
  int black;

  // count the number of black and white pixels
  white = countNonZero (input);
  black = input.size ().area () - white;

  // if the image is mostly white (white background), invert it
  if ( white > black )
    input = ~input;

  return input;

}               /*-----  blackOnWhite  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  convTo8bit
 |
 |  Description :
 |
 |   This function converts the image to 8 bit. Some tools like rotate or
 |   applying curves would crash on 16 bit values.
 |
 |  Input parameters :
 |   input           : image to convert to gray
 |
 |  Output parameter :
 |   (Mat)           : Resulting image, 8 bit
 |
 +-------------------------------------------------------------------------*/

Mat ImageMat::convTo8bit ( Mat input )
{
  Mat  imgOut;
  Mat  gray8;
  Mat  normImg;

  normImg = Mat::zeros ( input.size (), CV_8U );

  cv::normalize ( input, normImg, 0, 255, NORM_MINMAX );

  cv::convertScaleAbs ( normImg, imgOut );

  return imgOut;

}               /*-----  convTo8bit  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  convToGray  of  ImageMat
 |
 |  Description :
 |
 |   This function converts  the image first to gray  scale, currently it
 |   handles only 8 bit conversion.
 |
 |  Input parameters :
 |   input           : image to convert to gray
 |
 |  Output parameter :
 |   (Mat)           : Resulting image, 1 channel (gray)
 |
 +-------------------------------------------------------------------------*/

Mat ImageMat::convToGray ( Mat input )
{
  Mat  grayImg;
  qint channels;

  // number of channels, typically 3 for a colour image
  channels = input.channels ();

  if ( channels == 1 )
    grayImg = input.clone ();
  else
    cvtColor ( input, grayImg, COLOR_RGB2GRAY );

  return grayImg;

}               /*-----  convToGray  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  convMatImage  of  ImageMat
 |
 |  Description :
 |
 |   This function converts the given Mat to a QImage, it does not make a
 |   deep copy so the Mat must exist  as long as the QImage is used. This
 |   means it is  necessary to keep track  of the Mat. The  format of the
 |   mat image is not changed.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |   img             : Image to a new QImage instance
 |   format          : Image format for QImage
 |
 |  Output parameter :
 |   (QImage)        :
 |
 +-------------------------------------------------------------------------*/

QImage * ImageMat::convMatImage ( Mat & mat )
{
  QImage::Format format;
  QImage * img;

  evalMatInfo (mat);

  text = getMatType (mat);

  getImageFormat ( mat, format );

  img = new QImage ( mat.data, mat.cols, mat.rows, mat.step, format );

  return img;

}               /*-----  convMatImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  convMatImage  of  ImageMat
 |
 |  Description :
 |
 |   This function converts the given Mat to a QImage, it does not make a
 |   deep copy so the Mat must exist  as long as the QImage is used. This
 |   means  it is  necessary to  keep  track of  the Mat.  The QImage  is
 |   created with the given format.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |   img             : Image to a new QImage instance
 |   format          : Image format for QImage
 |
 |  Output parameter :
 |   (QImage)        : Pointer to a QImage
 |
 +-------------------------------------------------------------------------*/

QImage * ImageMat::convMatImage ( Mat & mat, QImage::Format format )
{
  QImage * img;

  img = new QImage ( mat.data, mat.cols, mat.rows, mat.step, format );

  return img;

}               /*-----  convMatImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  evalMatInfo  of  ImageMat
 |
 |  Description :
 |
 |   This function extracts some information from the given Mat like
 |   pixel type, number of channels etc.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |
 +-------------------------------------------------------------------------*/

void ImageMat::evalMatInfo ( Mat & mat )
{
  QString typeStr;

  elemSize = mat.elemSize ();   // matrix element size in bytes
  channels = mat.channels ();   // number of channels, typically 3 for a colour image
  depth    = mat.depth ();      // pixel type, eg. CV_8U for 8bit unsigned
  pixLen   = matTypeLen [depth];

  typeStr  = matTypeStr [depth];
  dataType = typeStr.last (1);

  if ( typeStr.size () == 2 )
    dataSize = typeStr.first (1).toInt ();
  else
    dataSize = typeStr.first (2).toInt ();

  cols  = mat.cols;
  rows  = mat.rows;
  steps = mat.step1 ();

  imgSize = cols * rows * channels * pixLen;   // size in bytes

}               /*-----  evalMatInfo  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getBlackRatio  of  ImageMat
 |
 |  Description :
 |
 |   This  function counts  the  pixel which  are closer  to  black by  a
 |   defined threshold value. It assumes that  the given matrix is a gray
 |   image (one  channel) with unsigned 8  bit values and white  on black
 |   text. It  calculates the ratio of  black to white pixel  and returns
 |   it. The black ratio is the ratio of black text to white pixel to all
 |   pixels.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |
 |  Output parameter :
 |   double          : ratio of white text to black space in a text doc
 |
 +-------------------------------------------------------------------------*/

double ImageMat::getBlackRatio ( Mat & mat )
{
  double ratio;
  int    size;
  int    black;
  int    white;
  Mat    srcMat;

  ratio = 0;
  black = 0;
  size  = mat.rows * mat.cols;

  if ( mat.type () != CV_8U )
    srcMat = convToGray (mat);     // convert to grayscale
  else
    srcMat = mat.clone ();         // do not change incoming matrix

  threshold ( srcMat, srcMat, 150, 255, 0 );

  white = countNonZero (srcMat);
  black = size - white;

  ratio = 1.0 * black / size;     // for white text on black background

  return ratio;

}               /*-----  getBlackRatio  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getWhiteRatio  of  ImageMat
 |
 |  Description :
 |
 |   This  function counts  the  pixel which  are closer  to  white by  a
 |   defined threshold value. It assumes that  the given matrix is a gray
 |   image (one  channel) with unsigned 8  bit values and white  on black
 |   text. It calculates  the ratio of the white pixel  to all pixels and
 |   returns it.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |
 |  Output parameter :
 |   double          : ratio of black text to white space in a text doc
 |
 +-------------------------------------------------------------------------*/

double ImageMat::getWhiteRatio ( Mat & mat )
{
  double ratio;
  int    size;
  int    white;
  Mat    srcMat;

  ratio = 0;
  size  = mat.rows * mat.cols;

  if ( mat.type () != CV_8U )
    srcMat = convToGray (mat);     // convert to grayscale
  else
    srcMat = mat.clone ();         // do not change incoming matrix

  threshold ( srcMat, srcMat, 150, 255, 0 );

  white = countNonZero (srcMat);

  ratio = 1.0 * white / size;     // for black text on white background

  return ratio;

}               /*-----  getWhiteRatio  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getWhitePoints  of  ImageMat
 |
 |  Description :
 |
 |   This function  puts every  point with  a value greater  than 0  to a
 |   vector of  points.  The image  must already  be binarized and  8 bit
 |   unsigned  grayscaled.  'Binarized'  means  white  text (content)  on
 |   black so that the points of the content are collected.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |
 |  Output parameter :
 |   (V_CvPoint)     : Vector of points
 |
 +-------------------------------------------------------------------------*/

V_CvPoint ImageMat::getWhitePoints ( Mat & mat )
{
  V_CvPoint points;
  cv::Point point;
  uchar     val;

  for ( int row = 0; row < mat.rows; row++ )
  {
    for ( int col = 0; col < mat.cols; col++ )
    {
      val = mat.at <uchar> ( row, col );

      if ( val > 0 )
      {
        point.x = col;
        point.y = row;

        points.push_back (point);
      }
    }
  }

  return points;

}               /*-----  getWhitePoints  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getWhitePoints  of  ImageMat
 |
 |  Description :
 |
 |   This function  puts every  point with  a value greater  than 0  to a
 |   vector of  points.  The image  must already  be binarized and  8 bit
 |   unsigned  grayscaled.  'Binarized'  means  white  text (content)  on
 |   black so that the points of  the content are collected. The check is
 |   limited to the given rect.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |
 |  Output parameter :
 |   (V_CvPoint)     : Vector of points
 |
 +-------------------------------------------------------------------------*/

V_CvPoint ImageMat::getWhitePoints ( Mat & mat, cv::Rect rect )
{
  V_CvPoint points;
  cv::Point point;
  uchar     val;

  for ( int row = rect.y; row < rect.height; row++ )
  {
    for ( int col = rect.x; col < rect.width; col++ )
    {
      val = mat.at <uchar> ( row, col );

      if ( val > 0 )
      {
        point.x = col;
        point.y = row;

        points.push_back (point);
      }
    }
  }

  return points;

}               /*-----  getWhitePoints  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getMatInfo  of  ImageMat
 |
 |  Description :
 |
 |   This  function extracts  some information  from the  given Mat  like
 |   pixel type, number of channels etc. and returns it as a string list.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |
 +-------------------------------------------------------------------------*/

L_String ImageMat::getMatInfo ( Mat & mat )
{
  L_String info;
  float    size;

  evalMatInfo (mat);

  size = 1.0 * imgSize / 1000;
  info.append ( "Image size : " + QString::number (size) + " KBytes" );
  info.append ( "Rows       : " + QString::number (rows) );
  info.append ( "Columns    : " + QString::number (cols) );
  info.append ( "Channels   : " + QString::number (channels) );
  info.append ( "Depth      : " + matDepthStr [depth] );

  return info;

}               /*-----  getMatInfo  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getImageFormat  of  ImageMat
 |
 |  Description :
 |
 |   This  function tries  to translates  the image  format in  Mat to  a
 |   QImage format.  It  assumes this image is in RGB  and that the alpha
 |   channel is  already removed.  Only 8  bit gray  and color  image are
 |   implemented.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |   format          : Reference to the format of a QImage
 |
 +-------------------------------------------------------------------------*/

void ImageMat::getImageFormat ( Mat & mat, QImage::Format & format )
{

  format = QImage::Format_Invalid;  // default

  if ( mat.empty () )
  {
    return;
  }

  switch ( mat.type () )
  {
    case CV_8UC3 :
      format = QImage::Format_RGB888;     // 8 bit per channel
    break;

    case CV_8U :    // identical to CV_8UC1
      format = QImage::Format_Grayscale8; // 8 bit in one channel
    break;

    /*
    case CV_8UC4 :
      format = QImage::Format_ARGB32;
    break;

    case CV_16UC1 :
      format = QImage::Format_ARGB32;
    break;

    case CV_16UC3 :
      format = QImage::Format_ARGB32;
    break;
    */
  }

}               /*-----  getImageFormat  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getMatType  of  ImageMat
 |
 |  Description :
 |
 |   This function returns the mat type as string.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |
 +-------------------------------------------------------------------------*/

QString ImageMat::getMatType ( Mat & mat )
{
  QString typeStr;
  uint8   depth;
  uint8   channels;
  int     type;

  type = mat.type ();

  depth = type & CV_MAT_DEPTH_MASK;
  channels = 1 + ( type >> CV_CN_SHIFT );

  if ( depth < matTypeStr.count () )
    typeStr = matTypeStr [depth];
  else
    typeStr = "User";

  text.setNum (channels);

  typeStr = typeStr + "C" + text;

  return typeStr;

}               /*-----  getMatType  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  invert  of  ImageMat
 |
 |  Description :
 |
 |   This function inverts the given matrix in place, no copy is taken.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |
 +-------------------------------------------------------------------------*/

Mat ImageMat::invert ( Mat & mat )
{
  Mat matNot;

  cv::bitwise_not ( mat, matNot );

  return matNot;

}               /*-----  invert  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  removeAlpha  of  ImageMat
 |
 |  Description :
 |
 |   This function  removes the alpha channel  if it exists. This  is the
 |   case if the image has 4 channels.
 |
 |  Input parameters :
 |   mat             : Input matrix
 |
 +-------------------------------------------------------------------------*/

void ImageMat::removeAlpha ( Mat & mat )
{

  if ( mat.channels () == 4 )
    cvtColor ( mat, mat, COLOR_BGRA2BGR );

}               /*-----  removeAlpha  -----*/

