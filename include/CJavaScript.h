#ifndef CJavaScript_H
#define CJavaScript_H

#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <memory>
#include <limits>
#include <ostream>
#include <sstream>
#include <cassert>
#include <cmath>

class CStrParse;
class CJavaScript;

class CJValue;

typedef std::shared_ptr<CJValue> CJValueP;

//------

class CJToken {
 public:
  enum class Type {
    // basic (parse) tokens
    None,
    Identifier,
    Keyword,
    Number,
    Undefined,
    Null,
    True,
    False,
    Operator,
    String,
    Array,
    Object,

    // extended (interp) tokens
    Variable,
    Identifiers,
    Expression,
    ExpressionList,
    Dictionary,
    AssignExpression,
    IndexExpression,
    IncrDecrExpression,
    Block,
    Function,
    For,
    If,
    While,
    Switch,
    Try,
    Question,
    Var,
    Break,
    Continue,
    Return
  };

 public:
  CJToken(Type type) :
   type_(type) {
  }

  virtual ~CJToken() { }

  Type type() const { return type_; }

  template<typename T> T *cast() { return static_cast<T *>(this); }

  virtual bool isValue() const { return false; }

  virtual CJValueP exec(CJavaScript *) { return CJValueP(); }

  virtual void print(std::ostream &os) const = 0;

  friend std::ostream &operator<<(std::ostream &os, const CJToken &rhs) {
    rhs.print(os);

    return os;
  }

 protected:
  Type type_;
};

typedef std::shared_ptr<CJToken> CJTokenP;

//------

class CJIdentifier : public CJToken {
 public:
  CJIdentifier(const std::string &id) :
   CJToken(Type::Identifier), id_(id) {
  }

  const std::string &name() const { return id_; }

  void print(std::ostream &os) const override { os << id_; }

 private:
  std::string id_;
};

//------

class CJKeyword : public CJToken {
 public:
  enum class Type {
    None,
    Break,
    Case,
    Catch,
    Class,
    Const,
    Continue,
    Default,
    Delete,
    Do,
    Else,
    Finally,
    For,
    Function,
    If,
    In,
    New,
    Return,
    Switch,
    This,
    Throw,
    Try,
    Var,
    Void,
    While,
    With
  };

 public:
  CJKeyword(const Type &type) :
   CJToken(CJToken::Type::Keyword), type_(type) {
  }

  Type type() const { return type_; }

  std::string name() const;

  void print(std::ostream &os) const override { os << name(); }

 private:
  Type type_;
};

//------

class CJObjectType {
 public:
  typedef std::set<std::string> Functions;
  typedef std::vector<CJValueP> Values;

 public:
  CJObjectType(const CJToken::Type type, const std::string &name) :
   type_(type), name_(name) {
  }

  const CJToken::Type &type() const { return type_; }

  const std::string &name() const { return name_; }

  const Functions &functions() const { return functions_; }

  void addFunction(const std::string &name) {
    functions_.insert(name);
  }

  bool hasFunction(const std::string &name) {
    return (functions_.find(name) != functions_.end());
  }

  virtual CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) = 0;

 protected:
  CJToken::Type type_;
  std::string   name_;
  Functions     functions_;
};

typedef std::shared_ptr<CJObjectType> CJObjectTypeP;

//------

class CJValue : public CJToken {
 public:
  CJValue(CJObjectTypeP valueType) :
   CJToken(valueType->type()), valueType_(valueType) {
  }

  virtual ~CJValue() { }

  virtual CJValue *dup(CJavaScript *) const = 0;

  virtual CJObjectTypeP valueType() const { return valueType_; }

  virtual std::string toString() const = 0;

  virtual double toReal() const = 0;

  virtual long toInteger() const { return int(toReal()); }

  virtual bool toBoolean() const = 0;

  bool isValue() const override { return true; }

  virtual bool hasIndex() const { return false; }

  virtual CJValueP indexValue(int) const { assert(false); return CJValueP(); }

  virtual void setIndexValue(int, CJValueP) { assert(false); }

  virtual int length() const { return 0; }

  virtual bool cmp(const CJValue *v) const {
    std::string s1 =    toString();
    std::string s2 = v->toString();

    if (s1 < s2) return -1;
    if (s1 > s2) return  1;

    return 0;
  }

 protected:
  CJObjectTypeP valueType_;
};

//------

// Undefined Type
class CJUndefinedType : public CJObjectType {
 public:
  CJUndefinedType() :
   CJObjectType(CJToken::Type::Undefined, "undefined") {
  }

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;
};

// Null Type
class CJNullType : public CJObjectType {
 public:
  CJNullType() :
   CJObjectType(CJToken::Type::Null, "null") {
  }

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;
};

// True Type
class CJTrueType : public CJObjectType {
 public:
  CJTrueType() :
   CJObjectType(CJToken::Type::True, "true") {
  }

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;
};

// False Type
class CJFalseType : public CJObjectType {
 public:
  CJFalseType() :
   CJObjectType(CJToken::Type::False, "false") {
  }

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;
};

// Number Type
class CJNumberType : public CJObjectType {
 public:
  CJNumberType() :
   CJObjectType(CJToken::Type::Number, "number") {
  }

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;
};

// String Type
class CJStringType : public CJObjectType {
 public:
  CJStringType() :
   CJObjectType(CJToken::Type::String, "string") {
  }

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;
};

// Array Type
class CJArrayType : public CJObjectType {
 public:
  CJArrayType() :
   CJObjectType(CJToken::Type::Array, "array") {
  }

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;
};

// Dictionary Type
class CJDictionaryType : public CJObjectType {
 public:
  CJDictionaryType() :
   CJObjectType(CJToken::Type::Dictionary, "dictionary") {
  }

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;
};

// Function Type
class CJFunctionType : public CJObjectType {
 public:
  CJFunctionType() :
   CJObjectType(CJToken::Type::Function, "function") {
  }

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;
};

//------

class CJUndefined : public CJValue {
 public:
  CJUndefined(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJUndefined(js); }

  std::string toString() const override { return "undefined"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const override { os << "undefined"; }
};

//------

class CJNull : public CJValue {
 public:
  CJNull(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJNull(js); }

  std::string toString() const override { return "null"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const override { os << "null"; }
};

//------

class CJTrue : public CJValue {
 public:
  CJTrue(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJTrue(js); }

  std::string toString() const override { return "true"; }

  double toReal() const override { return 1; }

  bool toBoolean() const override { return true; }

  void print(std::ostream &os) const override { os << "true"; }
};

//------

class CJFalse : public CJValue {
 public:
  CJFalse(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJFalse(js); }

  std::string toString() const override { return "false"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const override { os << "false"; }
};

//------

class CJNumber : public CJValue {
 public:
  CJNumber(CJavaScript *js, double real);

  CJNumber *dup(CJavaScript *js) const override { return new CJNumber(js, real_); }

  std::string toString() const override {
    std::ostringstream ss;

    ss.precision(std::numeric_limits<double>::max_digits10);

    ss << real_;

    return ss.str();
  }

  double real() const { return real_; }

  double toReal() const override { return real_; }

  bool toBoolean() const override { return (real_ != 0.0); }

  bool cmp(const CJValue *v) const override {
    double r1 =    toReal();
    double r2 = v->toReal();

    if (r1 < r2) return -1;
    if (r1 > r2) return  1;

    return 0;
  }

  void print(std::ostream &os) const override {
    os << toString();
  }

 private:
  double real_;
};

//------

class CJString : public CJValue {
 public:
  CJString(CJavaScript *js, const std::string &text="", char c='\"');

  CJString *dup(CJavaScript *js) const override { return new CJString(js, text_, c_); }

  const std::string &text() const { return text_; }
  void setText(const std::string &str) { text_ = str; }

  std::string toString() const override { return text_; }

  double toReal() const override { return atof(text_.c_str()); }

  long toInteger() const override { return atol(text_.c_str()); }

  bool toBoolean() const override { return (! text_.empty()); }

  int length() const override { return text_.size(); }

  void print(std::ostream &os) const override {
    os << "\'" << text_ << "\'";
  }

 private:
  std::string text_;
  char        c_;
};

//------

class CJArray : public CJValue {
 public:
  typedef std::deque<CJValueP> Values;

 public:
  CJArray(CJavaScript *js, int n=0);

  CJArray(CJavaScript *js, const Values &values);

  CJArray(CJavaScript *js, const std::vector<CJValueP> &values);

  CJArray *dup(CJavaScript *js) const override { return new CJArray(js, values_); }

  Values values() const { return values_; }

  std::string toString() const override { return ""; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return ! values_.empty(); }

  void addValue(CJValueP value) {
    values_.push_back(value);
  }

  CJValueP removeValue() {
    assert(! values_.empty());

    CJValueP value = values_.back();

    values_.pop_back();

    return value;
  }

  void addFrontValue(CJValueP value) {
    values_.push_front(value);
  }

  CJValueP removeFrontValue() {
    assert(! values_.empty());

    CJValueP value = values_.front();

    values_.pop_front();

    return value;
  }

  bool hasIndex() const override { return true; }

  CJValueP indexValue(int i) const override {
    if (i < 0 || i >= int(values_.size()))
      return CJValueP();

    return values_[i];
  }

  void setIndexValue(int i, CJValueP value) override {
    if (i < 0 || i >= int(values_.size()))
      return;

    values_[i] = value;
  }

  int length() const override { return values_.size(); }

  void print(std::ostream &os) const override {
    os << "[";

    int i = 0;

    for (auto &v : values_) {
      if (i > 0)
        os << ",";

      os << " ";

      if (v)
        os << *v;

      ++i;
    }

    if (i > 0)
      os << " ";

    os << "]";
  }

 private:
  Values values_;
};

//------

class CJOperator : public CJToken {
 public:
  enum class Type {
    None,
    UnaryPlus,
    Plus,
    PlusAssign,
    UnaryMinus,
    Minus,
    MinusAssign,
    Times,
    TimesAssign,
    Divide,
    DivideAssign,
    Modulus,
    ModulusAssign,
    Increment,
    Decrement,
    OpenRBracket,
    CloseRBracket,
    OpenSBracket,
    CloseSBracket,
    OpenBrace,
    CloseBrace,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    Equal,
    StrictEqual,
    NotEqual,
    StrictNotEqual,
    Assign,
    LeftShift,
    LeftShiftAssign,
    RightShift,
    RightShiftAssign,
    UnsignedRightShift,
    UnsignedRightShiftAssign,
    LogicalAnd,
    LogicalOr,
    LogicalNot,
    BitwiseAnd,
    BitwiseAndAssign,
    BitwiseOr,
    BitwiseOrAssign,
    BitwiseNot,
    BitwiseNotAssign,
    BitwiseXor,
    BitwiseXorAssign,
    Scope,
    Comma,
    SemiColon,
    Question,
    Colon,
    TypeOf
  };

  enum Associativty {
    Left,
    Right
  };

  enum Ary {
    None,
    Unary,
    Binary
  };

 public:
  CJOperator(const Type &type, int precedence, Associativty associativty, Ary ary) :
   CJToken(CJToken::Type::Operator), type_(type), precedence_(precedence),
   associativty_(associativty), ary_(ary) {
  }

  CJOperator *dup() const { return new CJOperator(type_, precedence_, associativty_, ary_); }

  Type type() const { return type_; }

  int precedence() const { return precedence_; }

  Ary ary() const { return ary_; }

  Associativty associativty() const { return associativty_; }

  bool isAssign() const {
    return (type_ == Type::PlusAssign ||
            type_ == Type::MinusAssign ||
            type_ == Type::TimesAssign ||
            type_ == Type::DivideAssign ||
            type_ == Type::ModulusAssign ||
            type_ == Type::Assign ||
            type_ == Type::LeftShiftAssign ||
            type_ == Type::RightShiftAssign ||
            type_ == Type::UnsignedRightShiftAssign ||
            type_ == Type::BitwiseAndAssign ||
            type_ == Type::BitwiseOrAssign ||
            type_ == Type::BitwiseNotAssign ||
            type_ == Type::BitwiseXorAssign);
  }

  void print(std::ostream &os) const override { os << name(); }

  std::string name() const;

 private:
  Type         type_         { Type::None };
  int          precedence_   { 0 };
  Associativty associativty_ { Associativty::Left };
  Ary          ary_          { Ary::Binary };
};

typedef std::shared_ptr<CJOperator> CJOperatorP;

//------

// <identifer> [.<identifier]*
class CJExecIdentifiers : public CJToken {
 public:
  typedef std::vector<CJIdentifier *> Identifiers;

 public:
  CJExecIdentifiers() :
   CJToken(CJToken::Type::Identifiers) {
  }

  void addIdentifier(CJIdentifier *identifier) {
    identifiers_.push_back(identifier);
  }

  const Identifiers &identifiers() const { return identifiers_; }

  void print(std::ostream &os) const override {
    int i = 0;

    for (auto &id : identifiers_) {
      if (i > 0)
        os << ".";

      os << id->name();

      ++i;
    }
  }

 private:
  Identifiers identifiers_;
};

typedef std::shared_ptr<CJExecIdentifiers> CJExecIdentifiersP;

//------

// <expression>
class CJExecExpression : public CJToken {
 public:
  typedef std::vector<CJTokenP> Tokens;

 public:
  CJExecExpression(Type type=CJToken::Type::Expression) :
   CJToken(type) {
  }

  void addToken(CJTokenP token) {
    tokens_.push_back(token);
  }

  const Tokens &tokens() const { return tokens_; }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    int i = 0;

    for (auto &t : tokens_) {
      if (i > 0)
        os << " ";

      os << *t;

      ++i;
    }
  }

 private:
  Tokens tokens_;
};

typedef std::shared_ptr<CJExecExpression> CJExecExpressionP;

//------

// <expression> [, <expression>]*
class CJExecExpressionList : public CJToken {
 public:
  typedef std::vector<CJExecExpressionP> Expressions;
  typedef std::vector<CJValueP>          Values;

 public:
  CJExecExpressionList(CJToken::Type type=CJToken::Type::ExpressionList) :
   CJToken(type) {
  }

  const Expressions &expressions() const { return expressions_; }

  void addExpression(CJExecExpressionP expr) {
    expressions_.push_back(expr);
  }

  CJExecExpressionP indexExpression(int i) {
    if (i < 0 || i >= int(expressions_.size()))
      return CJExecExpressionP();

    return expressions_[i];
  }

  Values getValues(CJavaScript *js) const {
    Values values;

    for (auto &e : expressions_) {
      assert(e);

      CJValueP value = e->exec(js);

      values.push_back(value);
    }

    return values;
  }

  CJValueP exec(CJavaScript *js) override {
    Values values = getValues(js);

    if (values.empty())
      return CJValueP();

    return values.back();
  }

  void print(std::ostream &os) const override {
    int i = 0;

    for (auto &e : expressions_) {
      if (i > 0)
        os << ", ";

      os << *e;

      ++i;
    }
  }

 private:
  Expressions expressions_;
};

typedef std::shared_ptr<CJExecExpressionList> CJExecExpressionListP;

//------

// builtin function
class CJFunction : public CJValue {
 public:
  enum class Type {
    Normal,
    Real,
    Real2,
    User,
    Object
  };

  typedef std::vector<CJValueP> Values;

 public:
  CJFunction(CJavaScript *js, const std::string &name, Type type=Type::Normal);

  CJValue *dup(CJavaScript *) const override { assert(false); return 0; }

  const std::string &name() const { return name_; }

  std::string toString() const override { return name_; }

  double toReal() const override { return 1; }

  bool toBoolean() const override { return true; }

  virtual CJValueP exec(CJavaScript *js, const Values &values) = 0;

  void print(std::ostream &os) const override {
    os << name_ << "()";
  }

 protected:
  std::string name_;
  Type        type_;
};

typedef std::shared_ptr<CJFunction> CJFunctionP;

//------

class CJNameSpace {
 public:
  CJNameSpace(const std::string &name) :
   name_(name) {
  }

  void setProperty(const std::string &name, CJValueP value) {
    properties_[name] = value;
  }

  CJValueP getProperty(const std::string &name) {
    auto p = properties_.find(name);
    if (p == properties_.end()) return CJValueP();

    return (*p).second;
  }

 private:
  typedef std::map<std::string,CJValueP> Properties;

  std::string name_;
  Properties  properties_;
};

//------

// { <expression_pair> [, <expression_pair>]* }
class CJDictionary : public CJValue {
 public:
  typedef std::map<std::string,CJValueP> KeyValues;

 public:
  CJDictionary(CJavaScript *js, const KeyValues &keyValues=KeyValues());

  CJValue *dup(CJavaScript *js) const override {
    return new CJDictionary(js, keyValues_);
  }

  void setProperty(CJavaScript *js, const std::string &key, double r) {
    CJValueP value(new CJNumber(js, r));

    setProperty(key, value);
  }

  void setProperty(CJavaScript *js, const std::string &key, long i) {
    CJValueP value(new CJNumber(js, i));

    setProperty(key, value);
  }

  void setProperty(CJavaScript *, const std::string &key, CJFunction *function) {
    CJValueP value(function);

    setProperty(key, value);
  }

  void setProperty(CJavaScript *, const std::string &key, CJValueP value) {
    setProperty(key, value);
  }

  void setProperty(const std::string &key, CJValueP value) {
    keyValues_[key] = value;
  }

  bool hasProperty(const std::string &key) const {
    return (keyValues_.find(key) != keyValues_.end());
  }

  CJValueP lookupProperty(const std::string &key) const {
    return indexValue(key);
  }

  void deleteProperty(const std::string &key) {
    keyValues_.erase(key);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;

    return ss.str();
  }

  const KeyValues &keyValues() const { return keyValues_; }

  bool toBoolean() const override { return ! keyValues_.empty(); }

  double toReal() const override { return toBoolean(); }

  CJValueP indexValue(const std::string &key) const {
    auto p = keyValues_.find(key);

    if (p != keyValues_.end())
      return (*p).second;

    return CJValueP();
  }

  void setIndexValue(CJavaScript *js, const std::string &key, CJValueP value) {
    return setIndexValue(CJValueP(new CJString(js, key)), value);
  }

  void setIndexValue(CJValueP ivalue, CJValueP value) {
    std::string key = ivalue->toString();

    for (auto &kv : keyValues_) {
      if (kv.first == key) {
        kv.second = value;
        return;
      }
    }
  }

  void print(std::ostream &os) const override {
    int i = 0;

    os << "{";

    for (auto &kv : keyValues_) {
      if (i > 0)
        os << ",";

      os << " " << kv.first << ": ";

      if (kv.second)
        os << *kv.second;
      else
        os << "<null>";

      ++i;
    }

    if (i > 0)
      os << " ";

    os << "}";
  }

 protected:
  KeyValues keyValues_;
};

typedef std::shared_ptr<CJDictionary> CJDictionaryP;

//------

// <identifiers> <assign_op> <expression>
class CJExecAssignExpression : public CJExecExpression {
 public:
  CJExecAssignExpression() :
   CJExecExpression(CJToken::Type::AssignExpression) {
  }

  void setLExpression(CJExecExpressionP lexpr) {
    lexpr_ = lexpr;
  }

  void setOperator(CJOperatorP op) {
    op_ = op;
  }

  void setRExpression(CJExecExpressionP rexpr) {
    rexpr_ = rexpr;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    if (lexpr_ && op_ && rexpr_) {
      os << *lexpr_ << " " << *op_ << " " << *rexpr_;
    }
  }

 private:
  CJExecExpressionP lexpr_;
  CJOperatorP       op_;
  CJExecExpressionP rexpr_;
};

//------

// <expression> [ <expression> ]
class CJExecIndexExpression : public CJExecExpression {
 public:
  CJExecIndexExpression() :
   CJExecExpression(CJToken::Type::IndexExpression) {
  }

  CJExecIdentifiersP identifiers() const { return identifiers_; }
  void setIdentifiers(CJExecIdentifiersP identifiers) { identifiers_ = identifiers; }

  void setExpression(CJExecExpressionP expr) { expr_ = expr; }

  CJExecExpressionP indexExpression() const  { return iexpr_; }
  void setIndexExpression(CJExecExpressionP iexpr) { iexpr_ = iexpr; }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    if      (expr_) {
      if (iexpr_)
        os << *expr_ << "[" << *iexpr_ << "]";
      else
        os << *expr_ << "[]";
    }
    else if (identifiers_) {
      if (iexpr_)
        os << *identifiers_ << "[" << *iexpr_ << "]";
      else
        os << *identifiers_ << "[]";
    }
  }

 private:
  CJExecIdentifiersP identifiers_;
  CJExecExpressionP  expr_;
  CJExecExpressionP  iexpr_;
};

//------

// --<identifiers> | <identifiers>-- | ++<identifiers> | <identifiers>++
class CJExecIncrDecrExpression : public CJExecExpression {
 public:
  CJExecIncrDecrExpression() :
   CJExecExpression(CJToken::Type::IncrDecrExpression) {
  }

  void setIdentifiers(CJExecIdentifiersP identifiers) {
    identifiers_ = identifiers;
  }

  void setExpression(CJExecExpressionP expr) {
    expr_ = expr;
  }

  void setOperator(CJOperatorP op) {
    op_ = op;
  }

  void setPost(bool b) {
    post_ = b;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    if (! op_) return;

    if (identifiers_) {
      if (post_)
        os << *identifiers_ << *op_;
      else
        os << *op_ << *identifiers_;
    }
    else {
      if (post_)
        os << *expr_ << *op_;
      else
        os << *op_ << *expr_;
    }
  }

 private:
  CJExecIdentifiersP identifiers_;
  CJExecExpressionP  expr_;
  CJOperatorP        op_;
  bool               post_ { false };
};

//------

// { [<statement>]* }
class CJExecBlock : public CJToken {
 public:
  enum class Type {
    Sequential,
    Iterative,
    Function
  };

 public:
  typedef std::vector<CJTokenP> ETokens;

 public:
  CJExecBlock(Type type) :
   CJToken(CJToken::Type::Block), type_(type) {
  }

  Type type() const { return type_; }

  void addToken(CJTokenP token) {
    tokens_.push_back(token);
  }

  bool anyTokens() const {
    return ! tokens_.empty();
  }

  ETokens &etokens() { return etokens_; }

  CJValueP getRetVal() const { return retVal_; }
  void setRetVal(CJValueP v) { retVal_ = v; }

  bool isBreakFlag() const { return breakFlag_; }
  void setBreakFlag(bool b) { breakFlag_ = b; }

  bool isContinueFlag() const { return continueFlag_; }
  void setContinueFlag(bool b) { continueFlag_ = b; }

  void interp(CJavaScript *js);

  void initExec() {
    retVal_       = 0;
    breakFlag_    = false;
    continueFlag_ = false;
  }

  CJValueP exec(CJavaScript *js);

  void print(std::ostream &os) const override {
    os << "{" << std::endl;

    if (! etokens_.empty()) {
      for (auto &t : etokens_)
        os << " " << *t;
    }
    else {
      for (auto &t : tokens_)
        os << " " << *t;
    }

    os << std::endl << "}";
  }

 private:
  typedef std::vector<CJTokenP> Tokens;

  Type     type_ { Type::Sequential };
  Tokens   tokens_;
  ETokens  etokens_;
  CJValueP retVal_;
  bool     breakFlag_ { false };
  bool     continueFlag_ { false };
};

typedef std::shared_ptr<CJExecBlock> CJExecBlockP;

//------

// function <name> ( <args> ) { [<statement>]* }
class CJExecFunction : public CJToken {
 public:
  CJExecFunction() :
   CJToken(CJToken::Type::Function) {
  }

  void setIdentifiers(CJExecIdentifiersP identifiers) {
    identifiers_ = identifiers;
  }

  void setExpression(CJExecExpressionP expr) {
    expr_ = expr;
  }

  void setExprList(CJExecExpressionListP exprList) {
    exprList_ = exprList;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    if      (identifiers_) {
      if (exprList_)
        os << *identifiers_ << "(" << *exprList_ << ")";
      else
        os << *identifiers_ << "()";
    }
    else if (expr_) {
      if (exprList_)
        os << *expr_ << "(" << *exprList_ << ")";
      else
        os << *expr_ << "()";
    }
  }

 private:
  CJExecIdentifiersP    identifiers_;
  CJExecExpressionP     expr_;
  CJExecExpressionListP exprList_;
};

//------

// var <identifiers> [= <value>]
class CJExecVar : public CJToken {
 public:
  CJExecVar() :
   CJToken(CJToken::Type::Var) {
  }

  CJExecIdentifiersP identifiers() const { return identifiers_; }
  void setIdentifiers(CJExecIdentifiersP identifiers) { identifiers_ = identifiers; }

  void setExprList(CJExecExpressionListP exprList) {
    exprList_ = exprList;
  }

  void setBlock(CJExecBlockP block) {
    block_ = block;
  }

  void setValue(CJValueP value) {
    value_ = value;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    os << "var";

    if (identifiers_)
      os << " " << *identifiers_;

    if (exprList_ || block_ || value_) {
      os << " = ";

      if      (exprList_)
        os << *exprList_;
      else if (block_)
        os << *block_;
      else if (value_)
        os << *value_;
    }
  }

 private:
  CJExecIdentifiersP    identifiers_;
  CJExecExpressionListP exprList_;
  CJExecBlockP          block_;
  CJValueP              value_;
};

typedef std::shared_ptr<CJExecVar> CJExecVarP;

//------

// break
class CJExecBreak : public CJToken {
 public:
  CJExecBreak() :
   CJToken(CJToken::Type::Break) {
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    os << "break";
  }
};

//------

// continue
class CJExecContinue : public CJToken {
 public:
  CJExecContinue() :
   CJToken(CJToken::Type::Return) {
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    os << "continue";
  }
};

//------

// return
class CJExecReturn : public CJToken {
 public:
  CJExecReturn(CJExecExpressionP expr) :
   CJToken(CJToken::Type::Return), expr_(expr) {
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    if (expr_)
      os << "return " << *expr_;
  }

 private:
  CJExecExpressionP expr_;
};

//------

// delete
class CJExecDelete : public CJToken {
 public:
  CJExecDelete() :
   CJToken(CJToken::Type::Return) {
  }

  CJValueP exec(CJavaScript *js) override;

  CJExecIdentifiersP identifiers() const { return identifiers_; }
  void setIdentifiers(CJExecIdentifiersP identifiers) { identifiers_ = identifiers; }

  void print(std::ostream &os) const override {
    if (identifiers_)
      os << "delete " << *identifiers_;
  }

 private:
  CJExecIdentifiersP identifiers_;
};

//------

// for (<expression_list> ; <expression_list> ; <expression_list> ) <block>
class CJExecFor : public CJToken {
 public:
  CJExecFor() :
   CJToken(CJToken::Type::For) {
  }

  void setExprList1(CJExecExpressionListP exprList) {
    exprList1_ = exprList;
  }

  void setVar(CJExecVarP var) {
    var_ = var;
  }

  void setExprList2(CJExecExpressionListP exprList) {
    exprList2_ = exprList;
  }

  void setExprList3(CJExecExpressionListP exprList) {
    exprList3_ = exprList;
  }

  void setIdentifiers(CJExecIdentifiersP identifiers, bool var) {
    identifiers_ = identifiers;
    ivar_        = var;
  }

  void setInExpr(CJExecExpressionP expr) {
    inExpr_ = expr;
  }

  void setBlock(CJExecBlockP block) {
    block_ = block;
  }

  void interp(CJavaScript *js);

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    os << "for (";

    if (var_) {
      os << *var_;
    }
    else {
      if (exprList1_)
        os << *exprList1_;
    }

    if (inExpr_)
      os << " in " << *inExpr_;
    else {
      os << ";";

      if (exprList2_)
        os << " " << *exprList2_;

      os << ";";

      if (exprList3_)
        os << " " << *exprList3_;
    }

    os << ")";

    if (block_)
      os << " " << *block_;
  }

 private:
  CJExecVarP            var_;
  CJExecExpressionListP exprList1_;
  CJExecExpressionListP exprList2_;
  CJExecExpressionListP exprList3_;
  CJExecExpressionP     inExpr_;
  CJExecIdentifiersP    identifiers_;
  bool                  ivar_ { false };
  CJExecBlockP          block_;
};

//------

// <expression> ? <expression> : <expression>
class CJExecQuestion : public CJToken {
 public:
  CJExecQuestion(CJExecExpressionP bexpr) :
   CJToken(CJToken::Type::Question), bexpr_(bexpr) {
  }

  void setExpr1(CJExecExpressionP expr) {
    expr1_ = expr;
  }

  void setExpr2(CJExecExpressionP expr) {
    expr2_ = expr;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    if (bexpr_ && expr1_ && expr2_) {
      os << *bexpr_ << " ? " << *expr1_ << " : " << *expr2_;
    }
  }

 private:
  CJExecExpressionP bexpr_;
  CJExecExpressionP expr1_;
  CJExecExpressionP expr2_;
};

//------

// if ( <expression> ) <block> [ else <block> ]
class CJExecIf : public CJToken {
 public:
  CJExecIf() :
   CJToken(CJToken::Type::If) {
  }

  void setExprList(CJExecExpressionListP exprList) {
    exprList_ = exprList;
  }

  void setIfBlock(CJExecBlockP block) {
    ifBlock_ = block;
  }

  void setElseBlock(CJExecBlockP block) {
    elseBlock_ = block;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    if (exprList_ && ifBlock_) {
      if (elseBlock_)
        os << "if (" << *exprList_ << ") " << *ifBlock_ << " else " << *elseBlock_;
      else
        os << "if (" << *exprList_ << ") " << *ifBlock_;
    }
  }

 private:
  CJExecExpressionListP exprList_;
  CJExecBlockP          ifBlock_;
  CJExecBlockP          elseBlock_;
};

//------

// while ( <expression> ) <block>
class CJExecWhile : public CJToken {
 public:
  CJExecWhile() :
   CJToken(CJToken::Type::While) {
  }

  void setExprList(CJExecExpressionListP exprList) {
    exprList_ = exprList;
  }

  void setBlock(CJExecBlockP block) {
    block_ = block;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    if (exprList_ && block_) {
      os << "while (" << *exprList_ << ") " << *block_;
    }
  }

 private:
  CJExecExpressionListP exprList_;
  CJExecBlockP          block_;
};

//------

// switch ( <expression> ) { [case <expr> : <exprList> ; break; ]* }
class CJExecSwitch : public CJToken {
 public:
  struct CaseBlock {
    CaseBlock(CJExecExpressionP expr1, CJExecBlockP block1) :
     expr(expr1), block(block1) {
    }

    CJExecExpressionP expr;
    CJExecBlockP      block;
  };

  struct DefaultBlock {
    CJExecBlockP block;
  };

 public:
  CJExecSwitch() :
   CJToken(CJToken::Type::Switch) {
  }

  void setExprList(CJExecExpressionListP exprList) {
    exprList_ = exprList;
  }

  void addCase(CJExecExpressionP expr, CJExecBlockP block) {
    caseBlocks_.push_back(CaseBlock(expr, block));
  }

  void setDefault(CJExecBlockP block) {
    defaultBlock_.block = block;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    os << "switch (";

    if (exprList_)
      os << *exprList_;

    os << ") {" << std::endl;

    for (const auto &c : caseBlocks_) {
      if (c.expr && c.block)
        os << "case " << *c.expr << " : " << *c.block << std::endl;
    }

    if (defaultBlock_.block) {
      os << "default : " << *defaultBlock_.block << std::endl;
    }

    os << "}";
  }

 private:
  typedef std::vector<CaseBlock> CaseBlocks;

  CJExecExpressionListP exprList_;
  CaseBlocks            caseBlocks_;
  DefaultBlock          defaultBlock_;
};

//------

// try <block> catch ( <identifiers> ) <block>
class CJExecTry : public CJToken {
 public:
  CJExecTry() :
   CJToken(CJToken::Type::Try) {
  }

  void setTryBlock(CJExecBlockP block) {
    tryBlock_ = block;
  }

  void setCatchIdentifiers(CJExecIdentifiersP identifiers) {
    catchIdentifiers_ = identifiers;
  }

  void setCatchBlock(CJExecBlockP block) {
    catchBlock_ = block;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    if (tryBlock_ && catchIdentifiers_ && catchBlock_) {
      os << "try " << *tryBlock_ << " catch (" << *catchIdentifiers_ << ") " << *catchBlock_;
    }
  }

 private:
  CJExecBlockP       tryBlock_;
  CJExecIdentifiersP catchIdentifiers_;
  CJExecBlockP       catchBlock_;
};

//------

// function calling API with signature: double result = fn(double r);
class CJRealFunction : public CJFunction {
 public:
  typedef double (*Fn)(double);

 public:
  CJRealFunction(CJavaScript *js, const std::string &name, Fn fn) :
   CJFunction(js, name, CJFunction::Type::Real), fn_(fn) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJRealFunction(js, name_, fn_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "double " << name_ << "(double)";
  }

 protected:
  Fn fn_;
};

//------

// function calling API with signature: double result = fn(double r1, double r2);
class CJReal2Function : public CJFunction {
 public:
  typedef double (*Fn)(double, double);

 public:
  CJReal2Function(CJavaScript *js, const std::string &name, Fn fn) :
   CJFunction(js, name, CJFunction::Type::Real2), fn_(fn) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJReal2Function(js, name_, fn_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "double " << name_ << "(double, double)";
  }

 protected:
  Fn fn_;
};

//------

// user defined function
class CJUserFunction : public CJFunction {
 public:
  typedef std::vector<std::string> Args;

 public:
  CJUserFunction(CJavaScript *js, const std::string &name, const Args &args, CJExecBlockP block) :
   CJFunction(js, name, CJFunction::Type::User), args_(args), block_(block) {
  }

  CJValue *dup(CJavaScript *js) const override {
    return new CJUserFunction(js, name_, args_, block_);
  }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    if (block_) {
      os << "function " << name() << "(";

      int i = 0;

      for (const auto &arg : args_) {
        if (i > 0)
          os << ", ";

        os << arg;

        ++i;
      }

      os << ") " << *block_;
    }
  }

 private:
  Args         args_;
  CJExecBlockP block_;
};

//------

// object function
class CJObjectFunction : public CJFunction {
 public:
  CJObjectFunction(CJavaScript *js, const std::string &name) :
   CJFunction(js, name, CJFunction::Type::Object) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJObjectFunction(js, name_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "object fn";
  }
};

//------

class CJLValue : public CJValue {
 public:
  CJLValue(CJObjectTypeP valueType) :
   CJValue(valueType) {
  }

  virtual ~CJLValue() { }

  virtual CJValueP value() const = 0;
  virtual void setValue(CJValueP value) = 0;
};

typedef std::shared_ptr<CJLValue> CJLValueP;

//------

class CJDictionaryRef : public CJLValue {
 public:
  CJDictionaryRef(CJavaScript *js, CJDictionary *dict, const std::string &name);

  CJValue *dup(CJavaScript *js) const override { return new CJDictionaryRef(js, dict_, name_); }

  CJValueP value() const;

  void setValue(CJValueP value);

  std::string toString() const override { return ""; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const override {
    os << name_;
  }

 private:
  CJavaScript*  js_ { 0 };
  CJDictionary* dict_ { 0 };
  std::string   name_;
};

//------

class CJObject : public CJValue {
 public:
  typedef std::set<std::string> Names;
  typedef std::vector<CJValueP> Values;

 public:
  CJObject(CJObjectTypeP type) :
   CJValue(type), type_(type) {
  }

  virtual ~CJObject() { }

  const std::string &name() const { return type_->name(); }

  virtual bool hasIndex() const { return false; }

  virtual CJValueP indexValue(int) const { assert(false); return CJValueP(); }

  virtual void setIndexValue(int, CJValueP) { assert(false); }

  virtual int length() const { return 0; }

  virtual CJValueP getNameValue(CJavaScript *, const std::string &) {
    return CJValueP();
  }

  virtual void setNameValue(CJavaScript *, const std::string &, CJValueP) {
  }

  virtual CJValueP execNameFn(CJavaScript *, const std::string &, const Values &) {
    return CJValueP();
  }

 protected:
  CJObjectTypeP type_;
};

typedef std::shared_ptr<CJObject> CJObjectP;

//------

class CJObjectValue : public CJLValue {
 public:
  typedef std::vector<CJValueP> Values;

 public:
  CJObjectValue(CJavaScript *js, CJObjectP obj, const std::string &name);

  CJValue *dup(CJavaScript *js) const override { return new CJObjectValue(js, obj_, name_); }

  CJValueP value() const;

  void setValue(CJValueP value);

  std::string toString() const override { return value()->toString(); }

  double toReal() const override { return value()->toReal(); }

  bool toBoolean() const override { return value()->toBoolean(); }

  void print(std::ostream &os) const override {
    os << *obj_ << " :" << name_;
  }

 private:
  CJavaScript* js_ { 0 };
  CJObjectP    obj_;
  std::string  name_;
};

//------

class CJDocumentType : public CJObjectType {
 public:
  CJDocumentType() :
   CJObjectType(CJToken::Type::Object, "Document") {
  }

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;
};

class CJDocumentObject : public CJObject {
 public:
  CJDocumentObject(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJDocumentObject(js); }

  std::string toString() const override { return "document"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "document"; }

 private:
  CJavaScript *js_;
};

//------

class CJConsoleType : public CJObjectType {
 public:
  CJConsoleType() :
   CJObjectType(CJToken::Type::Object, "Console") {
  }

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;
};

class CJConsoleObject : public CJObject {
 public:
  CJConsoleObject(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJDocumentObject(js); }

  std::string toString() const override { return "console"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return 0; }

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "console"; }

 private:
  CJavaScript *js_;
};

//------

class CJScope;

typedef std::shared_ptr<CJScope> CJScopeP;

class CJScope : public CJDictionary {
 public:
  CJScope(CJavaScript *js, const std::string &name);

  CJValue *dup(CJavaScript *js) const override {
    return new CJScope(js, name_);
  }

  const std::string &name() const { return name_; }

  CJScope *getParent() const { return parent_; }
  void setParent(CJScope *s) { parent_ = s; }

  void     addScope   (CJScopeP scope);
  void     removeScope(const std::string &name);
  CJScopeP lookupScope(const std::string &name);

  std::vector<std::string> getFunctionNames() const;
  std::vector<std::string> getVariableNames() const;

 private:
  typedef std::map<std::string, CJScopeP> Scopes;
  typedef std::map<std::string, CJValueP> Properties;

  CJavaScript* js_ { 0 };
  std::string  name_;
  CJScope*     parent_ { 0 };
  Scopes       scopes_;
};

//------

class CJDictValue : public CJLValue {
 public:
  CJDictValue(CJavaScript *js, CJDictionary *dict, const std::string &name);

  CJValue *dup(CJavaScript *js) const override { return new CJDictValue(js, dict_, name_); }

  std::string toString() const override {
    CJValueP v = value();

    return (v ? v->toString() : "");
  }

  double toReal() const override {
    CJValueP v = value();

    return (v ? v->toReal() : 0.0);
  }

  bool toBoolean() const override {
    CJValueP v = value();

    return (v ? v->toBoolean() : false);
  }

  CJValueP value() const { return dict_->indexValue(name_); }

  void setValue(CJValueP value) { return dict_->setProperty(js_, name_, value); }

  void print(std::ostream &os) const {
    CJValueP v = value();

    if (v)
      v->print(os);
  }

 private:
  CJavaScript*  js_;
  CJDictionary* dict_;
  std::string   name_;
};

//------

class CJScopeValue : public CJLValue {
 public:
  CJScopeValue(CJavaScript *js, CJScope *scope, const std::string &name);

  CJValue *dup(CJavaScript *js) const override { return new CJScopeValue(js, scope_, name_); }

  std::string toString() const override {
    CJValueP v = value();

    return (v ? v->toString() : "");
  }

  double toReal() const override {
    CJValueP v = value();

    return (v ? v->toReal() : 0.0);
  }

  bool toBoolean() const override {
    CJValueP v = value();

    return (v ? v->toBoolean() : false);
  }

  CJValueP value() const { return scope_->lookupProperty(name_); }

  void setValue(CJValueP value) { return scope_->setProperty(name_, value); }

  void print(std::ostream &os) const {
    CJValueP v = value();

    if (v)
      v->print(os);
  }

 private:
  CJScope*    scope_;
  std::string name_;
};

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

class CJTypeObjectFunction : public CJFunction {
 public:
  CJTypeObjectFunction(CJavaScript *js, CJObjectTypeP obj, const std::string &name) :
   CJFunction(js, name, Type::Normal), obj_(obj), name_(name) {
  }

  CJValue *dup(CJavaScript *js) const override {
    return new CJTypeObjectFunction(js, obj_, name_);
  }

  CJValueP exec(CJavaScript *js, const Values &values) {
    return obj_->exec(js, name_, values);
  }

 private:
  CJObjectTypeP obj_;
  std::string   name_;
};

//------

class CJavaScript {
 public:
  typedef std::vector<CJTokenP>            Tokens;
  typedef std::vector<CJTokenP>            ETokens;
  typedef std::vector<CJIdentifier *>      Identifiers;
  typedef std::pair<CJFunctionP, CJValueP> FunctionValuePair;

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

  CJObjectTypeP undefinedType() const { return undefinedType_; }
  CJObjectTypeP nullType     () const { return nullType_     ; }
  CJObjectTypeP trueType     () const { return trueType_     ; }
  CJObjectTypeP falseType    () const { return falseType_    ; }
  CJObjectTypeP numberType   () const { return numberType_   ; }
  CJObjectTypeP stringType   () const { return stringType_   ; }
  CJObjectTypeP arrayType    () const { return arrayType_    ; }
  CJObjectTypeP dictType     () const { return dictType_     ; }
  CJObjectTypeP funcType     () const { return funcType_     ; }
  CJObjectTypeP documentType () const { return documentType_ ; }
  CJObjectTypeP consoleType  () const { return consoleType_  ; }

  CJScopeP createScope(const std::string &name);
  void     deleteScope(const std::string &name);

  CJScopeP currentScope() const { return scope_; }

  void addFunction(const std::string &name, CJValueP fn);

  void setProperty(const std::string &name, CJValueP value);

  CJValueP lookupValue(const Identifiers &identifiers);
  CJValueP lookupValue(CJScope *scope, const Identifiers &identifiers);
  CJValueP lookupValue(CJValueP value, const Identifiers &identifiers);

  CJLValueP lookupLValue(const Identifiers &identifiers);
  CJLValueP lookupLValue(CJScope *scope, const Identifiers &identifiers);
  CJLValueP lookupLValue(CJValueP value, const Identifiers &identifiers);

  CJValueP lookupFunction(const Identifiers &identifiers);
  CJValueP lookupFunction(CJScope *scope, const Identifiers &identifiers);

  CJLValueP lookupProperty(const Identifiers &identifiers, bool create=false);
  CJLValueP lookupProperty(CJScope *scope, const Identifiers &identifiers, bool create=false);

  bool deleteProperty(const Identifiers &identifiers);
  bool deleteProperty(CJScope *scope, const Identifiers &identifiers);

  FunctionValuePair variableFunction(const Identifiers &identifiers);
  FunctionValuePair variableFunction(CJScope *scope, const Identifiers &identifiers);

  CJValueP setVariable(const Identifiers &identifiers, CJValueP value);

  void addTypeObject(CJToken::Type type, CJObjectTypeP obj);

  void addTypeFunction(CJToken::Type type, CJFunctionP fn);

  CJFunctionP getTypeFunction(CJObjectTypeP type, const std::string &name) const;

  CJExecBlockP getCurrentBlock() const { return block_; }

  std::vector<std::string> getFunctionNames() const;
  std::vector<std::string> getVariableNames() const;

  //---

  void startBlock(CJExecBlockP block);
  CJExecBlockP endBlock();

  void startScope(CJScopeP scope);
  CJScopeP endScope();

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
  void parseString(const std::string &str);

  void readIdentifier(CStrParse &parse);
  void readNumber(CStrParse &parse);
  void readOperator(CStrParse &parse, bool allowUnary);
  void readDoubleString(CStrParse &parse);
  void readSingleString(CStrParse &parse);

  CJToken::Type lastTokenType() const;

  std::pair<bool, CJKeyword::Type> isKeyword(const std::string &name) const;

  CJExecExpressionList* interpExpressionList();
  CJExecExpression*     interpExpression();
  CJExecIdentifiers*    interpIdentfiers();
  CJExecBlock*          interpBlock(CJExecBlock::Type type);
  CJExecFor*            interpFor();
  bool                  isInterpForIn();
  CJExecQuestion*       interpQuestion(CJExecExpression *bexpr);
  CJExecIf*             interpIf();
  CJExecWhile*          interpWhile();
  CJExecSwitch*         interpSwitch();
  CJExecTry*            interpTry();
  CJExecVar*            interpVar();
  CJArray *             interpArray();
  bool                  isInterpDictionary() const;
  CJDictionary*         interpDictionary();
  CJValue*              interpNew();
  CJExecDelete*         interpDelete();
  CJExecReturn*         interpReturn();
  CJUserFunction*       interpUserFunction(bool named);
  CJExecFunction*       interpFunction();

  bool isOperator(CJTokenP token, CJOperator::Type opType) const;
  bool isKeyword (CJTokenP token, CJKeyword ::Type opType) const;

  bool isExecOperator(CJOperator::Type opType) const;

 private:
  typedef std::vector<CJValueP>                 Values;
  typedef std::vector<ExecData *>               ExecDataStack;
  typedef std::vector<CJScopeP>                 ScopeStack;
  typedef std::vector<CJExecBlockP>             BlockStack;
  typedef std::map<std::string,CJFunctionP>     Functions;
  typedef std::map<CJToken::Type,CJObjectTypeP> TypeObject;

  bool          parseDebug_  { false };
  bool          interpDebug_ { false };
  bool          execDebug_   { false };
  CJObjectTypeP undefinedType_;
  CJObjectTypeP nullType_;
  CJObjectTypeP trueType_;
  CJObjectTypeP falseType_;
  CJObjectTypeP numberType_;
  CJObjectTypeP stringType_;
  CJObjectTypeP arrayType_;
  CJObjectTypeP dictType_;
  CJObjectTypeP funcType_;
  CJObjectTypeP documentType_;
  CJObjectTypeP consoleType_;
  CJScopeP      scope_;
  ScopeStack    scopeStack_;
  Tokens        tokens_;
  ETokens       etokens_;
  TypeObject    typeObject_;
  ExecData*     execData_ { 0 };
  ExecDataStack execDataStack_;
  CJExecBlockP  block_;
  BlockStack    blockStack_;
};

#endif
