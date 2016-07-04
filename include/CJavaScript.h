#ifndef CJavaScript_H
#define CJavaScript_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>

#include <ostream>
#include <sstream>

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
    Null,
    True,
    False,
    Operator,
    String,
    Array,
    Object,

    // extended (interp) tokens
    Identifiers,
    Expression,
    ExpressionList,
    AssignExpression,
    IndexExpression,
    IncrDecrExpression,
    Block,
    Function,
    For,
    If,
    While,
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
    Continue,
    Delete,
    Else,
    For,
    Function,
    If,
    In,
    New,
    Return,
    This,
    TypeOf,
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

class CJValue : public CJToken {
 public:
  CJValue(Type type) :
   CJToken(type) {
  }

  virtual ~CJValue() { }

  virtual CJValue *dup() const = 0;

  virtual std::string toString() const = 0;

  virtual double toReal() const = 0;

  virtual long toInteger() const { return int(toReal()); }

  virtual bool toBoolean() const = 0;

  bool isValue() const override { return true; }
};

//------

class CJNull : public CJValue {
 public:
  CJNull() :
   CJValue(Type::Null) {
  }

  CJValue *dup() const override { return new CJNull; }

  std::string toString() const override { return "null"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const { os << "null"; }
};

//------

class CJTrue : public CJValue {
 public:
  CJTrue() :
   CJValue(Type::True) {
  }

  CJValue *dup() const override { return new CJTrue; }

  std::string toString() const override { return "true"; }

  double toReal() const override { return 1; }

  bool toBoolean() const override { return true; }

  void print(std::ostream &os) const { os << "true"; }
};

//------

class CJFalse : public CJValue {
 public:
  CJFalse() :
   CJValue(Type::False) {
  }

  CJValue *dup() const override { return new CJFalse; }

  std::string toString() const override { return "false"; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return false; }

  void print(std::ostream &os) const { os << "false"; }
};

//------

class CJNumber : public CJValue {
 public:
  CJNumber(double real) :
   CJValue(Type::Number), real_(real) {
  }

  CJNumber *dup() const override { return new CJNumber(real_); }

  std::string toString() const override {
    std::stringstream ss;

    ss << real_;

    return ss.str();
  }

  double real() const { return real_; }

  double toReal() const override { return real_; }

  bool toBoolean() const override { return (real_ != 0.0); }

  void print(std::ostream &os) const { os << real_; }

 private:
  double real_;
};

//------

class CJString : public CJValue {
 public:
  CJString(const std::string &text="", char c='\"') :
   CJValue(Type::String), text_(text), c_(c) {
  }

  CJString *dup() const override { return new CJString(text_, c_); }

  const std::string &text() const { return text_; }
  void setText(const std::string &str) { text_ = str; }

  std::string toString() const override { return text_; }

  double toReal() const override { return atof(text_.c_str()); }

  long toInteger() const override { return atol(text_.c_str()); }

  bool toBoolean() const override { return (! text_.empty()); }

  void print(std::ostream &os) const {
    os << c_ << text_ << c_;
  }

 private:
  std::string text_;
  char        c_;
};

//------

class CJArray : public CJValue {
 public:
  typedef std::vector<CJValueP> Values;

 public:
  CJArray() :
   CJValue(Type::Array) {
  }

  CJArray(const Values &values) :
   CJValue(Type::Array), values_(values) {
  }

  CJArray *dup() const override { return new CJArray(values_); }

  std::string toString() const override { return ""; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return ! values_.empty(); }

  void addValue(CJValueP value) {
    values_.push_back(value);
  }

  CJValueP indexValue(int i) const {
    if (i < 0 || i >= int(values_.size()))
      return CJValueP();

    return values_[i];
  }

  void print(std::ostream &os) const {
    os << "[";

    int i = 0;

    for (auto &v : values_) {
      if (i > 0) os << " ";

      os << *v;

      ++i;
    }

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
    Colon
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

 public:
  CJExecExpressionList() :
   CJToken(CJToken::Type::ExpressionList) {
  }

  const Expressions &expressions() const { return expressions_; }

  void addExpression(CJExecExpressionP expr) {
    expressions_.push_back(expr);
  }

  CJValueP exec(CJavaScript *js) override {
    CJValueP value;

    for (auto &e : expressions_)
      value = e->exec(js);

    return value;
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

  void setIdentifiers(CJExecIdentifiersP identifiers) {
    identifiers_ = identifiers;
  }

  void setExpression(CJExecExpressionP expr) {
    expr_ = expr;
  }

  void setIndexExpression(CJExecExpressionP iexpr) {
    iexpr_ = iexpr;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    if (expr_) {
      if (iexpr_)
        os << *expr_ << "[" << *iexpr_ << "]";
      else
        os << *expr_ << "[]";
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

  void setOperator(CJOperatorP op) {
    op_ = op;
  }

  void setPost(bool b) {
    post_ = b;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    if (identifiers_ && op_) {
      if (post_)
        os << *identifiers_ << *op_;
      else
        os << *op_ << *identifiers_;
    }
  }

 private:
  CJExecIdentifiersP identifiers_;
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
  CJExecFunction(CJExecIdentifiersP identifiers) :
   CJToken(CJToken::Type::Function), identifiers_(identifiers) {
  }

  void setExprList(CJExecExpressionListP exprList) {
    exprList_ = exprList;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    if (identifiers_ && exprList_) {
      os << *identifiers_ << "(" << *exprList_ << ")";
    }
  }

 private:
  CJExecIdentifiersP    identifiers_;
  CJExecExpressionListP exprList_;
};

//------

// var <identifiers> [= <expression>]
class CJExecVar : public CJToken {
 public:
  CJExecVar() :
   CJToken(CJToken::Type::Var) {
  }

  void addVar(CJExecIdentifiersP identifiers, CJExecExpressionListP exprList) {
    idExprList_.push_back(IdExpr(identifiers, exprList));
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    int i = 0;

    for (const auto &ie : idExprList_) {
      if (i > 0)
        os << ", ";

      CJExecIdentifiersP    identifiers = ie.first;
      CJExecExpressionListP expr        = ie.second;

      os << "var " << *identifiers;

      if (expr)
        os << " = " << *expr;

      ++i;
    }

    os << ";";
  }

 private:
  typedef std::pair<CJExecIdentifiersP, CJExecExpressionListP> IdExpr;
  typedef std::vector<IdExpr>                                  IdExprList;

  IdExprList idExprList_;
};

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

// for (<expression_list> ; <expression_list> ; <expression_list> ) <block>
class CJExecFor : public CJToken {
 public:
  CJExecFor() :
   CJToken(CJToken::Type::For) {
  }

  void setExprList1(CJExecExpressionListP exprList) {
    exprList1_ = exprList;
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
    if (exprList1_ && exprList2_ && exprList3_ && block_) {
      os << "for (" << *exprList1_ << "; " << *exprList2_ << "; " <<
                       *exprList3_ << ") " << *block_;
    }
  }

 private:
  CJExecExpressionListP exprList1_;
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

// builtin function
// TODO: derive from CJToken
class CJFunction {
 public:
  enum class Type {
    Normal,
    Real,
    User,
    Object
  };

  typedef std::vector<CJValueP> Values;

 public:
  CJFunction(const std::string &name, Type type=Type::Normal) :
   name_(name), type_(type) {
  }

  const std::string &name() const { return name_; }

  virtual CJValueP exec(CJavaScript *js, const Values &values) = 0;

 protected:
  std::string name_;
  Type        type_;
};

typedef std::shared_ptr<CJFunction> CJFunctionP;

//------

// function calling API width signature: double r = fn(double);
class CJRealFunction : public CJFunction {
 public:
  typedef double (*Fn)(double);

 public:
  CJRealFunction(const std::string &name, Fn fn) :
   CJFunction(name, CJFunction::Type::Real), fn_(fn) {
  }

  CJValueP exec(CJavaScript *js, const Values &values) override;

 protected:
  Fn fn_;
};

//------

// user defined function
class CJUserFunction : public CJToken, public CJFunction {
 public:
  typedef std::vector<std::string> Args;

 public:
  CJUserFunction(const std::string &name, const Args &args, CJExecBlockP block) :
   CJToken(CJToken::Type::Function), CJFunction(name, CJFunction::Type::User),
   args_(args), block_(block) {
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

      os << *block_;
    }
  }

 private:
  Args         args_;
  CJExecBlockP block_;
};

//------

// object function
class CJObjectFunction : public CJToken, public CJFunction {
 public:
  CJObjectFunction(const std::string &name) :
   CJToken(CJToken::Type::Function), CJFunction(name, CJFunction::Type::Object) {
  }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "object fn";
  }
};

//------

class CJVariable {
 public:
  CJVariable(const std::string &name, const CJValueP &value=CJValueP()) :
   name_(name), value_(value) {
  }

  CJVariable(const std::string &name, double r) :
   name_(name) {
    value_ = CJValueP(new CJNumber(r));
  }

  const std::string &name() const { return name_; }

  CJValueP value(bool create=true);

  void setValue(CJValueP value) { value_ = value; }

 private:
  std::string name_;
  CJValueP    value_;
};

typedef std::shared_ptr<CJVariable> CJVariableP;

//------

class CJObject : public CJValue {
 public:
  typedef std::vector<CJValueP> Values;

 public:
  CJObject(const std::string &name) :
   CJValue(Type::Object), name_(name) {
  }

  const std::string &name() const { return name_; }

  virtual CJValueP execNameFn(CJavaScript *, const std::string &, const Values &) {
    return CJValueP();
  }

 private:
  std::string name_;
};

typedef std::shared_ptr<CJObject> CJObjectP;

//------

class CJScope;

typedef std::shared_ptr<CJScope> CJScopeP;

class CJScope {
 public:
  typedef std::vector<CJIdentifier *> Identifiers;

 public:
  CJScope(const std::string &name) :
   name_(name) {
  }

  const std::string &name() const { return name_; }

  CJScope *getParent() const { return parent_; }
  void setParent(CJScope *s) { parent_ = s; }

  void addScope   (CJScopeP    scope);
  void addFunction(CJFunctionP fn);
  void addVariable(CJVariableP var);
  void addObject  (CJObjectP   obj);

  CJScopeP    lookupScope   (const std::string &name);
  CJFunctionP lookupFunction(const std::string &name);
  CJVariableP lookupVariable(const std::string &name, bool create);

 private:
  typedef std::map<std::string, CJScopeP>    Scopes;
  typedef std::map<std::string, CJFunctionP> Functions;
  typedef std::map<std::string, CJVariableP> Variables;
  typedef std::map<std::string, CJObjectP>   Objects;

  std::string name_;
  CJScope*    parent_ { 0 };
  Scopes      scopes_;
  Functions   functions_;
  Variables   variables_;
  Objects     objects_;
};

typedef std::shared_ptr<CJScope> CJScopeP;

//------

class CJAlertFunction : public CJFunction {
 public:
  CJAlertFunction() :
   CJFunction("alert") {
  }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

class CJSetInterval : public CJFunction {
 public:
  CJSetInterval() :
   CJFunction("setInterval") {
  }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

class CJClearInterval : public CJFunction {
 public:
  CJClearInterval() :
   CJFunction("clearInterval") {
  }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// Document Class
class CJDocumentWriteFunction : public CJFunction {
 public:
  CJDocumentWriteFunction() :
   CJFunction("write") {
  }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

//------

// String Type Functions
class CJStringCharAtFunction : public CJFunction {
 public:
  CJStringCharAtFunction() :
   CJFunction("charAt") {
  }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

class CJStringIndexOfFunction : public CJFunction {
 public:
  CJStringIndexOfFunction() :
   CJFunction("indexOf") {
  }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

class CJStringLengthFunction : public CJFunction {
 public:
  CJStringLengthFunction() :
   CJFunction("length") {
  }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

class CJStringSubStringFunction : public CJFunction {
 public:
  CJStringSubStringFunction() :
   CJFunction("substring") {
  }

  CJValueP exec(CJavaScript *js, const Values &values) override;
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

    const ETokens &etokens() const { return etokens_; }

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
  };

 public:
  CJavaScript();

  bool isParseDebug() const { return parseDebug_; }
  void setParseDebug(bool b) { parseDebug_ = b; }

  bool isExecDebug() const { return execDebug_; }
  void setExecDebug(bool b) { execDebug_ = b; }

  CJScopeP createScope(const std::string &name);

  CJScopeP currentScope() const { return scope_; }

  void addFunction(CJFunctionP fn);
  void addVariable(CJVariableP var);

  CJFunctionP lookupFunction(const Identifiers &identifiers);
  CJFunctionP lookupFunction(CJScope *scope, const Identifiers &identifiers);

  CJVariableP lookupVariable(const Identifiers &identifiers, bool create);
  CJVariableP lookupVariable(CJScope *scope, const Identifiers &identifiers, bool create);

  CJValueP variableProperty(const Identifiers &identifiers);

  FunctionValuePair variableFunction(const Identifiers &identifiers);
  FunctionValuePair variableFunction(CJScope *scope, const Identifiers &identifiers);

  CJValueP valueProperty(CJValueP value, const std::string &name);

  CJVariableP setVariable(const Identifiers &identifiers, CJValueP value);

  void addTypeFunction(CJToken::Type type, CJFunctionP fn);
  CJFunctionP getTypeFunction(CJToken::Type type, const std::string &name) const;

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

  CJValueP boolValue(bool b) const {
    if (b)
      return CJValueP(new CJTrue());
    else
      return CJValueP(new CJFalse());
  }

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

  CJExecExpressionList* execExpressionList();
  CJExecExpression*     execExpression();
  CJExecIdentifiers*    execIdentfiers();
  CJExecBlock*          execBlock(CJExecBlock::Type type);
  CJExecFor*            execFor();
  CJExecQuestion*       execQuestion(CJExecExpression *bexpr);
  CJExecIf*             execIf();
  CJExecWhile*          execWhile();
  CJExecVar*            execVar();
  CJValue*              execNew();
  CJExecReturn*         execReturn();
  CJFunctionP           execUserFunction();
  CJExecFunction*       execFunction(CJExecIdentifiersP identifiers);

  bool isOperator(CJTokenP token, CJOperator::Type opType) const;
  bool isKeyword (CJTokenP token, CJKeyword ::Type opType) const;

  bool isExecOperator(CJOperator::Type opType) const;

 private:
  typedef std::vector<CJValueP>             Values;
  typedef std::vector<ExecData *>           ExecDataStack;
  typedef std::vector<CJScopeP>             ScopeStack;
  typedef std::vector<CJExecBlockP>         BlockStack;
  typedef std::map<std::string,CJFunctionP> Functions;
  typedef std::map<CJToken::Type,Functions> TypeFunctions;

  bool          parseDebug_ { false };
  bool          execDebug_ { false };
  CJScopeP      scope_;
  ScopeStack    scopeStack_;
  Tokens        tokens_;
  ETokens       etokens_;
  TypeFunctions typeFunctions_;
  ExecData*     execData_ { 0 };
  ExecDataStack execDataStack_;
  CJExecBlockP  block_;
  BlockStack    blockStack_;
};

#endif
