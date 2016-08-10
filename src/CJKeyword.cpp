#include <CJKeyword.h>

std::string
CJKeyword::
name() const
{
  switch (type_) {
    case CJKeyword::Type::None    : return "<none>";
    case CJKeyword::Type::Break   : return "break";
    case CJKeyword::Type::Case    : return "case";
    case CJKeyword::Type::Catch   : return "catch";
    case CJKeyword::Type::Class   : return "class";
    case CJKeyword::Type::Const   : return "const";
    case CJKeyword::Type::Continue: return "continue";
    case CJKeyword::Type::Default : return "default";
    case CJKeyword::Type::Delete  : return "delete";
    case CJKeyword::Type::Do      : return "do";
    case CJKeyword::Type::Else    : return "else";
    case CJKeyword::Type::Finally : return "finally";
    case CJKeyword::Type::For     : return "for";
    case CJKeyword::Type::Function: return "function";
    case CJKeyword::Type::If      : return "if";
    case CJKeyword::Type::In      : return "in";
    case CJKeyword::Type::New     : return "new";
    case CJKeyword::Type::Return  : return "return";
    case CJKeyword::Type::Switch  : return "switch";
    case CJKeyword::Type::Throw   : return "throw";
    case CJKeyword::Type::Try     : return "try";
    case CJKeyword::Type::Var     : return "var";
    case CJKeyword::Type::Void    : return "void";
    case CJKeyword::Type::While   : return "while";
    case CJKeyword::Type::With    : return "with";
  }

  assert(false);

  return "";
}
