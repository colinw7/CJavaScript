TEMPLATE = lib

TARGET = CQJavaScript

QT += widgets

DEPENDPATH += .

CONFIG += staticlib

QMAKE_CXXFLAGS += \
-std=c++17 \
-DCQUTIL_PEN \
-DCQUTIL_BRUSH \
-DCQUTIL_FONT \
-DCQUTIL_IMAGE \
-DCBRUSH_IMAGE \

MOC_DIR = .moc

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
CQJCanvasWidget.cpp \
CQJColorEdit.cpp \
CQJComboBox.cpp \
CQJDialog.cpp \
CQJDocument.cpp \
CQJEvent.cpp \
CQJForm.cpp \
CQJHtmlCollection.cpp \
CQJHtmlObj.cpp \
CQJImage.cpp \
CQJImageObj.cpp \
CQJInput.cpp \
CQJIntervalFunction.cpp \
CQJLineEdit.cpp \
CQJLocation.cpp \
CQJObject.cpp \
CQJRequestAnimationFrame.cpp \
CQJSlider.cpp \
CQJWindow.cpp \
CQJWindowTimer.cpp \
\
CQHistoryLineEdit.cpp \

HEADERS += \
../qinclude/CQJavaScript.h \
../qinclude/CQJCanvasContext2D.h \
../qinclude/CQJCanvasFontMetrics.h \
../qinclude/CQJCanvas.h \
../qinclude/CQJCanvasIFace.h \
../qinclude/CQJCanvasImageDataArray.h \
../qinclude/CQJCanvasImageData.h \
../qinclude/CQJCanvasLinearGradient.h \
../qinclude/CQJCanvasPattern.h \
../qinclude/CQJCanvasRadialGradient.h \
../qinclude/CQJCanvasWidget.h \
../qinclude/CQJColorEdit.h \
../qinclude/CQJComboBox.h \
../qinclude/CQJDialog.h \
../qinclude/CQJDocument.h \
../qinclude/CQJDocumentIFace.h \
../qinclude/CQJEvent.h \
../qinclude/CQJForm.h \
../qinclude/CQJFormIFace.h \
../qinclude/CQJFormInputIFace.h \
../qinclude/CQJHtmlCollection.h \
../qinclude/CQJHtmlObjCSSTagData.h \
../qinclude/CQJHtmlObj.h \
../qinclude/CQJHtmlObjIFace.h \
../qinclude/CQJImage.h \
../qinclude/CQJImageIFace.h \
../qinclude/CQJImageObj.h \
../qinclude/CQJInput.h \
../qinclude/CQJIntervalFunction.h \
../qinclude/CQJLineEdit.h \
../qinclude/CQJLocation.h \
../qinclude/CQJObject.h \
../qinclude/CQJRequestAnimationFrame.h \
../qinclude/CQJSlider.h \
../qinclude/CQJUtil.h \
../qinclude/CQJWindow.h \
../qinclude/CQJWindowIFace.h \
../qinclude/CQJWindowTimer.h \
\
CQHistoryLineEdit.h \

DESTDIR     = ../lib
OBJECTS_DIR = ../obj

INCLUDEPATH = \
../qinclude \
../include \
../../CWebGet/include \
../../CCSS/include \
../../CUrl/include \
../../CQApp/include \
../../CQUtil/include \
../../CQPixmapCache/include \
../../CJavaScript/include \
../../CQHistoryLineEdit/include \
../../CImageLib/include \
../../CFont/include \
../../CFile/include \
../../CReadLine/include \
../../CStrParse/include \
../../CStrUtil/include \
../../CRegExp/include \
../../CMath/include \
../../CUtil/include \
../../COptVal/include \
../../COS/include \
../../CAutoPtr/include \
