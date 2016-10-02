#include <CJExecStatement.h>

CJExecStatement::
CJExecStatement(CJTokenP token) :
 CJToken(CJToken::Type::Statement), token_(token)
{
}

CJValueP
CJExecStatement::
exec(CJavaScript *js)
{
  if (token_)
    return token_->exec(js);
  else
    return CJValueP();
}

std::string
CJExecStatement::
toString() const
{
  std::string str;

  if (token_)
    str += token_->toString();

  str += ";";

  return str;
}

void
CJExecStatement::
print(std::ostream &os) const
{
  if (token_)
    os << *token_;

  os << ";" << std::endl;
}
