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
 |  Modified  : Oskar Leirich                Last changes  : 09.Nov.2026
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
 |   The constructor accepts drops.
 |
 |  Input parameters :
 |   parent          : parent widget
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
 |   The destructor is empty and just placeholder for the future.
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
 |   This event  handler is  called when  a drag is  in progress  and the
 |   mouse enters this widget.
 |
 +-------------------------------------------------------------------------*/

void DropLineEdit::dragEnterEvent ( QDragEnterEvent * event )
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
 |   This  event handler  is  called when  the drag  is  dropped on  this
 |   widget.  It  extracts a filename  from the mimeData and  signal this
 |   name.
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

