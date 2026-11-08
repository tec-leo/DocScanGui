/*-------------------------------------------------------------------------
 |
 |  Include-File :  ImageFileDlg.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   ImageFileDlg.
 |
 +-------------------------------------------------------------------------*/

#ifndef ImageFileDlg_h
#define ImageFileDlg_h

#include "../Widgets/ParListGrid.h"

/*------------------------------------------*/
/*   C L A S S  -  D e c l a r a t i o n s  */
/*------------------------------------------*/

class FileConfig;
class FileSelect;

/*-------------------------------------------------------------------------
 |
 |  Class :  ImageFileDlg
 |
 |  Description :
 |
 |   This class implements a configuration  dialog for loading and saving
 |   the image file formats and options.
 |
 +-------------------------------------------------------------------------*/

class ImageFileDlg : public QDialog
{

  Q_OBJECT

  public:
    ImageFileDlg ( QWidget * parent );
    virtual ~ImageFileDlg ();

  protected slots:
    void  onCancel       ();
    void  onOk           ();

  protected:
    void  connectSlots   ();
    void  createControls ();
    void  createLayout   ();
    void  initDialog     ();
    void  fromGui        ();
    void  toGui          ();

  protected:
    FileConfig  * fileConfig;
    FileSelect  * cbOcrProg;
    QLineEdit   * txtOcrPar;

    ParList     * parList;    // original parameter list of fileConfig

    ParListGrid * grid_1;     // grid layout for load image and ocr
    ParList       list_1;     // dialog list for load image and ocr

    ParListGrid * grid_2;    // grid layout for save image
    ParList       list_2;    // dialog list for save image

    QPushButton * btnCancel;
    QPushButton * btnOk;

};

#endif
