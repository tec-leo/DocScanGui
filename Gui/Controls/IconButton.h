/*-------------------------------------------------------------------------
 |
 |  Include-File :  IconButton.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  IconButton.
 |
 +-------------------------------------------------------------------------*/

#ifndef IconButton_h
#define IconButton_h


/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class IconButton;

/*------------------------------------------*/
/*   T Y P E  -  D e c l a r a t i o n s    */
/*------------------------------------------*/

typedef QList < IconButton * > L_IconButton;

/*-------------------------------------------------------------------------
 |
 |  Class :  IconButton
 |
 |  Description :
 |
 |   This class is a  push button widget which draws an  icon at the left
 |   side and a text which is centered within the button.
 |
 +-------------------------------------------------------------------------*/

class IconButton : public QPushButton
{

  Q_OBJECT

  public:
    IconButton ( QWidget * parent = 0 );
    virtual ~IconButton ();

    void    setIcon         ( const QIcon & icon );
    void    setText         ( const QString & text );
    QString text            () const;

    QSize   sizeHint        () const;
    QSize   minimumSizeHint () const;

  signals:
    void    iconToggled     ( int index )  const;

  protected:
    void    paintEvent      ( QPaintEvent * event );
    bool    hitButton       ( const QPoint & pos ) const;

  protected:
    QIcon * icon;
    QString name;
    bool    toggleAlways;

};

#endif
