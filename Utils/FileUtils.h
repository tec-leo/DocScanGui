/*-------------------------------------------------------------------------
 |
 |  Include-File :  FileUtils.h
 |
 |  Description :
 |
 |   This include file contains the declarations of the module 'FileUtils'.
 |
 +-------------------------------------------------------------------------*/

#ifndef FileUtils_h
#define FileUtils_h


/*-------------------------------------------------------------------------
 |
 |  Class :  FileUtils
 |
 |  Description :
 |
 |   This class 'FileUtils' contains helper and utility functions to load
 |   text files, split file and path names and more.
 |
 +-------------------------------------------------------------------------*/

class  FileUtils : public QFile
{

  public:
    FileUtils ();
    FileUtils ( QString FileName );
    virtual ~FileUtils ();

  public:    // implement error logging ?
    void     addError       ( QString ) {};
    void     clearErrors    ()  {};

  public:
    QString  absFileName    ( QString fileName );
    QString  absolutePath   ( QString fileName );

    QString  cleanFileName  ( QString fileName );
    bool     compareFiles   ( QString file1, QString file2,
                              bool skipEmptyLines );
    bool     findFile       ( QString & fileName, L_String & dirs,
                              QString & foundDir );
    QString  findFile       ( QString name, L_String pathDir );
    L_String findRecursive  ( L_String nameFilters, L_String files );
    L_String findRecursive  ( L_String nameFilters, QString startDir );

    L_String getBaseNames   ( QString path, bool withSuffix );
    QString  getBaseName    ( QString fileName, bool withSuffix );

    QString  getFileName    ();
    void     setFileName    ( QString fileName );
    QFile  * getFile        ();
    void     closeFile      ();
    bool     createFile     ( QString newFile );
    bool     openFile       ( QIODevice::OpenMode mode );
    bool     openFile       ( bool text = true );

    bool     isDir          ( QString fileName );
    bool     makeFilePath   ( QString fileName );
    bool     makePath       ( QString dirPath );
    bool     readLine       ( QString & Line );
    bool     readLine       ( QFile & infile, QString & Line );
    bool     readLines      ( L_String & lines, int cnt );

    QString  joinFileName   ( L_String parts );
    L_String splitFileName  ( QString fileName );
    void     splitFileName  ( QString fileName, QString & path,
                              QString & base, QString & suffix, QString & name );

    bool     readFile       ( L_String & lines );
    bool     readFile       ( QString & content );
    bool     writeFile      ( L_String lines, bool append = false );
    bool     writeFile      ( QString content, bool append = false );
    bool     writeFile      ( QByteArray, bool append = false );

    L_String getSelFiles    ()   { return selFiles; }
    bool     selectFile     ( QString & fileName, QString filter,
                              bool load, bool mustExist = true );
    bool     selectFiles    ( L_String & files, QString filter, bool load );

  public:     // static functions
    static int  getSize     ( QString & name );
    static bool selectDir   ( QString & dirName, bool load );
    static bool selectDir   ( QString & dirName, bool load,
                              L_String & selected );

  protected:
    L_String selFiles;
    QDir     fileDir;
    QFile    inFile;
    QString  msg;
    QString  fileName;

};

#endif
