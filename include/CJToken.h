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
    This,
    For,
    If,
    While,
    Switch,
    Try,
    Throw,
    Question,
    Var,
    With,
    New,
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

  int lineNum() const { return lineNum_; }
  void setLineNum(int i) { lineNum_ = i; }

  bool isBoolType() const { return type_ == Type::True || type_ == Type::False; }

  template<typename T> T *cast() { return static_cast<T *>(this); }

  virtual bool isValue() const { return false; }

  virtual bool isProtoValue() const { return false; }

  virtual CJValueP exec(CJavaScript *) { return CJValueP(); }

  virtual std::string toString() const {
    std::ostringstream ss; ss << *this;
    return ss.str();
  }

  virtual void print(std::ostream &os) const = 0;

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
