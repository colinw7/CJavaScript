#ifndef CJOperator_H
#define CJOperator_H

#include <CJToken.h>

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
    Delete,
    TypeOf,
    InstanceOf,
    Void,
    In
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
  CJOperator(const Type &type, int precedence, Associativty associativty, Ary ary);

  CJOperator *dup() const { return new CJOperator(type_, precedence_, associativty_, ary_); }

  Type type() const { return type_; }

  int precedence() const { return precedence_; }

  Ary ary() const { return ary_; }

  Associativty associativty() const { return associativty_; }

  bool isAssign() const;

  static bool isAllowUnary(Type type);

  void print(std::ostream &os) const override { os << name(); }

  std::string name() const;

  static std::string typeName(Type type);

 private:
  Type         type_         { Type::None };
  int          precedence_   { 0 };
  Associativty associativty_ { Associativty::Left };
  Ary          ary_          { Ary::Binary };
};

#endif
