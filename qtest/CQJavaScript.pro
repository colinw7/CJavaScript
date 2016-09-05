TEMPLATE = app

QT += widgets

TARGET = CQJavaScript

DEPENDPATH += .

QMAKE_CXXFLAGS += -std=c++11

MOC_DIR = .moc

CONFIG += debug
CONFIG += silent

# Input
SOURCES += \
CQJavaScript.cpp \
CQJCanvasContext2D.cpp \
CQJCanvas.cpp \
CQJCanvasFontMetrics.cpp \
CQJCanvasImageDataArray.cpp \
CQJCanvasImageData.cpp \
CQJCanvasLinearGradient.cpp \
CQJCanvasPattern.cpp \
CQJCanvasRadialGradient.cpp \
CQJDialog.cpp \
CQJDocument.cpp \
CQJEvent.cpp \
CQJImage.cpp \
CQJIntervalFunction.cpp \
CQJObject.cpp \
CQJRequestAnimationFrame.cpp \
CQJSCanvas.cpp \
CQJWindow.cpp \
CQJWindowTimer.cpp \
\
CQHistoryLineEdit.cpp \

HEADERS += \
CQJavaScript.h \
CQJCanvasContext2D.h \
CQJCanvasFontMetrics.h \
CQJCanvas.h \
CQJCanvasImageDataArray.h \
CQJCanvasImageData.h \
CQJCanvasLinearGradient.h \
CQJCanvasPattern.h \
CQJCanvasRadialGradient.h \
CQJDialog.h \
CQJDocument.h \
CQJEvent.h \
CQJImage.h \
CQJIntervalFunction.h \
CQJObject.h \
CQJRequestAnimationFrame.h \
CQJSCanvas.h \
CQJUtil.h \
CQJWindow.h \
CQJWindowTimer.h \
\
CQHistoryLineEdit.h \

DESTDIR     = ../bin
OBJECTS_DIR = ../obj
LIB_DIR     = ../lib

INCLUDEPATH += \
. \
../include \
../../CJavaScript/include \
../../CQApp/include \
../../CQUtil/include \
../../CFont/include \
../../CImageLib/include \
../../CReadLine/include \
../../CFileUtil/include \
../../CFile/include \
../../COS/include \
../../CUtil/include \
../../CMath/include \
../../CStrUtil/include \
../../CRegExp/include \
../../CRGBName/include \
../../CCSS/include \
../../CXML/include \

unix:LIBS += \
-L$$LIB_DIR \
-L../../CJavaScript/lib \
-L../../CQUtil/lib \
-L../../CReadLine/lib \
-L../../CFont/lib \
-L../../CUtil/lib \
-L../../CImageLib/lib \
-L../../CConfig/lib \
-L../../CFileUtil/lib \
-L../../CFile/lib \
-L../../COS/lib \
-L../../CStrUtil/lib \
-L../../CRegExp/lib \
-L../../CRGBName/lib \
-lCJavaScript -lCQUtil -lCReadLine -lCConfig -lCUtil -lCFont -lCImageLib \
-lCRGBName -lCFileUtil -lCFile -lCOS -lCStrUtil -lCRegExp \
-lpng -ljpeg -ltre -lreadline