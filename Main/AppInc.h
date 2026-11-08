/*-------------------------------------------------------------------------
 |
 |  Include-File :  AppInc.h
 |
 |  Description  :
 |
 |   This  is the include  file for  standard   system include  files, or
 |   project specific  include  files that  are used  frequently, but are
 |   changed  not very often. This file  also contain the includes of all
 |   support dll includes in dll.h.
 |
 +-------------------------------------------------------------------------*/

#ifndef AppInc_h
#define AppInc_h

#define _ALLOW_RTCc_IN_STL
#pragma warning ( disable : 4828 )

// commonly used Qt classes
#include <QtWidgets>
#include <QtSystemDetection>

#ifdef Q_OS_WIN64
  #pragma warning ( disable : 4828 )
#endif

#define  AppIcons   "DocScanGui"
#define  versionStr "V 1.0.0"

#ifdef _DEBUG
#define  buildStr   "Debug Nov. 2026"
#else
#define  buildStr   "Release Nov. 2026"
#endif

#ifdef _DEBUG
const bool debugMode = true;
#else
const bool debugMode = false;
#endif

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const  QString  cCompanyName   = "DocScanGui";
const  QString  cCompanyDomain = "github.com";
const  QString  cProgramName   = "DocScanGui";
const  QString  cProgramConfig = "GlobalConfig";

// common data types
#include "DataTypes.h"
#include "../Utils/Color.h"
#include "../Utils/FileUtils.h"
#include "../Utils/GraphicUtils.h"
#include "../Utils/HistoryList.h"
#include "../Utils/RegExp.h"
#include "../Utils/RegFile.h"
#include "../Utils/StringUtils.h"
#include "../Utils/VarUtils.h"
#include "../Utils/ParFile.h"

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/geometry.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;
//using namespace cv::text;

/*------------------------------------------*/
/*   T Y P E   -  D e c l a r a t i o n s   */
/*------------------------------------------*/

// Open CV container type definitions

typedef  QList       < cv::Mat     >  L_CvMat;
typedef  QList       < cv::Mat *   >  L_CvMatPtr;

typedef  std::vector < cv::Vec2i   >  V_CvVec2i;
typedef  std::vector < cv::Vec3i   >  V_CvVec3i;
typedef  std::vector < cv::Vec4i   >  V_CvVec4i;
typedef  std::vector < cv::Vec2f   >  V_CvVec2f;
typedef  std::vector < cv::Vec3f   >  V_CvVec3f;
typedef  std::vector < cv::Vec4f   >  V_CvVec4f;
typedef  std::vector < cv::Point   >  V_CvPoint;
typedef  std::vector < cv::Point2f >  V_CvPoint2f;
typedef  std::vector < cv::Rect    >  V_CvRect;
typedef  std::vector < cv::Mat     >  V_CvMat;
typedef  std::vector < string      >  V_StdString;
typedef  std::vector < int         >  V_StdInt;
typedef  std::vector < float       >  V_StdFloat;
typedef  std::vector < V_CvPoint   >  VV_CvPoint;
typedef  std::vector < V_CvVec2i   >  VV_CvVec2i;

#endif
