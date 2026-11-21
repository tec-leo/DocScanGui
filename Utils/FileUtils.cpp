/*-------------------------------------------------------------------------
 |
 |  File Name   :  FileUtils.cpp
 |
 |  Description :
 |
 |   This module provides the class 'FileUtils' which contains helper and
 |   utility functions to load text files,  split file and path names and
 |   more.
 |
 |  Developer : Oskar Leirich                Creation date : 09.Jan.2022
 |  Modified  : Oskar Leirich                Last changes  : 19.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of FileUtils :
 |    FileUtils                 ~FileUtils                absFileName
 |    absolutePath              cleanFileName             closeFile
 |    compareFiles              createFile                findFile
 |    findRecursive             getBaseName               getBaseNames
 |    getFile                   getFileName               getSize
 |    isDir                     joinFileName              makeFilePath
 |    makePath                  openFile                  readFile
 |    readLine                  readLines                 selectDir
 |    selectFile                selectFiles               setFileName
 |    splitFileName             writeFile
 |
 +-------------------------------------------------------------------------*/

#include "FileUtils.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  FileUtils
 |
 |  Description :
 |
 |   The constructor initializes sets the given filename.
 |
 |  Input parameters :
 |   FileName        : file for the next operations
 |
 +-------------------------------------------------------------------------*/

FileUtils::FileUtils ( QString fileName ) : QFile ()
{

  setFileName (fileName);

}               /*-----  FileUtils  -----*/


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  FileUtils
 |
 |  Description :
 |
 |   This is the default constructor.
 |
 +-------------------------------------------------------------------------*/

FileUtils::FileUtils ()
{

}               /*-----  FileUtils  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  of  FileUtils
 |
 |  Description :
 |
 |   The destructor is emtpy and just placeholder for the future.
 |
 +-------------------------------------------------------------------------*/

FileUtils::~FileUtils ()
{

}               /*-----  ~FileUtils  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  absFileName  of  FileUtils
 |
 |  Description :
 |
 |   This function returns the absolute file name for the given name.
 |   It does not check if the files exists.
 |
 |  Input parameters :
 |   fileName        : original file name
 |
 |  Output parameter :
 |   (QString)       : absolute file name
 |
 +-------------------------------------------------------------------------*/

QString FileUtils::absFileName ( QString fileName )
{
  QFileInfo fileInfo;

  fileInfo.setFile (fileName);

  return fileInfo.absoluteFilePath ();

}               /*-----  absFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  absolutePath  of  FileUtils
 |
 |  Description :
 |
 |   This function returns  the absolute (canonical) path  the given file
 |   name.   The  path is  canonical,  this  means  it does  not  contain
 |   symbolic links or relative paths.
 |
 |  Input parameters :
 |   fileName        : file name
 |
 |  Output parameter :
 |   (QString)       : absolute path
 |
 +-------------------------------------------------------------------------*/

QString FileUtils::absolutePath ( QString fileName )
{
  QFileInfo fileInfo;
  QString   dirName;

  fileInfo.setFile (fileName);

  if ( fileInfo.exists () )
    dirName = fileInfo.canonicalPath ();
  else
    dirName = fileInfo.absolutePath ();

  return dirName;

}               /*-----  absolutePath  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  closeFile  of  FileUtils
 |
 |  Description :
 |
 |   This function closes the input file.
 |
 +-------------------------------------------------------------------------*/

void FileUtils::closeFile ()
{

  inFile.close ();

}               /*-----  closeFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createFile  of  FileUtils
 |
 |  Description :
 |
 |   This function creates  a file with the given name.  If the path does
 |   not exist it will be created first.
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::createFile ( QString newFile )
{
  QString path;

  path = absolutePath (newFile);

  if ( not fileDir.mkpath (path) )
    return false;

  setFileName (newFile);

  writeFile ( QString ( " " ) );

  return true;

}               /*-----  createFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  compareFiles  of  FileUtils
 |
 |  Description :
 |
 |   This function compares two files with  the given names line by line.
 |   When 'skipEmptyLines' is true then all empty lines are removed first
 |   from the string lists.
 |
 |  Input parameters :
 |   file1           : pathname for the first file
 |   file2           : pathname for the second file
 |   skipEmptyLines  : ignore empty files or not
 |
 |  Output parameter :
 |   (bool)          : true - both files are identical
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::compareFiles ( QString file1, QString file2,
                               bool skipEmptyLines )
{
  L_String content1;
  L_String content2;

  setFileName (file1);
  readFile (content1);

  setFileName (file2);
  readFile (content2);

  if (skipEmptyLines)
  {
    content1 = StringUtils::trimList (content1);
    content2 = StringUtils::trimList (content2);
  }

  if ( content1.count () != content2.count () )
    return false;

  for ( int i = 0; i < content1.count (); i++ )
  {
    if ( content1 [i] != content2 [i] )
      return false;
  }

  return true;

}               /*-----  compareFiles  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  cleanFileName  of  FileUtils
 |
 |  Description :
 |
 |   This  function returns  a  formated string  after replacing  invalid
 |   characters.
 |
 |  Input parameters :
 |   fileName        : original file name
 |
 |  Output parameter :
 |   (QString)       : valid file name
 |
 +-------------------------------------------------------------------------*/

QString FileUtils::cleanFileName ( QString fileName )
{
  QString tempFileName;

  tempFileName = fileName;

  tempFileName.replace ( " ", "_" );
  tempFileName.replace ( "*", "" );
  tempFileName.replace ( ">", "" );
  tempFileName.replace ( "<", "" );
  tempFileName.replace ( "/", "_" );
  tempFileName.replace ( "\\", "_" );
  tempFileName.replace ( "!", "" );
  tempFileName.replace ( "?", "" );
  tempFileName.replace ( "|", "" );

  return tempFileName;

}               /*-----  cleanFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getBaseName  of  FileUtils
 |
 |  Description :
 |
 |   This function  returns the base  file name  for the given  name.  It
 |   does not  check if the files  exists.  If 'withSuffix' is  true then
 |   the extension is added to the file name.
 |
 |  Input parameters :
 |   fileName        : file name without path
 |   withSuffix      : add extension (true) or not
 |
 |  Output parameter :
 |   (QString)       : base file name
 |
 +-------------------------------------------------------------------------*/

QString FileUtils::getBaseName ( QString fileName, bool withSuffix )
{
  QFileInfo fileInfo;
  QString   name;

  fileInfo.setFile (fileName);

  name = fileInfo.baseName ();

  if (withSuffix)
    name = name + "." + fileInfo.suffix ();

  return name;

}               /*-----  getBaseName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getBaseNames  of  FileUtils
 |
 |  Description :
 |
 |   This function  reads all files in  a directory and returns  only the
 |   unique base name without double  entries in a list.  If 'withSuffix'
 |   is true then the extension is added to the file name.
 |
 |  Input parameters :
 |   path            : Directory path for the search
 |   withSuffix      : add suffix (true) or not
 |
 |  Output parameter :
 |   (L_String)      : String list with the base names
 |
 +-------------------------------------------------------------------------*/

L_String FileUtils::getBaseNames ( QString path, bool withSuffix )
{
  QString   name;
  QFileInfo info;
  L_String  files;
  L_String  basenames;

  fileDir.setPath (path);

  // get all files in the directory
  files = fileDir.entryList ();

  basenames.clear ();

  // search for unique file names (base name)
  for ( int i = 0; i < files.size (); i++ )
  {
    info.setFile ( files.at (i) );

    name = info.baseName ();

    if ( name.isEmpty () )
      continue;

    if (withSuffix)
      name = name + "." + info.suffix ();

    if ( not basenames.contains ( name, Qt::CaseInsensitive ) )
    {
      basenames.append (name);
    }
  }

  return basenames;

}               /*-----  getBaseNames  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  openFile  of  FileUtils
 |
 |  Description :
 |
 |   This function tries  to open the input file in  with the given mode.
 |   The return value is always true, errors are collected.
 |
 |  Output parameter :
 |   (bool)          :  success or not
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::openFile ( QIODevice::OpenMode mode )
{

  if ( inFile.isOpen () )
    closeFile ();

  inFile.setFileName (fileName);

  if ( not inFile.open (mode) )
  {
    msg = "Cannot open file : " + fileName + "!!";
    addError (msg);

    return false;
  }

  return true;

}               /*-----  openFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  openFile  of  FileUtils
 |
 |  Description :
 |
 |   This  function tries  to  open the  input file  in  read only  mode.
 |   Optional it  can be opened in  text mode depending on  the parameter
 |   'text'.  The return value is success or not.
 |
 |  Output parameter :
 |   (bool)          :  success or not
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::openFile ( bool text )
{
  QIODevice::OpenMode mode;

  mode = QIODevice::ReadOnly;

  if (text)
    mode = mode | QIODevice::Text;

  return openFile (mode);

}               /*-----  openFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getFile  of  FileUtils
 |
 |  Description :
 |
 |   This function returns a pointer to the 'inFile' (QFile).
 |
 |  Output parameter :
 |   (inFile)        :  return a pointer to inFile
 |
 +-------------------------------------------------------------------------*/

QFile * FileUtils::getFile ()
{

  return & inFile;

}               /*-----  getFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getFileName  of  FileUtils
 |
 |  Description :
 |
 |   This function returns the current 'fileName'.
 |
 |  Output parameter :
 |   (QString)       : return variable fileName
 |
 +-------------------------------------------------------------------------*/

QString FileUtils::getFileName ()
{

  return fileName;

}               /*-----  getFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getSize  of  FileUtils
 |
 |  Description :
 |
 |   This function returns the size of the file with the  given  name  in
 |   bytes.
 |
 |  Input parameters :
 |   name            : Name of the file
 |
 |  Output parameter :
 |   (int)           : Size of the file
 |
 +-------------------------------------------------------------------------*/

int FileUtils::getSize ( QString & name )
{
  QFile  test;

  test.setFileName (name);

  if ( not test.exists () )
    return 0;
  else
    return test.size ();

}               /*-----  getSize  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findFile  of  FileUtils
 |
 |  Description :
 |
 |   This function searches in the given list of directories for the file
 |   'name' and returns  the full path if  it can be located  or an empty
 |   string if it was not found.
 |
 |  Input parameters :
 |   name            : name of the file
 |   pathDir         : list of directories to search
 |
 |  Output parameter :
 |   (QString)       : final directory if found or empty
 |
 +-------------------------------------------------------------------------*/

QString FileUtils::findFile ( QString name, L_String pathDir )
{
  QString dir;

  findFile ( name, pathDir, dir );

  return dir;

}               /*-----  findFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findFile  of  FileUtils
 |
 |  Description :
 |
 |   This function searches  a file with the given name  and path list on
 |   the hard drive.
 |
 |  Input parameters :
 |   fileName        : name of the file
 |   dirs            : list of directories to search for
 |   foundDir        : directory if found
 |
 |  Output parameter :
 |   (bool)          : true - file found in the expected location
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::findFile ( QString & fileName, L_String & dirs,
                           QString & foundDir )
{
  QFileInfo fileInfo;
  QString   dirPath;
  QString   fileDirPath;

  for ( int i = 0; i < dirs.count (); i++ )
  {
    dirPath = dirs.at (i);
    dirPath = QDir::fromNativeSeparators (dirPath);

    if ( not dirPath.endsWith ("/") )
    {
      dirPath.append ("/");
    }

    fileDirPath = dirPath + fileName;

    fileInfo.setFile (fileDirPath);

    if ( fileInfo.exists () )
    {
      foundDir = dirPath;
      return true;
    }
  }

  foundDir = "";

  return false;

}               /*-----  findFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findRecursive  of  FileUtils
 |
 |  Description :
 |
 |   This function  searches from  the given start  directory recursively
 |   for all files with the  given wildcard (globbing) filter. It returns
 |   all found  files in a string  list which contains the  full path for
 |   every match.
 |
 |  Input parameters :
 |   nameFilters     : file name filter - wildcard (globbing)
 |   startDir        : start directory list of directories to search
 |
 |  Output parameter :
 |   (QString)       : final list of found files
 |
 +-------------------------------------------------------------------------*/

L_String FileUtils::findRecursive ( L_String nameFilters, QString startDir )
{
  L_String foundList;
  L_String dirList;
  L_String subDirList;
  QString  curDir;
  QString  curFile;
  QDir     dir;

  dir.setPath (startDir);

  if ( not dir.exists () )
    return foundList;

  // search for all files which matches the nameFilters
  foundList = dir.entryList ( nameFilters, QDir::Files );

  // update all found files with the complete file path
  for ( int i = 0; i < foundList.count (); i++ )
  {
    curFile = foundList [i];
    curFile = dir.absoluteFilePath (curFile);
    foundList [i] = curFile;
  }

  // search for all subdirectories
  dirList = dir.entryList ( QDir::Dirs | QDir::NoDot | QDir::NoDotDot );

  // search for all files in all subdirectories recursively
  for ( int i = 0; i < dirList.count (); i++ )
  {
    curDir = startDir + "/" + dirList [i];

    subDirList = findRecursive ( nameFilters, curDir );

    // add found files to the final list
    if ( subDirList.count () > 0 )
      foundList.append (subDirList);
  }

  return foundList;

}               /*-----  findRecursive  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findRecursive  of  FileUtils
 |
 |  Description :
 |
 |   This function  checks for every file  or directory in 'files'  if it
 |   can  be found.   When  the entry  is a  directory  then it  searches
 |   recursively from this  start directory for all files  with the given
 |   wildcard (globbing) filter.  It returns  all found files in a string
 |   list.
 |
 |  Input parameters :
 |   nameFilters     : file name filter - wildcard (globbing)
 |   files           : files or directory to search for
 |
 |  Output parameter :
 |   (QString)       : final list of found files
 |
 +-------------------------------------------------------------------------*/

L_String FileUtils::findRecursive ( L_String nameFilters, L_String files )
{
  L_String  foundList;
  L_String  subDirList;
  QFileInfo fileInfo;
  QString   curFile;

  foundList.clear ();

  for ( int i = 0; i < files.count (); i++ )
  {
    curFile = files [i];

    fileInfo.setFile (curFile);

    if ( fileInfo.isFile () )
    {
      foundList.append (curFile);
      continue;
    }

    if ( fileInfo.isDir () )
    {
      subDirList = findRecursive ( nameFilters, curFile );

      // add found files to the final list
      if ( subDirList.count () > 0 )
        foundList.append (subDirList);
    }
  }

  return foundList;

}               /*-----  findRecursive  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  isDir  of  FileUtils
 |
 |  Description :
 |
 |   This function returns if the file with the given  name is a directory
 |   or not.
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::isDir ( QString fileName )
{
  QFileInfo fileInfo;

  fileInfo.setFile (fileName);

  return fileInfo.isDir ();

}               /*-----  isDir  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  makeFilePath  of  FileUtils
 |
 |  Description :
 |
 |   This function  creates a directory  for the  given file name  if the
 |   path to this file does not exist.
 |
 |  Input parameters :
 |   fileName        : file name
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::makeFilePath ( QString fileName )
{
  QFileInfo fileInfo;
  QString   path;
  QDir      dir;
  bool      ok;

  fileInfo.setFile (fileName);

  path = fileInfo.absolutePath ();

  dir.setPath (path);

  ok = true;

  if ( not dir.exists () )
  {
    ok = dir.mkpath (path);
  }

  return ok;

}               /*-----  makeFilePath  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  makePath  of  FileUtils
 |
 |  Description :
 |
 |   This function creates a directory for  the given path if it does not
 |   exist.
 |
 |  Input parameters :
 |   fileName        : path name
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::makePath ( QString dirPath )
{
  QString   path;
  QDir      dir;
  bool      ok;

  dir.setPath (dirPath);

  path = dir.absolutePath ();

  dir.setPath (path);

  ok = true;

  if ( not dir.exists () )
  {
    ok = dir.mkpath (path);
  }

  return ok;

}               /*-----  makePath  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  joinFileName  of  FileUtils
 |
 |  Description :
 |
 |   This function  joins the given  parts of  a complete file  name with
 |   path, base name and suffix and returns it in a string. The functions
 |   is the opposite of splitFileName.
 |
 |  Input parameters :
 |   parts           : parts of the file name
 |                      - absolutePath
 |                      - baseName
 |                      - suffix
 |                      - baseName + suffix (optional) -  not used
 |
 +-------------------------------------------------------------------------*/

QString FileUtils::joinFileName ( L_String parts )
{
  QString name;

  name = parts [0] + "/" +  parts [1] + "." + parts [2];

  return name;

}               /*-----  joinFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  splitFileName  of  FileUtils
 |
 |  Description :
 |
 |   This function  splits the given file  name into path, base  name and
 |   suffix and name + suffix.
 |
 |  Input parameters :
 |   fileName        : original file name
 |
 +-------------------------------------------------------------------------*/

void FileUtils::splitFileName ( QString fileName, QString & path,
                                QString & base,   QString & suffix,
                                QString & name )
{
  QFileInfo fileInfo;

  fileInfo.setFile (fileName);

  path   = fileInfo.path ();

  if ( path.isEmpty () or ( path == "." ) )
    path = "";
  else
    path = fileInfo.absolutePath ();

  base   = fileInfo.baseName ();
  suffix = fileInfo.suffix ();
  name   = base + "." + suffix;

}               /*-----  splitFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  splitFileName  of  FileUtils
 |
 |  Description :
 |
 |   This function  splits the given file  name into path, base  name and
 |   suffix and name + suffix and returns it in a string list.
 |
 |  Input parameters :
 |   fileName        : original file name
 |
 |  Output parameter :
 |   (L_String)      : list of the parts of the filename :
 |                      - absolutePath
 |                      - baseName
 |                      - suffix
 |                      - baseName + suffix
 |
 +-------------------------------------------------------------------------*/

L_String FileUtils::splitFileName ( QString fileName )
{
  QFileInfo fileInfo;
  L_String  parts;
  QString   path;
  QString   base;
  QString   suffix;
  QString   name;

  fileInfo.setFile (fileName);

  path   = fileInfo.absolutePath ();
  base   = fileInfo.baseName ();
  suffix = fileInfo.suffix ();
  name   = base + "." + suffix;

  parts.append (path);
  parts.append (base);
  parts.append (suffix);
  parts.append (name);

  return parts;

}               /*-----  splitFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  selectDir  of  FileUtils
 |
 |  Description :
 |
 |   This  function is  called when  one  or more  directories should  be
 |   selected.  The native dialog is not used because multiselection does
 |   not work on native dialog.
 |
 |  Input parameters :
 |   dirName         : Reference for start path and selected file
 |   load            : true - open file, false - save file
 |   selected        : all selected dialogs
 |
 |  Output parameter :
 |   (bool)          : true - file selected, false - selection canceled
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::selectDir ( QString & dirName, bool load, L_String & selected )
{
  QFileDialog fileDlg;
  bool        ok;

  fileDlg.setFileMode  (QFileDialog::Directory);
  fileDlg.setOption    (QFileDialog::ShowDirsOnly);
  fileDlg.setDirectory (dirName);
  fileDlg.setViewMode  (QFileDialog::Detail);
  fileDlg.setOption    ( QFileDialog::DontUseNativeDialog, true );

  if (load)
    fileDlg.setAcceptMode ( QFileDialog::AcceptOpen );
  else
    fileDlg.setAcceptMode ( QFileDialog::AcceptSave );
	
  ok = false;

  selected.clear ();

  if ( fileDlg.exec () == QDialog::Accepted )
  {
    selected = fileDlg.selectedFiles ();

    if ( selected.count () > 0 )
    {
      dirName = selected.at (0);  // the first selection in dirName
      ok = true;
    }
  }

  return ok;

}               /*-----  selectDir  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  selectDir  of  FileUtils
 |
 |  Description :
 |
 |   This function  is called when  a directory should be  selected, here
 |   the native dialog is used and multiselection does not work.
 |
 |  Input parameters :
 |   dirName         : Reference for start path and selected file
 |   load            : true - open file, false - save file
 |
 |  Output parameter :
 |   (bool)          : true - file selected, false - selection canceled
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::selectDir ( QString & dirName, bool load )
{
  QFileDialog fileDlg;
  L_String    tmpList;
  bool        ok;

  fileDlg.setFileMode  (QFileDialog::Directory);
  fileDlg.setOption    (QFileDialog::ShowDirsOnly);
  fileDlg.setDirectory (dirName);
  fileDlg.setViewMode  (QFileDialog::Detail);

  if (load)
    fileDlg.setAcceptMode ( QFileDialog::AcceptOpen );
  else
    fileDlg.setAcceptMode ( QFileDialog::AcceptSave );

  ok = false;

  if ( fileDlg.exec () == QDialog::Accepted )
  {
    tmpList = fileDlg.selectedFiles ();

    if ( tmpList.count () > 0 )
    {
      dirName = tmpList.at (0);
      ok = true;
    }
  }

  return ok;

}               /*-----  selectDir  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  selectFile  of  FileUtils
 |
 |  Description :
 |
 |   This function is called when a file should be selected for opening.
 |
 |  Input parameters :
 |   fileName        : Reference for start path and selected file
 |   filter          : Filter separated by semicolon
 |   load            : true - open file, false - save file
 |
 |  Output parameter :
 |   (bool)          : true - file selected, false - selection canceled
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::selectFile ( QString & fileName, QString filter,
                             bool load, bool mustExist )
{
  QFileDialog fileDlg;
  QFileInfo   info;
  L_String    filterList;
  QString     dirName;
  QString     suffix;
  bool        ok;

  filterList = filter.split ( ";" );

  fileDlg.setFilter (QDir::Files);
  filterList.append ( "All files (*.*)" );
  fileDlg.setNameFilters (filterList);

  info.setFile (fileName);

  suffix = info.completeSuffix ();

  if ( suffix.isEmpty () )     // fileName is a directory
  {
    dirName = fileName;

    if ( not dirName.isEmpty () )
      makePath (fileName);
  }
  else
  {
    dirName = absolutePath (fileName);

    fileDlg.selectFile (fileName);
  }

  fileDlg.setDirectory (dirName);

  if (load)
  {
    if (mustExist)
      fileDlg.setFileMode ( QFileDialog::ExistingFile );
    else
      fileDlg.setFileMode ( QFileDialog::AnyFile );

    fileDlg.setAcceptMode ( QFileDialog::AcceptOpen );
  }
  else
    fileDlg.setAcceptMode ( QFileDialog::AcceptSave );

  ok = false;

  if ( fileDlg.exec () == QDialog::Accepted )
  {
    selFiles = fileDlg.selectedFiles ();

    if ( selFiles.count () > 0 )
    {
      fileName = selFiles.at (0);
      ok = true;
    }
  }

  return ok;

}               /*-----  selectFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  selectFiles  of  FileUtils
 |
 |  Description :
 |
 |   This function is called when a file should be selected for opening.
 |
 |  Input parameters :
 |   fileName        : Reference for start path and selected file
 |   filter          : Filter separated by semicolon
 |   load            : true - open file, false - save file
 |
 |  Output parameter :
 |   (bool)          : true - file selected, false - selection canceled
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::selectFiles ( L_String & files, QString filter, bool load )
{
  QString fileName;
  bool    ok;

  if ( files.isEmpty () )
    fileName = "";
  else
    fileName = files [0];

  ok = selectFile ( fileName, filter, load );

  files = selFiles;

  return ok;

}               /*-----  selectFiles  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setFileName  of  FileUtils
 |
 |  Description :
 |
 |   This function sets  a new value for 'fileName', sets  it also in the
 |   QFile and clears all errors.
 |
 |  Input parameters :
 |   fileName        : New value for variable fileName
 |
 +-------------------------------------------------------------------------*/

void FileUtils::setFileName ( QString fileName )
{

  this->fileName = fileName;

  inFile.setFileName (fileName);

  clearErrors ();    // not yet implemented

}               /*-----  setFileName  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  readLine  of  FileUtils
 |
 |  Description :
 |
 |   This function reads one line from the given text file.
 |
 |  Input parameters :
 |   infile          : Reference to the open text file
 |   line            :
 |
 |  Output parameter :
 |   (bool)          : Status: true - no error, false - end of file
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::readLine ( QFile & file, QString & line )
{
  QByteArray buffer;
  bool       status;

  status = true;

  line.clear ();
  msg.clear ();

  buffer = file.readLine ();

  if ( buffer.isEmpty () )
  {
    status = false;       // end of file encountered
    msg    = "End of file encountered !";
  }
  else
  {
    line = buffer.trimmed ();    // remove trailing line feeds
  }

  return status;

}               /*-----  readLine  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  readLine  of  FileUtils
 |
 |  Description :
 |
 |   This function reads one line from the input file.
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::readLine ( QString & line )
{

  return readLine ( inFile, line );

}               /*-----  readLine  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  readLines  of  FileUtils
 |
 |  Description :
 |
 |   This function  reads the number of  lines from the given  text file.
 |   If it does not contains this amount of lines then the status 'false'
 |   is given back.
 |
 |  Input parameters :
 |   infile          : Reference to the open text file
 |   line            :
 |
 |  Output parameter :
 |   (bool)          : Status: true - no error, false - end of file
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::readLines ( L_String & lines, int cnt )
{
  QByteArray buffer;
  QString    line;
  bool       status;

  status = true;

  lines.clear ();
  line.clear ();
  msg.clear ();

  for ( int i = 0; i < cnt; i++ )
  {
    buffer = inFile.readLine ();

    if ( buffer.isEmpty () )
    {
      status = false;       // end of file encountered
      break;
    }
    else
    {
      line = buffer.trimmed ();    // remove trailing line feeds
      lines.append (line);
    }
  }

  return status;

}               /*-----  readLines  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  readFile  of  FileUtils
 |
 |  Description :
 |
 |   This function reads a text file into a string list.
 |
 |  Input parameters :
 |   lines           : String array containing all lines of the file
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::readFile ( L_String & lines )
{
  QString  text;

  if ( not readFile (text) )
    return false;

  lines = text.split ( "\n" );

  return true;

}               /*-----  readFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  readFile  of  FileUtils
 |
 |  Description :
 |
 |   This function reads a text file into  a string. On 32 bit system the
 |   file size was limited  to 2 GB, on 64 bit  systems the limitation is
 |   only the system hardware and operating system.
 |
 |  Input parameters :
 |   lines           : String containing the content of the file
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::readFile ( QString & content )
{
  qint64 fileSize;

  inFile.setFileName (fileName);

  if ( not inFile.open ( QIODevice::ReadOnly | QIODevice::Text ) )
  {
    msg = "Cannot open file : " + fileName +  " for reading !!";
    addError (msg);

    return false;
  }

  fileSize = inFile.size ();

  content = inFile.readAll ();

  inFile.close ();

  return true;

}               /*-----  readFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writeFile  of  FileUtils
 |
 |  Description :
 |
 |   This function writes the given string list as a text file with  the
 |   name given to this class when creating it.
 |
 |  Input parameters :
 |   lines           : String array containing all lines of the file
 |   append          : Open in AppendMode (true) or WriteOnly (default)
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::writeFile ( L_String lines, bool append )
{
  QByteArray  text;
  bool  ok;

  if (append)
    ok = inFile.open ( QIODevice::Append );
  else
    ok = inFile.open ( QIODevice::WriteOnly );

  if ( not ok )
  {
    msg = "Cannot open file : " + fileName +  " for writing !!";
    addError (msg);

    return false;
  }

  for ( int i = 0; i < lines.count (); i++ )
  {
    text = lines.at (i).toUtf8 () + "\n";
    inFile.write (text);
  }

  inFile.close ();      // close file

  return true;

}               /*-----  writeFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writeFile  of  FileUtils
 |
 |  Description :
 |
 |   This function writes  the given string as a text  file with the name
 |   given to this class when creating it.
 |
 |  Input parameters :
 |   content         : String containing content of the file
 |   append          : Open in AppendMode (true) or WriteOnly (default)
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::writeFile ( QString content, bool append )
{
  QByteArray  text;
  bool  ok;

  if (append)
    ok = inFile.open ( QIODevice::Append );
  else
    ok = inFile.open ( QIODevice::WriteOnly );

  if ( not ok )
  {
    msg = "Cannot open file : " + fileName +  " for writing !!";
    addError (msg);

    return false;
  }

  text = content.toLocal8Bit ();

  inFile.write (text);

  inFile.close ();      // close file

  return true;

}               /*-----  writeFile  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  writeFile  of  FileUtils
 |
 |  Description :
 |
 |   This function  writes the given byte  array as a text  file with the
 |   name given to this class when creating it.
 |
 |  Input parameters :
 |   content         : byte array containing content of the file
 |   append          : Open in AppendMode (true) or WriteOnly (default)
 |
 +-------------------------------------------------------------------------*/

bool FileUtils::writeFile ( QByteArray content, bool append )
{
  bool  ok;

  if (append)
    ok = inFile.open ( QIODevice::Append );
  else
    ok = inFile.open ( QIODevice::WriteOnly );

  if ( not ok )
  {
    msg = "Cannot open file : " + fileName +  " for writing !!";
    addError (msg);

    return false;
  }

  inFile.write (content);

  inFile.close ();      // close file

  return true;

}               /*-----  writeFile  -----*/

