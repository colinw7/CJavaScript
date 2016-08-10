CC = g++
AR = ar
RM = rm

CDEBUG = -g

INC_DIR = ../include
OBJ_DIR = ../obj
LIB_DIR = ../lib
BIN_DIR = ../bin

all: dirs $(LIB_DIR)/libCJavaScript.a

dirs:
	@if [ ! -e ../obj ]; then mkdir ../obj; fi
	@if [ ! -e ../lib ]; then mkdir ../lib; fi

SRC = \
CJArray.cpp \
CJArrayFunction.cpp \
CJavaScript.cpp \
CJBooleanFunction.cpp \
CJConsole.cpp \
CJDebug.cpp \
CJDictionary.cpp \
CJDictionaryRef.cpp \
CJDictionaryValue.cpp \
CJDocument.cpp \
CJExecArray.cpp \
CJExecAssignExpression.cpp \
CJExecBlock.cpp \
CJExecBreak.cpp \
CJExecContinue.cpp \
CJExecDelete.cpp \
CJExecDictionary.cpp \
CJExecExpression.cpp \
CJExecFor.cpp \
CJExecFunction.cpp \
CJExecIdentifiers.cpp \
CJExecIf.cpp \
CJExecIncrDecrExpression.cpp \
CJExecIndexExpression.cpp \
CJExecNew.cpp \
CJExecQuestion.cpp \
CJExecReturn.cpp \
CJExecSwitch.cpp \
CJExecThrow.cpp \
CJExecTry.cpp \
CJExecVar.cpp \
CJExecWhile.cpp \
CJExecWith.cpp \
CJFalse.cpp \
CJFunction.cpp \
CJKeyword.cpp \
CJNameSpace.cpp \
CJNull.cpp \
CJNumber.cpp \
CJNumberFunction.cpp \
CJObject.cpp \
CJObjectFunction.cpp \
CJObjectType.cpp \
CJObjectValue.cpp \
CJOperator.cpp \
CJRealFunction.cpp \
CJString.cpp \
CJStringFunction.cpp \
CJTrue.cpp \
CJTypeFunction.cpp \
CJTypeValue.cpp \
CJUndefined.cpp \
CJUserFunction.cpp \
CJUserObject.cpp \
CJValue.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
-std=c++11 \
$(CDEBUG) \
-I. \
-I$(INC_DIR) \
-I../../CFile/include \
-I../../CStrUtil/include \
-I../../COS/include \

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(LIB_DIR)/libCJavaScript.a

.SUFFIXES: .cpp

$(OBJS): $(OBJ_DIR)/%.o: %.cpp
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(LIB_DIR)/libCJavaScript.a: $(OBJS)
	$(AR) crv $(LIB_DIR)/libCJavaScript.a $(OBJS)
