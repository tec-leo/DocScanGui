/*-------------------------------------------------------------------------
 |
 |  File Name   :  AppCurves.cpp
 |
 |  Description :
 |
 |   This class  contains the  the loading and  storing of  several color
 |   curves presets. The settings are in  an own file because they should
 |   be global and independent from the other settings.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 18.Feb.2025
 |  Modified  : Oskar Leirich                Last changes  : 06.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of AppCurves:
 |    AppCurves                 ~AppCurves                addCurve
 |    findByName                getCurrent                load
 |    loadChannel               loadCurve                 save
 |    saveChannel               saveCurve                 selectCurve
 |    setCurrent                setDefaults               setIniName
 |
 +-------------------------------------------------------------------------*/

#include "AppCurves.h"

/*------------------------------------------*/
/*  C O N S T -  D e c l a r a t i o n s    */
/*------------------------------------------*/

const QString cParCurves = "Curves";
const QString cLastCurve = "Latest";

/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  AppCurves
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 +-------------------------------------------------------------------------*/

AppCurves::AppCurves () : ParBase ()
{

  setDefaults ();

  parFile = new ParFile ();

}                   /*-----  AppCurves  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  AppCurves
 |
 |  Description :
 |
 |   The destructor does the clean-up.
 |
 +-------------------------------------------------------------------------*/

AppCurves::~AppCurves ()
{

  delete parFile;

  qDeleteAll (colorCurves);

}                   /*-----  ~AppCurves  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  addCurve  of  AppCurves
 |
 |  Description :
 |
 |   This function adds the given color  level with the given name to the
 |   list of  color levels. If  there is already  one with the  same name
 |   then it is replaced.
 |
 +-------------------------------------------------------------------------*/

void AppCurves::addCurve ( ColorCurves * level, QString name )
{
  ColorCurves * colCurve;

  if ( curveNames.indexOf (name) < 0 )
  {
    curveNames.append (name);

    colCurve = new ColorCurves ();
    * colCurve = * level;

    colCurve->setCurveName (name);    // assign after copy

    colorCurves.append (colCurve);
  }
  else
  {
    colCurve = findByName (name);

    * colCurve = * level;
  }

  selectCurve (name);

  save ();    // save always

}               /*-----  addCurve  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : findByName  of  AppCurves
 |
 |  Description :
 |
 |   This function  searches in the level  colors for one with  the given
 |   name and returns a  pointer to it or a null pointer  if it cannot be
 |   found.
 |
 +-------------------------------------------------------------------------*/

ColorCurves * AppCurves::findByName ( QString name )
{
  ColorCurves * colorCurve;

  colorCurve = nullptr;

  for ( int i = 0; i < colorCurves.count (); i++ )
  {
    if ( name == colorCurves [i]->getCurveName () )
    {
      colorCurve = colorCurves [i];
      break;
    }
  }

  return colorCurve;

}               /*-----  findByName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : getCurrent  of  AppCurves
 |
 |  Description :
 |
 |   This function returns a pointer to the default level color.
 |
 +-------------------------------------------------------------------------*/

ColorCurves * AppCurves::getCurrent ()
{

  return lastCurve;

}               /*-----  getCurrent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setCurrent  of  AppCurves
 |
 |  Description :
 |
 |   This  function copies  the given  level color  to the  default level
 |   color.
 |
 +-------------------------------------------------------------------------*/

void AppCurves::setCurrent ( ColorCurves curColor )
{

  * lastCurve = curColor;

  lastCurve->setCurveName ( cLastCurve );

}               /*-----  setCurrent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  loadChannel  of  AppCurves
 |
 |  Description :
 |
 |   This function loads the curves and levels settings from the ini file.
 |
 |   Example for an entry:
 |    [Latest-Gray]
 |    curveType=linear
 |    sigma=0.0700
 |    offset=0.0000
 |    curvePoints=(0,0) (1,1)
 |
 +-------------------------------------------------------------------------*/

void AppCurves::loadChannel ( QString curveName, ColorChannel * curve )
{
  L_String points;
  L_String pntTxt;
  QPointF  point;
  QString  entry;
  double   x, y;
  int      cnt;

  parFile->setSection  (curveName);

  curve->initDefault ();

  parFile->findPar ( "curveType",   curve->curveType );
  parFile->findPar ( "curvePoints", entry );
  parFile->findPar ( "sigma",       curve->sigma );
  parFile->findPar ( "offset",      curve->offset );

  points = entry.split ( " " );

  cnt = points.count ();

  if ( cnt < 2 )  // two points = minimum
    return;

  curve->points.clear ();

  for ( int i = 0; i < cnt; i++ )
  {
    text = points [i];
    text.replace ( "(", "" );
    text.replace ( ")", "" );

    pntTxt = text.split ( "," );

    if ( pntTxt.count () != 2 )
      return;

    // limit values
    x = qBound ( 0.0, pntTxt [0].toDouble (), 1.0 );
    y = qBound ( 0.0, pntTxt [1].toDouble (), 1.0 );

    point.setX (x);
    point.setY (y);

    curve->points.append (point);
  }

  std::sort ( curve->points.begin (), curve->points.end (), ColorChannel::ascendingX );

  // finally copy points
  curve->setCurvePoints ();

}                   /*-----  loadChannel  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  loadCurve  of  AppCurves
 |
 |  Description :
 |
 |   This function  loads the  levels settings from  the ini  file. Curve
 |   means  all  colors  of  a  curve   if  they  are  available  in  the
 |   config-ini. Colors are gray, red, blue, green.
 |
 +-------------------------------------------------------------------------*/

void AppCurves::loadCurve ( QString curveName )
{
  ColorCurves * colorCurve;
  bool          isGray;

  if ( not parFile->sectionExists (curveName) )
    return;

  colorCurve = new ColorCurves ();
  colorCurve->setCurveName (curveName);

  parFile->setSection  (curveName);

  if ( parFile->findPar ( "isGray", isGray ) )
    colorCurve->setIsGray (isGray);

  // load gray curve
  section = curveName + "-Gray";
  parFile->setSection  (section);

  loadChannel ( section, colorCurve->getChannelGray () );

  colorCurves.append (colorCurve);

  // load red curve
  section = curveName + "-Red";

  if ( parFile->sectionExists (section) )
    loadChannel ( section, colorCurve->getChannelRed () );

  // load green curve
  section = curveName + "-Green";

  if ( parFile->sectionExists (section) )
    loadChannel ( section, colorCurve->getChannelGreen () );

  // load red curve
  section = curveName + "-Blue";

  if ( parFile->sectionExists (section) )
    loadChannel ( section, colorCurve->getChannelBlue () );

}                   /*-----  loadCurve  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  load  of  AppCurves
 |
 |  Description :
 |
 |   This function loads the curves and levels settings from the ini file.
 |
 +-------------------------------------------------------------------------*/

void AppCurves::load ()
{
  int index;

  parFile->load ();

  parFile->setSection  (cParCurves);

  ParBase::load ();

  qDeleteAll (colorCurves);
  colorCurves.clear ();

  if ( parFile->findPar ( "curves", text ) )
  {
    curveNames = text.split (cComma);
  }

  index = curveNames.indexOf (cLastCurve);

  if ( curveNames.contains (cLastCurve) )
  {
    curveNames.remove (index);
    curveNames.sort ();
    curveNames.prepend (cLastCurve);
  }
  else
    curveNames.sort ();

  for ( int i = 0; i < curveNames.count (); i++ )
  {
    text = curveNames [i];
    loadCurve (text);
  }

  lastCurve = findByName (cLastCurve);

  if ( lastCurve == nullptr )
  {
    lastCurve = new ColorCurves ();
    lastCurve->initDefault ();
    lastCurve->setCurveName ( cLastCurve );

    curveNames.prepend (cLastCurve);
    colorCurves.prepend (lastCurve);
  }

}                   /*-----  load  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  saveChannel  of  AppCurves
 |
 |  Description :
 |
 |   This function saves the curves and levels settings to the ini file.
 |
 |   An entry looks for example like:
 |    [Linear-Gray]
 |    curveType=linear
 |    sigma=0.0700
 |    offset=0.0000
 |    curvePoints=(0,0) (1,1)
 |
 +-------------------------------------------------------------------------*/

void AppCurves::saveChannel ( QString curveName, ColorChannel * curve )
{
  L_String pntTxt;
  QPointF  point;
  QString  entry;
  int      cnt;

  parFile->setSection  (curveName);

  parFile->writePar ( "curveType", curve->curveType );
  parFile->writePar ( "sigma",     curve->sigma );
  parFile->writePar ( "offset",    curve->offset );

  if ( curve->curveType == cTypeSigmoid )
    return;

  cnt = curve->points.count ();
  pntTxt.clear ();

  for ( int i = 0; i < cnt; i++ )
  {
    point = curve->points [i];

    text = QString ( "(%1,%2)" ).arg ( point.x () ).arg ( point.y () );

    pntTxt.append (text);
  }

  entry = pntTxt.join ( " " );

  parFile->writePar ( "curvePoints", entry );

}                   /*-----  saveChannel  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  saveCurve  of  AppCurves
 |
 |  Description :
 |
 |   This function saves the levels settings to the ini file. Curve means
 |   all colors of a curve, as there is gray, red, blue, green.
 |
 +-------------------------------------------------------------------------*/

void AppCurves::saveCurve ( ColorCurves * colorCurve )
{
  QString curveName;

  curveName = colorCurve->getCurveName ();

  parFile->setSection (curveName);
  parFile->writePar ( "isGray", colorCurve->getIsGray () );

  // load gray curve
  section = curveName + "-Gray";
  parFile->setSection (section);

  saveChannel ( section, colorCurve->getChannelGray () );

  if ( colorCurve->getIsGray () )    // only gray channel
    return;

  // load red curve
  section = curveName + "-Red";
  parFile->setSection (section);
  saveChannel ( section, colorCurve->getChannelRed () );

  // load green curve
  section = curveName + "-Green";
  parFile->setSection (section);
  saveChannel ( section, colorCurve->getChannelGreen () );

  // load red curve
  section = curveName + "-Blue";
  parFile->setSection (section);
  saveChannel ( section, colorCurve->getChannelBlue () );

}                   /*-----  saveCurve  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  save  of  AppCurves
 |
 |  Description :
 |
 |   This function saves the curves and levels settings to the ini file.
 |
 +-------------------------------------------------------------------------*/

void AppCurves::save ()
{
  ColorCurves * colorCurve;
  L_String      names;
  QString       curveName;

  parFile->clear ();     // delete all section hashes and names

  parFile->setSection (cParCurves);

  text = curveNames.join (cComma);

  parFile->writePar ( "curves", text );

  for ( int i = 0; i < colorCurves.count (); i++ )
  {
    colorCurve = colorCurves [i];

    saveCurve (colorCurve);

    curveName = colorCurve->getCurveName ();

    if ( not curveNames.contains (curveName) )
      curveNames.append (curveName);
  }

  ParBase::save ();

  parFile->save ();

}                   /*-----  save  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : selectCurve  of  AppCurves
 |
 |  Description :
 |
 |   This function  searches in the level  colors for one with  the given
 |   name and updates the pointer of the selected color curve.
 |
 +-------------------------------------------------------------------------*/

ColorCurves * AppCurves::selectCurve ( QString name )
{
  ColorCurves * colorCurve;

  colorCurve = findByName (name);

  if (colorCurve)
  {
    * lastCurve = * colorCurve;     // make a copy to default
    lastCurve->setCurveName ( cLastCurve );
  }

  return colorCurve;

}               /*-----  selectCurve  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function : setDefaults  of  AppCurves
 |
 |  Description :
 |
 |   This function sets all parameter to the default value.
 |
 +-------------------------------------------------------------------------*/

void AppCurves::setDefaults ()
{

  ParBase::setDefaults ();

}                   /*-----  setDefaults  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setIniName  of  AppCurves
 |
 |  Description :
 |
 |   This function sets the par file to the given name. It will be loaded
 |   when load is called.
 |
 +-------------------------------------------------------------------------*/

void AppCurves::setIniName ( QString name )
{

  parFile->setFileName (name);

}               /*-----  setIniName  -----*/
