#include <CJExecAssert.h>
#include <CJExecExpressionList.h>
#include <CJExecExpression.h>
#include <CJavaScript.h>
#include <iostream>

CJExecAssert::
CJExecAssert() :
 CJToken(CJToken::Type::ExecAssert)
{
}

CJValueP
CJExecAssert::
exec(CJavaScript *js)
{
  if (! exprList_)
    return js->createFalseValue();

  uint n = exprList_->expressions().size();

  if (n == 0)
    return js->createFalseValue();

  CJValueP value = exprList_->indexExpression(0)->exec(js);

  if (value && value->toBoolean())
    return js->createTrueValue();

  std::cerr << "Assert: ";

  for (uint i = 0; i < n; ++i) {
    if (i > 0) std::cerr << " ";

    exprList_->indexExpression(i)->print(std::cerr);
  }

  std::cerr << std::endl;

  return js->createFalseValue();
}

void
CJExecAssert::
print(std::ostream &os) const
{
  os << "assert";

  if (exprList_)
    os << " " << *exprList_;

  os << ";";
}
