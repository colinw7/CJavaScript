#include <CJOperator.h>

CJOperator::
CJOperator(const Type &type, int precedence, Associativty associativty, Ary ary) :
 CJToken(CJToken::Type::Operator), type_(type), precedence_(precedence),
 associativty_(associativty), ary_(ary)
{
}

CJOperator::
CJOperator(const CJOperator &op) :
 CJToken(op), type_(op.type_), precedence_(op.precedence_),
 associativty_(op.associativty_), ary_(op.ary_)
{
}

bool
CJOperator::
isAssign() const
{
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

bool
CJOperator::
isAllowUnary(Type type)
{
  switch (type) {
    case Type::Plus:
    case Type::PlusAssign:
    case Type::Minus:
    case Type::MinusAssign:
    case Type::Times:
    case Type::TimesAssign:
    case Type::Divide:
    case Type::DivideAssign:
    case Type::Modulus:
    case Type::ModulusAssign:
    case Type::Increment:
    case Type::Decrement:
    case Type::OpenRBracket:
    case Type::OpenSBracket:
    case Type::OpenBrace:
    case Type::Less:
    case Type::LessEqual:
    case Type::Greater:
    case Type::GreaterEqual:
    case Type::Equal:
    case Type::StrictEqual:
    case Type::NotEqual:
    case Type::StrictNotEqual:
    case Type::Assign:
    case Type::LeftShift:
    case Type::LeftShiftAssign:
    case Type::RightShift:
    case Type::RightShiftAssign:
    case Type::UnsignedRightShift:
    case Type::UnsignedRightShiftAssign:
    case Type::LogicalAnd:
    case Type::LogicalOr:
    case Type::LogicalNot:
    case Type::BitwiseAnd:
    case Type::BitwiseAndAssign:
    case Type::BitwiseOr:
    case Type::BitwiseOrAssign:
    case Type::BitwiseNot:
    case Type::BitwiseNotAssign:
    case Type::BitwiseXor:
    case Type::BitwiseXorAssign:
    case Type::Comma:
    case Type::SemiColon:
    case Type::Question:
    case Type::Colon:
    case Type::TypeOf:
    case Type::InstanceOf:
    case Type::In:
      return true;
    default:
      return false;
  }
}

std::string
CJOperator::
name() const
{
  return typeName(type_);
}

std::string
CJOperator::
typeName(Type type)
{
  switch (type) {
    case Type::None                    : return "<none>";
    case Type::UnaryPlus               : return "+";
    case Type::Plus                    : return "+";
    case Type::PlusAssign              : return "+=";
    case Type::UnaryMinus              : return "-";
    case Type::Minus                   : return "-";
    case Type::MinusAssign             : return "-=";
    case Type::Times                   : return "*";
    case Type::TimesAssign             : return "*=";
    case Type::Divide                  : return "/";
    case Type::DivideAssign            : return "/=";
    case Type::Modulus                 : return "%";
    case Type::ModulusAssign           : return "%=";
    case Type::Increment               : return "++";
    case Type::Decrement               : return "--";
    case Type::OpenRBracket            : return "(";
    case Type::CloseRBracket           : return ")";
    case Type::OpenSBracket            : return "[";
    case Type::CloseSBracket           : return "]";
    case Type::OpenBrace               : return "{";
    case Type::CloseBrace              : return "}";
    case Type::Less                    : return "<";
    case Type::LessEqual               : return "<=";
    case Type::Greater                 : return ">";
    case Type::GreaterEqual            : return ">=";
    case Type::Equal                   : return "==";
    case Type::StrictEqual             : return "===";
    case Type::NotEqual                : return "!=";
    case Type::StrictNotEqual          : return "!==";
    case Type::Assign                  : return "=";
    case Type::LeftShift               : return "<<";
    case Type::LeftShiftAssign         : return "<<=";
    case Type::RightShift              : return ">>";
    case Type::RightShiftAssign        : return ">>=";
    case Type::UnsignedRightShift      : return ">>>";
    case Type::UnsignedRightShiftAssign: return ">>>=";
    case Type::LogicalAnd              : return "&&";
    case Type::LogicalOr               : return "||";
    case Type::LogicalNot              : return "!";
    case Type::BitwiseAnd              : return "&";
    case Type::BitwiseAndAssign        : return "&=";
    case Type::BitwiseOr               : return "|";
    case Type::BitwiseOrAssign         : return "|=";
    case Type::BitwiseNot              : return "~";
    case Type::BitwiseNotAssign        : return "~=";
    case Type::BitwiseXor              : return "^";
    case Type::BitwiseXorAssign        : return "^=";
    case Type::Scope                   : return ".";
    case Type::Comma                   : return ",";
    case Type::SemiColon               : return ";";
    case Type::Question                : return "?";
    case Type::Colon                   : return ":";
    case Type::Delete                  : return "delete";
    case Type::TypeOf                  : return "typeof";
    case Type::InstanceOf              : return "instanceof";
    case Type::In                      : return "in";
    case Type::Void                    : return "void";
    default                            : assert(false);
  }

  return "";
}

std::string
CJOperator::
toString() const
{
  return name();
}

void
CJOperator::
print(std::ostream &os) const
{
  os << name();
}
