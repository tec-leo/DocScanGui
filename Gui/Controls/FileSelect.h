/*-------------------------------------------------------------------------
 |
 |  Include-File :  FileSelect.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   FileSelect.
 |
 +-------------------------------------------------------------------------*/

#ifndef FileSelect_h
#define FileSelect_h

#include "../Controls/DropCombo.h"


/*-------------------------------------------------------------------------
 |
 |  Class :  FileSelect
 |
 |  Description :
 |
 |   This  class  implements  a  file selection  widget  as  a  reuseable
 |   component. It consists  of a label, a drop combo  box with a history
 |   and a browse button to select a file.
 |
 +-------------------------------------------------------------------------*/

class FileSelect : public QWidget
{

  Q_OBJECT

  public:
    FileSelect ( QWidget * parent );
    virtual ~FileSelect ();

    void          create         ();
    void          load           ();
    void          save           ();
    void          fromGui        ();
    void          toGui          ();

    void          addFile        ( QString fileName );
    void          addWidget      ( QWidget * widget );

    void          clearFiles     ();
    QString       getFileName    ();
    void          setFilter      ( QString filter );
    void          setHistory     ( QString section, QString key );
    void          setLabel       ( QString label );
    void          setIconName    ( QString name ) { iconName = name; }
    void          setIconSize    ( int size )     { iconSize = size; }
    void          setMustExist   ( bool mustExist );
    void          setLoadFile    ( bool load );
    void          setEditable    ( bool edit );

  signals:
    void          textChanged    ( QString text );

  protected slots:
    void          createLayout   ();
    void          connectSlots   ();
    void          onSelectFile   ();
    void          onIndexChanged ( int index );
    void          onTextChanged  ( const QString & text );
    void          onTextDropped  ( QString fileName );

  protected:
    QString       getCurrentText ();
    void          fileChanged    ( QString fileName );

  protected:
    FileUtils     fileUtils;
    HistoryList   history;

    QLabel      * lbCombo;
    DropCombo   * dropCombo;
    QPushButton * btnBrowse;
    QHBoxLayout * hLayout;
    QString       filter;
    QString       iconName;
    QString       labelText;
    QString       section;
    QString       key;
    bool          loadFile;
    bool          editable;
    bool          mustExist;
    bool          withHistory;
    int           iconSize;

};

#endif
