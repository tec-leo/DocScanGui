/*-------------------------------------------------------------------------
 |
 |  Include-File :  JobInfo.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  TcpMessage.
 |
 +-------------------------------------------------------------------------*/

#ifndef JobInfo_h
#define JobInfo_h


/*-------------------------------------------------------------------------
 |
 |  Class :  JobInfo
 |
 |  Description :
 |
 |   This  class defines  a  small  'job info'  class  with  only a  few
 |   properties like topic, command ...
 |
 +-------------------------------------------------------------------------*/

class JobInfo
{

  public:
    JobInfo ();
    virtual ~JobInfo ();

    void     init     ();
    void     parse    ( QString text );
    QString  toString ();

  public:
    M_StrStr infoMap;
    QString  topic;
    QString  command;
    QString  jobFile;
    QString  inFile;
    int      jobNo;

};

/*------------------------------------------*/
/*   T Y P E   -  D e c l a r a t i o n s   */
/*------------------------------------------*/

typedef  QQueue < JobInfo >  Q_JobInfo;

#endif
