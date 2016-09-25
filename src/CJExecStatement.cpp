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

void
CJExecStatement::
print(std::ostream &os) const
{
  if (token_)
    os << *token_;

  os << ";" << std::endl;
}
