/*-------------------------------------------------------------------------
 |
 |  Include-File :  Histogram.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   Histogram.
 |
 +-------------------------------------------------------------------------*/

#ifndef Histogram_h
#define Histogram_h

/*-------------------------------------------------------------------------
 |
 |  Class :  Histogram
 |
 |  Description :
 |
 |   This class calculates  a histogram from a given OpenCV  image with a
 |   defined number of bins. It the  given image is not grayscale then it
 |   will be converted to gray first.
 |
 +-------------------------------------------------------------------------*/

class Histogram
{

  public:
    Histogram ();
    virtual ~Histogram ();

  public:
    void    create        ( cv::Mat image );

    float   getHistSum    ()            { return histSum; }
    int     getPixels     ()            { return pixels; }
    int     getMaxValue   ()            { return maxValue; }
    L_Float getValues     ()            { return histValues; }
    L_Float getNormalized ( QString type );
    int     getNumBins    ()            { return numBins; }
    void    setNumBins    ( int num )   { numBins = num; }

  protected:
    void    calcHistogram ();

  protected:
    L_Float histValues;
    cv::Mat matImg;
    Mat     matHist;
    float   maxValue;
    int     numBins;      // number of bins (samples)
    int     pixels;
    float   histSum;

};

#endif
