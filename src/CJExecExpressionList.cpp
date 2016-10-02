#include <CJExecExpressionList.h>
#include <CJExecExpression.h>

CJExecExpressionList::
CJExecExpressionList(CJToken::Type type) :
 CJToken(type)
{
}

void
CJExecExpressionList::
addExpression(CJExecExpressionP expr)
{
  expressions_.push_back(expr);
}

CJExecExpressionP
CJExecExpressionList::
indexExpression(int i)
{
  if (i < 0 || i >= int(expressions_.size()))
    return CJExecExpressionP();

  return expressions_[i];
}

CJExecExpressionList::Values
CJExecExpressionList::
getValues(CJavaScript *js) const
{
  Values values;

  for (auto &e : expressions_) {
    assert(e);

    CJValueP value = e->exec(js);

    values.push_back(value);
  }

  return values;
}

bool
CJExecExpressionList::
isEmpty() const
{
  return (expressions_.empty() || (expressions_.size() == 1 && ! expressions_[0]->hasTokens()));
}

CJValueP
CJExecExpressionList::
exec(CJavaScript *js)
{
  Values values = getValues(js);

  if (values.empty())
    return CJValueP();

  return values.back();
}

std::string
CJExecExpressionList::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJExecExpressionList::
print(std::ostream &os) const
{
  int i = 0;

  for (auto &e : expressions_) {
    if (i > 0)
      os << ", ";

    os << *e;

    ++i;
  }
}
