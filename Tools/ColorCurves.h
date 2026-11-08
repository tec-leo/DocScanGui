/*-------------------------------------------------------------------------
 |
 |  Include-File :  ColorCurves.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  ColorCurves.
 |
 +-------------------------------------------------------------------------*/

#ifndef ColorCurves_h
#define ColorCurves_h

#include "ImageMat.h"
#include "ColorChannel.h"

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class ColorCurves;

/*------------------------------------------*/
/*   T Y P E   -  D e c l a r a t i o n s   */
/*------------------------------------------*/

typedef  QList  < ColorCurves  * >  L_ColorCurves;
typedef  QList  < ColorChannel * >  L_ColorChannel;

/*-------------------------------------------------------------------------
 |
 |  Class :  ColorCurves
 |
 |  Description :
 |
 |   This  class implements  functions  to apply  black  and white  point
 |   convertion  and color  space stretching.   It can  apply a  color or
 |   grayscale channel to modify an image from OpenCV.
 |
 +-------------------------------------------------------------------------*/

class ColorCurves
{

    // friend class AppCurves;   // for curve presets

  public:
    ColorCurves ();

    void           initDefault     ();

    cv::Mat        applyCurves     ( cv::Mat & matIn );
    void           applyCurve      ( cv::Mat & matIn, cv::Mat & matOut );

    ColorChannel * getChannelRed   ()   { return & chanRed;   };
    ColorChannel * getChannelGreen ()   { return & chanGreen; };
    ColorChannel * getChannelBlue  ()   { return & chanBlue;  };
    ColorChannel * getChannelGray  ()   { return & chanGray;  };

    float          getLuminosity   ( cv::Vec3b color );    // RGB for color image
    QString        getCurveName    () { return curveName; }
    void           setCurveName    ( QString name ) { curveName = name; }

    bool           getApplyColors  ()               { return applyColors; }
    void           setApplyColors  ( bool merge )   { applyColors = merge; }

    bool           getIsGray       ()               { return isGray; }
    void           setIsGray       ( bool gray )    { isGray = gray; }
    void           resetColors     ();
    void           setImage        ( cv::Mat & image );
    void           setBlackPnt     ( cv::Vec3b color );    // RGB for color image
    void           setWhitePnt     ( cv::Vec3b color );    // RGB for color image

  protected:
    L_ColorChannel colorChannels;

    ColorChannel   chanRed;
    ColorChannel   chanGreen;
    ColorChannel   chanBlue;
    ColorChannel   chanGray;

    QString       curveName;
    ImageMat      imageMat;
    cv::Mat       matIn;
    cv::Vec3b     color;
    bool          isGray;
    bool          applyColors;  // only used in applyCurve

};


#endif
