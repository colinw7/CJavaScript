#ifndef CJTypes_H
#define CJTypes_H

#include <memory>

class CJavaScript;

class CJDictionary;
class CJExecBlock;
class CJFunction;
class CJObjectType;
class CJObject;
class CJOperator;
class CJUserFunction;
class CJValue;

class CJExecArray;
class CJExecDictionary;
class CJExecExpression;
class CJExecExpressionList;
class CJExecFunction;
class CJExecIdentifiers;
class CJExecVar;

typedef std::shared_ptr<CJDictionary>   CJDictionaryP;
typedef std::shared_ptr<CJExecBlock>    CJExecBlockP;
typedef std::shared_ptr<CJFunction>     CJFunctionP;
typedef std::shared_ptr<CJObjectType>   CJObjectTypeP;
typedef std::shared_ptr<CJObject>       CJObjectP;
typedef std::shared_ptr<CJOperator>     CJOperatorP;
typedef std::shared_ptr<CJUserFunction> CJUserFunctionP;
typedef std::shared_ptr<CJValue>        CJValueP;

typedef std::shared_ptr<CJExecArray>          CJExecArrayP;
typedef std::shared_ptr<CJExecDictionary>     CJExecDictionaryP;
typedef std::shared_ptr<CJExecExpression>     CJExecExpressionP;
typedef std::shared_ptr<CJExecExpressionList> CJExecExpressionListP;
typedef std::shared_ptr<CJExecFunction>       CJExecFunctionP;
typedef std::shared_ptr<CJExecIdentifiers>    CJExecIdentifiersP;
typedef std::shared_ptr<CJExecVar>            CJExecVarP;

#endif
