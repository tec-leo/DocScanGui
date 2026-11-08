/*-------------------------------------------------------------------------
 |
 |  File Name   :  Filters.cpp
 |
 |  Description :
 |
 |   This module  implements the  class 'Filters' which  contains several
 |   functions for filtering, sharpen and  morphological image processing.
 |
 |  Developer : Oskar Leirich                Creation date : 20.Nov.2024
 |  Modified  : Oskar Leirich                Last changes  : 13.Jul.2025
 |
 |
 |  This unit contains following member functions :
 |   of Filters:
 |    Filters                   ~Filters                  sharpGaussian
 |    sharpHighpass             sharpLaplace              unsharpMask
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "Filters.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  Filters
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 +-------------------------------------------------------------------------*/

Filters::Filters ()
{

}               /*----- Filters -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  Filters
 |
 |  Description :
 |
 |   The destructor is emtpy.
 |
 +-------------------------------------------------------------------------*/

Filters::~Filters ()
{

}               /*-----  ~Filters  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  sharpHighpass  of  Filters
 |
 |  Description :
 |
 |   This  function  sharpens the  given  image  by first  subtracting  a
 |   blurred input to  get the difference and then adding  it weighted to
 |   the input.
 |
 |   Default values:
 |     kSize =  0.0
 |     alpha =  1.0
 |     beta  =  1.0
 |     gamma =  0.0
 |     sigma =  3.0
 |
 +-------------------------------------------------------------------------*/

Mat Filters::sharpHighpass ( Mat input, int kSize, double sigma, double alpha,
                             double beta, double gamma )
{
  Mat highp;
  Mat sharp;
  Mat blur;

  size.height = kSize;
  size.width  = kSize;

  cv::GaussianBlur ( input, blur, size, sigma );
  cv::subtract     ( input, blur, highp );   // subtract blurred

  // calculate the weighted sum of two arrays.
  cv::addWeighted  ( input, alpha, highp, beta, gamma, sharp );

  return sharp;

}               /*-----  sharpHighpass  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  sharpGaussian  of  Filters
 |
 |  Description :
 |
 |   This function sharpens the given image  by first bluring it and then
 |   subracting the  blured image with a  certain multiplication factors.
 |   For this it calls addWeighted with the following parameters:
 |
 |    InputArray  src1     --> first input array
 |    double      alpha    --> weight of the first array elements
 |    InputArray  src2     --> second input array (same size and channels)
 |    double      beta     --> weight of the second array elements
 |    double      gamma    --> scalar added to each sum
 |    OutputArray dst      --> output array
 |    int         dtype    --> -1 means the same depth as source image
 |
 |  The function addWeighted calculates the weighted sum of two arrays:
 |
 |    dst (I) O = saturate ( src1 (I) * alpha + src2 (i) * beta + gamma )
 |
 |   Default values:
 |     alpha =   1.5
 |     beta  = - 0.5
 |     gamma =   0.0
 |
 +-------------------------------------------------------------------------*/

Mat Filters::sharpGaussian ( Mat input, double alpha,
                             double beta, double gamma )
{
  Mat sharp;
  Mat blur;

  cv::GaussianBlur ( input, blur, cv::Size (0, 0), 3 );
  cv::addWeighted  ( input, alpha, blur, beta, gamma, sharp );

  return sharp;

}               /*-----  sharpGaussian  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  sharpLaplace  of  Filters
 |
 |  Description :
 |
 |   This function sharpens  the given image by  calculating the Laplacian
 |   of the  source image  by adding  up the second  x and  y derivatives
 |   calculated using the Sobel operator.
 |
 |   Default values:
 |     Gaussian  - kernel size = 3
 |     Gaussian  - sigma       = 0
 |     Laplacian - kernel size = 3
 |
 +-------------------------------------------------------------------------*/

Mat Filters::sharpLaplace ( Mat input, int kSize, double sigma,
                            int gaussKernel )
{
  Mat    blur;
  Mat    gray;
  Mat    dst;
  Mat    sharp;
  int    ddepth;  // desired depth of the destination image
  double scale;   // optional scale factor for the computed Laplacian values
  double delta;   // Optional delta value that is added to the results

  scale  = 1.0;
  delta  = 0.0;
  ddepth = CV_16S;   // to avoid overflow

  size.height = kSize;
  size.width  = kSize;

  // reduce noise with a Gaussian filter, kernel size = 3, sigma = 0
  cv::GaussianBlur ( input, blur, size, sigma, sigma, BORDER_DEFAULT );

  // convert the image to grayscale
  if ( blur.channels () == 3 )
    cvtColor( blur, gray, COLOR_BGR2GRAY );
  else
    gray = blur.clone ();

  // kSize is the aperture size for computing the second-derivative filters

  cv::Laplacian ( gray, dst, ddepth, gaussKernel, scale, delta, BORDER_DEFAULT );

  // converting back to CV_8U
  cv::convertScaleAbs ( dst, sharp );

  sharp = ~sharp;   // invert - convert to white on black

  return sharp;

}               /*-----  sharpLaplace  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  unsharpMask  of  Filters
 |
 |  Description :
 |
 |   This  function sharpens  the given  image by  applying an  'unsharp'
 |   mask.
 |   The typical blending formula for unsharp masking is
 |   sharpened = original + (original - blurred) x amount
 |
 |   Default values:
 |     Gaussian  - kernel size = 0
 |     Gaussian  - sigma       = 1.0
 |     brightness - threshold  = 5
 |     contrast added - amount = 1
 |
 +-------------------------------------------------------------------------*/

Mat Filters::unsharpMask ( Mat input, int kSize, double amount,
                           double sigma, double thresh )
{
  Mat sharp;
  Mat blur;
  Mat lowContrast;

  size.height = kSize;
  size.width  = kSize;

  cv::GaussianBlur ( input, blur, size, sigma, sigma );

  lowContrast = cv::abs ( input - blur ) < thresh;

  //  sharp = input * ( 1 + amount ) + blur * ( -amount );
  sharp = input  + ( input - blur ) * amount;

  input.copyTo ( sharp, lowContrast );

  return sharp;

}               /*-----  unsharpMask  -----*/

