/*-------------------------------------------------------------------------
 |
 |  Include-File :  Filters.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   Filters.
 |
 +-------------------------------------------------------------------------*/

#ifndef Filters_h
#define Filters_h


/*-------------------------------------------------------------------------
 |
 |  Class :  Filters
 |
 |  Description :
 |
 |   This class  contains several functions for  filtering, morphological
 |   image processing, histograms etc.
 |
 +-------------------------------------------------------------------------*/

class Filters
{

  public:
    Filters ();
    virtual ~Filters ();

  public:
    cv::Mat sharpGaussian ( Mat input, double alpha,
                            double beta, double gamma );

    cv::Mat sharpHighpass ( Mat input, int kSize, double sigma, double alpha,
                            double beta, double gamma );

    cv::Mat sharpLaplace  ( Mat input, int kSize, double sigma,
                            int gaussKernel );

    cv::Mat unsharpMask   ( Mat input, int kSize, double amount,
                            double sigma, double thresh );

  protected:
    cv::Size size;

};

#endif
