CC = g++
RM = rm

CDEBUG = -g
LDEBUG = -g

INC_DIR = ../include
OBJ_DIR = .
LIB_DIR = ../lib
BIN_DIR = ../bin

all: dirs $(BIN_DIR)/CJavaScriptTest

dirs:
	@if [ ! -e ../bin ]; then mkdir ../bin; fi

SRC = \
CJavaScriptTest.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
$(CDEBUG) \
-std=c++11 \
-I. \
-I$(INC_DIR) \
-I../../CReadLine/include \
-I../../CUtil/include \

LFLAGS = \
$(LDEBUG) \
-L$(LIB_DIR) \
-L../../CJavaScript/lib \
-L../../CReadLine/lib \
-L../../CFile/lib \
-L../../CStrUtil/lib \
-L../../COS/lib \

LIBS = \
-lCJavaScript -lCReadLine -lCFile -lCStrUtil -lCOS \
-lreadline

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(BIN_DIR)/CJavaScriptTest

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(BIN_DIR)/CJavaScriptTest: $(OBJS) $(LIB_DIR)/libCJavaScript.a
	$(CC) $(LDEBUG) -o $(BIN_DIR)/CJavaScriptTest $(OBJS) $(LFLAGS) $(LIBS)
