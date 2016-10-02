#include <CJNumber.h>
#include <CJavaScript.h>
#include <CJUtil.h>

CJNumberTypeP CJNumberType::type_;

CJNumberTypeP
CJNumberType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJNumberTypeP(new CJNumberType(js));

    type_->init();

    js->addObjectType(type_->name(), type_);

    js->addTypeObject(CJToken::Type::Number, type_);
  }

  return type_;
}

CJNumberType::
CJNumberType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Number, "Number")
{
}

void
CJNumberType::
init()
{
  addTypeFunction(js_, "isNaN"     , type_);
  addTypeFunction(js_, "parseInt"  , type_);
  addTypeFunction(js_, "parseFloat", type_);
  addTypeFunction(js_, "toString"  , type_);

  addObjectFunction(js_, "toString", type_);
  addObjectFunction(js_, "toFixed" , type_);

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
execType(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  // values[0] is CJNumberFunction

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
    if (values.size() <= 1)
      return js->createNumberValue(CJUtil::getNaN());

    std::string str = (values[1] ? values[1]->toString() : std::string());

    COptInt base;

    if (values.size() > 1 && values[1])
      base = values[1]->toInteger();

    if (base.isValid() && (base.getValue() < 2 || base.getValue() > 36))
      return js->createNumberValue(CJUtil::getNaN());

    COptLong integer = CJString::parseInt(values[1]->toString(), base, /*extraChars*/true);

    if (integer.isValid())
      return js->createNumberValue(integer.getValue());
    else
      return js->createNumberValue(CJUtil::getNaN());
  }
  else if (name == "parseFloat") {
    if (values.size() <= 1)
      return js->createNumberValue(CJUtil::getNaN());

    COptReal real = CJString::parseFloat(values[1]->toString(), /*extraChars*/true);

    return js->createNumberValue(real.getValue(CJUtil::getNaN()));
  }
  else if (name == "toString") {
    return js->createStringValue("function Number() { }");
  }
  else
    return CJValueP();
}

CJValueP
CJNumberType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJNumberP num = CJValue::cast<CJNumber>(values[0]);
  assert(num);

  //---

  // object functions
  if      (name == "toString") {
    return js->createStringValue(num->toString());
  }
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
  if (! isPrimitive())
    return "[Number: " + realString() + "]";
  else
    return realString();
}

std::string
CJNumber::
realString() const
{
  return CJUtil::realToString(real_);
}
