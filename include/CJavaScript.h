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
#include <CJObjectType.h>
#include <CJValue.h>
#include <CJUndefined.h>
#include <CJNull.h>
#include <CJTrue.h>
#include <CJFalse.h>
#include <CJNumber.h>
#include <CJString.h>
#include <CJArray.h>
#include <CJOperator.h>

//------

#include <CJExecIdentifiers.h>
#include <CJExecExpression.h>
#include <CJExecExpressionList.h>

//------

#include <CJFunction.h>
#include <CJDictionary.h>

//------

#include <CJExecAssignExpression.h>
#include <CJExecIndexExpression.h>
#include <CJExecIncrDecrExpression.h>
#include <CJExecBlock.h>
#include <CJExecFunction.h>
#include <CJExecVar.h>
#include <CJExecWith.h>
#include <CJExecNew.h>
#include <CJExecArray.h>
#include <CJExecDictionary.h>
#include <CJExecBreak.h>
#include <CJExecContinue.h>
#include <CJExecReturn.h>
#include <CJExecDelete.h>
#include <CJExecFor.h>
#include <CJExecQuestion.h>
#include <CJExecIf.h>
#include <CJExecWhile.h>
#include <CJExecSwitch.h>
#include <CJExecTry.h>
#include <CJExecThrow.h>

//------

#include <CJRealFunction.h>
#include <CJRandFunction.h>
#include <CJUserFunction.h>
#include <CJTypeFunction.h>
#include <CJNumberFunction.h>
#include <CJStringFunction.h>
#include <CJBooleanFunction.h>
#include <CJArrayFunction.h>
#include <CJObjectFunction.h>

//------

#include <CJLValue.h>
#include <CJDictionaryRef.h>
#include <CJObject.h>
#include <CJTypeValue.h>
#include <CJObjectValue.h>
#include <CJUserObject.h>
#include <CJDictionaryValue.h>

//------

class CJAlertFunction : public CJFunction {
 public:
  CJAlertFunction(CJavaScript *js) :
   CJFunction(js, "alert") {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJAlertFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

class CJSetInterval : public CJFunction {
 public:
  CJSetInterval(CJavaScript *js) :
   CJFunction(js, "setInterval") {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJSetInterval(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

class CJClearInterval : public CJFunction {
 public:
  CJClearInterval(CJavaScript *js) :
   CJFunction(js, "clearInterval") {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJClearInterval(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

#include <CJTypeObjectFunction.h>

//------

struct CJPropertyData {
  bool          modifiable { false };
  bool          create { false };
  CJDictionaryP dict;
  CJObjectP     obj;
  CJValueP      objValue;
  CJValueP      value;
  CJLValueP     lvalue;
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

  CJObjectTypeP addObjectType(const std::string &name, CJObjectTypeP type);
  CJObjectTypeP getObjectType(const std::string &name) const;

  CJDictionaryP rootScope() const { return rootScope_; }
  CJDictionaryP currentScope() const { return currentScope_; }

  void addFunction(const std::string &name, CJValueP fn);

  CJValueP setProperty(const std::string &name, CJValueP value);

  bool lookupPropertyData(const Identifiers &identifiers, CJPropertyData &data);

  bool lookupPropertyData(CJDictionaryP scope, const Identifiers &identifiers,
                          CJPropertyData &data, int ind);
  bool lookupPropertyData(CJObjectP obj, const Identifiers &identifiers,
                          CJPropertyData &data, int ind);
  bool lookupPropertyData(CJValueP value, const Identifiers &identifiers,
                          CJPropertyData &data, int ind);

  CJValueP lookupValue(const Identifiers &identifiers);

  CJLValueP lookupLValue(const Identifiers &identifiers);

  CJValueP lookupFunction(const Identifiers &identifiers);

  CJLValueP lookupProperty(const Identifiers &identifiers, bool create=false);

  ValuePair lookupObjectProperty(const Identifiers &identifiers, bool create=false);

  bool deleteProperty(const Identifiers &identifiers);
  bool deleteProperty(CJDictionaryP scope, const Identifiers &identifiers);

  CJValueP setVariable(const Identifiers &identifiers, CJValueP value);

  void addTypeObject(CJToken::Type type, CJObjectTypeP obj);

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

  void loadFile(const std::string &filename);

  void loadString(const std::string &str);

  void interp(ExecData &execData);

  CJValueP exec();

  int cmp(CJValueP value1, CJValueP value2);

  CJValueP execBinaryOp(CJOperator::Type op, CJValueP value1, CJValueP value2);
  CJValueP execUnaryOp (CJOperator::Type op, CJValueP value);

  CJValueP createBoolValue(bool b) const {
    CJavaScript *th = const_cast<CJavaScript *>(this);

    if (b)
      return CJValueP(new CJTrue(th));
    else
      return CJValueP(new CJFalse(th));
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

  virtual long setInterval(const std::string & /*proc*/, double /*msecs*/) { return -1; }

  virtual void clearInterval(int /*timer*/) { }

  void errorMsg(const std::string &msg) const;

 private:
  void loadText(const std::string &str);

  void parseString(const std::string &str);

  void skipSpace(CStrParse &parse);

  void readIdentifier(CStrParse &parse);
  void readNumber(CStrParse &parse);
  void readOperator(CStrParse &parse, bool allowUnary);
  void readDoubleString(CStrParse &parse);
  void readSingleString(CStrParse &parse);

  CJToken::Type lastTokenType() const;

  std::pair<bool, CJKeyword::Type> isKeyword(const std::string &name) const;

  CJExecExpressionListP interpExpressionList();
  CJExecExpressionP     interpExpression();
  CJExecIdentifiersP    interpIdentfiers();
  CJExecBlockP          interpBlock(CJExecBlock::Type type);
  CJExecForP            interpFor();
  bool                  isInterpForIn();
  CJExecQuestionP       interpQuestion(CJExecExpressionP bexpr);
  CJExecIfP             interpIf();
  CJExecWhileP          interpWhile();
  CJExecSwitchP         interpSwitch();
  CJExecTryP            interpTry();
  CJExecThrowP          interpThrow();
  CJExecVarP            interpVar();
  CJExecWithP           interpWith();
  CJExecArrayP          interpArray();
  bool                  isInterpDictionary() const;
  CJExecDictionaryP     interpDictionary();
  CJExecNewP            interpNew();
  CJExecDeleteP         interpDelete();
  CJExecReturnP         interpReturn();
  CJUserFunctionP       interpUserFunction(bool named);
  CJExecFunctionP       interpFunction();

  bool isOperator(CJTokenP token, CJOperator::Type opType) const;
  bool isKeyword (CJTokenP token, CJKeyword ::Type opType) const;

  bool isExecOperator(CJOperator::Type opType) const;

 private:
  typedef std::vector<CJValueP>                 Values;
  typedef std::vector<ExecData *>               ExecDataStack;
  typedef std::vector<CJDictionaryP>            ScopeStack;
  typedef std::vector<ScopeStack>               ScopeStackStack;
  typedef std::vector<CJExecBlockP>             BlockStack;
  typedef std::vector<CJUserFunctionP>          FunctionStack;
  typedef std::map<std::string,CJFunctionP>     Functions;
  typedef std::map<CJToken::Type,CJObjectTypeP> TypeObject;
  typedef std::map<std::string,CJObjectTypeP>   NamedType;

  bool            parseDebug_  { false };
  bool            interpDebug_ { false };
  bool            execDebug_   { false };
  std::string     fileName_;
  int             lineNum_ { 0 };
  NamedType       namedType_;
  CJDictionaryP   rootScope_;
  CJDictionaryP   currentScope_;
  ScopeStack      scopeStack_;
  ScopeStack      currentScopeStack_;
  ScopeStackStack scopeStackStack_;
  Tokens          tokens_;
  ETokens         etokens_;
  TypeObject      typeObject_;
  ExecData*       execData_ { 0 };
  ExecDataStack   execDataStack_;
  CJExecBlockP    block_;
  BlockStack      blockStack_;
  UserFunctions   userFunctions_;
  FunctionStack   functions_;
};

#endif
