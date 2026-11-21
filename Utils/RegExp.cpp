/*-------------------------------------------------------------------------
 |
 |  File Name   :  RegExp.cpp
 |
 |  Description :
 |
 |   This module contains the class  'RegExp' which is a convenient class
 |   derived  from QRegularExpression.   The  motivation is  to make  the
 |   'QRegularExpression' class more 'compatible'  to the older 'QRegExp'
 |   class and implements some comfortable functions.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 08.Dec.2023
 |  Modified  : Oskar Leirich                Last changes  : 19.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of RegExp :
 |    RegExp                    captured                  exactMatch
 |    indexIn                   isEmpty                   isValid
 |    match                     setIngoreCase
 |
 +-------------------------------------------------------------------------*/

#include "RegExp.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  RegExp
 |
 |  Description :
 |
 |   The constructor  provides the  just empty  derivation from  the base
 |   class QRegularExpression.
 |
 +-------------------------------------------------------------------------*/

RegExp::RegExp () : QRegularExpression ()
{


}               /*-----  RegExp  -----*/


RegExp::RegExp ( const QString & pattern, RegExpOptions options )
                                  : QRegularExpression ( pattern, options )
{

  rxPattern = pattern;

}               /*-----  RegExp  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  captured  of  RegExp
 |
 |  Description :
 |
 |   This   function    returns   the   text   captured    by   the   nth
 |   subexpression. The  entire match has  index 0 and  the parenthesized
 |   subexpressions have indexes starting from 1 (excluding non-capturing
 |   parentheses).
 |
 |  Input parameters :
 |   index           : index of last captured match
 |
 |  Output parameter :
 |   (QString)       : Text captured by the match with given index
 |
 +-------------------------------------------------------------------------*/

QString RegExp::captured ( int index )
{
  QString text;

  text = rxMatch.captured (index);

  return text;

}               /*-----  captured  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  exactMatch  of  RegExp
 |
 |  Description :
 |
 |   This function returns true if the  text string is matched exactly by
 |   this regular expression, otherwise it returns false.
 |
 |   You can  determine how  much of  the string  was matched  by calling
 |   matchedLength ().  For a given  regexp string R, exactMatch ("R") is
 |   the  equivalent of  indexIn("^R$") since  exactMatch ()  effectively
 |   encloses  the regexp  in  the  start of  string  and  end of  string
 |   anchors, except that it sets matchedLength () differently.
 |
 |   For example,  if the regular  expression is blue,  then exactMatch()
 |   returns true  only for input  blue. For inputs bluebell,  blutak and
 |   lightblue,  exactMatch () returns  false  and  matchedLength () will
 |   return 4, 3 and 0 respectively.
 |
 |  Output parameter :
 |   (bool)          : Pattern string valid or not
 |
 +-------------------------------------------------------------------------*/

bool RegExp::exactMatch ( const QString & text )
{

  rxMatch = match (text);

  if ( rxMatch.capturedLength () == text.length () )
    return true;
  else
    return false;

}               /*-----  exactMatch  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  indexIn  of  RegExp
 |
 |  Description :
 |
 |   This function attempts to find a match in the given text string from
 |   the position  offset (0 by  default).  If  offset is -1,  the search
 |   starts at the last character; if  -2, at the next to last character;
 |   etc. It returns the first found match  as index or -1 if there is no
 |   match.
 |
 |  Output parameter :
 |   (int)           : Index of Pattern string valid or not
 |
 +-------------------------------------------------------------------------*/

int RegExp::indexIn ( const QString & text, int offset )
{
  int index;

  rxMatch = match ( text, offset );

  if ( not rxMatch.hasMatch () )
    return -1;

  index = rxMatch.capturedStart ();

  return index;

}               /*-----  indexIn  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  hasMatch  of  RegExp
 |
 |  Description :
 |
 |   This function returns true if the regular expression matched against
 |   the subject string, or false otherwise returns false.
 |
 |  Output parameter :
 |   (bool)          : true - reg exp matched against the subject string
 |
 +-------------------------------------------------------------------------*/

bool RegExp::hasMatch ()
{

  return rxMatch.hasMatch ();

}               /*-----  hasMatch  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  hasMatch  of  RegExp
 |
 |  Description :
 |
 |   This function returns true if the regular expression matched against
 |   the subject string, or false otherwise returns false.
 |
 |  Output parameter :
 |   (bool)          : true - reg exp matched against the subject string
 |
 +-------------------------------------------------------------------------*/

bool RegExp::hasMatch ( const QString & text )
{

  match (text);

  return rxMatch.hasMatch ();

}               /*-----  hasMatch  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  isEmpty  of  RegExp
 |
 |  Description :
 |
 |   This function returns true if the pattern string is empty, otherwise
 |   returns false.
 |
 |  Output parameter :
 |   (bool)          : Pattern string empty or not
 |
 +-------------------------------------------------------------------------*/

bool RegExp::isEmpty ()
{
  QString rxPattern;

  rxPattern = pattern ();

  return rxPattern.isEmpty ();

}               /*-----  isEmpty  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  match  of  RegExp
 |
 |  Description :
 |
 |   This function attempts  to match the regular  expression against the
 |   given subject  string, starting  at the  position offset  inside the
 |   subject,  using a  match of  type matchType  and honoring  the given
 |   matchOptions.
 |   This function  overwrites the original  one to store  regExMatch and
 |   subject.
 |
 |  Output parameter :
 |   (RegExpMatch)   : Matcher for regular expression
 |
 +-------------------------------------------------------------------------*/

RegExpMatch RegExp::match ( const QString & subject,
                            qsizetype     offset,
                            RxMatchType   matchType,
                            RxMatchOption matchOptions )
{

  rxMatch = QRegularExpression::match ( subject, offset,
                                        matchType, matchOptions );

  this->subject = subject;

  return rxMatch;

}               /*-----  match  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setIgnoreCase  of  RegExp
 |
 |  Description :
 |
 |   This function  sets the case  sensitivity depending on  ignore. When
 |   ignore is true then the case matching is insentive.
 |
 |  Input parameters :
 |   ignore          : ignore case in matching
 |
 +-------------------------------------------------------------------------*/

void RegExp::setIgnoreCase ( bool ignore )
{
  RegExpOptions options;

  if (ignore)
    setPatternOptions ( QRegularExpression::CaseInsensitiveOption );
  else
  {
    options = patternOptions () & ~QRegularExpression::CaseInsensitiveOption;

    setPatternOptions (options);
  }

}               /*-----  setIgnoreCase  -----*/

