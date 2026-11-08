/*-------------------------------------------------------------------------
 |
 |  Include-File :  ScrollWidget.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  ScrollWidget.
 |
 +-------------------------------------------------------------------------*/

#ifndef ScrollWidget_h
#define ScrollWidget_h


/*-------------------------------------------------------------------------
 |
 |  Class :  ScrollWidget
 |
 |  Description :
 |
 |   This  class implements  a widget  which provides  a scroll  area for
 |   another given widget.   The scroll area displays the  contents of the
 |   given child  widget within a frame.  It is added to  the main layout
 |   (QBoxLayout) of this widget.
 |
 +-------------------------------------------------------------------------*/

class ScrollWidget : public QWidget
{

  Q_OBJECT

  public:
    ScrollWidget ( QWidget * parent, QWidget * innerWidget );
    virtual ~ScrollWidget ();

  public:
    QScrollArea * getScrollArea ();

  protected:
    void   createLayout ( QWidget * innerWidget );

  protected:
    QScrollArea  * scrollArea;

};

#endif
