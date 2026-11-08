/*-------------------------------------------------------------------------
 |
 |  Include-File :  ImageTools.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  ImageTools.
 |
 +-------------------------------------------------------------------------*/

#ifndef ImageTools_h
#define ImageTools_h

#include "ImageMat.h"

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const double cAngleTolerance = 0.1;

/*------------------------------------------*/
/*   T Y P E   -  D e c l a r a t i o n s   */
/*------------------------------------------*/

typedef  QList < cv::RotatedRect >  L_RotatedRect;
typedef  QList < cv::Rect2f      >  L_Rect2f;
typedef  QList < cv::Rect        >  L_Rect;
typedef  QList < L_Rect          >  LL_Rect;

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class  AppConfig;

/*-------------------------------------------------------------------------
 |
 |  Class :  ImageTools
 |
 |  Description :
 |
 |   This class implements several OpenCV functions like finding regions,
 |   contours, deskewing images etc.
 |
 +-------------------------------------------------------------------------*/

class ImageTools
{

  public:
    ImageTools ();

    Mat       addMargins      ( Mat input );
    Mat       cropImage       ( Mat input, bool detect );
    Mat       cropToContent   ( Mat input );
    float     detectAngle     ( Mat input );
    void      detectContour   ( Mat input, V_CvPoint & points );
    Mat       deskewAndCrop   ( Mat input, const RotatedRect & box );
    void      findRegions     ( Mat input, L_RotatedRect & areas );

    void      getBoundingBox  ( Mat input, RotatedRect & box );
    void      getBoundingBox  ( RotatedRect & rotRect );
    void      getBoundingRect ( Mat input, cv::Rect & box );
    cv::Rect  getContourRect   ( Mat input );
    void      getContourPts   ( Mat input, V_CvPoint & points );
    float     getDistance     ( cv::Point p1, cv::Point p2 );

    V_CvPoint getDocPoints    ( int rows, int cols, V_CvPoint & points );
    cv::Rect  getMarginsBox   ( Mat input );
    Mat       getSubRect      ( Mat input, cv::Rect2f rect );
    Mat       getSubRect      ( Mat input, cv::Rect2f rect, RotatedRect & box );
    Mat       mergeImages     ( Mat img1, Mat img2 );
    Mat       rotateImage     ( Mat input, bool detect );
    void      rectToGraph     ( QGraphicsItemGroup * root );
    void      setConfig       ( AppConfig * config )  { this->config = config; }

  protected:
    Mat      rotateImage     ( Mat input, float & angle );

  protected:    // static, for sorting of points
    static bool compareAngle ( double first, double second );

  protected:
    AppConfig   * config;
    ImageMat      imageMat;
    RotatedRect   box;      // helper variable
    L_RotatedRect regions;
    cv::Mat       matIn;
    cv::Scalar    black;
    cv::Scalar    white;

    GraphicUtils  graphUtils;
    StringUtils   stringUtils;

};


#endif
