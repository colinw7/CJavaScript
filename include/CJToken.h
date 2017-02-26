#ifndef CJToken_H
#define CJToken_H

#include <CJTypes.h>
#include <iostream>
#include <sstream>
#include <cassert>

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
    Newline,

    // basic objects
    String,
    Array,
    Boolean,
    Arguments,
    Object,
    RegExp,
    Date,
    Function,

    // error objects
    Error,
    EvalError,
    InternalError,
    RangeError,
    ReferenceError,
    SyntaxError,
    TypeError,
    URIError,

    // extended (interp) tokens
    Variable,
    Identifiers,
    Expression,
    ExpressionList,
    Dictionary,
    TokenValue,
    AssignExpression,
    IndexExpression,
    IncrDecrExpression,
    Block,
    Const,
    ExecFunction,
    FunctionBase,
    UserObject,
    ObjValue,
    This,
    For,
    If,
    Switch,
    Try,
    Throw,
    Question,
    Var,
    With,
    New,
    GetterSetter,
    ExecAssert,
    ExecBreak,
    ExecContinue,
    ExecDo,
    ExecLabel,
    ExecReturn,
    ExecWhile,
    Statement
  };

 public:
  template<typename T>
  static std::shared_ptr<T> cast(CJTokenP token) {
    return std::static_pointer_cast<T>(token);
  }

 public:
  explicit CJToken(Type type);

  CJToken(const CJToken &t);

  virtual ~CJToken() { }

  Type type() const { return type_; }

  int lineNum() const { return lineNum_; }
  void setLineNum(int i) { lineNum_ = i; }

  bool isBoolType() const { return type_ == Type::True || type_ == Type::False; }

  template<typename T> T *castP() { return static_cast<T *>(this); }

  virtual bool isValue() const { return false; }

  virtual CJValueP exec(CJavaScript *) { return CJValueP(); }

  // string representation
  virtual std::string toString() const { return ""; }

  // print details
  virtual void print(std::ostream &os) const {
    if (type_ == Type::Newline)
      os << "<nl>";
    else
      os << "<unknown>";
  }

  friend std::ostream &operator<<(std::ostream &os, const CJToken &rhs) {
    rhs.print(os);

    return os;
  }

 protected:
  Type type_    { Type::None };
  int  lineNum_ { 0 };
};

typedef std::shared_ptr<CJToken> CJTokenP;

#endif
