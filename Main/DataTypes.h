/*-------------------------------------------------------------------------
 |
 |  Include-File :  DataTypes.h
 |
 |  Description  :
 |
 |   This  is the include  file for  standard   system include  files, or
 |   project specific  include  files that  are used  frequently, but are
 |   changed  not very often. This file  also contain the includes of all
 |   support dll includes in dll.h.
 |
 +-------------------------------------------------------------------------*/

#ifndef DataTypes_h
#define DataTypes_h

/*------------------------------------------*/
/*   C O N S T -  D e c l a r a t i o n s   */
/*------------------------------------------*/

const  QString  cEqualStr  = "=\"";
const  QString  cQuoteStr  = "\"";

const  QChar    cTab     = '\t';
const  QChar    cSpace   = ' ';
const  QChar    cSemiCol = ';';
const  QChar    cColon   = ':';
const  QChar    cComma   = ',';
const  QChar    cPoint   = '.';

const  double   eps     = + 1e-8;
const  double   POS_VAL = + 1e+8;
const  double   NEG_VAL = - 1e+8;

#define  ToString(string) (string).toLocal8Bit().constData()

#define mod %


/*------------------------------------------*/
/*   T Y P E  -  D e c l a r a t i o n s    */
/*------------------------------------------*/

typedef  size_t  qint;

// some basic types
typedef signed char         int8;        // 8 bits
typedef short int           int16;       // 16 bits
typedef int                 int32;       // 32 bits
typedef int64_t             int64;       // 64 bits
typedef unsigned char       uint8;       // 8 bits
typedef unsigned short int  uint16;      // 16 bits
typedef unsigned long int   uint32;      // 32 bits
typedef uint64_t            uint64;      // 64 bits

// common container types

typedef  QMap       < QString, QStringList >  M_StrStrList;
typedef  QMap       < QString, QString     >  M_StrStr;
typedef  QMap       < QString, int         >  M_StrInt;
typedef  QMap       < QString, double      >  M_StrDouble;
typedef  QMap       < QString, float       >  M_StrFloat;
typedef  QMap       < uint,    void *      >  M_UIntPtr;
typedef  QMap       < QString, void *      >  M_StrPtr;
typedef  QMap       < int,     QString     >  M_IntStr;
typedef  QMap       < int,     int         >  M_IntInt;
typedef  QMap       < int,     QStringList >  ML_IntStr;

typedef  QMultiMap  < QString, QString     >  MM_StrStr;
typedef  QMultiMap  < int,     int         >  MM_IntInt;
typedef  QMultiMap  < int,     QString     >  MM_IntString;
typedef  QMultiHash < int,     QString     >  MH_IntString;

typedef  QList      < bool                 >  L_Bool;
typedef  QList      < int                  >  L_Int;
typedef  QList      < float                >  L_Float;
typedef  QList      < double               >  L_Double;
typedef  QList      < double *             >  L_DoublePtr;
typedef  QList      < void *               >  L_Pointer;

// QStringList offers a few convenience functions
typedef  QList      < QStringList          >  LL_String;
typedef  QList      < L_Int                >  LL_Int;
typedef  QStringList                          L_String;
// typedef  QList   < QString              >  L_String;
// QStringList offers a few convenience functions like join, filter ...

typedef  QVector    < bool                 >  V_Bool;
typedef  QVector    < int                  >  V_Int;
typedef  QVector    < float                >  V_Float;
typedef  QVector    < double               >  V_Double;
typedef  QVector    < QByteArray           >  V_ByteArray;
typedef  QVector    < QString              >  V_String;
typedef  QVector    < QVariant             >  V_Variant;

typedef  QVector    < void  *              >  V_Pointer;
typedef  QVector    < float *              >  V_FloatPtr;
typedef  QVector    < V_Float              >  VV_Float;
typedef  QVector    < V_Double             >  VV_Double;
typedef  QVector    < V_Pointer            >  VV_Pointer;

typedef  QList      < QWidget *            >  L_Widgets;
typedef  QList      < QColor               >  L_QColor;
typedef  QList      < QImage *             >  L_ImagePtr;
typedef  QList      < QLabel *             >  L_LabelPtr;

typedef  QList      < QTreeWidgetItem  *   >  L_QTreeItems;
typedef  QList      < QListWidgetItem  *   >  L_QListItems;
typedef  QList      < QTableWidgetItem *   >  L_QTableItems;

typedef  QMap       < QTreeWidgetItem  *, QString > M_TreeItemStr;

#endif
