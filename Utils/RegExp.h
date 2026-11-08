/*-------------------------------------------------------------------------
 |
 |  Include-File :  RegExp.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  RegExp.
 |
 +-------------------------------------------------------------------------*/

#ifndef RegExp_h
#define RegExp_h

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

#define RegExpValidator QRegularExpressionValidator
#define RegExpOptions   QRegularExpression::PatternOptions

#define RegExpMatch     QRegularExpressionMatch
#define RxMatchType     QRegularExpression::MatchType
#define RxMatchOption   QRegularExpression::MatchOption


/*-------------------------------------------------------------------------
 |
 |  Class :  RegExp
 |
 |  Description :
 |
 |   This class  is a  convenient class derived  from QRegularExpression.
 |   The  motivation  is  to  make the  'QRegularExpression'  class  more
 |   'compatible'  to  the  older  'QRegExp'  class  and  implement  some
 |   comfortable functions.
 |
 +-------------------------------------------------------------------------*/

class RegExp : public QRegularExpression
{

  public:
    RegExp ();
    RegExp ( const QString & pattern,
             RegExpOptions options = NoPatternOption );

    QString  captured      ( int index = 0 );
    bool     exactMatch    ( const QString & text );
    int      indexIn       ( const QString & text, int offset = 0 );
    bool     hasMatch      ( const QString & text );
    bool     hasMatch      ();
    bool     isEmpty       ();
    void     setIgnoreCase ( bool ignore );

    RegExpMatch match      ( const QString & subject,
                             qsizetype offset = 0,
                             RxMatchType   matchType    = NormalMatch,
                             RxMatchOption matchOptions = NoMatchOption );
  protected:
    RegExpMatch rxMatch;
    QString     rxPattern;
    QString     subject;

};

#endif
