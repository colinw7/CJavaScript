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

CJValueP
CJExecExpressionList::
exec(CJavaScript *js)
{
  Values values = getValues(js);

  if (values.empty())
    return CJValueP();

  return values.back();
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
