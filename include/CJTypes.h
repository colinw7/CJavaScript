#ifndef CJTypes_H
#define CJTypes_H

#include <memory>

class CJavaScript;

class CJArray;
class CJArguments;
class CJBoolean;
class CJDate;
class CJDictionary;
class CJFunctionBase;
class CJFunction;
class CJGetterSetter;
class CJJSON;
class CJLValue;
class CJMath;
class CJNameSpace;
class CJNumber;
class CJObj;
class CJObject;
class CJObjType;
class CJObjTypeFunction;
class CJOperator;
class CJRegExp;
class CJString;
class CJSystem;
class CJTokenValue;
class CJTypeFunction;
class CJUserObject;
class CJValue;

class CJErrorBase;
class CJError;
class CJTypeError;
class CJReferenceError;
class CJEvalError;
class CJRangeError;
class CJSyntaxError;
class CJURIError;

typedef std::shared_ptr<CJArray>           CJArrayP;
typedef std::shared_ptr<CJArguments>       CJArgumentsP;
typedef std::shared_ptr<CJBoolean>         CJBooleanP;
typedef std::shared_ptr<CJDate>            CJDateP;
typedef std::shared_ptr<CJDictionary>      CJDictionaryP;
typedef std::shared_ptr<CJFunctionBase>    CJFunctionBaseP;
typedef std::shared_ptr<CJFunction>        CJFunctionP;
typedef std::shared_ptr<CJGetterSetter>    CJGetterSetterP;
typedef std::shared_ptr<CJJSON>            CJJSONP;
typedef std::shared_ptr<CJLValue>          CJLValueP;
typedef std::shared_ptr<CJLValue>          CJLValueP;
typedef std::shared_ptr<CJMath>            CJMathP;
typedef std::shared_ptr<CJNameSpace>       CJNameSpaceP;
typedef std::shared_ptr<CJNumber>          CJNumberP;
typedef std::shared_ptr<CJObj>             CJObjP;
typedef std::shared_ptr<CJObject>          CJObjectP;
typedef std::shared_ptr<CJObjType>         CJObjTypeP;
typedef std::shared_ptr<CJObjTypeFunction> CJObjTypeFunctionP;
typedef std::shared_ptr<CJOperator>        CJOperatorP;
typedef std::shared_ptr<CJRegExp>          CJRegExpP;
typedef std::shared_ptr<CJString>          CJStringP;
typedef std::shared_ptr<CJSystem>          CJSystemP;
typedef std::shared_ptr<CJTokenValue>      CJTokenValueP;
typedef std::shared_ptr<CJTypeFunction>    CJTypeFunctionP;
typedef std::shared_ptr<CJUserObject>      CJUserObjectP;
typedef std::shared_ptr<CJValue>           CJValueP;

typedef std::shared_ptr<CJErrorBase>      CJErrorBaseP;
typedef std::shared_ptr<CJError>          CJErrorP;
typedef std::shared_ptr<CJTypeError>      CJTypeErrorP;
typedef std::shared_ptr<CJReferenceError> CJReferenceErrorP;
typedef std::shared_ptr<CJEvalError>      CJEvalErrorP;
typedef std::shared_ptr<CJRangeError>     CJRangeErrorP;
typedef std::shared_ptr<CJSyntaxError>    CJSyntaxErrorP;
typedef std::shared_ptr<CJURIError>       CJURIErrorP;

//------

class CJExecArray;
class CJExecAssert;
class CJExecAssignExpression;
class CJExecBlock;
class CJExecBreak;
class CJExecConst;
class CJExecContinue;
class CJExecDictionary;
class CJExecDo;
class CJExecExpression;
class CJExecExpressionList;
class CJExecFor;
class CJExecFunction;
class CJExecIdentifiers;
class CJExecIf;
class CJExecIncrDecrExpression;
class CJExecIndexExpression;
class CJExecLabel;
class CJExecNew;
class CJExecQuestion;
class CJExecReturn;
class CJExecSwitch;
class CJExecThis;
class CJExecThrow;
class CJExecTry;
class CJExecVar;
class CJExecWhile;
class CJExecWith;

typedef std::shared_ptr<CJExecArray>              CJExecArrayP;
typedef std::shared_ptr<CJExecAssert>             CJExecAssertP;
typedef std::shared_ptr<CJExecAssignExpression>   CJExecAssignExpressionP;
typedef std::shared_ptr<CJExecBlock>              CJExecBlockP;
typedef std::shared_ptr<CJExecBreak>              CJExecBreakP;
typedef std::shared_ptr<CJExecConst>              CJExecConstP;
typedef std::shared_ptr<CJExecContinue>           CJExecContinueP;
typedef std::shared_ptr<CJExecDictionary>         CJExecDictionaryP;
typedef std::shared_ptr<CJExecDo>                 CJExecDoP;
typedef std::shared_ptr<CJExecExpression>         CJExecExpressionP;
typedef std::shared_ptr<CJExecExpressionList>     CJExecExpressionListP;
typedef std::shared_ptr<CJExecFor>                CJExecForP;
typedef std::shared_ptr<CJExecFunction>           CJExecFunctionP;
typedef std::shared_ptr<CJExecIdentifiers>        CJExecIdentifiersP;
typedef std::shared_ptr<CJExecIf>                 CJExecIfP;
typedef std::shared_ptr<CJExecIncrDecrExpression> CJExecIncrDecrExpressionP;
typedef std::shared_ptr<CJExecIndexExpression>    CJExecIndexExpressionP;
typedef std::shared_ptr<CJExecLabel>              CJExecLabelP;
typedef std::shared_ptr<CJExecNew>                CJExecNewP;
typedef std::shared_ptr<CJExecQuestion>           CJExecQuestionP;
typedef std::shared_ptr<CJExecReturn>             CJExecReturnP;
typedef std::shared_ptr<CJExecSwitch>             CJExecSwitchP;
typedef std::shared_ptr<CJExecThis>               CJExecThisP;
typedef std::shared_ptr<CJExecThrow>              CJExecThrowP;
typedef std::shared_ptr<CJExecTry>                CJExecTryP;
typedef std::shared_ptr<CJExecVar>                CJExecVarP;
typedef std::shared_ptr<CJExecWhile>              CJExecWhileP;
typedef std::shared_ptr<CJExecWith>               CJExecWithP;

//------

enum class CJExceptionType {
  NONE,
  INDEX_SIZE_ERR = 1,
  DOMSTRING_SIZE_ERR = 2,
  HIERARCHY_REQUEST_ERR = 3,
  WRONG_DOCUMENT_ERR = 4,
  INVALID_CHARACTER_ERR = 5,
  NO_DATA_ALLOWED_ERR = 6,
  NO_MODIFICATION_ALLOWED_ERR = 7,
  NOT_FOUND_ERR = 8,
  NOT_SUPPORTED_ERR = 9,
  INUSE_ATTRIBUTE_ERR = 10,
  INVALID_STATE_ERR = 11,
  SYNTAX_ERR = 12,
  INVALID_MODIFICATION_ERR = 13,
  NAMESPACE_ERR = 14,
  INVALID_ACCESS_ERR = 15
};

#endif
