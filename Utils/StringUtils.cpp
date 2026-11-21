/*-------------------------------------------------------------------------
 |
 |  File Name   :  StringUtils.cpp
 |
 |  Description :
 |
 |   This module  contains the class 'StringUtils'  which implements some
 |   utility functions  for string and text  formatting, manipulation and
 |   converting.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 28.Jan.2021
 |  Modified  : Oskar Leirich                Last changes  : 20.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of StringUtils :
 |    StringUtils               beautifyReal              boolToString
 |    defInt                    defReal                   deleteRegion
 |    findNextWord              findPrevWord              formatF
 |    formatI                   formatIndex               fromString
 |    getDoubleList             getFloatList              getIntList
 |    getQuoted                 getString                 getStringList
 |    indexOf                   listToMap                 stringToBool
 |    toString                  trimList                  validInt
 |    validReal
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "StringUtils.h"

/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  StringUtils
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 +-------------------------------------------------------------------------*/

StringUtils::StringUtils ()
{

  fill      = QChar ( ' ' );
  format    = 'f';
  precision = 2;
  width     = 3;

}               /*-----  StringUtils  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  beautifyReal  of  StringUtils
 |
 |  Description :
 |
 |   This function  returns a  formatted string  from the  given floating
 |   point  value.  It  uses the  'f'  format, and  a precision  of 6  by
 |   default.  After the conversion any trailing '0' are removed and then
 |   a sole '.' if it has the resolution of an integer.
 |
 +-------------------------------------------------------------------------*/

QString StringUtils::beautifyReal ( double value, double precision )
{
  QString text;

  text.setNum ( value, 'f', precision );

  while ( text.endsWith ( '0' ) )
  {
    text.chop (1);
  }

  if ( text.endsWith ( '.' ) )
    text.chop (1);

  return text;

}               /*-----  beautifyReal  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  beautifyReal  of  StringUtils
 |
 |  Description :
 |
 |   This function  returns a  formatted string  from the  given floating
 |   point  value.  It  uses the  'f'  format, and  a precision  of 6  by
 |   default.  After the conversion any trailing '0' are removed and then
 |   a sole '.' if it has the resolution of an integer.
 |
 +-------------------------------------------------------------------------*/

QString StringUtils::beautifyReal ( float value, double precision )
{
  QString text;

  text.setNum ( value, 'f', precision );

  while ( text.endsWith ( '0' ) )
  {
    text.chop (1);
  }

  if ( text.endsWith ( '.' ) )
    text.chop (1);

  return text;

}               /*-----  beautifyReal  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  defInt  of  StringUtils
 |
 |  Description :
 |
 |   This  function returns  a formatted  string from  the given  integer
 |   value.  It uses the default width, base and fill char.
 |
 +-------------------------------------------------------------------------*/

QString StringUtils::defInt ( int value )
{
  QString text;

  text = QString ( "%1" ).arg ( value, width, 10, fill );

  return text;

}               /*-----  defInt  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  defReal  of  StringUtils
 |
 |  Description :
 |
 |   This function  returns a  formatted string  from the  given floating
 |   point value.  It uses the  default width, format, precision and fill
 |   char.
 |
 +-------------------------------------------------------------------------*/

QString StringUtils::defReal ( double value )
{
  QString text;

  text.setNum ( value, format, precision );

  text = QString ( "%1" ).arg ( value, width, format, precision, fill );

  return text;

}               /*-----  defReal  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  deleteRegion  of  StringUtils
 |
 |  Description :
 |
 |   This function deletes a text region and returns the remaining text.
 |
 +-------------------------------------------------------------------------*/

QString StringUtils::deleteRegion ( QString text, int begIdx, int endIdx )
{
  QString leftStr;
  QString rightStr;
  QString subStr;
  int     len;

  len = text.size () - endIdx;

  leftStr  = text.first  ( begIdx );
  rightStr = text.sliced ( endIdx, len );

  subStr   = leftStr + rightStr;

  return subStr;

}               /*-----  deleteRegion  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findNextWord  of  StringUtils
 |
 |  Description :
 |
 |   This function extracts a substring in the given string starting from
 |   the given index forward until the delimiter is found. It returns the
 |   substring and the index (reference)  of the delimiter in the string.
 |   The delimiter can be single character or a substring.
 |
 |  Input parameters :
 |    block          : text string
 |    delim          : delimiter to search for
 |    begIdx         : start index in the text string
 |    endIdx         : end index in the text string
 |
 |  Output parameter :
 |    word           : found word
 |
 +-------------------------------------------------------------------------*/

QString StringUtils::findNextWord ( QString block, QString delim,
                                    int begIdx, int & endIdx )
{
  int len;

  endIdx = block.indexOf ( delim, begIdx );

  len = endIdx - begIdx;

  return block.sliced ( begIdx, len );

}               /*-----  findNextWord  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  findPrevWord  of  StringUtils
 |
 |  Description :
 |
 |   This function extracts a substring in the given string starting from
 |   the given index  backward until the delimiter is  found.  It returns
 |   the  substring and  the index  (reference) of  the delimiter  in the
 |   string.  The delimiter can be single character or a substring.
 |
 |  Note : endIdx is before begIdx
 |
 |  Input parameters :
 |    block          : text string
 |    delim          : delimiter to search for
 |    begIdx         : start index in the text string
 |    endIdx         : begin index of word in the text string
 |
 |  Output parameter :
 |    word           : found word
 |
 +-------------------------------------------------------------------------*/

QString StringUtils::findPrevWord ( QString block, QString delim,
                                    int begIdx, int & endIdx )
{
  QString text;
  QString word;
  int     len;
  int     idx;

  len = delim.size ();

  idx = begIdx - len;

  endIdx = -1;
  word   = "";

  while ( idx >= 0 )
  {
    text = block.sliced ( idx, len );

    if ( text == delim )
    {
      endIdx = idx + len;

      len = begIdx - endIdx;

      word = block.sliced ( endIdx, len );

      break;
    }

    idx--;
  }

  return word;

}               /*-----  findPrevWord  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  formatIndex  of  StringUtils
 |
 |  Description :
 |
 |   This function  returns a formatted  string from the 'index'  of this
 |   method.  Optional a prepending and appending string can be given.
 |
 +-------------------------------------------------------------------------*/

QString StringUtils::formatIndex ( int index, QString pre, QString add )
{
  QString text;
  QString help;

  help.setNum (index);
  text  = pre + help + add;

  return text;

}               /*-----  formatIndex  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  formatI  of  StringUtils
 |
 |  Description :
 |
 |   This  function returns  a formatted  string from  the given  integer
 |   value.  It uses a width of 2 and a fill character '0' as default.
 |
 +-------------------------------------------------------------------------*/

QString StringUtils::formatI ( int value, int width, QChar fill )
{
  QString text;

  text = QString ( "%1" ).arg ( value, width, 10, fill );

  return text;

}               /*-----  formatI  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  formatF  of  StringUtils
 |
 |  Description :
 |
 |   This function  returns a  formatted string  from the  given floating
 |   point  value.  It  uses a  precision of  3 and  a fixed  notation as
 |   default.
 |
 +-------------------------------------------------------------------------*/

QString StringUtils::formatF ( double value, int precision, char format )
{
  QString text;

  text.setNum ( value, format, precision );

  return text;

}               /*-----  formatF  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getDoubleList  of  StringUtils
 |
 |  Description :
 |
 |   This function splits  the given string with the  given separator and
 |   converts each element to a double and returns all in a double list.
 |
 |  Input parameters :
 |   text            : Text string separated with 'sep'
 |   sep             : Separator used for splitting, default is ','
 |
 |  Output parameter :
 |   (L_Double)      : List of double floats
 |
 +-------------------------------------------------------------------------*/

L_Double StringUtils::getDoubleList ( QString text, QString sep )
{
  L_String parts;
  L_Double doubles;
  QString  value;
  double   val;
  bool     ok;

  parts = text.split (sep);

  for ( int i = 0; i < parts.count (); i++ )
  {
    value = parts [i];

    val = value.toDouble ( & ok );

    if (ok)
      doubles.append (val);
  }

  return doubles;

}               /*-----  getDoubleList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getFloatList  of  StringUtils
 |
 |  Description :
 |
 |   This function splits  the given string with the  given separator and
 |   converts each element to a float and returns all in a float list.
 |
 |  Input parameters :
 |   text            : Text string separated with 'sep'
 |   sep             : Separator used for splitting, default is ','
 |
 |  Output parameter :
 |   (L_Float)       : List of floats
 |
 +-------------------------------------------------------------------------*/

L_Float StringUtils::getFloatList ( QString text, QString sep )
{
  L_String parts;
  L_Float  floats;
  QString  value;
  float    val;
  bool     ok;

  parts = text.split (sep);

  for ( int i = 0; i < parts.count (); i++ )
  {
    value = parts [i];

    val = value.toFloat ( & ok );

    if (ok)
      floats.append (val);
  }

  return floats;

}               /*-----  getFloatList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getIntList  of  StringUtils
 |
 |  Description :
 |
 |   This function splits  the given string with the  given separator and
 |   converts each  element to  an integer  and return  all in  a integer
 |   list.
 |
 |  Input parameters :
 |   text            : Text string separated with 'sep'
 |   sep             : Separator used for splitting, default is ','
 |
 |  Output parameter :
 |   (L_Int)         : List of integers
 |
 +-------------------------------------------------------------------------*/

L_Int StringUtils::getIntList ( QString text, QString sep )
{
  L_String parts;
  L_Int    intList;
  QString  value;

  parts = text.split (sep);

  for ( int i = 0; i < parts.count (); i++ )
  {
    value = parts [i];

    if ( validInt (value) )
      intList.append ( value.toInt () );
  }

  return intList;

}               /*-----  getIntList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getQuoted  of  StringUtils
 |
 |  Description :
 |
 |   This function  puts double quotes  at the  beginning and end  of the
 |   given string and returns it.
 |
 |  Input parameters :
 |   text            : Input text string
 |
 |  Output parameter :
 |   (String)        : Quoted of string
 |
 +-------------------------------------------------------------------------*/

QString StringUtils::getQuoted ( QString text )
{
  QString quoted;

  quoted = cQuoteStr + text + cQuoteStr;

  return quoted;

}               /*-----  getQuoted  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getString  of  StringUtils
 |
 |  Description :
 |
 |   This  function splits  the  given integers  list  and converts  each
 |   element a string and joins them with the given separator.
 |
 |  Input parameters :
 |   intList         : List of integers
 |   sep             : Separator used for splitting, default is ','
 |
 |  Output parameter :
 |   (QString)       : Joined string with separator
 |
 +-------------------------------------------------------------------------*/

QString StringUtils::getString ( L_Int intList, QString sep )
{
  L_String parts;
  QString  text;

  parts = getStringList (intList);

  text = parts.join (sep);

  return text;

}               /*-----  getStringList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getStringList  of  StringUtils
 |
 |  Description :
 |
 |   This  function splits  the  given integers  list  and converts  each
 |   element to a string and returns all in a string list..
 |
 |  Input parameters :
 |   intList         : List of integers
 |   sep             : Separator used for joining
 |
 |  Output parameter :
 |   (L_String)      : List of strings
 |
 +-------------------------------------------------------------------------*/

L_String StringUtils::getStringList ( L_Int intList )
{
  L_String parts;
  QString  text;

  for ( int i = 0; i < intList.count (); i++ )
  {
    text.setNum ( intList [i] );

    parts.append (text);
  }

  return parts;

}               /*-----  getStringList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  indexOf  of  StringUtils
 |
 |  Description :
 |
 |   This function searches the given pattern (regular expression) in the
 |   string list  'lines'. It uses  indexOf of QStringList and  allows to
 |   use the regular expression either case sensitive or not.
 |
 |  Input parameters :
 |   lines           : string list to search
 |   pattern         : text pattern to search for
 |   ignoreCase      : case insensitive or not
 |
 |  Output parameter :
 |   (int)           : index in string list, -1 if not found
 |
 +-------------------------------------------------------------------------*/

int StringUtils::indexOf ( L_String lines, QString pattern, bool ignoreCase )
{
  QRegularExpression regEx;
  int     index;

  if (ignoreCase)
    regEx.setPatternOptions ( QRegularExpression::CaseInsensitiveOption );

  regEx.setPattern (pattern);

  index = lines.indexOf (regEx);

  return index;

}               /*-----  indexOf  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  boolToString  of  StringUtils
 |
 |  Description :
 |
 |   This function 'converts' the given boolean variable to a string with
 |   the text representation 'true' or 'false'.
 |
 |  Input parameters :
 |   inVar           : bool variable
 |
 |  Output parameter :
 |   (QString)       : text representation for bool
 |
 +-------------------------------------------------------------------------*/

QString StringUtils::boolToString ( bool inVar )
{

  if (inVar)
    return "true";
  else
    return "false";

}               /*-----  boolToString  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  stringToBool  of  StringUtils
 |
 |  Description :
 |
 |   This function 'converts' the given  string to a boolean variable, it
 |   does not  contain any error  handling if the  given string is  not a
 |   boolean representation.
 |
 |  Input parameters :
 |   textStr         : Text string of the variable
 |
 |  Output parameter :
 |   (bool)          : value for bool, defaults to false
 |
 +-------------------------------------------------------------------------*/

bool StringUtils::stringToBool ( QString textStr )
{

  if ( textStr.compare ( "true", Qt::CaseInsensitive ) == 0 )
    return true;
  else
    return false;

}               /*-----  stringToBool  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  listToMap  of  StringUtils
 |
 |  Description :
 |
 |   This function splits  every entry of the given string  list with the
 |   separator "="  and converts  the parts  into a key  and a  value and
 |   returns it in a string map.
 |
 |  Input parameters :
 |   entries         : String list, entries should be key=value
 |
 |  Output parameter :
 |   (M_StrStr)      : Converted string map
 |
 +-------------------------------------------------------------------------*/

M_StrStr StringUtils::listToMap ( L_String entries )
{
  L_String parts;
  M_StrStr entryMap;
  QString  text;
  QString  key;

  for ( int i = 0; i < entries.count (); i++ )
  {
    text = entries [i];
    parts = text.split ( "=" );

    key  = parts [0].trimmed ();
    text = parts [1].trimmed ();

    entryMap [key] = text;
  }

  return entryMap;

}               /*-----  listToMap  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  listToMap  of  StringUtils
 |
 |  Description :
 |
 |   This function splits every entry of the given string list  with  the
 |   given separator and converts the parts in to key  and  a  value  and
 |   returns it in a string map.
 |
 |  Input parameters :
 |   entry           : Text string separated with 'sep'
 |   sep             : Separator used for splitting, default is ','
 |
 |  Output parameter :
 |   (M_StrStr)      : Converted string map
 |
 +-------------------------------------------------------------------------*/

M_StrStr StringUtils::listToMap ( QString entry, QString sep )
{
  L_String entries;

  entries = entry.split (sep);

  return listToMap (entries);

}               /*-----  listToMap  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  toString  of  StringUtils
 |
 |  Description :
 |
 |   This function 'converts' the given  pointer to void number to string
 |   representation 'varStr', depending on the a variable type 'varType'.
 |
 |  Input parameters :
 |   varStr          : Text result of the conversion
 |   number          : Void pointer to the variable
 |   varType         : Type of the variable
 |   decimals        : Decimals in case of a float variable
 |
 |  Output parameter :
 |   (bool)          : success result of the conversion
 |
 +-------------------------------------------------------------------------*/

bool StringUtils::toString ( QString & varStr, void * number,
                             QString varType,  int decimals )
{
  bool bValue;

  varStr = "";

  if ( varType == "string" )
  {
    varStr = * (QString *) number;
    return true;
  }

  if ( varType == "bool" )
  {
    bValue = * (bool *) number;

    varStr = boolToString (bValue);
    return true;
  }

  if ( varType == "int" )
  {
    varStr.setNum ( * ( int * ) number );
    return true;
  }

  if ( varType == "long" )
  {
    varStr.setNum ( * ( long * ) number );
    return true;
  }

  if ( varType == "float" )
  {
    varStr.setNum ( * ( float * ) number, 'g', decimals );
    return true;
  }

  if ( varType == "double" )
  {
    varStr.setNum ( * ( double * ) number, 'g', decimals );
    return true;
  }

  return false;

}               /*-----  toString  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  fromString  of  StringUtils
 |
 |  Description :
 |
 |   This function  'converts' the given  string to a variable  where the
 |   pointer 'number' of type 'varType' points to.
 |
 |  Input parameters :
 |   varStr          : Text representation of the variable
 |   number          : Void pointer to the variable
 |   varType         : Type of the variable
 |
 |  Output parameter :
 |   (bool)          :  true  -  conversion without errors
 |                      false -  error during conversion
 |
 +-------------------------------------------------------------------------*/

bool StringUtils::fromString ( QString varStr, void * number, QString varType )
{
  bool ok;

  if ( varStr.isEmpty () )
    return false;        // empty String, no conversion possible

  if ( varType == "string" )
  {
    * (QString *) number = varStr;
    return true;
  }

  if ( varType == "bool" )
  {
    if ( varStr.compare ( "true", Qt::CaseInsensitive ) == 0 )
      * (bool *) number = true;
    else
      * (bool *) number = false;

    return true;
  }

  if ( varType == "int" )
  {
    * (int *) number = varStr.toInt ( & ok );
    return ok;
  }

  if ( varType == "long" )
  {
    * (long *) number = varStr.toLong ( & ok );
    return ok;
  }

  if ( varType == "float" )
  {
    * (float *) number = varStr.toFloat (  & ok );
    return ok;
  }

  if ( varType == "double" )
  {
    * (double *) number = varStr.toDouble ( & ok );
    return ok;
  }

  return false;

}               /*-----  fromString  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  trimList  of  StringUtils
 |
 |  Description :
 |
 |   This function  removes all emtpy  lines or lines which  contain only
 |   whitespaces from the given string list.
 |
 |  Input parameters :
 |   lines           : String list
 |
 |  Output parameter :
 |   (L_String)      : trimmed string list
 |
 +-------------------------------------------------------------------------*/

L_String StringUtils::trimList ( L_String lines )
{
  RegExp   regExp;
  L_String tmpList;
  QString  line;

  regExp.setPattern ( "^\\s*$" );

  for ( int i = 0; i < lines.count (); i++ )
  {
    line = lines [i];

    if ( not line.contains (regExp) )
      tmpList.append (line);
  }

  return tmpList;

}               /*-----  trimList  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  validReal  of  StringUtils
 |
 |  Description :
 |
 |   This function checks if the given string is a valid  floating  point
 |   number.
 |
 |  Input parameters :
 |   value           : String to check
 |
 |  Output parameter :
 |   (bool)          : true - valid
 |
 +-------------------------------------------------------------------------*/

bool StringUtils::validReal ( QString value )
{
  bool ok;

  value.toFloat ( & ok );

  return ok;

}               /*-----  validReal  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  validInt  of  StringUtils
 |
 |  Description :
 |
 |   This function checks if the given string is a valid  floating  point
 |   number.
 |
 |  Input parameters :
 |   value           : String to check
 |
 |  Output parameter :
 |   (bool)          : true - valid
 |
 +-------------------------------------------------------------------------*/

bool StringUtils::validInt ( QString value )
{
  bool ok;

  value.toInt ( & ok );

  return ok;

}               /*-----  validInt  -----*/

