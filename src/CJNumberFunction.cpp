#include <CJNumberFunction.h>
#include <CJavaScript.h>

CJNumberFunction::
CJNumberFunction(CJavaScript *js) :
 CJObjTypeFunction(js, "Number", CJNumberType::instance(js))
{
}

CJValueP
CJNumberFunction::
exec(CJavaScript *js, const Values &values)
{
  CJNumberP number;

  if (values.size() <= 1)
    number = js->createNumberValue(0L);
  else {
    double r = values[1]->toReal();

    number = js->createNumberValue(r);
  }

  number->setIsPrimitive(true);

  return number;
}

CJValueP
CJNumberFunction::
execNew(CJavaScript *js, const Values &values)
{
  CJNumberP number;

  if (values.size() <= 1)
    number = js->createNumberValue(0L);
  else {
    double r = values[1]->toReal();

    number = js->createNumberValue(r);
  }

  number->setIsPrimitive(false);

  return number;
}

std::string
CJNumberFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJNumberFunction::
print(std::ostream &os) const
{
  os << "[Function: Number]";
}
