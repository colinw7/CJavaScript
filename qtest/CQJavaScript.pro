TEMPLATE = app

QT += widgets webkitwidgets

TARGET = CQJavaScriptTest

DEPENDPATH += .

MOC_DIR = .moc

QMAKE_CXXFLAGS += -std=c++11

CONFIG += debug

SOURCES += \
CQJMain.cpp \
CQJFrameWidget.cpp \

HEADERS += \
CQJFrameWidget.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

PRE_TARGETDEPS = \
$(LIB_DIR)/libCQJavaScript.a \
$(LIB_DIR)/libCJavaScript.a \

INCLUDEPATH = \
. \
../qinclude \
../include \
../../CWebGet/include \
../../CQApp/include \
../../CQUtil/include \
../../CJavaScript/include \
../../CQHistoryLineEdit/include \
../../CRegExp/include \
../../CMath/include \
../../CUtil/include \
../../COptVal/include \
../../COS/include \
../../CAutoPtr/include \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CJavaScript/qlib \
-L../../CJavaScript/lib \
-L../../CJson/lib \
-L../../CHtml/lib \
-L../../CCSS/lib \
-L../../CWebGet/lib \
-L../../CHtml/lib \
-L../../CXML/lib \
-L../../CQUtil/lib \
-L../../CImageLib/lib \
-L../../CFont/lib \
-L../../CCeil/lib \
-L../../CArgs/lib \
-L../../CReadLine/lib \
-L../../CConfig/lib \
-L../../CReadLine/lib \
-L../../CFile/lib \
-L../../CFileUtil/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../CGlob/lib \
-L../../CThread/lib \
-L../../CUtil/lib \
-L../../COS/lib \
\
-lCQJavaScript -lCQUtil \
-lCJavaScript -lCWebGet -lCCSS -lCXML -lCJson \
-lCConfig -lCReadLine -lCFont -lCImageLib \
-lCFile -lCStrUtil -lCGlob -lCRegExp \
-lCUtil -lCOS -lCFileUtil \
-lpng -ljpeg -ltre -lfreetype
