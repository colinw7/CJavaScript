APPNAME = CQJavaScript

include($$(MAKE_DIR)/qt_app.mk)

SOURCES += \
CQJMain.cpp \
CQJavaScript.cpp \
CQJCanvas.cpp \
CQJCanvasContext2D.cpp \
CQJCanvasFontMetrics.cpp \
CQJCanvasImageDataArray.cpp \
CQJCanvasImageData.cpp \
CQJCanvasLinearGradient.cpp \
CQJCanvasPattern.cpp \
CQJCanvasRadialGradient.cpp \
CQJCanvasWidget.cpp \
CQJDialog.cpp \
CQJDocument.cpp \
CQJEvent.cpp \
CQJImage.cpp \
CQJObject.cpp \
CQJWindow.cpp \
CQJWindowTimer.cpp \
CQJIntervalFunction.cpp \
CQJRequestAnimationFrame.cpp \

HEADERS += \
CQJavaScript.h \
CQJCanvas.h \
CQJCanvasContext2D.h \
CQJCanvasFontMetrics.h \
CQJCanvasImageDataArray.h \
CQJCanvasImageData.h \
CQJCanvasLinearGradient.h \
CQJCanvasPattern.h \
CQJCanvasRadialGradient.h \
CQJCanvasWidget.h \
CQJDialog.h \
CQJDocument.h \
CQJEvent.h \
CQJImage.h \
CQJObject.h \
CQJWindow.h \
CQJWindowTimer.h \
CQJIntervalFunction.h \
CQJRequestAnimationFrame.h \

PRE_TARGETDEPS = \
$(LIB_DIR)/libCJavaScript.a \

INCLUDEPATH = \
$(INC_DIR) \
$(INC_DIR)/CQApp \
$(INC_DIR)/CQUtil \
$(INC_DIR)/CQPixmapCache \
$(INC_DIR)/CJavaScript \
$(INC_DIR)/CQHistoryLineEdit \
$(INC_DIR)/CStrParse \
$(INC_DIR)/CRegExp \
$(INC_DIR)/CMath \
$(INC_DIR)/CUtil \
$(INC_DIR)/COptVal \
$(INC_DIR)/COS \
$(INC_DIR)/CAutoPtr \

unix:LIBS += -L$(LIB_DIR) \
-lCQStyle $$QT_APP_LIBS -lCQPixmapCache \
-lCQHistoryLineEdit -lCHistory \
-lCJavaScript -lCJson -lCFont -lCImageLib -lCMath -lCFile \
-lCFreeType -lCUtil -lCStrUtil -lCGlob -lCRegExp -lCEnv \
-lCRGBUtil -lCEncode -lCTempFile \
-lCOSTime -lCOSUser -lCOSFile -lCOSNaN \
-lCAssert -lCStrParse -lCFileUtil -lCPrintF -lCStrNCase \
$$IMAGE_LIBS -ltre -lfreetype
