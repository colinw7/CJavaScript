#include <CJExecArray.h>
#include <CJArray.h>
#include <CJExecExpression.h>
#include <CJavaScript.h>

CJExecArray::
CJExecArray() :
 CJToken(CJToken::Type::Array)
{
}

CJValueP
CJExecArray::
exec(CJavaScript *js)
{
  CJArrayP array = js->createArrayValue();

  for (const auto &v : values_) {
    CJValueP value = v->exec(js);

    array->addValue(value);
  }

  return array;
}

std::string
CJExecArray::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
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
