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
    TypeOf,
    InstanceOf
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

  static bool isAllowUnary(Type type);

  void print(std::ostream &os) const override { os << name(); }

  std::string name() const;

 private:
  Type         type_         { Type::None };
  int          precedence_   { 0 };
  Associativty associativty_ { Associativty::Left };
  Ary          ary_          { Ary::Binary };
};

typedef std::shared_ptr<CJOperator> CJOperatorP;

#endif
