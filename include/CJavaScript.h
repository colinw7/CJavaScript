#ifndef CJavaScript_H
#define CJavaScript_H

#include <string>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <cassert>

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

  void print(std::ostream &os) const { os << id_; }

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

  void print(std::ostream &os) const { os << name(); }

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

// Variable Type
class CJVariableType : public CJObjectType {
 public:
  CJVariableType() :
   CJObjectType(CJToken::Type::Variable, "variable") {
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

  void print(std::ostream &os) const { os << "undefined"; }
};

//------

class CJNull : public CJValue {
 public:
  CJNull(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJNull(js); }

  std::string toString() const override { return "null"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const { os << "null"; }
};

//------

class CJTrue : public CJValue {
 public:
  CJTrue(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJTrue(js); }

  std::string toString() const override { return "true"; }

  double toReal() const override { return 1; }

  bool toBoolean() const override { return true; }

  void print(std::ostream &os) const { os << "true"; }
};

//------

class CJFalse : public CJValue {
 public:
  CJFalse(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJFalse(js); }

  std::string toString() const override { return "false"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const { os << "false"; }
};

//------

class CJNumber : public CJValue {
 public:
  CJNumber(CJavaScript *js, double real);

  CJNumber *dup(CJavaScript *js) const override { return new CJNumber(js, real_); }

  std::string toString() const override {
    std::stringstream ss;

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

  void print(std::ostream &os) const { os << real_; }

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

  void print(std::ostream &os) const {
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

  void print(std::ostream &os) const {
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
    Plus,
    PlusAssign,
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

  void print(std::ostream &os) const { os << name(); }

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

class CJExecKeyValue {
 public:
  CJExecKeyValue(CJValueP key, CJValueP value) :
   key_(key), value_(value) {
  }

  CJValueP key() const { return key_; }

  CJValueP value() const { return value_; }
  void setValue(CJValueP value) { value_ = value; }

  void print(std::ostream &os) const {
    printKey(os);

    os << ": ";

    if (value_)
      os << *value_;
    else
      os << "<null>";
  }

  void printKey(std::ostream &os) const {
    if      (! key_)
      os << "<null>";
    else if (key_->type() == CJValue::Type::String)
      os << key_->cast<CJString>()->text();
    else
      os << *key_;
  }

  friend std::ostream &operator<<(std::ostream &os, const CJExecKeyValue &rhs) {
    rhs.print(os);

    return os;
  }

 private:
  CJValueP key_;
  CJValueP value_;
};

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

// { <expression_pair> [, <expression_pair>]* }
class CJDictionary : public CJValue {
 public:
  typedef std::vector<CJExecKeyValue> KeyValues;

 public:
  CJDictionary(CJavaScript *js, const KeyValues &keyValues=KeyValues());

  CJValue *dup(CJavaScript *js) const override {
    return new CJDictionary(js, keyValues_);
  }

  void addProperty(CJavaScript *js, const std::string &key, double value) {
    CJExecKeyValue keyValue(CJValueP(new CJString(js, key)), CJValueP(new CJNumber(js, value)));

    addKeyValue(keyValue);
  }

  void addProperty(CJavaScript *js, const std::string &key, int value) {
    CJExecKeyValue keyValue(CJValueP(new CJString(js, key)), CJValueP(new CJNumber(js, value)));

    addKeyValue(keyValue);
  }

  void addProperty(CJavaScript *js, const std::string &key, CJFunction *function) {
    CJExecKeyValue keyValue(CJValueP(new CJString(js, key)), CJValueP(function));

    addKeyValue(keyValue);
  }

  void addProperty(CJavaScript *js, const std::string &key, CJValueP value) {
    CJExecKeyValue keyValue(CJValueP(new CJString(js, key)), value);

    addKeyValue(keyValue);
  }

  void addKeyValue(const CJExecKeyValue &keyValue) {
    keyValues_.push_back(keyValue);
  }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;

    return ss.str();
  }

  const KeyValues &keyValues() const { return keyValues_; }

  bool toBoolean() const override { return ! keyValues_.empty(); }

  double toReal() const override { return toBoolean(); }

  CJValueP indexValue(CJavaScript *js, const std::string &key) const {
    return indexValue(CJValueP(new CJString(js, key)));
  }

  CJValueP indexValue(CJValueP ivalue) const {
    for (auto &kv : keyValues_) {
      if (kv.key()->cmp(ivalue.get()) == 0)
        return kv.value();
    }

    return CJValueP();
  }

  void setIndexValue(CJavaScript *js, const std::string &key, CJValueP value) {
    return setIndexValue(CJValueP(new CJString(js, key)), value);
  }

  void setIndexValue(CJValueP ivalue, CJValueP value) {
    for (auto &kv : keyValues_) {
      if (kv.key()->cmp(ivalue.get()) == 0)
        return kv.setValue(value);
    }
  }

  void print(std::ostream &os) const override {
    int i = 0;

    os << "{";

    for (auto &kv : keyValues_) {
      if (i > 0)
        os << ",";

      os << " " << kv;

      ++i;
    }

    if (i > 0)
      os << " ";

    os << "}";
  }

 private:
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

    for (auto &t : etokens_)
      os << " " << *t;

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

  void addVar(CJExecIdentifiersP identifiers, CJValueP value) {
    identifiers_ = identifiers;
    value_       = value;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    os << "var " << *identifiers_;

    if (value_)
      os << " = " << *value_;
  }

 private:
  CJExecIdentifiersP identifiers_;
  CJValueP           value_;
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

  void setBlock(CJExecBlockP block) {
    block_ = block;
  }

  void interp(CJavaScript *js);

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    if      (var_ && exprList2_ && exprList3_ && block_) {
      os << "for (" << *var_ << "; " << *exprList2_ << "; " <<
                       *exprList3_ << ") " << *block_;
    }
    else if (exprList1_ && exprList2_ && exprList3_ && block_) {
      os << "for (" << *exprList1_ << "; " << *exprList2_ << "; " <<
                       *exprList3_ << ") " << *block_;
    }
  }

 private:
  CJExecExpressionListP exprList1_;
  CJExecVarP            var_;
  CJExecExpressionListP exprList2_;
  CJExecExpressionListP exprList3_;
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

  virtual CJValueP value() = 0;
  virtual void setValue(CJValueP value) = 0;
};

typedef std::shared_ptr<CJLValue> CJLValueP;

//------

class CJVariable : public CJLValue {
 public:
  CJVariable(CJavaScript *js, const std::string &name, CJValueP value=CJValueP());

  CJVariable(CJavaScript *js, const std::string &name, double r);

  CJValue *dup(CJavaScript *js) const override { return new CJVariable(js, name_, value_); }

  const std::string &name() const { return name_; }

  CJValueP value() { return value_; }
  void setValue(CJValueP value) { value_ = value; }

  CJObjectTypeP valueType() const {
    if (value_)
      return value_->valueType();
    else
      return CJObjectTypeP();
  }

  std::string toString() const override {
    if (value_)
      return value_->toString();
    else
      return "";
  }

  double toReal() const override {
    if (value_)
      return value_->toReal();
    else
      return 0;
  }

  bool toBoolean() const override {
    if (value_)
      return value_->toBoolean();
    else
      return false;
  }

  bool hasIndex() const override {
    if (value_)
      return value_->hasIndex();
    else
      return false;
  }

  CJValueP indexValue(int ind) const override {
    assert(value_ && value_->hasIndex());

    return value_->indexValue(ind);
  }

  void setIndexValue(int ind, CJValueP value) override {
    assert(value_ && value_->hasIndex());

    value_->setIndexValue(ind, value);
  }

  void print(std::ostream &os) const override {
    os << name_ << " = " << *value_;
  }

 private:
  std::string name_;
  CJValueP    value_;
};

typedef std::shared_ptr<CJVariable> CJVariableP;

//------

class CJDictionaryRef : public CJLValue {
 public:
  CJDictionaryRef(CJavaScript *js, CJDictionary *dict, const std::string &name);

  CJValue *dup(CJavaScript *js) const override { return new CJDictionaryRef(js, dict_, name_); }

  CJValueP value();

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

  virtual CJValueP execNameFn(CJavaScript *, const std::string &, const Values &) {
    return CJValueP();
  }

 protected:
  CJObjectTypeP type_;
};

typedef std::shared_ptr<CJObject> CJObjectP;

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

  void print(std::ostream &os) const { os << "document"; }

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

  void print(std::ostream &os) const { os << "console"; }

 private:
  CJavaScript *js_;
};

//------

class CJScope;

typedef std::shared_ptr<CJScope> CJScopeP;

class CJScope {
 public:
  typedef std::vector<CJIdentifier *> Identifiers;

 public:
  CJScope(CJavaScript *js, const std::string &name) :
   js_(js), name_(name) {
  }

  const std::string &name() const { return name_; }

  CJScope *getParent() const { return parent_; }
  void setParent(CJScope *s) { parent_ = s; }

  void addScope   (CJScopeP scope);
  void removeScope(const std::string &name);

  void addFunction(CJFunctionP fn);
  void addVariable(CJVariableP var);
  void addObject  (CJObjectP   obj);

  CJScopeP    lookupScope   (const std::string &name);
  CJFunctionP lookupFunction(const std::string &name);
  CJVariableP lookupVariable(const std::string &name);
  bool        deleteVariable(const std::string &name);

 private:
  typedef std::map<std::string, CJScopeP>    Scopes;
  typedef std::map<std::string, CJFunctionP> Functions;
  typedef std::map<std::string, CJVariableP> Variables;
  typedef std::map<std::string, CJObjectP>   Objects;

  CJavaScript* js_ { 0 };
  std::string  name_;
  CJScope*     parent_ { 0 };
  Scopes       scopes_;
  Functions    functions_;
  Variables    variables_;
  Objects      objects_;
};

typedef std::shared_ptr<CJScope> CJScopeP;

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
  CJObjectTypeP varType      () const { return varType_      ; }
  CJObjectTypeP documentType () const { return documentType_ ; }
  CJObjectTypeP consoleType  () const { return consoleType_  ; }

  CJScopeP createScope(const std::string &name);
  void     deleteScope(const std::string &name);

  CJScopeP currentScope() const { return scope_; }

  CJVariableP createVariable(const Identifiers &identifiers);
  CJVariableP createVariable(const std::string &name);
  CJVariableP createVariable(const std::string &name, CJValueP value);

  void addFunction(CJFunctionP fn);
  void addVariable(CJVariableP var);

  CJValueP lookupValue(const Identifiers &identifiers);
  CJValueP lookupValue(CJScope *scope, const Identifiers &identifiers);
  CJValueP lookupValue(CJValueP value, const Identifiers &identifiers);

  CJLValueP lookupLValue(const Identifiers &identifiers);
  CJLValueP lookupLValue(CJScope *scope, const Identifiers &identifiers);
  CJLValueP lookupLValue(CJValueP value, const Identifiers &identifiers);

  CJFunctionP lookupFunction(const Identifiers &identifiers);
  CJFunctionP lookupFunction(CJScope *scope, const Identifiers &identifiers);

  CJVariableP lookupVariable(const Identifiers &identifiers);
  CJVariableP lookupVariable(CJScope *scope, const Identifiers &identifiers);

  bool deleteVariable(const Identifiers &identifiers);
  bool deleteVariable(CJScope *scope, const Identifiers &identifiers);

  CJValueP variableProperty(const Identifiers &identifiers);

  FunctionValuePair variableFunction(const Identifiers &identifiers);
  FunctionValuePair variableFunction(CJScope *scope, const Identifiers &identifiers);

  CJValueP valueProperty(CJValueP value, const std::string &name);

  CJVariableP setVariable(const Identifiers &identifiers, CJValueP value);

  void addTypeObject(CJToken::Type type, CJObjectTypeP obj);

  void addTypeFunction(CJToken::Type type, CJFunctionP fn);

  CJFunctionP getTypeFunction(CJObjectTypeP type, const std::string &name) const;

  CJExecBlockP getCurrentBlock() const { return block_; }

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
  void readOperator(CStrParse &parse);
  void readDoubleString(CStrParse &parse);
  void readSingleString(CStrParse &parse);

  CJToken::Type lastTokenType() const;

  std::pair<bool, CJKeyword::Type> isKeyword(const std::string &name) const;

  CJExecExpressionList* interpExpressionList();
  CJExecExpression*     interpExpression();
  CJExecIdentifiers*    interpIdentfiers();
  CJExecBlock*          interpBlock(CJExecBlock::Type type);
  CJExecFor*            interpFor();
  CJExecQuestion*       interpQuestion(CJExecExpression *bexpr);
  CJExecIf*             interpIf();
  CJExecWhile*          interpWhile();
  CJExecTry*            interpTry();
  CJExecVar*            interpVar();
  CJArray *             interpArray();
  bool                  isInterpDictionary() const;
  CJDictionary*         interpDictionary();
  CJValue*              interpNew();
  CJExecDelete*         interpDelete();
  CJExecReturn*         interpReturn();
  CJFunctionP           interpUserFunction(bool named);
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
  CJObjectTypeP varType_;
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
