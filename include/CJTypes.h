#ifndef CJTypes_H
#define CJTypes_H

#include <memory>

class CJavaScript;

class CJDictionary;
class CJFunction;
class CJMath;
class CJNameSpace;
class CJObjType;
class CJObj;
class CJObject;
class CJOperator;
class CJSystem;
class CJUserFunction;
class CJValue;

typedef std::shared_ptr<CJDictionary>   CJDictionaryP;
typedef std::shared_ptr<CJFunction>     CJFunctionP;
typedef std::shared_ptr<CJMath>         CJMathP;
typedef std::shared_ptr<CJNameSpace>    CJNameSpaceP;
typedef std::shared_ptr<CJObjType>      CJObjTypeP;
typedef std::shared_ptr<CJObject>       CJObjectP;
typedef std::shared_ptr<CJObj>          CJObjP;
typedef std::shared_ptr<CJObjType>      CJObjTypeP;
typedef std::shared_ptr<CJOperator>     CJOperatorP;
typedef std::shared_ptr<CJSystem>       CJSystemP;
typedef std::shared_ptr<CJUserFunction> CJUserFunctionP;
typedef std::shared_ptr<CJValue>        CJValueP;

//------

class CJExecArray;
class CJExecAssignExpression;
class CJExecBlock;
class CJExecBreak;
class CJExecConst;
class CJExecContinue;
class CJExecDelete;
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
class CJExecVoid;
class CJExecWhile;
class CJExecWith;

typedef std::shared_ptr<CJExecArray>              CJExecArrayP;
typedef std::shared_ptr<CJExecAssignExpression>   CJExecAssignExpressionP;
typedef std::shared_ptr<CJExecBlock>              CJExecBlockP;
typedef std::shared_ptr<CJExecBreak>              CJExecBreakP;
typedef std::shared_ptr<CJExecConst>              CJExecConstP;
typedef std::shared_ptr<CJExecContinue>           CJExecContinueP;
typedef std::shared_ptr<CJExecDelete>             CJExecDeleteP;
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
typedef std::shared_ptr<CJExecVoid>               CJExecVoidP;
typedef std::shared_ptr<CJExecWhile>              CJExecWhileP;
typedef std::shared_ptr<CJExecWith>               CJExecWithP;

#endif
