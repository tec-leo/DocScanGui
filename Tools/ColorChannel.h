/*-------------------------------------------------------------------------
 |
 |  Include-File :  ColorChannel.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   ColorChannel.
 |
 +-------------------------------------------------------------------------*/

#ifndef ColorChannel_h
#define ColorChannel_h

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const QString cTypeLinear  = "linear";
const QString cTypeSigmoid = "sigmoid";
const QString cTypeSpline  = "spline";
const QString cTypeAuto    = "auto";

/*------------------------------------------*/
/*   T Y P E   -  D e c l a r a t i o n s   */
/*------------------------------------------*/

typedef  QList  < QPointF >  L_PointF;

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

/*-------------------------------------------------------------------------
 |
 |  Class :  ChannelInfo
 |
 |  Description :
 |
 |   This class provides  a (only) readable copy of some  of the internal
 |   variables. For member variables which  can be set from outside there
 |   are set functions defined.
 |
 +-------------------------------------------------------------------------*/

class ChannelInfo
{

  public:
    L_PointF points;
    L_PointF sigmoid;
    QString  curveType;    // linear, spline or sigmoid

    bool     lineBeg;      // line in the beginning
    bool     lineEnd;      // line at the end
    bool     splines;      // points in between exists

    float    lowInput;
    float    highInput;

    float    blackPnt;
    float    whitePnt;

    float    sigma;
    float    offset;

    int      splineBeg;    // index of point where a spline begins
    int      splineEnd;    // index of point where a spline ends
    int      numBins;      // number of bins (samples)

};


/*-------------------------------------------------------------------------
 |
 |  Class :  ColorChannel
 |
 |  Description :
 |
 |   This class  implements functions for setting,  modifying and reading
 |   curve points and  samples for a color or gray  channel.  It contains
 |   functions for  the conversion  from points  to curves,  handling and
 |   creating of lookup tables.
 |
 +-------------------------------------------------------------------------*/

class ColorChannel
{

  friend class AppCurves;     // for curve presets
  friend class ColorCurves;   // for curve presets

  public:
    ColorChannel ();
    virtual ~ColorChannel ();

    void      initDefault     ();
    cv::Mat   getLookup       ()  { return cvLut; }
    void      getChannelInfo  ( ChannelInfo & info );
    bool      checkLinear     ();
    void      createSamples   ();
    void      createSigmoid   ();
    void      sigmoidToSpline ();
    cv::Mat * getChannelMat   ()               { return & cvChannel; }
    void      setChannelMat   ( cv::Mat & mat );

    // point functions
    int       addPoint        ( QPointF point );
    void      delPoint        ( QPointF point );
    int       findPoint       ( QPointF point );
    void      setPoint        ( QPointF point );
    void      setPoint        ( int & index, QPointF point );

    void      evalPoints      ();
    void      resetPoints     ();

    L_PointF  getSamplePnts   ()               { return samples; }
    void      setBlackPnt     ( float x );
    void      setWhitePnt     ( float x );
    void      setLowHigh      ( float low, float high );
    void      setSigma        ( float val )   { sigma = val; }
    void      setOffset       ( float val )   { offset = val; }

    QString   getCurveType    ()               { return curveType; }
    void      setCurveType    ( QString type );
    void      setCurvePoints  ();
    void      setNumBins      ( int num )      { numBins = num; }

  protected:
    void      createLine      ( QPointF begPnt, QPointF endPnt );
    void      createSpline    ( L_PointF inPoints );
    void      createLinear    ();
    void      createSplines   ();
    void      calcSamplePnts  ();
    void      evalSpline      ();
    void      limitPoint      ( QPointF & point );
    bool      isEqual         ( float y1, float y2 );

  protected:    // static, for sorting of points
    static bool ascendingX  ( QPointF firstObj, QPointF secondObj );

  protected:
    cv::Mat   cvChannel;    // channel matrix
    cv::Mat   cvLut;        // data of the lookup table
    L_PointF  samples;
    L_PointF  points;
    L_PointF  linePts;
    L_PointF  splinePts;
    L_PointF  sigmoid;
    QString   curveType;    // linear, spline, sigmoid or auto
    bool      lineBeg;      // line in the beginning
    bool      lineEnd;      // line at the end
    bool      splines;      // points in between exists
    float     offset;
    float     sigma;
    float     lowInput;
    float     highInput;
    float     delta;
    int       numBins;      // number of bins (samples)
    int       splineBeg;    // index of point where a spline begins
    int       splineEnd;    // index of point where a spline ends

};

#endif
