/*-------------------------------------------------------------------------
 |
 |  Include-File :  FileConfig.h
 |
 |  Description :
 |
 |   This  include file  contains all class   definitions for  the module
 |   'FileConfig'.
 |
 +-------------------------------------------------------------------------*/

#ifndef FileConfig_h
#define FileConfig_h

#include "ParBase.h"


/*-------------------------------------------------------------------------
 |
 |  Class :  FileConfig
 |
 |  Description :
 |
 |   This class contains the parameter (settings) for job files, load and
 |   save configuration options.
 |
 +-------------------------------------------------------------------------*/

class FileConfig : public ParBase
{

  public:
    FileConfig ();
    virtual ~FileConfig ();

    virtual void load        ();
    virtual void save        ();
    virtual void setDefaults ();
    virtual void fillParList ();

  public:
    QString  jobFile;

    // parameter load and save
    bool     loadGray;
    bool     load8bit;

    bool     saveAsIs;
    bool     saveGray;
    bool     save8bit;
    int      saveJpgComp;      // compression level for jpg
    QString  saveType;         // image type (format) like jpg, png etc.

    // parameter for ocrmypdf
    QString  ocrFile;
    QString  ocrConfig;
    QString  language;
    int      imageDpi;
    bool     createPdf;        // for job manager
    bool     openPdf;

};

#endif
