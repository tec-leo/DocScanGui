/*-------------------------------------------------------------------------
 |
 |  Include-File :  AppCurves.h
 |
 |  Description :
 |
 |   This  include file  contains all class   definitions for  the module
 |   'AppCurves'.
 |
 +-------------------------------------------------------------------------*/

#ifndef AppCurves_h
#define AppCurves_h

#include "ParBase.h"
#include "../Tools/ColorCurves.h"


/*-------------------------------------------------------------------------
 |
 |  Class :  AppCurves
 |
 |  Description :
 |
 |   This class contains the loading  and storing of several color curves
 |   presets. The  settings are  in an  own file  because they  should be
 |   global and independent from the other settings.
 |
 +-------------------------------------------------------------------------*/

class AppCurves : public ParBase
{

  public:
    AppCurves ();
    virtual ~AppCurves ();

    void          load          ();
    void          save          ();
    void          setDefaults   ();

    void          addCurve      ( ColorCurves * level, QString name );
    ColorCurves * selectCurve   ( QString name );
    ColorCurves * findByName    ( QString name );
    ColorCurves * getCurrent    ();
    void          setCurrent    ( ColorCurves curColor );
    L_String      getCurveNames () { return curveNames; }
    void          loadCurve     ( QString curveName );
    void          loadChannel   ( QString channelName, ColorChannel * curve );

    void          saveCurve     ( ColorCurves * level );
    void          saveChannel   ( QString channelName, ColorChannel * curve );

    void          setIniName    ( QString name );

  protected:
    L_String      curveNames;
    ColorCurves * lastCurve;
    L_ColorCurves colorCurves;

  protected:
    ParFile * parFile;
    QString   section;

};

#endif
