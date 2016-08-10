#include <CJObjectType.h>
#include <CJTypeFunction.h>

void
CJObjectType::
addFunction(CJavaScript *js, const std::string &name)
{
  setFunctionProperty(js, CJFunctionP(new CJTypeFunction(js, name)));
}
