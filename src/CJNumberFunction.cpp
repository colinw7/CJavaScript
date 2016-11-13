#include <CJNumberFunction.h>
#include <CJavaScript.h>

CJNumberFunction::
CJNumberFunction(CJavaScript *js) :
 CJObjTypeFunction(js, "Number", CJNumberType::instance(js))
{
}

CJNumberFunction::
CJNumberFunction(const CJNumberFunction &fn) :
 CJObjTypeFunction(fn)
{
}

CJValueP
CJNumberFunction::
exec(CJavaScript *js, const Values &values)
{
  // create number primitive
  CJNumberP number;

  if (values.size() <= 1)
    return js->createNumberValue(0L);

  COptReal r = values[1]->toReal();

  if (r.isValid())
    number = js->createNumberValue(r.getValue());
  else
    number = js->createNumberValue(CJUtil::getNaN());

  return number;
}

CJValueP
CJNumberFunction::
execNew(CJavaScript *js, const Values &values)
{
  // create number object
  CJNumberP number;

  if (values.size() <= 1)
    number = js->createNumberObject(0L);
  else {
    double r = values[1]->toReal().getValue(0.0);

    number = js->createNumberObject(r);
  }

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
