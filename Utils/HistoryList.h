/*-------------------------------------------------------------------------
 |
 |  Include-File :  HistoryList.h
 |
 |  Description :
 |
 |   This  include file  contains all  class definitions  for the  module
 |   HistoryList.
 |
 +-------------------------------------------------------------------------*/

#ifndef HistoryList_h
#define HistoryList_h


/*-------------------------------------------------------------------------
 |
 |  Class :  HistoryList
 |
 |  Description :
 |
 |   This class allows  to load and save a list  of recently used entries
 |   like files and it provides also 'actions' for them.
 |
 +-------------------------------------------------------------------------*/

class HistoryList : public QObject
{

  Q_OBJECT

  public:
    HistoryList ();
    virtual ~HistoryList ();

    void     addActions     ( QMenu * menu );
    void     append         ( QString entry );
    void     deleteItem     ( QString entry );
    void     deleteItems    ();
    void     load           ( QString section, QString label, int size );
    void     save           ();
    L_String getItems       ();
    QString  getFirst       ();
    void     setSortList    ( bool sortList );
    void     setAppendList  ( L_String appendList );
    void     setPrependList ( L_String prependList );

  signals:
    void     itemSelected   ( QString file );

  protected slots:
    void     itemTriggered  ();

  protected:
    void     createActions  ();
    void     updateActions  ();

  protected:
    QList < QAction * > menuActions;
    L_String itemList;
    L_String appendList;
    L_String prependList;

    QString  section;
    QString  label;
    int      maxSize;
    int      maxItems;
    bool     sortList;
    bool     withMenu;     // use menu and menu actions

};

#endif
