/*-------------------------------------------------------------------------
 |
 |  File Name   :  RegFile.cpp
 |
 |  Description :
 |
 |   This module contains the class 'RegFile' which is a convenient class
 |   derived from QSettings. It uses per  default the ini file format and
 |   writes all information into the users 'Application Data' directory.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 28.Mar.2023
 |  Modified  : Oskar Leirich                Last changes  : 12.Dec.2024
 |
 |
 |  This unit contains following member functions :
 |   of RegFile :
 |    RegFile                   getPath                   readValue
 |    setPath                   setValue                  value
 |    writeValue
 |
 +-------------------------------------------------------------------------*/

#include "RegFile.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  RegFile
 |
 |  Description :
 |
 |   The constructor initializes the  QSetting with the default behavior,
 |   which   means   using   ini   file  format   in   the   user   scope
 |   (user\AppData\Roaming) and sets the company name Automotive) and the
 |   program name.
 |
 |  Input parameters :
 |   path            : Name for the start of a group
 |   parent          : Parent widget
 |
 +-------------------------------------------------------------------------*/

RegFile::RegFile ( QObject * parent ) :
                     QSettings ( QSettings::IniFormat, QSettings::UserScope,
                                 QCoreApplication::organizationName (),
                                 QCoreApplication::applicationName (), parent )
{

  setDefaultFormat (QSettings::IniFormat);

  companyName   = QCoreApplication::organizationName ();
  companyDomain = QCoreApplication::organizationDomain ();
  programName   = QCoreApplication::applicationName ();

}               /*-----  RegFile  -----*/


RegFile::RegFile ( const QString & fileName, QObject * parent ) :
                   QSettings ( fileName, QSettings::IniFormat, parent )
{

  setDefaultFormat (QSettings::IniFormat);

  companyName   = QCoreApplication::organizationName ();
  companyDomain = QCoreApplication::organizationDomain ();
  programName   = QCoreApplication::applicationName ();

}               /*-----  RegFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setValue  of  RegFile
 |
 |  Description :
 |
 |   This function is  an overloaded function, it writes  the given value
 |   to the registry file.
 |
 |  Input parameters :
 |   key             : Key of value to write
 |   value           : Value to write
 |
 +-------------------------------------------------------------------------*/

void RegFile::setValue ( QString key, QVariant value )
{

  QSettings::setValue ( key, value );

}               /*-----  setValue  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  value  of  RegFile
 |
 |  Description :
 |
 |   This function is  an overloaded function, it reads a value
 |   from the registry file.
 |
 |  Input parameters :
 |   key             : Key of value to find
 |   defaultValue    : If value not found return this
 |
 |  Output parameter :
 |   (QVariant)      : Value found for given key or default value
 |
 +-------------------------------------------------------------------------*/

QVariant RegFile::value ( QString key, QVariant defaultValue )
{
  QVariant  entry;

  entry = QSettings::value ( key, defaultValue );

  return entry;

}               /*-----  value  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getPath  of  RegFile
 |
 |  Description :
 |
 |   This function returns the current path
 |
 |  Output parameter :
 |   (QString)       : Current path
 |
 +-------------------------------------------------------------------------*/

QString RegFile::getPath ()
{

  return group ();

}               /*-----  getPath  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setPath  of  RegFile
 |
 |  Description :
 |
 |   This function sets current path.
 |
 |  Input parameters :
 |   path            :
 |
 +-------------------------------------------------------------------------*/

void RegFile::setPath ( QString path )
{

  // close all groups
  while ( not group ().isEmpty () )
  {
    endGroup ();
  }

  beginGroup (path);

}               /*-----  setPath  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writeValue  of  RegFile
 |
 |  Description :
 |
 |   This function writes one value directly to the registry file.
 |
 |  Input parameters :
 |   path            : Path to key
 |   key             : Key of value to write
 |   value           : Value to write
 |
 +-------------------------------------------------------------------------*/

void RegFile::writeValue ( QString path, QString key, QVariant value )
{
  RegFile settings (path);

  settings.setValue ( key, value );

}               /*-----  writeValue  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  readValue  of  RegFile
 |
 |  Description :
 |
 |   This function reads one value directly from the registry file.
 |
 |  Input parameters :
 |   path            : Path to the key
 |   key             : Key of value to find
 |   defaultValue    : If no value is found return defaultValue
 |
 |  Output parameter :
 |   (QVariant)      : Value found for key or default value
 |
 +-------------------------------------------------------------------------*/

QVariant RegFile::readValue ( QString path, QString key, QVariant defaultValue )
{
  RegFile settings (path);

  return settings.value ( key, defaultValue );

}               /*-----  readValue  -----*/
