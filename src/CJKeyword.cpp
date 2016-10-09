#include <CJKeyword.h>
#include <map>

CJKeyword::Type
CJKeyword::
nameToType(const std::string &name)
{
  typedef std::map<std::string,Type> Keywords;

  static Keywords keywords;

  if (keywords.empty()) {
    keywords["assert"   ] = Type::Assert;
    keywords["assertEq" ] = Type::AssertEq;
    keywords["break"    ] = Type::Break;
    keywords["case"     ] = Type::Case;
    keywords["catch"    ] = Type::Catch;
    keywords["class"    ] = Type::Class;
    keywords["const"    ] = Type::Const;
    keywords["continue" ] = Type::Continue;
    keywords["default"  ] = Type::Default;
    keywords["do"       ] = Type::Do;
    keywords["else"     ] = Type::Else;
    keywords["finally"  ] = Type::Finally;
    keywords["for"      ] = Type::For;
    keywords["function" ] = Type::Function;
    keywords["if"       ] = Type::If;
    keywords["new"      ] = Type::New;
    keywords["return"   ] = Type::Return;
    keywords["switch"   ] = Type::Switch;
    keywords["this"     ] = Type::This;
    keywords["throw"    ] = Type::Throw;
    keywords["try"      ] = Type::Try;
    keywords["var"      ] = Type::Var;
    keywords["while"    ] = Type::While;
    keywords["with"     ] = Type::With;
  }

  auto p = keywords.find(name);

  if (p != keywords.end())
    return (*p).second;
  else
    return Type::None;
}

CJKeyword::
CJKeyword(const Type &type) :
 CJToken(CJToken::Type::Keyword), type_(type)
{
}

std::string
CJKeyword::
name() const
{
  switch (type_) {
    case Type::None    : return "<none>";
    case Type::Assert  : return "assert";
    case Type::AssertEq: return "assertEq";
    case Type::Break   : return "break";
    case Type::Case    : return "case";
    case Type::Catch   : return "catch";
    case Type::Class   : return "class";
    case Type::Const   : return "const";
    case Type::Continue: return "continue";
    case Type::Default : return "default";
    case Type::Do      : return "do";
    case Type::Else    : return "else";
    case Type::Finally : return "finally";
    case Type::For     : return "for";
    case Type::Function: return "function";
    case Type::If      : return "if";
    case Type::New     : return "new";
    case Type::Return  : return "return";
    case Type::Switch  : return "switch";
    case Type::This    : return "this";
    case Type::Throw   : return "throw";
    case Type::Try     : return "try";
    case Type::Var     : return "var";
    case Type::While   : return "while";
    case Type::With    : return "with";
  }

  assert(false);

  return "";
}

std::string
CJKeyword::
toString() const
{
  return name();
}

void
CJKeyword::
print(std::ostream &os) const
{
  os << name();
}
