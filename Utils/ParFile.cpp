/*-------------------------------------------------------------------------
 |
 |  File Name   :  ParFile.cpp
 |
 |  Description :
 |
 |   This module implements the  class 'ParFile' which provides functions
 |   for accessing  the sections and  values of  a parameter file  in the
 |   style  of a  config-ini. It  provides a  lot of  overdefinitions for
 |   different data types to read and write the entries and sections.
 |
 |   The content  of a config file  is loaded into the  internal hash and
 |   map structures. The  application works only on  this structures over
 |   the provided functions and is only  written to a file when 'save' is
 |   called.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 03.Apr.2022
 |  Modified  : Oskar Leirich                Last changes  : 19.Nov.2026
 |
 |
 |  This unit contains the following member functions :
 |   of ParFile :
 |    ParFile                   ~ParFile                  clear
 |    deleteSection             emptySection              entryExists
 |    findPar                   getAsString               getAsStringList
 |    getFileName               getSection                getSectionHash
 |    getSectionKeys            getSectionMap             getSectionNames
 |    getSectionText            load                      save
 |    sectionExists             setFileName               setSection
 |    writePar
 |
 +-------------------------------------------------------------------------*/

#include "ParFile.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R   of  ParFile
 |
 |  Description :
 |
 |   The constructor initializes the strings  for file name, section name
 |   etc.
 |
 +-------------------------------------------------------------------------*/

ParFile::ParFile ()
{

  fileName    = "";
  section     = "";
  sectionPtr  = nullptr;
  sectionMap  = nullptr;

  ok          = false;

}               /*-----  ParFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  ParFile
 |
 |  Description :
 |
 |   The destructor call 'clear' to  delete the hash structures and lists
 |   of the config file.
 |
 +-------------------------------------------------------------------------*/

ParFile::~ParFile ()
{

  clear ();

}              /*-----  ~ParFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  clear  of  ParFile
 |
 |  Description :
 |
 |   This function deletes hash structures and lists of the config file.
 |
 +-------------------------------------------------------------------------*/

void ParFile::clear ()
{
  L_String sectionList;
  QString  key;

  sectionList = parHash.keys ();

  for ( int i = 0; i < sectionList.count (); i++ )
  {
    key = sectionList [i];
    sectionMap = parHash.value (key);
    delete sectionMap;
  }

  parHash.clear ();
  sectionNames.clear ();
  sectionMap = nullptr;

}               /*-----  clear  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getSectionText  of  ParFile
 |
 |  Description :
 |
 |   This  function retrieves  a complete  section  and returns  it in  a
 |   string list.
 |
 |   The entries in the list are key / value pairs:
 |   [Tools]
 |   useSharpen=false
 |   useMorph=true
 |   useBlur=false
 |
 +-------------------------------------------------------------------------*/

L_String ParFile::getSectionText ()
{
  L_String entries;
  QString  line;
  QString  key;
  QString  entry;

  for ( int i = 0; i < sectionMap->keyList.count (); i++ )
  {
    key = sectionMap->keyList [i];
    entry = sectionMap->keyHash [key];
    line  = key + "=" + entry;
    entries.append (line);
  }

  return entries;

}              /*-----  getSectionText  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getSectionHash  of  ParFile
 |
 |  Description :
 |
 |   This  function returnes  the  all  keys value  pairs  of the  active
 |   section in a string / string hash.
 |
 +-------------------------------------------------------------------------*/

H_StrStr ParFile::getSectionHash ()
{

  return sectionMap->keyHash;

}              /*-----  getSectionHash  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getSectionMap  of  ParFile
 |
 |  Description :
 |
 |   This  function returnes  the  all  keys value  pairs  of the  active
 |   section in a string / string map.
 |
 +-------------------------------------------------------------------------*/

M_StrStr ParFile::getSectionMap ()
{
  L_String keys;
  M_StrStr sectMap;
  QString  entry;
  QString  key;

  sectMap.clear ();

  keys = sectionMap->keyList;

  for ( int i = 0; i < keys.count (); i++ )
  {
    key   = keys [i];
    entry = sectionMap->keyHash [key];

    sectMap [key] = entry;
  }

  return sectMap;

}              /*-----  getSectionMap  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getSectionKeys  of  ParFile
 |
 |  Description :
 |
 |   This function retrieves  all keys of the active  section and returns
 |   it in a string list.
 |
 +-------------------------------------------------------------------------*/

L_String ParFile::getSectionKeys ()
{

  return sectionMap->keyList;

}              /*-----  getSectionKeys  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getSectionNames  of  ParFile
 |
 |  Description :
 |
 |   This function returns all section names in a string list.
 |
 +-------------------------------------------------------------------------*/

L_String ParFile::getSectionNames ()
{

  return sectionNames;

}              /*-----  getSectionNames  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  deleteSection  of  ParFile
 |
 |  Description :
 |
 |   This function deletes the  complete section. If the 'incSubSections'
 |   parameter is  true then all subsections (sections  starting with the
 |   given name + "/") are also deleted.
 |
 |  Input parameters  :
 |   section          : Name of the section to be deleted.
 |   incSubSections   : Delete also the sub sections.
 |
 +-------------------------------------------------------------------------*/

void ParFile::deleteSection ( QString section, bool incSubSections )
{

  if (incSubSections)
  {
    // delete all sections starting with the name of the section + "/" or
    // equal to the section name
    for ( int i = 0; i < sectionNames.count (); i++ )
    {
      if ( ( section == sectionNames [i] ) or
           sectionNames [i].startsWith ( section + "/", Qt::CaseInsensitive ) )
      {
        deleteSection ( sectionNames [i] );
      }
    }
  }
  else
  {
    if ( parHash.contains (section) )
    {
      delete  parHash.value (section);

      parHash.remove (section);
      sectionNames.removeOne (section);
    }
  }

}              /*-----  deleteSection  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  emptySection  of  ParFile
 |
 |  Description :
 |
 |   This function  deletes the content  of the section, this  means only
 |   the  entries of  the  section but  not the  section  itself. If  the
 |   'incSubSections' parameter  is true  then all  subsections (sections
 |   starting with the given name + "/") are also cleared.
 |
 |  Input parameters  :
 |   section          : Name of the section to be cleared.
 |   incSubSections   : Delete also the sub sections.
 |
 +-------------------------------------------------------------------------*/

void ParFile::emptySection ( QString section, bool incSubSections )
{
  SectionMap  * entryMap;

  if (incSubSections)
  {
    // delete all sections starting with the name of the section + "/" or
    // equal to the section name
    for ( int i = 0; i < sectionNames.count (); i++ )
    {
      if ( ( section == sectionNames [i] ) or
           sectionNames [i].startsWith ( section + "/", Qt::CaseInsensitive ) )
      {
        emptySection ( sectionNames [i] );
      }
    }
  }
  else
  {
    if ( parHash.contains (section) )
    {
      entryMap = parHash.value (section);
      entryMap->keyHash.clear ();
      entryMap->keyList.clear ();
    }
  }

}              /*-----  emptySection  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getAsString  of  ParFile
 |
 |  Description :
 |
 |   This function returns the content of the parameter hash structure as
 |   string.
 |
 +-------------------------------------------------------------------------*/

QString ParFile::getAsString ()
{
  L_String keyList;
  QString  entry;
  QString  key;
  QString  content;

  content.clear ();

  for ( int i = 0; i < sectionNames.count (); i++ )
  {
    key = sectionNames [i];
    sectionMap = parHash.value (key);

    if ( !sectionMap )
      continue;

    sectionPtr = & sectionMap->keyHash;

    entry = "[" + key + "]\r\n";
    content.append (entry);

    keyList = sectionMap->keyList;

    for ( int j = 0; j < keyList.count (); j++ )
    {
      key   = keyList [j];
      entry = sectionPtr->value (key);

      entry = key + "=" + entry + "\r\n";
      content.append (entry);
    }
  }

  return content;

}               /*-----  getAsString  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getAsStringList  of  ParFile
 |
 |  Description :
 |
 |   This function returns the content of the parameter hash structure as
 |   string.
 |
 +-------------------------------------------------------------------------*/

L_String ParFile::getAsStringList ()
{
  L_String keyList;
  L_String content;
  QString  entry;
  QString  key;

  content.clear ();

  for ( int i = 0; i < sectionNames.count (); i++ )
  {
    key = sectionNames [i];
    sectionMap = parHash.value (key);

    if ( not sectionMap )
      continue;

    sectionPtr = & sectionMap->keyHash;

    entry = key;

    keyList = sectionMap->keyList;

    for ( int j = 0; j < keyList.count (); j++ )
    {
      entry = key + "/" + keyList [j];
      content.append (entry);
    }
  }

  return content;

}               /*-----  getAsStringList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getFileName  of  ParFile
 |
 |  Description :
 |
 |   This function returns the name of the parameter file  to  the  given
 |   name.
 |
 +-------------------------------------------------------------------------*/

QString ParFile::getFileName ()
{

  return fileName;

}              /*-----  getFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setFileName  of  ParFile
 |
 |  Description :
 |
 |   This function sets the name of the parameter file to the given name.
 |
 +-------------------------------------------------------------------------*/

void ParFile::setFileName ( QString fileName )
{

  this->fileName = fileName;

}              /*-----  setFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getSection  of  ParFile
 |
 |  Description :
 |
 |   This function returns the name of the current parameter section.
 |
 +-------------------------------------------------------------------------*/

QString ParFile::getSection ()
{

  return section;

}              /*-----  getSection  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setSection  of  ParFile
 |
 |  Description :
 |
 |   This function sets the parameter section to the given name.
 |
 +-------------------------------------------------------------------------*/

void ParFile::setSection ( QString section )
{

  this->section = section;

  if ( not parHash.contains (section) )
  {
    sectionMap = new SectionMap ();
    parHash.insert ( section, sectionMap );
    sectionNames.append (section);
  }
  else
  {
    sectionMap = parHash [section];
  }

  sectionPtr = & sectionMap->keyHash;

}              /*-----  setSection  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  entryExists  of  ParFile
 |
 |  Description :
 |
 |   This  function checks  if  the  given key  in  'section' exists  and
 |   returns the result.
 |
 |  Input parameters :
 |   entry           :  Entry in this section
 |
 |  Output parameter :
 |   (bool)          :  entry found or not
 |
 +-------------------------------------------------------------------------*/

bool ParFile::entryExists ( QString key )
{
  bool ok;

  ok = true;

  if ( sectionPtr and sectionPtr->contains (key) )
    ok = true;
  else
    ok = false;

  return ok;

}                   /*-----  entryExists  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  sectionExists  of  ParFile
 |
 |  Description :
 |
 |   This function  checks if  the given section  exists and  returns the
 |   result.
 |
 |  Input parameters :
 |   section         :  section name
 |
 |  Output parameter :
 |   (bool)          :  section found or not
 |
 +-------------------------------------------------------------------------*/

bool ParFile::sectionExists ( QString section )
{

  if ( sectionNames.indexOf (section) < 0 )
    return false;
  else
    return true;

}                   /*-----  sectionExists  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPar  of  ParFile
 |
 |  Description :
 |
 |   Here are  some overloaded  functions for  several basic  data types.
 |   This functions  try to read  the given  entry in 'section'  from the
 |   current   parameter   file   and   stores   it   in   the   variable
 |   'entryValue'. The calling function converts  it then to the specific
 |   data type.
 |
 |  Input parameters :
 |   key             :  Key in this section
 |
 |  Output parameter :
 |            (bool) :  true  - Key found
 |                      false - Key not found
 |
 +-------------------------------------------------------------------------*/

bool ParFile::findPar ( QString key )
{

  if ( sectionPtr and ( sectionPtr->count () > 0 ) and
       sectionPtr->contains (key) )
    entryValue = sectionPtr->value (key);
  else
    entryValue = "";

  if ( entryValue.isEmpty () )
    return false;
  else
    return true;

}                   /*-----  findPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "bool".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::findPar ( QString key, bool & par )
{

  if ( not findPar (key) )
    return false;

  if ( entryValue == "true" )
    par = true;
  else
    par = false;

  return true;

}                   /*-----  findPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "char".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::findPar ( QString key, char & par )
{

  if ( not findPar (key) )
    return false;

  par = (char) entryValue.toShort ( & ok );

  return ok;

}                   /*-----  findPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "unsigned char".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::findPar ( QString key, uchar & par )
{

  if ( not findPar (key) )
    return false;

  par = (uchar) entryValue.toShort ( & ok );

  return ok;

}                   /*-----  findPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "signed short".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::findPar ( QString key, short & par )
{

  if ( not findPar (key) )
    return false;

  par = entryValue.toShort ( & ok );

  return ok;

}                   /*-----  findPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "unsigned short".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::findPar ( QString key, ushort & par )
{

  if ( not findPar (key) )
    return false;

  par = entryValue.toUShort ( & ok );

  return ok;

}                   /*-----  findPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "int".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::findPar ( QString key, int & par )
{

  if ( not findPar (key) )
    return false;

  par = entryValue.toInt ( & ok );

  return ok;

}                   /*-----  findPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "uint".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::findPar ( QString key, uint & par )
{

  if ( not findPar (key) )
    return false;

  par = entryValue.toUInt ( & ok );

  return ok;

}                   /*-----  findPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "long".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::findPar ( QString key, long & par )
{

  if ( not findPar (key) )
    return false;

  par = entryValue.toLong ( & ok );

  return ok;

}                   /*-----  findPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "ulong".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::findPar ( QString key, ulong & par )
{

  if ( not findPar (key) )
    return false;

  par = entryValue.toULong ( & ok );

  return ok;

}                   /*-----  findPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "float".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::findPar ( QString key, float & par )
{

  if ( not findPar (key) )
    return false;

  par = entryValue.toFloat ( & ok );

  return ok;

}                   /*-----  findPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "double".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::findPar ( QString key, double & par )
{

  if ( not findPar (key) )
    return false;

  par = entryValue.toDouble ( & ok );

  return ok;

}                   /*-----  findPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "QString".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::findPar ( QString key, QString & par )
{

  if ( not findPar (key) )
    return false;

  par = entryValue;

  return true;

}                   /*-----  findPar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writePar  of  ParFile
 |
 |  Description :
 |
 |   This are  some overloaded  functions for  several basic  data types.
 |   They  convert  the  given  variable   to  a  string  with  the  name
 |   'entryValue' and call this function  to write the variable under the
 |   given key into 'section' of the given parameter file.
 |
 |  Input parameter   :
 |   key              :  Entry in this section
 |
 |  Output parameter  :
 |             (bool) :   true - Entry could be converted and written
 |                       false - Entry not be written or conversion error
 |
 +-------------------------------------------------------------------------*/

bool ParFile::writePar ( QString key )
{
  bool  ok;

  ok = false;

  if ( not parHash.contains (section) )
  {
    setSection (section);
  }

  // add the key if it is not already in the list
  if ( not sectionPtr->contains (key) )
    sectionMap->keyList.append (key);

  sectionPtr->insert ( key, entryValue );

  ok = true;

  return ok;

}                   /*-----  writePar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writePar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "bool".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::writePar ( QString key, bool par )
{

  if (par)
    entryValue = "true";
  else
    entryValue = "false";

  return writePar (key);

}                   /*-----  writePar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writePar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "short".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::writePar ( QString key, char par )
{

  entryValue = entryValue.setNum ( (short) par );

  return writePar (key);

}                   /*-----  writePar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writePar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "unsigned char".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::writePar ( QString key, uchar par )
{

  entryValue = entryValue.setNum ( (ushort) par );

  return writePar (key);

}                   /*-----  writePar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writePar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "signed short".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::writePar ( QString key, short par )
{

  entryValue = entryValue.setNum ( par );

  return writePar (key);

}                   /*-----  writePar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writePar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "unsigned short".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::writePar ( QString key, ushort par )
{

  entryValue = entryValue.setNum ( par );

  return writePar (key);

}                   /*-----  writePar  -----*/

/*-------------------------------------------------------------------------
 |
 |  Member function :  writePar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "int".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::writePar ( QString key, int par )
{

  entryValue = entryValue.setNum (par);

  return writePar (key);

}                   /*-----  writePar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writePar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "uint".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::writePar ( QString key, uint par )
{

  entryValue = entryValue.setNum (par);

  return writePar (key);

}                   /*-----  writePar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writePar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "ulong".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::writePar ( QString key, ulong par )
{

  entryValue = entryValue.setNum (par);

  return writePar (key);

}                   /*-----  writePar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writePar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "long".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::writePar ( QString key, long par )
{

  entryValue = entryValue.setNum (par);

  return writePar (key);

}                   /*-----  writePar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writePar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "float".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::writePar ( QString key, float par, int decimals )
{

  entryValue = entryValue.setNum ( par, 'f', decimals );

  return writePar (key);

}                   /*-----  writePar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writePar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "double".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::writePar ( QString key, double par, int decimals )
{

  entryValue = entryValue.setNum ( par, 'f', decimals );

  return writePar (key);

}                   /*-----  writePar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writePar  of  ParFile
 |
 |  Description :
 |
 |   This function is the overloaded version for "uint".
 |
 +-------------------------------------------------------------------------*/

bool ParFile::writePar ( QString key, QString par )
{

  entryValue = par;

  return writePar (key);

}                   /*-----  writePar  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  load  of  ParFile
 |
 |  Description :
 |
 |   This function loads the complete  content of the parameter file into
 |   a hash structure.
 |
 +-------------------------------------------------------------------------*/

bool ParFile::load ()
{
  QByteArray buffer;
  L_String   lineList;
  QString    line;
  QString    entry;
  QString    key;
  int        pos;
  QFile      inFile;
  bool       done;

  clear ();

  inFile.setFileName (fileName);

  if ( not inFile.open ( QIODevice::ReadOnly | QIODevice::Text ) )
  {
    return false;
  }

  done = false;

  // read file into string array
  while ( not done )
  {
    buffer = inFile.readLine ();

    if ( buffer.isEmpty () )
    {
      done = true;       // end of file encountered
    }
    else
    {
      line = QString::fromLatin1 ( buffer.trimmed () );

      if ( line.startsWith ( "[" ) and line.endsWith ( "]" ) )
      {
        line = line.remove ( 0, 1 );
        line = line.remove ( -1, 1 );

        // use setSection to take care for the correct structures
        setSection (line);
        continue;
      }

      if ( not sectionMap )
        continue;

      pos = line.indexOf ( "=" );

      if ( pos <= 0 )
        continue;

      key   = line.left (pos);
      entry = line.mid ( pos + 1 );
      key = key.trimmed ();
      entry = entry.trimmed ();

      sectionPtr->insert ( key, entry );
      sectionMap->keyList.append (key);
    }
  }

  inFile.close ();      // close file

  return true;

}               /*-----  load  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  save  of  ParFile
 |
 |  Description :
 |
 |   This function  saves the parameter  hash structure to  the parameter
 |   file.
 |
 +-------------------------------------------------------------------------*/

bool ParFile::save ()
{
  L_String keyList;
  QString  entry;
  QString  key;
  QFile    outFile;

  outFile.setFileName (fileName);

  if ( not outFile.open ( QIODevice::WriteOnly | QIODevice::Text ) )
  {
    return false;
  }

  for ( int i = 0; i < sectionNames.count (); i++ )
  {
    key = sectionNames [i];
    sectionMap = parHash.value (key);

    if ( !sectionMap )
      continue;

    sectionPtr = & sectionMap->keyHash;

    entry = "\n[" + key + "]\n";
    outFile.write ( entry.toLatin1 () );

    keyList = sectionMap->keyList;

    for ( int j = 0; j < keyList.count (); j++ )
    {
      key   = keyList [j];
      entry = sectionPtr->value (key);

      entry = key + "=" + entry + "\n";
      outFile.write ( entry.toLatin1 () );
    }
  }

  outFile.close ();      // close file

  return true;

}               /*-----  save  -----*/
