#ifndef CJTypes_H
#define CJTypes_H

#include <memory>

class CJavaScript;

class CJArray;
class CJArrayType;
class CJArguments;
class CJBoolean;
class CJBooleanType;
class CJDate;
class CJDateType;
class CJDictionary;
class CJFalse;
class CJFalseType;
class CJFunctionBase;
class CJFunction;
class CJGetterSetter;
class CJGlobalFunction;
class CJIdentifier;
class CJJSON;
class CJKeyword;
class CJLValue;
class CJMath;
class CJMathFunction;
class CJMaxFunction;
class CJMinFunction;
class CJNameSpace;
class CJNumber;
class CJNumberType;
class CJObj;
class CJObject;
class CJObjectType;
class CJObjectTypeFunction;
class CJObjFunction;
class CJObjType;
class CJObjTypeFunction;
class CJOperator;
class CJRandFunction;
class CJReal2Function;
class CJRegExp;
class CJRegExpType;
class CJString;
class CJStringType;
class CJSystem;
class CJToken;
class CJTokenValue;
class CJTrue;
class CJTrueType;
class CJTypeFunction;
class CJUndefined;
class CJUndefinedType;
class CJUserObject;
class CJValue;

class CJErrorBase;
class CJError;
class CJErrorType;
class CJTypeError;
class CJTypeErrorType;
class CJReferenceError;
class CJReferenceErrorType;
class CJEvalError;
class CJEvalErrorType;
class CJRangeError;
class CJRangeErrorType;
class CJSyntaxError;
class CJSyntaxErrorType;
class CJURIError;
class CJURIErrorType;

typedef std::shared_ptr<CJArray>              CJArrayP;
typedef std::shared_ptr<CJArrayType>          CJArrayTypeP;
typedef std::shared_ptr<CJArguments>          CJArgumentsP;
typedef std::shared_ptr<CJBoolean>            CJBooleanP;
typedef std::shared_ptr<CJBooleanType>        CJBooleanTypeP;
typedef std::shared_ptr<CJDate>               CJDateP;
typedef std::shared_ptr<CJDateType>           CJDateTypeP;
typedef std::shared_ptr<CJDictionary>         CJDictionaryP;
typedef std::shared_ptr<CJFalse>              CJFalseP;
typedef std::shared_ptr<CJFalseType>          CJFalseTypeP;
typedef std::shared_ptr<CJFunctionBase>       CJFunctionBaseP;
typedef std::shared_ptr<CJFunction>           CJFunctionP;
typedef std::shared_ptr<CJGetterSetter>       CJGetterSetterP;
typedef std::shared_ptr<CJGlobalFunction>     CJGlobalFunctionP;
typedef std::shared_ptr<CJIdentifier>         CJIdentifierP;
typedef std::shared_ptr<CJJSON>               CJJSONP;
typedef std::shared_ptr<CJKeyword>            CJKeywordP;
typedef std::shared_ptr<CJLValue>             CJLValueP;
typedef std::shared_ptr<CJLValue>             CJLValueP;
typedef std::shared_ptr<CJMath>               CJMathP;
typedef std::shared_ptr<CJMathFunction>       CJMathFunctionP;
typedef std::shared_ptr<CJMaxFunction>        CJMaxFunctionP;
typedef std::shared_ptr<CJMinFunction>        CJMinFunctionP;
typedef std::shared_ptr<CJNameSpace>          CJNameSpaceP;
typedef std::shared_ptr<CJNumber>             CJNumberP;
typedef std::shared_ptr<CJNumberType>         CJNumberTypeP;
typedef std::shared_ptr<CJObj>                CJObjP;
typedef std::shared_ptr<CJObject>             CJObjectP;
typedef std::shared_ptr<CJObjectType>         CJObjectTypeP;
typedef std::shared_ptr<CJObjectTypeFunction> CJObjectTypeFunctionP;
typedef std::shared_ptr<CJObjFunction>        CJObjFunctionP;
typedef std::shared_ptr<CJObjType>            CJObjTypeP;
typedef std::shared_ptr<CJObjTypeFunction>    CJObjTypeFunctionP;
typedef std::shared_ptr<CJOperator>           CJOperatorP;
typedef std::shared_ptr<CJRandFunction>       CJRandFunctionP;
typedef std::shared_ptr<CJReal2Function>      CJReal2FunctionP;
typedef std::shared_ptr<CJRegExp>             CJRegExpP;
typedef std::shared_ptr<CJRegExpType>         CJRegExpTypeP;
typedef std::shared_ptr<CJString>             CJStringP;
typedef std::shared_ptr<CJStringType>         CJStringTypeP;
typedef std::shared_ptr<CJSystem>             CJSystemP;
typedef std::shared_ptr<CJToken>              CJTokenP;
typedef std::shared_ptr<CJTokenValue>         CJTokenValueP;
typedef std::shared_ptr<CJTrue>               CJTrueP;
typedef std::shared_ptr<CJTrueType>           CJTrueTypeP;
typedef std::shared_ptr<CJTypeFunction>       CJTypeFunctionP;
typedef std::shared_ptr<CJUndefined>          CJUndefinedP;
typedef std::shared_ptr<CJUndefinedType>      CJUndefinedTypeP;
typedef std::shared_ptr<CJUserObject>         CJUserObjectP;
typedef std::shared_ptr<CJValue>              CJValueP;

typedef std::shared_ptr<CJErrorBase>          CJErrorBaseP;
typedef std::shared_ptr<CJError>              CJErrorP;
typedef std::shared_ptr<CJErrorType>          CJErrorTypeP;
typedef std::shared_ptr<CJTypeError>          CJTypeErrorP;
typedef std::shared_ptr<CJTypeErrorType>      CJTypeErrorTypeP;
typedef std::shared_ptr<CJReferenceError>     CJReferenceErrorP;
typedef std::shared_ptr<CJReferenceErrorType> CJReferenceErrorTypeP;
typedef std::shared_ptr<CJEvalError>          CJEvalErrorP;
typedef std::shared_ptr<CJEvalErrorType>      CJEvalErrorTypeP;
typedef std::shared_ptr<CJRangeError>         CJRangeErrorP;
typedef std::shared_ptr<CJRangeErrorType>     CJRangeErrorTypeP;
typedef std::shared_ptr<CJSyntaxError>        CJSyntaxErrorP;
typedef std::shared_ptr<CJSyntaxErrorType>    CJSyntaxErrorTypeP;
typedef std::shared_ptr<CJURIError>           CJURIErrorP;
typedef std::shared_ptr<CJURIErrorType>       CJURIErrorTypeP;

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
class CJExecStatement;
class CJExecSwitch;
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
typedef std::shared_ptr<CJExecStatement>          CJExecStatementP;
typedef std::shared_ptr<CJExecSwitch>             CJExecSwitchP;
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

//------

typedef double (*CJRealFn1)(double);
typedef double (*CJRealFn2)(double, double);

#endif
