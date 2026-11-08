/*-------------------------------------------------------------------------
 |
 |  Include-File :  RegFile.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  RegFile.
 |
 +-------------------------------------------------------------------------*/

#ifndef RegFile_h
#define RegFile_h


/*-------------------------------------------------------------------------
 |
 |  Class :  RegFile
 |
 |  Description :
 |
 |   This class is a convenient class derived from QSettings. It uses per
 |   default  the ini  file format  and writes  all information  into the
 |   users 'Application Data'.
 |
 +-------------------------------------------------------------------------*/

class RegFile : public QSettings
{

  public:
    RegFile ( QObject * parent = nullptr );
    RegFile ( const QString & fileName, QObject * parent = nullptr );

    void     setValue    ( QString key, QVariant value );
    QVariant value       ( QString key,
                           QVariant defaultValue = QVariant () );

    QString  getPath     ();
    void     setPath     ( QString path );

    // static functions
    void     writeValue  ( QString path, QString key, QVariant value );
    QVariant readValue   ( QString path, QString key,
                           QVariant defaultValue = QVariant () );

  protected:
    QString  companyDomain;
    QString  companyName;
    QString  programName;

};

#endif
