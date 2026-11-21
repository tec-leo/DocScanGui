/*-------------------------------------------------------------------------
 |
 |  Include-File :  ImageMat.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  ImageMat.
 |
 +-------------------------------------------------------------------------*/

#ifndef ImageMat_h
#define ImageMat_h


/*-------------------------------------------------------------------------
 |
 |  Class :  ImageMat
 |
 |  Description :
 |
 |   This class implements some helper  functions evaluating an image and
 |   several conversion  functions like  get a QImage  from a  'Mat' from
 |   OpenCV.
 |
 +-------------------------------------------------------------------------*/

class ImageMat
{

  public:
    ImageMat ();

    Mat       binarize       ( Mat input, int threshold = -1 );
    Mat       blackOnWhite   ( Mat input );
    void      copyTo         ( Mat src, Mat dst, cv::Rect rect );
    Mat       convTo8bit     ( Mat input );
    Mat       convToGray     ( Mat input );
    QImage  * convMatImage   ( Mat & mat, QImage::Format format );
    QImage  * convMatImage   ( Mat & mat );
    Mat       invert         ( Mat & mat );
    void      evalMatInfo    ( Mat & mat );
    double    getBlackRatio  ( Mat & mat );
    double    getWhiteRatio  ( Mat & mat );
    V_CvPoint getWhitePoints ( Mat & mat );
    V_CvPoint getWhitePoints ( Mat & mat, cv::Rect rect );
    QString   getMatInfoStr  ( Mat & mat );
    L_String  getMatInfo     ( Mat & mat );
    QString   getMatType     ( Mat & mat );
    void      getImageFormat ( Mat & mat, QImage::Format & format );

    qint      getElemSize    () { return elemSize; }
    qint      getImgSize     () { return imgSize;  }
    qint      getDepth       () { return depth;    }
    QString   getDataType    () { return dataType; }
    qint      getDataSize    () { return dataSize; }
    qint      getPixLen      () { return pixLen;   }
    qint      getChannels    () { return channels; }
    int       getCols        () { return cols;     }
    int       getRows        () { return rows;     }
    qint      getSteps       () { return steps;    }

    void      removeAlpha    ( Mat & input );

  protected:
    L_String  matDepthStr;
    L_String  matTypeStr;
    L_Int     matTypeLen;
    QString   text;
    qint      elemSize;    // matrix element size in bytes
    qint      imgSize;     // overall size of the image
    qint      depth;       // pixel type, eg. CV_8U for 8bit unsigned
    qint      pixLen;      // number of bytes per pixel per channel
    qint      channels;    // number of channels, typically 3 for a colour image
    int       cols;
    int       rows;
    qint      steps;
    int       dataSize;
    QString   dataType;

};

#endif
