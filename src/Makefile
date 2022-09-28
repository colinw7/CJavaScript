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
	@if [ ! -e ../bin ]; then mkdir ../bin; fi

SRC = \
CJAlertFunction.cpp \
CJArguments.cpp \
CJArray.cpp \
CJArrayFunction.cpp \
CJavaScript.cpp \
CJBindFunction.cpp \
CJBoolean.cpp \
CJBooleanFunction.cpp \
CJCallFunction.cpp \
CJConsole.cpp \
CJDate.cpp \
CJDateFunction.cpp \
CJDebug.cpp \
CJDictionary.cpp \
CJDocument.cpp \
CJError.cpp \
CJErrorFunction.cpp \
CJExecArray.cpp \
CJExecAssert.cpp \
CJExecAssignExpression.cpp \
CJExecBlock.cpp \
CJExecBreak.cpp \
CJExecConst.cpp \
CJExecContinue.cpp \
CJExecData.cpp \
CJExecDictionary.cpp \
CJExecDo.cpp \
CJExecExpression.cpp \
CJExecExpressionList.cpp \
CJExecFor.cpp \
CJExecFunction.cpp \
CJExecIdentifiers.cpp \
CJExecIf.cpp \
CJExecIncrDecrExpression.cpp \
CJExecIndexExpression.cpp \
CJExecLabel.cpp \
CJExecNew.cpp \
CJExecQuestion.cpp \
CJExecReturn.cpp \
CJExecStatement.cpp \
CJExecSwitch.cpp \
CJExecThrow.cpp \
CJExecTry.cpp \
CJExecVar.cpp \
CJExecWhile.cpp \
CJExecWith.cpp \
CJFalse.cpp \
CJFunctionBase.cpp \
CJFunction.cpp \
CJFunctionFunction.cpp \
CJGetterSetter.cpp \
CJGlobalFunction.cpp \
CJIdentifier.cpp \
CJIntervalFunction.cpp \
CJJSON.cpp \
CJKeyword.cpp \
CJLValue.cpp \
CJMath.cpp \
CJNameSpace.cpp \
CJNameSpaceValue.cpp \
CJNull.cpp \
CJNumber.cpp \
CJNumberFunction.cpp \
CJObj.cpp \
CJObject.cpp \
CJObjectFunction.cpp \
CJObjectTypeFunction.cpp \
CJObjectValue.cpp \
CJObjFunction.cpp \
CJObjTypeFunction.cpp \
CJObjValue.cpp \
CJOperator.cpp \
CJPropertyData.cpp \
CJPropertyValue.cpp \
CJRandFunction.cpp \
CJRealFunction.cpp \
CJRegExp.cpp \
CJRegExpFunction.cpp \
CJRequireFunction.cpp \
CJString.cpp \
CJStringFunction.cpp \
CJSystem.cpp \
CJToken.cpp \
CJTokenValue.cpp \
CJTrue.cpp \
CJTypeFunction.cpp \
CJTypeObjectFunction.cpp \
CJTypeValue.cpp \
CJUndefined.cpp \
CJUserObject.cpp \
CJValue.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
-std=c++17 \
$(CDEBUG) \
-I. \
-I$(INC_DIR) \
-I../../CJson/include \
-I../../CFile/include \
-I../../CRegExp/include \
-I../../CStrUtil/include \
-I../../CUtil/include \
-I../../COS/include \

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(LIB_DIR)/libCJavaScript.a

.SUFFIXES: .cpp

$(OBJS): $(OBJ_DIR)/%.o: %.cpp
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

$(LIB_DIR)/libCJavaScript.a: $(OBJS)
	$(AR) crv $(LIB_DIR)/libCJavaScript.a $(OBJS)
