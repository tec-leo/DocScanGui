/*-------------------------------------------------------------------------
 |
 |  File Name   :  JobInfo.cpp
 |
 |  Description :
 |
 |   This module contains  the class 'JobInfo' which defines  only a few
 |   properties like topic, command ...
 |
 |
 |  Developer : Oskar Leirich                Creation date : 23.Sep.2025
 |  Modified  : Oskar Leirich                Last changes  : 18.Nov.2026
 |
 |
 |  This unit contains following member functions :
 |   of JobInfo :
 |    JobInfo                  ~JobInfo                 init
 |    parse                     toString
 |
 +-------------------------------------------------------------------------*/

#include "AppInc.h"
#include "JobInfo.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  JobInfo
 |
 |  Description :
 |
 |   The  constructor   calls  the  init  function   to  initialize  some
 |   variables.
 |
 +-------------------------------------------------------------------------*/

JobInfo::JobInfo ()
{

  init ();

}               /*-----  JobInfo  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  of  JobInfo
 |
 |  Description :
 |
 |   The destructor is empty and just placeholder for the future.
 |
 +-------------------------------------------------------------------------*/

JobInfo::~JobInfo ()
{


}               /*-----  ~JobInfo  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  init  of  JobInfo
 |
 |  Description :
 |
 |   This function initializes the variables.
 |
 +-------------------------------------------------------------------------*/

void JobInfo::init ()
{

  infoMap.clear ();

  command = "";
  jobFile = "";
  inFile  = "";
  topic   = "";
  jobNo   = -1;

}               /*-----  init  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  parse  of  JobInfo
 |
 |  Description :
 |
 |   This  function  parses the  given  string  and  splits it  into  the
 |   infos. Currently not implemented.
 |
 +-------------------------------------------------------------------------*/

void JobInfo::parse ( QString text )
{

  text = "";

}               /*-----  parse  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  toString  of  JobInfo
 |
 |  Description :
 |
 |   This  function joins  all parts  of  a jog  info returns  them as  a
 |   string.
 |
 +-------------------------------------------------------------------------*/

QString JobInfo::toString ()
{
  QString  entry;

  entry = "Topic " + topic + " " + command;

  return entry;

}               /*-----  toString  -----*/
