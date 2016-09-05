#include <CJNumber.h>
#include <CJavaScript.h>
#include <CJUtil.h>

CJObjTypeP CJNumberType::type_;

CJObjTypeP
CJNumberType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJNumberType(js));

    js->addObjectType("number", type_);

    js->addTypeObject(CJToken::Type::Number, type_);
  }

  return type_;
}

CJNumberType::
CJNumberType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Number, "number")
{
  addTypeFunction(js, "isNaN");
  addTypeFunction(js, "parseInt");
  addTypeFunction(js, "parseFloat");

  addObjectFunction(js, "toFixed");

  addPseudoProperty("MAX_VALUE");
  addPseudoProperty("MIN_VALUE");
  addPseudoProperty("NaN");
}

CJValueP
CJNumberType::
getProperty(CJavaScript *js, const std::string &key) const
{
  if      (key == "MAX_VALUE") {
    return js->createNumberValue(std::numeric_limits<double>::max());
  }
  else if (key == "MIN_VALUE") {
    return js->createNumberValue(std::numeric_limits<double>::min());
  }
  else if (key == "NaN") {
    return js->createNumberValue(CJUtil::getNaN());
  }

  return CJObjType::getProperty(js, key);
}

CJValueP
CJNumberType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJNumber *num = values[0]->cast<CJNumber>();
  assert(num);

  //---

  // type functions
  if      (name == "isNaN") {
    if (values.size() > 1) {
      double r = values[1]->toReal();

      return js->createBoolValue(CJUtil::isNaN(r));
    }
    else
      return CJValueP();
  }
  else if (name == "parseInt") {
    if (values.size() > 1) {
      COptLong integer = CJString::parseInt(values[1]->toString());

      return js->createNumberValue(integer.getValue(0));
    }
    else
      return CJValueP();
  }
  else if (name == "parseFloat") {
    if (values.size() > 1) {
      COptReal real = CJString::parseFloat(values[1]->toString());

      return js->createNumberValue(real.getValue(CJUtil::getNaN()));
    }
    else
      return CJValueP();
  }
  // object functions
  else if (name == "toFixed") {
    double r = num->toReal();
    long   n = 0;

    if (values.size() > 1)
      n = std::max(values[1]->toInteger(), 0L);

    char buffer[256];

    sprintf(buffer, "%.*f", int(n), r);

    return CJValueP(js->createStringValue(buffer));
  }
  else
    return CJValueP();
}

//------

CJNumber::
CJNumber(CJavaScript *js, double real) :
 CJObj(js, CJNumberType::instance(js)), real_(real)
{
}

std::string
CJNumber::
toString() const
{
  return CJUtil::realToString(real_);
}
