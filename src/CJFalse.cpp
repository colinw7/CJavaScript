#include <CJFalse.h>
#include <CJavaScript.h>

CJFalseTypeP CJFalseType::type_;

CJFalseTypeP
CJFalseType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = std::make_shared<CJFalseType>(js);

    js->addObjectType("false", type_);
  }

  return type_;
}

CJFalseType::
CJFalseType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::False, "false")
{
  addObjectFunction(js, "toString");
  addObjectFunction(js, "valueOf");
}

CJValueP
CJFalseType::
exec(CJavaScript *js, const std::string &name, const Values &)
{
  if      (name == "toString")
    return js->createStringValue("false");
  else if (name == "valueOf")
    return js->createFalseValue();
  else
    return CJValueP();
}

//------

CJFalseP
CJFalse::
value(CJavaScript *js)
{
  static CJFalseP falseValue;

  if (! falseValue)
    falseValue = std::make_shared<CJFalse>(js);

  return falseValue;
}

CJFalse::
CJFalse(CJavaScript *js) :
 CJValue(CJFalseType::instance(js))
{
}

void
CJFalse::
print(std::ostream &os) const
{
  os << "false";
}
