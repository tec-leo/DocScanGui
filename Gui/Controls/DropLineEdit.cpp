/*-------------------------------------------------------------------------
 |
 |  File Name   :  DropLineEdit.cpp
 |
 |  Description :
 |
 |   This module contains the class  'DropLineEdit' which is derived from
 |   QLineEdit and  implements the drag  and drop mechanism for  the line
 |   edit field.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 19.Feb.2023
 |  Modified  : Oskar Leirich                Last changes  : 20.Apr.2025
 |
 |
 |  This unit contains following member functions :
 |   of DropLineEdit:
 |    DropLineEdit                 ~DropLineEdit                dragEnterEvent
 |    dropEvent
 |
 +-------------------------------------------------------------------------*/

#include "DropLineEdit.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  DropLineEdit
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 +-------------------------------------------------------------------------*/

DropLineEdit::DropLineEdit ( QWidget * parent ) : QLineEdit (parent)
{

  setAcceptDrops (true);

}               /*-----  DropLineEdit  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  DropLineEdit
 |
 |  Description :
 |
 |   The destructor deletes the Dialog-Info structures.
 |
 +-------------------------------------------------------------------------*/

DropLineEdit::~DropLineEdit ()
{

}               /*-----  ~DropLineEdit  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  dragEnterEvent  of  DropLineEdit
 |
 |  Description :
 |
 |   This function initiates the drag process.
 |
 +-------------------------------------------------------------------------*/

void   DropLineEdit::dragEnterEvent ( QDragEnterEvent * event )
{

  event->setDropAction ( Qt::CopyAction );

  if ( event->mimeData ()->hasText () )
  {
    event->acceptProposedAction ();
  }

}               /*-----  dragEnterEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  dropEvent  of  DropLineEdit
 |
 |  Description :
 |
 |   This function implements  the drop event. It will  move the selected
 |   items to the  index marked by the drop location  and will signal the
 |   moved indexes.
 |
 +-------------------------------------------------------------------------*/

void  DropLineEdit::dropEvent ( QDropEvent * event )
{
  QString fileName;

  fileName = event->mimeData ()->text ();
  fileName = fileName.remove ( "file:///" );

  emit textDropped (fileName);

  event->acceptProposedAction ();

}               /*-----  dropEvent  -----*/

