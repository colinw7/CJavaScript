#include <CJExecArray.h>
#include <CJArray.h>
#include <CJExecExpression.h>

CJExecArray::
CJExecArray() :
 CJToken(CJToken::Type::Array)
{
}

CJValueP
CJExecArray::
exec(CJavaScript *js)
{
  CJArray *array = new CJArray(js);

  for (const auto &v : values_) {
    CJValueP value = v->exec(js);

    array->addValue(value);
  }

  return CJValueP(array);
}

void
CJExecArray::
print(std::ostream &os) const
{
  os << "[";

  int i = 0;

  for (const auto &v : values_) {
    if (i > 0) os << " ";

    os << *v;

    ++i;
  }

  os << "]";
}
