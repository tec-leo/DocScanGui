#-----------------------------------------------------------------------
#
#  This Qt project file contains the settings for DocScanGui
#
#-----------------------------------------------------------------------

TEMPLATE = app
TARGET = DocScanGui
INCLUDEPATH += .
INCLUDEPATH += $$PWD/
INCLUDEPATH += ./Main
#DEPENDPATH += $$PWD/

QMAKE_CXXFLAGS += -FS
QMAKE_CXXFLAGS_WARN_OFF -= -Wsign-compare
#QMAKE_LFLAGS += -static-libstdc++ -static-libgcc

CONFIG += precompile_header

PRECOMPILED_HEADER = Main/AppInc.h

QT += core gui widgets charts

include($$PWD/DocScanGui.pri)

# platform and compiler-specific values are in
# d:\Dev\Qt\6.12.0\msvc2022_64\mkspecs\...


# On windows, a shared object is a .dll
win32 {
  LibExt=lib

  OPENCV_BASE = "E:/Dev/Libs/OpenCV"
  OPENCV_LIBS = $${OPENCV_BASE}/lib
  OPENCV_INC  = $${OPENCV_BASE}/include

  INCLUDEPATH  += $${OPENCV_INC}

  CONFIG(debug, debug|release) {
    TARGET = DocScanGui_d
    LIBS  += $${OPENCV_BASE}/lib/opencv_world500d.lib
  }

  CONFIG(release, debug|release) {
    TARGET = DocScanGui
    LIBS  += $${OPENCV_BASE}/lib/opencv_world500.lib
  }

  LIBS += -lgdi32
  LIBS += -lgdiplus
  LIBS += -luser32
  LIBS += -lComDlg32
  LIBS += -lKernel32
  LIBS += -lWindowsApp
  LIBS += -lWS2_32
}

linux-g++ {

  # add linker path
  debug:OPENCV_LIBS_D = "../../opencv/debug/lib"
  release:OPENCV_LIBS = "../../opencv/release/lib"

  debug:LibExt=so    # shared library
  release:LibExt=so  # shared library

  OPENCV_INC    = "../../opencv/include/opencv5"
  INCLUDEPATH  += $${OPENCV_INC}

  CONFIG(debug, debug|release) {
    TARGET         = DocScanGui_d
    OPENCV_DIR     = $${OPENCV_LIBS_D}
    QMAKE_RPATHDIR = $${OPENCV_LIBS_D}
  }

  CONFIG(release, debug|release) {
    TARGET         = DocScanGui
    OPENCV_DIR     = $${OPENCV_LIBS}
    QMAKE_RPATHDIR = $${OPENCV_LIBS}
  }

  QMAKE_LIBDIR += $${OPENCV_DIR}

  LIBS += -lopencv_core
  LIBS += -lopencv_imgcodecs
  LIBS += -lopencv_highgui
  LIBS += -lopencv_imgproc
  LIBS += -lopencv_flann
  LIBS += -lopencv_geometry
}

