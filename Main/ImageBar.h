/*-------------------------------------------------------------------------
 |
 |  Include-File :  ImageBar.h
 |
 |  Description :
 |
 |  This include file contains all class definitions  for  the  module
 |  ImageBar.
 |
 +-------------------------------------------------------------------------*/

#ifndef ImageBar_h
#define ImageBar_h


/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class MainWnd;


/*-------------------------------------------------------------------------
 |
 |  Class :  ImageBar
 |
 |  Description :
 |
 |   This class implements a toolbar for the image editing functionlity.
 |
 +-------------------------------------------------------------------------*/

class ImageBar : public QToolBar
{

  friend class MainWnd;

  Q_OBJECT

  public:
    ImageBar ( MainWnd * mainWnd );
    virtual ~ImageBar ();

  protected:
    void   createActions  ();
    void   connectActions ();
    void   buildToolBar   ();

  protected:
    QAction * curves;
    QAction * crop;
    QAction * copy;
    QAction * rotate;
    QAction * deskew;
    QAction * merge;

    QAction * cancel;
    QAction * apply;

};

#endif
