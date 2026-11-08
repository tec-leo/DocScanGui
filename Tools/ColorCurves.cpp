/*-------------------------------------------------------------------------
 |
 |  File Name   :  ColorCurves.cpp
 |
 |  Description :
 |
 |   This  module  contains  the  class  'ColorCurves'  which  implements
 |   functions to apply black and  white point convertion and color space
 |   stretching.  It can apply a color  or grayscale channel to modify an
 |   image from OpenCV.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 17.Feb.2025
 |  Modified  : Oskar Leirich                Last changes  : 10.Jun.2026
 |
 |
 |  This unit contains following member functions :
 |   of ColorCurves :
 |    ColorCurves               applyCurve                applyCurves
 |    getLuminosity             resetColors               setBlackPnt
 |    setImage                  setWhitePnt
 |
 +-------------------------------------------------------------------------*/

#include "ColorCurves.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  ColorCurves
 |
 |  Description :
 |
 |   The constructor  initializes some variables  like the lists  for the
 |   image formats.
 |
 +-------------------------------------------------------------------------*/

ColorCurves::ColorCurves ()
{

  isGray      = true;
  applyColors = false;

  colorChannels.append ( & chanRed );
  colorChannels.append ( & chanGreen );
  colorChannels.append ( & chanBlue );
  colorChannels.append ( & chanGray );   // gray at the end

}               /*-----  ColorCurves  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  applyCurves  of  ColorCurves
 |
 |  Description :
 |
 |   This function merges the color channels, calculates the gray channel
 |   from them and returns them as cv::Mat.
 |
 +-------------------------------------------------------------------------*/

cv::Mat ColorCurves::applyCurves ( cv::Mat & matIn )
{
  ColorChannel * colChannel;
  V_CvMat channels;
  cv::Mat gray;
  cv::Mat color;
  cv::Mat lut;

  cv::split ( matIn, channels );

  for ( int i = 0; i < 3; i++ )
  {
    colChannel = colorChannels [i];

    colChannel->setChannelMat ( channels [i] );

    colChannel->createLinear ();
    lut = colChannel->getLookup ();

    cv::LUT ( channels [i], lut, channels [i] );
  }

  cv::merge ( channels, color );

  cvtColor ( color, gray, COLOR_RGB2GRAY );

  chanGray.setChannelMat (gray);

  return color;

}               /*-----  applyCurves  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  applyCurve  of  ColorCurves
 |
 |  Description :
 |
 |   This function  is called from the  tools widget when some  curves or
 |   black or  white points  has changed  in the  curves dialog.   Then a
 |   lookup table is  calulated from the curves and applied  to the given
 |   image mat and stored in the output image mat.
 |
 |  Input parameters :
 |   matIn           : Reference to matrix of OpenCV
 |   matOut          : Reference to output matrix after applying LUT
 |
 +-------------------------------------------------------------------------*/

void ColorCurves::applyCurve ( cv::Mat & matIn, cv::Mat & matOut )
{
  V_CvMat channels;
  cv::Mat lut;
  cv::Mat ylum;   // luminance of ycrcb
  cv::Mat ycrcb;

  setImage (matIn);

  if (applyColors)
  {
    matOut = applyCurves (matIn);
    return;
  }

  chanGray.createSamples ();

  lut = chanGray.getLookup ();

  if (isGray)   // only gray scale
  {
    cv::LUT ( matIn, lut, matOut );
    return;
  }

  // convert RGB to luminance space YCrCb
  cv::cvtColor ( matIn, ycrcb, cv::COLOR_RGB2YCrCb );

  cv::split ( ycrcb, channels );

  cv::LUT ( channels [0], lut, ylum );   // apply your 1D LUT to luminance only
  channels [0] = ylum;

  cv::merge ( channels, ycrcb );

  // convert luminance space YCrCb to RGB
  cv::cvtColor ( ycrcb, matOut, cv::COLOR_YCrCb2RGB );

}               /*-----  applyCurve  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : getLuminosity  of  ColorCurves
 |
 |  Description :
 |
 |   This function calculates  the luminosity value from  the given color
 |   and returns  it.  The  color is  in uchar  format, therefore  it its
 |   converted to floating point value in  the range between 0 and 1. For
 |   the calculation  of the luminosity  the OpenCV formula is  used. The
 |   given color is in RGB format.
 |
 |   There are 2 different formula for the conversion from color to gray:
 |
 |   OpenCV luminosity formula:
 |   Y = 0.299 R + 0.587 G + 0.114 B
 |
 |   Gimp luminosity formula:
 |   Y = 0.222 R + 0.717 G + 0.061 B
 |
 |  Input parameters :
 |   color           : Vector for 3 colors in uchar format
 |
 |  Output parameter :
 |   (float)         : Gray value as floating point
 |
 |
 +-------------------------------------------------------------------------*/

float ColorCurves::getLuminosity ( Vec3b color )
{
  float gray;
  bool  useGimp;

  useGimp = true;

  if (useGimp)
  {
    gray = ( 0.222 * color [0] ) +     // red
           ( 0.717 * color [1] ) +     // green
           ( 0.061 * color [2] );      // blue
  }
  else
  {
    gray = ( 0.299 * color [0] ) +     // red
           ( 0.587 * color [1] ) +     // green
           ( 0.114 * color [2] );      // blue
  }

  gray = qMin ( 1.0, gray / 255.0 );

  return gray;

}               /*-----  getLuminosity  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : ColorCurves  of  ColorChannel
 |
 |  Description :
 |
 |   This function initializes all channels  with type linear and the two
 |   points black and white from 0 to 1.0.
 |
 +-------------------------------------------------------------------------*/

void ColorCurves::initDefault ( )
{

  isGray = true;

  resetColors ();

  chanGray.resetPoints ();

}               /*-----  initDefault  -----*/

/*-------------------------------------------------------------------------
 |
 |  Member function : resetColors  of  ColorCurves
 |
 |  Description :
 |
 |   This function resets  all color channel curves with  the type linear
 |   and the two points black and white linear from 0 to 1.0.
 |
 +-------------------------------------------------------------------------*/

void ColorCurves::resetColors ()
{

  applyColors = false;

  // all color channels linear from 0 to 1.0
  chanRed  .resetPoints ();
  chanGreen.resetPoints ();
  chanBlue .resetPoints ();

}               /*-----  resetColors  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setImage  of  ColorCurves
 |
 |  Description :
 |
 |   This function takes the given image and makes a copy of it. If it is
 |   a color  image then the color  channels are split and  stored in the
 |   red, green and blue channels. The color order is RGB.
 |
 +-------------------------------------------------------------------------*/

void ColorCurves::setImage ( cv::Mat & image )
{
  V_CvMat channels;
  cv::Mat gray;

  matIn = image.clone ();   // make a copy

  imageMat.evalMatInfo (image);

  if ( imageMat.getChannels () == 1 )
  {
    isGray = true;

    chanGray.setChannelMat (image);
  }
  else
  {
    isGray = false;

    cv::split ( image, channels );

    chanRed  .setChannelMat ( channels [0] );
    chanGreen.setChannelMat ( channels [1] );
    chanBlue .setChannelMat ( channels [2] );

    // convert to gray using the luminosity formula of open cv
    cvtColor ( image, gray, COLOR_RGB2GRAY );

    chanGray.setChannelMat (gray);
  }

}               /*-----  setImage  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setBlackPnt  of  ColorCurves
 |
 |  Description :
 |
 |   This  function sets  the  given  color as  'black  point' for  every
 |   channel color.  The luminosity is  calculated from the color and set
 |   as 'black point' for the gray channel.
 |
 +-------------------------------------------------------------------------*/

void ColorCurves::setBlackPnt ( Vec3b color )
{
  float gray;

  // all color channels linear from 0 to 1.0
  resetColors ();

  gray = getLuminosity (color);

  chanGray.setBlackPnt (gray);

}               /*-----  setBlackPnt  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setWhitePnt  of  ColorCurves
 |
 |  Description :
 |
 |   This  function sets  the  given  color as  'white  point' for  every
 |   channel color.  The luminosity is  calculated from the color and set
 |   as 'white point' for the gray channel.
 |
 +-------------------------------------------------------------------------*/

void ColorCurves::setWhitePnt ( Vec3b color )
{
  float gray;

  // all color channels linear from 0 to 1.0
  resetColors ();

  gray = getLuminosity (color);

  chanGray.setWhitePnt (gray);

}               /*-----  setWhitePnt  -----*/

