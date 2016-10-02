#include <CJArrayFunction.h>
#include <CJArray.h>
#include <CJavaScript.h>

CJArrayFunction::
CJArrayFunction(CJavaScript *js) :
 CJObjTypeFunction(js, "Array", CJArrayType::instance(js))
{
}

CJValueP
CJArrayFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() <= 1)
    return js->createArrayValue();

  if (values.size() == 2) {
    CJValueP protoValue = values[1];

    if (CJArrayType::canCreateArrayFromValue(protoValue))
      return CJArrayType::createArrayFromValue(js, protoValue);

    long n = values[1]->toInteger();

    return js->createArrayValue(n);
  }

  CJArrayP array = js->createArrayValue();

  for (uint i = 1; i < values.size(); ++i)
    array->addValue(values[i]);

  return array;
}

CJValueP
CJArrayFunction::
execNew(CJavaScript *js, const Values &values)
{
  return exec(js, values);
}

std::string
CJArrayFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJArrayFunction::
print(std::ostream &os) const
{
  os << "[Function: Array]"; 
}
