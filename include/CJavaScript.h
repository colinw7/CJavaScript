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

//------

#include <CJLValue.h>
#include <CJDictionaryRef.h>
#include <CJObject.h>
#include <CJTypeValue.h>
#include <CJUserObject.h>
#include <CJTypeFunction.h>

//------

#include <CJExecBlock.h>
#include <CJTypeObjectFunction.h>
#include <CJExecData.h>
#include <CJPropertyData.h>

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

  bool hasIndexValue(CJValueP value, CJValueP ivalue) const;
  bool indexValue(CJValueP value, CJValueP ivalue, CJValueP &rvalue) const;
  bool setIndexValue(CJValueP value, CJValueP ivalue, CJValueP rvalue);
  bool deleteIndexValue(CJValueP value, CJValueP ivalue);

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
  bool lookupValuePropertyData(CJValueP value, const Identifiers &identifiers,
                               CJPropertyData &data, int ind=0);

  CJValueP valueToObj(CJValueP value) const;

  CJValueP lookupValue(const Identifiers &identifiers);

  CJLValueP lookupLValue(const Identifiers &identifiers);

  CJValueP lookupFunction(const Identifiers &identifiers);

  CJLValueP lookupProperty(const Identifiers &identifiers, bool create=false);

  ValuePair lookupObjectProperty(const Identifiers &identifiers, bool create=false);

  bool deleteToken(const Tokens &tokens);

  bool deleteProperty(const Identifiers &identifiers, const Values &values=Values());

  bool deleteProperty(CJDictionaryP scope, const Identifiers &identifiers,
                      const Values &values=Values());

  CJValueP setVariable(const Identifiers &identifiers, CJValueP value);

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

  COptInt cmp(CJValueP value1, CJValueP value2);

  CJValueP execBinaryOp(CJOperator::Type op, CJValueP value1, CJValueP value2);
  CJValueP execUnaryOp (CJOperator::Type op, CJValueP value);

  CJValueP createBoolValue(bool b) const {
    if (b)
      return createTrueValue();
    else
      return createFalseValue();
  }

  CJValueP createTrueValue() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    //return CJValueP(new CJTrue(th));
    return CJTrue::value(th);
  }

  CJValueP createFalseValue() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    //return CJValueP(new CJFalse(th));
    return CJFalse::value(th);
  }

  CJValueP createNumberValue(long l) const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return CJValueP(new CJNumber(th, l));
  }

  CJValueP createNumberValue(double r) const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return CJValueP(new CJNumber(th, r));
  }

  CJValueP createStringValue(const std::string &s, char c='\"') const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return CJValueP(new CJString(th, s, c));
  }

  CJValueP createNullValue() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    //return CJValueP(new CJNull(th));
    return CJNull::value(th);
  }

  CJValueP createUndefinedValue() const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    //return CJValueP(new CJUndefined(th));
    return CJUndefined::value(th);
  }

  CJValueP createRegExpValue(const std::string &s, const std::string &f="") const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return CJValueP(new CJRegExp(th, s, f));
  }

  CJValueP createDateValue(const Values &values) const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    return CJValueP(new CJDate(th, values));
  }

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

  void throwException(CJExceptionType type);

  void throwTypeError     (CJTokenP token, const std::string &msg);
  void throwTypeError     (CJToken *token, const std::string &msg);
  void throwSyntaxError   (CJTokenP token, const std::string &msg);
  void throwSyntaxError   (CJToken *token, const std::string &msg);
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

  void readIdentifier(CStrParse &parse);

  bool isRealValueName(const std::string &name, bool allowUnary,
                       CJNumber::RealType &realType) const;
  void readNumber(CStrParse &parse);

  bool isOperatorName(const std::string &name, CJOperator::Type &type) const;
  void readOperator(CStrParse &parse, bool allowUnary);

  void readDoubleString(CStrParse &parse);
  void readSingleString(CStrParse &parse);
  void readRegExp(CStrParse &parse);

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
