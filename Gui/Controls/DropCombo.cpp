/*-------------------------------------------------------------------------
 |
 |  File Name   :  DropCombo.cpp
 |
 |  Description :
 |
 |   This module  contains the  class 'DropCombo'  which is  derived from
 |   QComboBox and implements  the drag and drop mechanism  for the combo
 |   box.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 22.Aug.2015
 |  Modified  : Oskar Leirich                Last changes  : 30.Apr.2026
 |
 |
 |  This unit contains following member functions :
 |   of DropCombo:
 |    DropCombo                 ~DropCombo                dragEnterEvent
 |    dropEvent
 |
 +-------------------------------------------------------------------------*/

#include "DropCombo.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  from  DropCombo
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 |  Input parameters :
 |   parent          : parent widget
 |
 +-------------------------------------------------------------------------*/

DropCombo::DropCombo ( QWidget * parent ) : QComboBox (parent)
{

  setAcceptDrops (true);

  //  setDragEnabled (true);

}               /*-----  DropCombo  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  DropCombo
 |
 |  Description :
 |
 |   The destructor is empty and just placeholder for the future.
 |
 +-------------------------------------------------------------------------*/

DropCombo::~DropCombo ()
{

}               /*-----  ~DropCombo  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  dragEnterEvent  of  DropCombo
 |
 |  Description :
 |
 |   This function initiates the drag process.
 |
 +-------------------------------------------------------------------------*/

void DropCombo::dragEnterEvent ( QDragEnterEvent * event )
{

  event->setDropAction ( Qt::CopyAction );

  if ( event->mimeData ()->hasText () )
  {
    event->acceptProposedAction ();
  }

}               /*-----  dragEnterEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  dropEvent  of  DropCombo
 |
 |  Description :
 |
 |   This function implements  the drop event. It will  move the selected
 |   items to the  index marked by the drop location  and will signal the
 |   moved indexes.
 |
 +-------------------------------------------------------------------------*/

void DropCombo::dropEvent ( QDropEvent * event )
{
  QString fileName;

  fileName = event->mimeData ()->text ();
  fileName = fileName.remove ( "file:///" );

  emit textDropped (fileName);

  event->acceptProposedAction ();

}               /*-----  dropEvent  -----*/

