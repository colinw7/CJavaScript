#ifndef CJavaScript_H
#define CJavaScript_H

#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <limits>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <cmath>

class CStrParse;

//------

#include <CJTypes.h>
#include <CJToken.h>
#include <CJKeyword.h>
#include <CJNameSpace.h>
#include <CJIdentifier.h>
#include <CJObj.h>
#include <CJValue.h>
#include <CJUndefined.h>
#include <CJNull.h>
#include <CJTrue.h>
#include <CJFalse.h>
#include <CJBoolean.h>
#include <CJNumber.h>
#include <CJString.h>
#include <CJArray.h>
#include <CJOperator.h>
#include <CJFunctionBase.h>
#include <CJDictionary.h>
#include <CJRegExp.h>
#include <CJDate.h>
#include <CJFunction.h>

//------

#include <CJLValue.h>
#include <CJObject.h>
#include <CJTypeValue.h>
#include <CJUserObject.h>
#include <CJTypeFunction.h>
#include <CJGetterSetter.h>

//------

#include <CJExecBlock.h>
#include <CJExecStatement.h>
#include <CJExecExpression.h>
#include <CJExecExpressionList.h>

#include <CJTypeObjectFunction.h>
#include <CJExecData.h>
#include <CJPropertyData.h>
#include <CJGlobalFunction.h>
#include <CJRealFunction.h>
#include <CJRandFunction.h>

//------

class CJavaScript {
 public:
  typedef std::vector<CJTokenP>                Tokens;
  typedef std::vector<CJTokenP>                ETokens;
  typedef std::vector<CJIdentifier *>          Identifiers;
  typedef std::pair<CJFunctionBaseP, CJValueP> FunctionValuePair;
  typedef std::pair<CJValueP, CJValueP>        ValuePair;
  typedef std::vector<CJFunctionP>             UserFunctions;
  typedef std::vector<CJValueP>                Values;

 public:
  CJavaScript();

  virtual ~CJavaScript() { }

  //---

  bool isParseDebug() const { return debugData_.parse; }
  void setParseDebug(bool b) { debugData_.parse = b; }

  bool isInterpDebug() const { return debugData_.interp; }
  void setInterpDebug(bool b) { debugData_.interp = b; }

  bool isExecDebug() const { return debugData_.exec; }
  void setExecDebug(bool b) { debugData_.exec = b; }

  bool isExprDebug() const { return debugData_.expr; }
  void setExprDebug(bool b) { debugData_.expr = b; }

  bool isExceptDebug() const { return debugData_.except; }
  void setExceptDebug(bool b) { debugData_.except = b; }

  //---

  void loadStartpFile();

  //---

  CJObjTypeP addObjectType(const std::string &name, CJObjTypeP type);
  CJObjTypeP getObjectType(const std::string &name) const;

  CJDictionaryP rootScope() const { return rootScope_; }

  CJDictionaryP currentScope() const { return currentScope_; }
  void setCurrentScope(CJDictionaryP scope) { currentScope_ = scope; }

  CJDictionaryP thisScope() const;

  void pushThis(CJDictionaryP scope);
  void popThis();

  CJValueP setProperty(const std::string &name, CJValueP value);

  bool hasIndexValue(CJValueP value, CJValueP ivalue, bool inherit=true) const;
  bool indexValue(CJValueP value, CJValueP ivalue, CJValueP &rvalue) const;
  bool setIndexValue(CJValueP value, CJValueP ivalue, CJValueP rvalue);
  bool deleteIndexValue(CJValueP value, CJValueP ivalue);

  bool lookupPropertyData(const CJExecIdentifiersP &eidentifiers, CJPropertyData &data);
  bool lookupPropertyData(const Identifiers &identifiers, CJPropertyData &data);

  bool lookupScopePropertyData(CJDictionaryP dict, const Identifiers &identifiers,
                               CJPropertyData &data, int ind=0);
  bool lookupFunctionPropertyData(CJFunctionBaseP fn, const Identifiers &identifiers,
                                  CJPropertyData &data, int ind=0);
  bool lookupScopeValuePropertyData(CJNameSpaceP scope, CJValueP value,
                                    const Identifiers &identifiers,
                                    CJPropertyData &data, int ind=0);
  bool lookupObjPropertyData(CJObjP obj, const Identifiers &identifiers,
                             CJPropertyData &data, int ind=0);

  bool lookupValuePropertyData(CJValueP value, const CJExecIdentifiersP &eidentifiers,
                               CJPropertyData &data, int ind=0);
  bool lookupValuePropertyData(CJValueP value, const Identifiers &identifiers,
                               CJPropertyData &data, int ind=0);

  CJValueP valueToObj(CJValueP value) const;

  //---

  CJValueP lookupValue(const CJExecIdentifiersP &eidentifiers);
  CJValueP lookupValue(const Identifiers &identifiers);

  CJLValueP lookupLValue(const CJExecIdentifiersP &eidentifiers);
  CJLValueP lookupLValue(const Identifiers &identifiers);

  CJValueP lookupFunction(const Identifiers &identifiers);

  CJLValueP lookupProperty(const CJExecIdentifiersP &eidentifiers, bool create=false);
  CJLValueP lookupProperty(const Identifiers &identifiers, bool create=false);

  ValuePair lookupObjectProperty(const CJExecIdentifiersP &eidentifiers, bool create=false);
  ValuePair lookupObjectProperty(const Identifiers &identifiers, bool create=false);

  bool deleteToken(const Tokens &tokens);

  bool deleteProperty(const CJExecIdentifiersP &eidentifiers, const Values &values=Values());
  bool deleteProperty(const Identifiers &identifiers, const Values &values=Values());
  bool deleteProperty(CJDictionaryP scope, const Identifiers &identifiers,
                      const Values &values=Values());

  CJValueP setVariable(const Identifiers &identifiers, CJValueP value);

  //---

  void addTypeObject(CJToken::Type type, CJObjTypeP obj);
  CJObjTypeP getTypeObject(CJToken::Type type) const;

  void addTypeFunction(CJToken::Type type, CJFunctionBaseP fn);

  CJExecBlockP getCurrentBlock() const { return block_; }

  std::vector<std::string> getFunctionNames() const;
  std::vector<std::string> getVariableNames() const;

  //---

  void startBlock(CJExecBlockP block);
  CJExecBlockP endBlock();

  //---

  void startScope(CJDictionaryP scope);
  CJDictionaryP endScope();

  void startFunctionScope(CJFunctionP fn);
  CJFunctionP endFunctionScope();

  void initFunctionScope(CJFunctionP fn);
  void termFunctionScope(CJFunctionP fn);

  CJFunctionP currentFunction() const;

  void printScopeStack(const std::string &msg="") const;

  void printScopeChain(CJDictionaryP scope, const std::string &msg="") const;

  //---

  void pushUserFunction(CJFunctionP fn);
  void popUserFunction();

  const UserFunctions &userFunctions() const { return userFunctions_; }

  CJDictionaryP currentUserFunction() const;

  void printUserFunctions(const std::string &msg="") const;

  //---

  bool isEmptyLine(const std::string &str) const;

  int isCompleteLine(const std::string &str) const;

  bool loadFile(const std::string &filename);

  bool loadString(const std::string &str);

  CJExecBlockP interpFunctionBlock(const std::string &str);

  CJValueP interpString(const std::string &str);

  bool loadSubFile(const std::string &filename);

  void interp(CJExecData &execData);

  CJValueP exec();

  CJFunctionBaseP valueToFunction(CJValueP value) const;

  CJValueP valueToObject(CJValueP value) const;

  CJObjTypeFunctionP valueTypeFunction(CJValueP value) const;

  //---

  COptInt rcmp     (CJValueP  value1 , CJValueP  value2 ) const;
  COptInt cmpArray (CJArrayP  array1 , CJArrayP  array2 ) const;
  COptInt cmpObject(CJObjectP object1, CJObjectP object2) const;
  COptInt cmp      (CJValueP  value1 , CJValueP  value2 ) const;

  //---

  CJValueP execBinaryOp(CJOperator::Type op, CJValueP value1, CJValueP value2);
  CJValueP execUnaryOp (CJOperator::Type op, CJValueP value);

  //---

  CJOperatorP createOpToken(const CJOperator::Type &type, int precedence,
                            CJOperator::Associativty associativty, CJOperator::Ary ary) const {
    return std::make_shared<CJOperator>(type, precedence, associativty, ary);
  }

  CJKeywordP createKeyword(const CJKeyword::Type &type) {
    return std::make_shared<CJKeyword>(type);
  }

  CJIdentifierP createIdentifier(const std::string &name) {
    return std::make_shared<CJIdentifier>(name);
  }

  //---

  CJValueP createBoolValue(bool b=false) const {
    if (b)
      return createTrueValue();
    else
      return createFalseValue();
  }

  CJBooleanP createBoolObject(bool b=false) const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJBoolean>(th, b);
  }

  CJTrueP createTrueValue() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return CJTrue::value(th);
  }

  CJFalseP createFalseValue() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return CJFalse::value(th);
  }

  CJNumberP createNumberValue(long l) const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJNumber>(th, l);
  }

  CJNumberP createNumberValue(double r) const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJNumber>(th, r);
  }

  CJNumberP createNumberObject(long l) const {
    CJNumberP number = createNumberValue(l);
    number->setIsPrimitive(false);
    return number;
  }

  CJNumberP createNumberObject(double r) const {
    CJNumberP number = createNumberValue(r);
    number->setIsPrimitive(false);
    return number;
  }

  CJStringP createStringValue(const std::string &s, char c='\"') const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJString>(th, s, c);
  }

  CJStringP createStringObject(const std::string &s, char c='\"') const {
    CJStringP str = createStringValue(s, c);
    str->setIsPrimitive(false);
    return str;
  }

  CJValueP createNullValue() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return CJNull::value(th);
  }

  CJUndefinedP createUndefinedValue() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return CJUndefined::value(th);
  }

  CJArrayP createArrayValue() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJArray>(th);
  }

  CJArrayP createArrayValue(long n) const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJArray>(th, n);
  }

  CJDictionaryP createDictValue(const std::string &s="") const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJDictionary>(th, s);
  }

  CJRegExpP createRegExpValue(const std::string &s="", const std::string &f="") const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJRegExp>(th, s, f);
  }

  CJDateP createDateValue(const Values &values) const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJDate>(th, values);
  }

  CJObjectP createObject() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJObject>(th);
  }

  CJFunctionP createFunction(const std::string &name="",
                             const CJFunction::Args &args=CJFunction::Args(),
                             CJExecBlockP block=CJExecBlockP()) const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJFunction>(th, name, args, block);
  }

  CJGetterSetterP createGetterSetter() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJGetterSetter>(th);
  }

  //---

  CJGlobalFunctionP createGlobalFunction(const std::string &name) const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJGlobalFunction>(th, name);
  }

  CJMathFunctionP createMathFunction(const std::string &name, CJRealFn1 fn=0) const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJMathFunction>(th, name, fn);
  }

  CJReal2FunctionP createReal2Function(const std::string &name, CJRealFn2 fn=0) const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJReal2Function>(th, name, fn);
  }

  CJMinFunctionP createMinFunction() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJMinFunction>(th);
  }

  CJMaxFunctionP createMaxFunction() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJMaxFunction>(th);
  }

  CJRandFunctionP createRandFunction() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJRandFunction>(th);
  }

  //---

  CJExecStatementP createExecStatement(CJTokenP token=CJTokenP()) const {
    //CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJExecStatement>(token);
  }

  CJExecExpressionP createExecExpression(CJToken::Type type=CJToken::Type::Expression) const {
    //CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJExecExpression>(type);
  }

  CJExecExpressionListP
  createExecExpressionList(CJToken::Type type=CJToken::Type::ExpressionList) const {
    //CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJExecExpressionList>(type);
  }

  CJExecBlockP createExecBlock(CJExecBlock::Type type) const {
    //CJavaScript *th = const_cast<CJavaScript *>(this);

    return std::make_shared<CJExecBlock>(type);
  }

  //---

  virtual long setInterval(const std::string & /*proc*/, double /*msecs*/) { return -1; }

  virtual void clearInterval(int /*timer*/) { }

  //---

  bool isUndefinedValue(CJValueP value) const {
    return (! value || value->type() == CJToken::Type::Undefined);
  }

  bool isUndefinedOrNullValue(CJValueP value) const {
    return (! value || value->type() == CJToken::Type::Undefined ||
                       value->type() == CJToken::Type::Null);
  }

  //---

  CJValueP execFunction(CJFunctionBaseP fn, const CJFunctionBase::Values &values,
                        CJValueP thisValue=CJValueP());

  //---

  void throwException(CJExceptionType type);

  void throwTypeError     (CJTokenP token, const std::string &msg);
  void throwTypeError     (CJToken *token, const std::string &msg);
  void throwSyntaxError   (CJTokenP token, const std::string &msg);
  void throwSyntaxError   (CJToken *token, const std::string &msg);
  void throwRangeError    (CJTokenP token, const std::string &msg);
  void throwRangeError    (CJToken *token, const std::string &msg);
  void throwReferenceError(CJTokenP token, const std::string &msg);
  void throwReferenceError(CJToken *token, const std::string &msg);

  void throwError(CJToken *token, CJErrorBaseP error);

  void errorMsg(CJTokenP token, const std::string &msg) const;
  void errorMsg(CJToken *token, const std::string &msg) const;
  void errorMsg(const std::string &msg) const;

  //---

 private:
  void initNameMaps();

  template<typename FUNC, typename PROTO>
  void defineObjectT(CJavaScript *js, const std::string &name) {
    js->defineObject(name, CJObjTypeFunctionP(new FUNC(js)), CJObjP(new PROTO(js)));
  }

  void defineObject(const std::string &name, CJObjTypeFunctionP typeFn, CJObjP value);

  bool loadText(const std::string &str);

  void parseString(const std::string &str);

  void skipSpace(CStrParse &parse);

  std::string getIdentifier(CStrParse &parse, bool allowUnary) const;

  void readIdentifier(CStrParse &parse, bool allowKeyword=true);

  bool isRealValueName(const std::string &name, bool allowUnary,
                       CJNumber::RealType &realType) const;
  void readNumber(CStrParse &parse);

  bool isOperatorName(const std::string &name, CJOperator::Type &type) const;
  void readOperator(CStrParse &parse, bool allowUnary);

  void readDoubleString(CStrParse &parse);
  void readSingleString(CStrParse &parse);
  void readRegExp(CStrParse &parse);

  CJTokenP      lastToken() const;
  CJToken::Type lastTokenType() const;

  std::pair<bool, CJKeyword::Type> isKeyword(const std::string &name) const;

  bool isInterpDictionary() const;
  bool isInterpForIn() const;
  bool isExecLabel() const;

  bool canInterpIdentifiers() const;

  CJExecArrayP           interpExecArray();
  CJExecAssertP          interpExecAssert();
  CJExecBlockP           interpExecBlock(CJExecBlock::Type type);
  CJExecBreakP           interpExecBreak();
  CJExecConstP           interpExecConst();
  CJExecContinueP        interpExecContinue();
  CJExecDictionaryP      interpExecDictionary();
  CJExecDoP              interpExecDo();
  CJExecLabelP           interpExecLabel();
  CJExecExpressionListP  interpExpressionList();
  CJExecExpressionP      interpExpression();
  CJExecForP             interpExecFor();
  CJExecFunctionP        interpFunction();
  CJExecIdentifiersP     interpIdentifiers();
  CJExecIfP              interpExecIf();
  CJExecIndexExpressionP interpIndexExpression();
  CJExecNewP             interpExecNew();
  CJExecQuestionP        interpExecQuestion(CJExecExpressionP bexpr);
  CJExecReturnP          interpExecReturn();
  CJExecSwitchP          interpExecSwitch();
  CJExecThrowP           interpExecThrow();
  CJExecTryP             interpExecTry();
  CJExecVarP             interpExecVar();
  CJExecWhileP           interpExecWhile();
  CJExecWithP            interpExecWith();
  CJFunctionP            interpUserFunction(bool named);

  void pushExecData(CJExecData &execData);
  void popExecData();

  bool interpExecKeyword(CJKeyword::Type type);
  bool isExecKeyword    (CJKeyword::Type type) const;
  bool isKeyword        (CJTokenP token, CJKeyword::Type type) const;

  bool interpExecOperator(CJOperator::Type type);
  bool isExecOperator    (CJOperator::Type type) const;
  bool isOperator        (CJTokenP token, CJOperator::Type type) const;

  int execLineNum() const;

 private:
  typedef std::map<std::string,CJOperator::Type>   NameOperatorMap;
  typedef std::map<std::string,CJNumber::RealType> NameRealTypeMap;
  typedef std::vector<CJExecData *>                ExecDataStack;
  typedef std::vector<CJDictionaryP>               ScopeStack;
  typedef std::vector<ScopeStack>                  ScopeStackStack;
  typedef std::vector<CJExecBlockP>                BlockStack;
  typedef std::vector<CJFunctionP>                 FunctionStack;
  typedef std::map<std::string,CJFunctionBaseP>    Functions;
  typedef std::map<CJToken::Type,CJObjTypeP>       TypeObject;
  typedef std::map<std::string,CJObjTypeP>         NamedType;
  typedef std::map<std::string,CJObjTypeFunctionP> TypeFunctions;

  struct DebugData {
    bool parse  { false };
    bool interp { false };
    bool exec   { false };
    bool expr   { false };
    bool except { false };
  };

  DebugData       debugData_;
  std::string     fileName_;
  NamedType       namedType_;
  NameOperatorMap nameOperatorMap_;
  NameRealTypeMap nameRealTypeMap_;
  TypeFunctions   typeFunctions_;
  CJDictionaryP   rootScope_;
  CJDictionaryP   currentScope_;
  ScopeStack      scopeStack_;
  ScopeStack      currentScopeStack_;
  ScopeStackStack scopeStackStack_;
  ScopeStack      thisStack_;
  CJMathP         math_;
  CJJSONP         json_;
  Tokens          tokens_;
  TypeObject      typeObject_;
  CJExecData*     execData_ { 0 };
  ExecDataStack   execDataStack_;
  CJExecBlockP    block_;
  BlockStack      blockStack_;
  UserFunctions   userFunctions_;
  FunctionStack   functions_;
};

#endif
