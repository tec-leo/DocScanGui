#-------------------------------------------------------------------------
#
#  Include-File :  Project include file
#
#  Description  :
#
#   This  is the include  file for source and header files and is included
#   from the project file (.pro).
#
#-------------------------------------------------------------------------*/

RESOURCES += Res/MainApp.qrc

include ($$PWD/Config/Config.pri)
include ($$PWD/Main/Main.pri)
include ($$PWD/Gui/Gui.pri)
include ($$PWD/Tools/Tools.pri)
include ($$PWD/Utils/Utils.pri)
