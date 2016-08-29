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
#include <CJNumber.h>
#include <CJString.h>
#include <CJArray.h>
#include <CJOperator.h>
#include <CJFunction.h>
#include <CJDictionary.h>
#include <CJRegExp.h>
#include <CJDate.h>

//------

#include <CJLValue.h>
#include <CJDictionaryRef.h>
#include <CJObject.h>
#include <CJTypeValue.h>
#include <CJObjectValue.h>
#include <CJUserObject.h>
#include <CJNameSpaceValue.h>

//------

#include <CJExecBlock.h>
#include <CJTypeObjectFunction.h>

//------

struct CJPropertyData {
  bool         modifiable { false };
  bool         create { false };
  CJNameSpaceP scope;
  CJObjP       obj;
  CJValueP     objValue;
  CJValueP     value;
  CJLValueP    lvalue;
};

//------

class CJavaScript {
 public:
  typedef std::vector<CJTokenP>            Tokens;
  typedef std::vector<CJTokenP>            ETokens;
  typedef std::vector<CJIdentifier *>      Identifiers;
  typedef std::pair<CJFunctionP, CJValueP> FunctionValuePair;
  typedef std::pair<CJValueP, CJValueP>    ValuePair;
  typedef std::vector<CJUserFunctionP>     UserFunctions;
  typedef std::vector<CJValueP>            Values;

  class ExecData {
   public:
    ExecData() { }

    bool isBlock() const { return block_; }
    void setBlock(bool b) { block_ = b; }

    bool eof() const {
      return (pos_ >= len_);
    }

    void initExec(Tokens &tokens) {
      len_    = tokens.size();
      tokens_ = tokens;
    }

    void addEToken(CJTokenP etoken) {
      etokens_.push_back(etoken);
    }

    int pos() const { return pos_; }
    void setPos(int i) { pos_ = i; }

    const ETokens &etokens() const { return etokens_; }

    CJValueP unstackEValue(CJavaScript *js) {
      CJValueP value;

      for (auto &t : etokens_) {
        value = t->exec(js);
      }

      etokens_.clear();

      return value;
    }

    CJTokenP token() const {
      if (eof()) return 0;

      return tokens_[pos_];
    }

    void next() {
      ++pos_;
    }

   private:
    int     pos_ { 0 };
    int     len_ { 0 };
    Tokens  tokens_;
    ETokens etokens_;
    bool    block_ { false };
  };

 public:
  CJavaScript();

  virtual ~CJavaScript() { }

  bool isParseDebug() const { return parseDebug_; }
  void setParseDebug(bool b) { parseDebug_ = b; }

  bool isInterpDebug() const { return interpDebug_; }
  void setInterpDebug(bool b) { interpDebug_ = b; }

  bool isExecDebug() const { return execDebug_; }
  void setExecDebug(bool b) { execDebug_ = b; }

  bool isExprDebug() const { return exprDebug_; }
  void setExprDebug(bool b) { exprDebug_ = b; }

  CJObjTypeP addObjectType(const std::string &name, CJObjTypeP type);
  CJObjTypeP getObjectType(const std::string &name) const;

  CJDictionaryP rootScope() const { return rootScope_; }
  CJDictionaryP currentScope() const { return currentScope_; }

  CJDictionaryP thisScope() const;

  void pushThis(CJDictionaryP scope);
  void popThis();

  CJValueP setProperty(const std::string &name, CJValueP value);

  bool lookupPropertyData(const Identifiers &identifiers, CJPropertyData &data);

  bool lookupPropertyData(CJDictionaryP dict, const Identifiers &identifiers,
                          CJPropertyData &data, int ind=0);
  bool lookupPropertyData(CJFunctionP fn, const Identifiers &identifiers,
                          CJPropertyData &data, int ind=0);
  bool lookupPropertyData(CJNameSpaceP scope, CJValueP value, const Identifiers &identifiers,
                          CJPropertyData &data, int ind=0);
  bool lookupPropertyData(CJObjP obj, const Identifiers &identifiers,
                          CJPropertyData &data, int ind=0);
  bool lookupPropertyData(CJValueP value, const Identifiers &identifiers,
                          CJPropertyData &data, int ind=0);

  CJValueP lookupValue(const Identifiers &identifiers);

  CJLValueP lookupLValue(const Identifiers &identifiers);

  CJValueP lookupFunction(const Identifiers &identifiers);

  CJLValueP lookupProperty(const Identifiers &identifiers, bool create=false);

  ValuePair lookupObjectProperty(const Identifiers &identifiers, bool create=false);

  bool deleteProperty(const Identifiers &identifiers);
  bool deleteProperty(CJDictionaryP scope, const Identifiers &identifiers);

  CJValueP setVariable(const Identifiers &identifiers, CJValueP value);

  void addTypeObject(CJToken::Type type, CJObjTypeP obj);

  void addTypeFunction(CJToken::Type type, CJFunctionP fn);

  CJExecBlockP getCurrentBlock() const { return block_; }

  std::vector<std::string> getFunctionNames() const;
  std::vector<std::string> getVariableNames() const;

  //---

  void startBlock(CJExecBlockP block);
  CJExecBlockP endBlock();

  //---

  void startScope(CJDictionaryP scope);
  CJDictionaryP endScope();

  void startFunctionScope(CJUserFunctionP fn);
  CJUserFunctionP endFunctionScope();

  void initFunctionScope(CJUserFunctionP fn);
  void termFunctionScope(CJUserFunctionP fn);

  CJUserFunctionP currentFunction() const;

  void printScopeStack(const std::string &msg="") const;

  void printScopeChain(CJDictionaryP scope, const std::string &msg="") const;

  //---

  void pushUserFunction(CJUserFunctionP fn);
  void popUserFunction();

  const UserFunctions &userFunctions() const { return userFunctions_; }

  void printUserFunctions(const std::string &msg="") const;

  //---

  int isCompleteLine(const std::string &str) const;

  bool loadFile(const std::string &filename);

  bool loadString(const std::string &str);

  CJExecBlockP interpFunctionBlock(const std::string &str);

  bool loadSubFile(const std::string &filename);

  void interp(ExecData &execData);

  CJValueP exec();

  int cmp(CJValueP value1, CJValueP value2);

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

  void errorMsg(CJTokenP token, const std::string &msg) const;
  void errorMsg(CJToken *token, const std::string &msg) const;
  void errorMsg(const std::string &msg) const;

 private:
  bool loadText(const std::string &str);

  void parseString(const std::string &str);

  void skipSpace(CStrParse &parse);

  void readIdentifier(CStrParse &parse);
  void readNumber(CStrParse &parse);
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
  CJExecBlockP           interpExecBlock(CJExecBlock::Type type);
  CJExecBreakP           interpExecBreak();
  CJExecConstP           interpExecConst();
  CJExecContinueP        interpExecContinue();
  CJExecDeleteP          interpExecDelete();
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
  CJExecThisP            interpExecThis();
  CJExecThrowP           interpExecThrow();
  CJExecTryP             interpExecTry();
  CJExecVarP             interpExecVar();
  CJExecVoidP            interpExecVoid();
  CJExecWhileP           interpExecWhile();
  CJExecWithP            interpExecWith();
  CJUserFunctionP        interpUserFunction(bool named);

  bool interpExecKeyword(CJKeyword::Type type);
  bool isExecKeyword    (CJKeyword::Type type) const;
  bool isKeyword        (CJTokenP token, CJKeyword::Type type) const;

  bool interpExecOperator(CJOperator::Type type);
  bool isExecOperator    (CJOperator::Type type) const;
  bool isOperator        (CJTokenP token, CJOperator::Type type) const;

  int execLineNum() const;

 private:
  typedef std::vector<ExecData *>            ExecDataStack;
  typedef std::vector<CJDictionaryP>         ScopeStack;
  typedef std::vector<ScopeStack>            ScopeStackStack;
  typedef std::vector<CJExecBlockP>          BlockStack;
  typedef std::vector<CJUserFunctionP>       FunctionStack;
  typedef std::map<std::string,CJFunctionP>  Functions;
  typedef std::map<CJToken::Type,CJObjTypeP> TypeObject;
  typedef std::map<std::string,CJObjTypeP>   NamedType;

  bool            parseDebug_  { false };
  bool            interpDebug_ { false };
  bool            execDebug_   { false };
  bool            exprDebug_   { false };
  std::string     fileName_;
  NamedType       namedType_;
  CJDictionaryP   rootScope_;
  CJDictionaryP   currentScope_;
  ScopeStack      scopeStack_;
  ScopeStack      currentScopeStack_;
  ScopeStackStack scopeStackStack_;
  ScopeStack      thisStack_;
  CJMathP         math_;
  Tokens          tokens_;
  TypeObject      typeObject_;
  ExecData*       execData_ { 0 };
  ExecDataStack   execDataStack_;
  CJExecBlockP    block_;
  BlockStack      blockStack_;
  UserFunctions   userFunctions_;
  FunctionStack   functions_;
};

#endif
