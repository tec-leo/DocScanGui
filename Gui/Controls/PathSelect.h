/*-------------------------------------------------------------------------
 |
 |  Include-File :  PathSelect.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   PathSelect.
 |
 +-------------------------------------------------------------------------*/

#ifndef PathSelect_h
#define PathSelect_h

#include "DropCombo.h"

/*-------------------------------------------------------------------------
 |
 |  Class :  PathSelect
 |
 |  Description :
 |
 |   This class implements a path or file selection widget as a reuseable
 |   component. It consists of a label, an edit field and a browse button
 |   to select a file or directory.
 |
 +-------------------------------------------------------------------------*/

class PathSelect : public QWidget
{

  Q_OBJECT

  public:
    PathSelect ( QWidget * parent );
    virtual ~PathSelect ();

    void          addPath        ( QString pathName );
    void          create         ();
    void          load           ();
    void          save           ();
    void          setHistory     ( QString section, QString key );
    void          setLabel       ( QString label );
    void          setIconName    ( QString name ) { iconName = name; }
    void          setIconSize    ( int size )     { iconSize = size; }

    QString       getPath        ();
    L_String      getSelection   ();
    void          setEditable    ( bool edit );
    void          setFilter      ( QString filter );
    void          setOnlyPath    ( bool onlyPath );

  signals:
    void          textChanged    ( QString text );

  protected slots:
    void          createLayout   ();
    void          connectSlots   ();
    void          onSelect       ();
    void          onTextChanged  ( int index );
    void          onTextChanged  ( QString pathName );

  protected:
    void          fromGui        ();
    void          toGui          ();

  protected:
    FileUtils     fileUtils;
    HistoryList   history;
    L_String      selFiles;

    QLabel      * lbPath;
    DropCombo   * dropCombo;
    QPushButton * btnBrowse;

    QString       iconName;
    QString       filter;
    QString       labelText;
    QString       editText;
    QString       section;
    QString       key;

    bool          onlyPath;
    bool          editable;
    int           iconSize;

};

#endif
