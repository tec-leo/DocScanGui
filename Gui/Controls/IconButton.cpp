/*-------------------------------------------------------------------------
 |
 |  File Name   :  IconButton.cpp
 |
 |  Description :
 |
 |   This module contains  the class 'IconButton' which draws  an icon at
 |   the left side and a text which is centered within the button.
 |
 |
 |  Developer : Oskar Leirich                Creation date : 16.Jul.2023
 |  Modified  : Oskar Leirich                Last changes  : 26.May.2025
 |
 |
 |  This unit contains following member functions :
 |   of IconButton :
 |    IconButton                ~IconButton               minimumSizeHint
 |    paintEvent                setIcon                   setText
 |    sizeHint                  text
 |
 +-------------------------------------------------------------------------*/

#include "IconButton.h"

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const int cIconSize = 24;

/*-------------------------------------------------------------------------
 |
 |  C O N S T R U C T O R  of  IconButton
 |
 |  Description :
 |
 |   The constructor initializes some variables.
 |
 |  Input parameters :
 |   parent          :
 |
 +-------------------------------------------------------------------------*/

IconButton::IconButton ( QWidget * parent ) : QPushButton (parent)
{
  QString style;

  icon         = nullptr;
  toggleAlways = false;

  setStyleSheet ( "QPushButton {background-color:  #eee9e9}" );

}               /*-----  IconButton  -----*/


/*-------------------------------------------------------------------------
 |
 |   E S T R U C T O R  of  IconButton
 |
 |  Description :
 |
 |   The destructor deletes some objects.
 |
 +-------------------------------------------------------------------------*/

IconButton::~IconButton ()
{


}               /*-----  ~IconButton  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  hitButton  of  IconButton
 |
 |  Description :
 |
 |   This function returns true if the pos is inside the clickable button
 |   rectangle;  otherwise  returns false.   The  clickable  area is  the
 |   entire widget. An additional check is  done if the pos is within the
 |   icon, in this  case the signal 'iconToggled' is sent  with the index
 |   of this button.
 |
 +-------------------------------------------------------------------------*/

bool IconButton::hitButton ( const QPoint & pos ) const
{
  QPoint hit;
  QPoint pnt;
  QRect  rect;
  int    index;
  bool   ok;

  if ( not VarUtils::containsPoint ( (QWidget *) this, pos ) )
    return false;

  ok   = false;

  hit  = mapToGlobal (pos);
  pnt  = mapToGlobal ( this->pos () );

  rect = this->rect ();
  rect.translate (pnt);

  if ( ( hit.x () < ( pnt.x () + cIconSize ) )and
       ( hit.y () < ( pnt.y () + cIconSize ) ) )
  {
    index = property ( "Index" ).toInt ();
    emit iconToggled (index);

    if (toggleAlways)
      ok = QPushButton::hitButton (pos);
  }
  else
    ok = QPushButton::hitButton (pos);

  return ok;

}               /*-----  hitButton  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  paintEvent  of  IconButton
 |
 |  Description :
 |
 |   This function is the event handler for painting widget.
 |
 |  Input parameters :
 |   event           : paint event
 |
 +-------------------------------------------------------------------------*/

void IconButton::paintEvent ( QPaintEvent * event )
{
  QPainter  painter;
  int       w, h;

  QPushButton::paintEvent (event);

  h = height () - 8;
  w = width  ();

  painter.begin (this);

  icon->paint ( & painter, QRect ( 4, 4, h, h ) );

  h = h + 8;
  painter.drawText ( QRect ( h, 0, w - h, h), Qt::AlignCenter, name );

  painter.end ();

}               /*-----  paintEvent  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setIcon  of  IconButton
 |
 |  Description :
 |
 |   This function sets the new icon for this button.
 |
 |  Input parameters :
 |   icon           : new icon
 |
 +-------------------------------------------------------------------------*/

void IconButton::setIcon ( const QIcon & icon )
{

  this->icon = (QIcon *) & icon;

}               /*-----  setIcon  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  setText  of  IconButton
 |
 |  Description :
 |
 |   This function sets the new text for this button.
 |
 |  Input parameters :
 |   text           : new text
 |
 +-------------------------------------------------------------------------*/

void IconButton::setText ( const QString & text )
{

  name = text;

  QPushButton::setText ("");

}               /*-----  setText  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  text  of  IconButton
 |
 |  Description :
 |
 |   This function returns the text for this button.
 |
 |  Output parameter  :
 |   QString          : text of the led
 |
 +-------------------------------------------------------------------------*/

QString IconButton::text () const
{

  return name;

}               /*-----  text  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  sizeHint  of  IconButton
 |
 |  Description :
 |
 |   This function returns the minimum size for the widget.
 |
 +-------------------------------------------------------------------------*/

QSize IconButton::sizeHint () const
{

  return QSize ( cIconSize, cIconSize );

}               /*-----  sizeHint  -----*/


/*-------------------------------------------------------------------------
 |
 |  Member function :  minimumSizeHint  of  IconButton
 |
 |  Description :
 |
 |   This function returns the recommended minimum size for the widget.
 |
 +-------------------------------------------------------------------------*/

QSize IconButton::minimumSizeHint () const
{

  return QSize ( cIconSize, cIconSize );

}               /*-----  minimumSizeHint  -----*/
