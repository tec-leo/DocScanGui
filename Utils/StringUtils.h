/*-------------------------------------------------------------------------
 |
 |  Include-File :  StringUtils.h
 |
 |  Description :
 |
 |   This  include file  contains all class   definitions for  the module
 |   'StringUtils'.
 |
 +-------------------------------------------------------------------------*/

#ifndef StringUtils_h
#define StringUtils_h


/*-------------------------------------------------------------------------
 |
 |  Class :  StringUtils
 |
 |  Description :
 |
 |   This  class  contains  some   'QString'  and  'QStringList'  utility
 |   functions e.g. for easy formatting.
 |
 +-------------------------------------------------------------------------*/

class StringUtils
{

  public:
    StringUtils ();
    virtual  ~StringUtils () {}

  public:  // static functions
    static  QString  beautifyReal ( double value, double precision = 6 );
    static  QString  beautifyReal ( float  value, double precision = 6 );

    static  QString  boolToString ( bool inVar );
    static  bool     stringToBool ( QString textStr );

    static  QString  formatI      ( int value, int width = 2,
                                    QChar fill = QChar ('0') );
    static  QString  formatF      ( double value, int precision = 3,
                                    char format = 'f' );
    static  QString  formatIndex  ( int index, QString pre = QString (),
                                    QString add  = QString () );
    static  L_String trimList     ( L_String lines );
    static  bool     validReal    ( QString value );
    static  bool     validInt     ( QString value );


  public:
    QString  defInt        ( int    value );
    QString  defReal       ( double value );

    QString  deleteRegion  ( QString text, int begIdx, int endIdx );

    QString  findNextWord  ( QString block, QString delim,
                             int begIdx, int & endIdx );
    QString  findPrevWord  ( QString block, QString delim,
                             int begIdx, int & endIdx );

    QString  getQuoted     ( QString text );
    QString  getString     ( L_Int intList, QString sep = QString ( "," ) );
    L_String getStringList ( L_Int intList );
    L_Double getDoubleList ( QString text, QString sep = QString ( "," ) );
    L_Float  getFloatList  ( QString text, QString sep = QString ( "," ) );
    L_Int    getIntList    ( QString text, QString sep = QString ( "," ) );

    bool     toString      ( QString & varStr, void * number,
                             QString varType, int decimals );
    bool     fromString    ( QString varStr, void * number,
                             QString varType );
    int      indexOf       ( L_String lines, QString pattern,
                             bool ignoreCase = true );
    M_StrStr listToMap     ( L_String entries );
    M_StrStr listToMap     ( QString entry, QString sep );

    void     setPrecision  ( int prec     ) { precision = prec;      }
    void     setWidth      ( int width    ) { this->width = width;   }
    void     setFill       ( QChar fill   ) { this->fill = fill;     }
    void     setFormat     ( char format  ) { this->format = format; }

  protected:
    QChar   fill;
    char    format;
    int     precision;
    int     width;

};

#endif
