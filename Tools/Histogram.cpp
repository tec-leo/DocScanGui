/*-------------------------------------------------------------------------
 |
 |  File Name   :  Histogram.cpp
 |
 |  Description :
 |
 |   This  module  contains  the  class 'Histogram'  which  calculates  a
 |   histogram  from  a given  OpenCV  image  with  a defined  number  of
 |   bins. It the given image is  not grayscale then it will be converted
 |   to gray first.
 |
 |  Developer : Oskar Leirich                Creation date : 17.Apr.2025
 |  Modified  : Oskar Leirich                Last changes  : 06.Jun.2026
 |
 |
 |  This unit contains following member functions :
 |   of Histogram:
 |    Histogram                 ~Histogram                calcHistogram
 |    create                    getNormalized
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "Histogram.h"
#include "ImageTools.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  Histogram
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 |  Input parameters :
 |   parent          :  Pointer to the parent window
 |
 +-------------------------------------------------------------------------*/

Histogram::Histogram ()
{

  numBins  = 256;  // default number of bins
  maxValue = 0.0;
  pixels   = 0.0;

}               /*----- Histogram -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  Histogram
 |
 |  Description :
 |
 |   The destructor deletes all  dynamically created objects if there are
 |   any.
 |
 +-------------------------------------------------------------------------*/

Histogram::~Histogram ()
{


}               /*-----  ~Histogram  -----*/

/*-------------------------------------------------------------------------
 |
 |  Member function :  create  of  Histogram
 |
 |  Description :
 |
 |   This function creates  a histogram of a grayscale  or colored image.
 |   If the image is colored then it is first converted to grayscale with
 |   OpenCV  which calculates  luminosity of  the image  with the  OpenCV
 |   formula.
 |
 +-------------------------------------------------------------------------*/

void Histogram::create ( cv::Mat image )
{
  ImageMat imageMat;

  matImg = imageMat.convToGray (image);

  pixels = image.cols * image.rows;

  calcHistogram ();

}               /*-----  create  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : calcHistogram  of  Histogram
 |
 |  Description :
 |
 |   This function calculates a histogram of a the given channel.
 |
 +-------------------------------------------------------------------------*/

void Histogram::calcHistogram ()
{
  bool  accumulate;   // true - histogram is not cleared in the beginning
  bool  uniform;      // histogram is uniform or not
  float histY;

  uniform    = true;  // set histogram param
  accumulate = false; // clear histogram in the beginning

  // set the ranges ( for B, G, R )
  float range [] = { 0, 256 };  // the upper boundary is exclusive
  const float * histRange [] = {range};

  calcHist ( & matImg, 1, 0, Mat (), matHist, 1, & numBins,
             histRange, uniform, accumulate );

  histValues.clear ();

  maxValue = 0;
  histSum  = 0;

  for ( int i = 0; i < numBins; i++ )
  {
    histY  = matHist.at <float> (i);

    histValues.append (histY);
    histSum = histSum + histY;

    maxValue = qMax ( maxValue, histY );
  }

}               /*-----  calcHistogram  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : getNormalized  of  Histogram
 |
 |  Description :
 |
 |   This function returns  the normalized histogram values,  they can be
 |   normalized  to the  maximum value  or to  the sum  of all  histogram
 |   values.
 |
 +-------------------------------------------------------------------------*/

L_Float Histogram::getNormalized ( QString type )
{
  L_Float normValues;
  float   curValue;

  normValues.resize (numBins);

  for ( int i = 0; i < numBins; i++ )
  {
    curValue = histValues [i];

    if ( type == "Max" )
      curValue = curValue / maxValue;
    else
      curValue = curValue / histSum;

    normValues [i] = curValue;
  }

  return normValues;

}               /*-----  getNormalized  -----*/
