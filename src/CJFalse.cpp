#include <CJFalse.h>
#include <CJavaScript.h>

CJObjTypeP CJFalseType::type_;

CJObjTypeP
CJFalseType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJFalseType(js));

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

CJValueP
CJFalse::
value(CJavaScript *js)
{
  static CJValueP falseValue;

  if (! falseValue)
    falseValue = CJValueP(new CJFalse(js));

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
