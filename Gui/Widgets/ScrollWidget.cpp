/*-------------------------------------------------------------------------
 |
 |  File Name   :  ScrollWidget.cpp
 |
 |  Description :
 |
 |   This module contains the  widget class 'ScrollWidget' which provides
 |   a scroll  area for another  given widget.  The scroll  area displays
 |   the contents of the given child  widget within a frame.  It is added
 |   to the main layout (QBoxLayout) of this widget.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 07.Dec.2025
 |  Modified  : Oskar Leirich                Last changes  : 07.Dec.2025
 |
 |
 |  This unit contains following member functions :
 |   of ScrollWidget :
 |    ScrollWidget              ~ScrollWidget             createLayout
 |    getScrollArea
 |
 +-------------------------------------------------------------------------*/

#include "ScrollWidget.h"


/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  ScrollWidget
 |
 |  Description :
 |
 |   This class contains icon buttons with state and name.
 |
 |  Input parameters :
 |   parent          : parent widget
 |
 +-------------------------------------------------------------------------*/

ScrollWidget::ScrollWidget ( QWidget * parent,
                             QWidget * innerWidget ) : QWidget (parent)
{

  createLayout (innerWidget);

}               /*-----  ScrollWidget  -----*/


/*-------------------------------------------------------------------------
 |
 |  D E S T R U C T O R  from  ScrollWidget
 |
 |  Description :
 |
 |   The destructor does the clean-up.
 |
 +-------------------------------------------------------------------------*/

ScrollWidget::~ScrollWidget ()
{

}                   /*-----  ScrollWidget  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  createLayout  of  ScrollWidget
 |
 |  Description :
 |
 |   This function creates the layout with a scroll area and a widget for
 |   all buttons.
 |
 +-------------------------------------------------------------------------*/

void ScrollWidget::createLayout ( QWidget * innerWidget )
{
  QHBoxLayout * hBox;

  hBox = new QHBoxLayout  ();

  scrollArea = new QScrollArea (this);

  scrollArea->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
  scrollArea->setVerticalScrollBarPolicy   ( Qt::ScrollBarAsNeeded );
  scrollArea->setWidgetResizable (true);

  scrollArea->setWidget (innerWidget);

  hBox->addWidget (scrollArea);

  setLayout (hBox);

}               /*-----  createLayout  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  getScrollArea  of  ScrollWidget
 |
 |  Description :
 |
 |   This  function  returns  a  pointer  to  the  'scrollArea'  of  this
 |   widget. It can be used to controll size and scroll policy.
 |
 |  Output parameter :
 |   (QScrollArea)   :  Pointer to the scrollArea
 |
 +-------------------------------------------------------------------------*/

QScrollArea * ScrollWidget::getScrollArea ()
{

  return scrollArea;

}               /*-----  getScrollArea  -----*/

