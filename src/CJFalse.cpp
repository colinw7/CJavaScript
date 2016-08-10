#include <CJFalse.h>
#include <CJavaScript.h>

CJObjectTypeP CJFalseType::type_;

CJObjectTypeP
CJFalseType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CJFalseType(js));

    js->addObjectType("false", type_);
  }

  return type_;
}

CJFalseType::
CJFalseType(CJavaScript *js) :
 CJObjectType(CJToken::Type::False, "false")
{
  addFunction(js, "toString");
  addFunction(js, "valueOf");
}

CJValueP
CJFalseType::
exec(CJavaScript *js, const std::string &name, const Values &)
{
  if      (name == "toString")
    return js->createStringValue("false");
  else if (name == "valueOf")
    return CJValueP(new CJFalse(js));
  else
    return CJValueP();
}

//------

CJFalse::
CJFalse(CJavaScript *js) :
 CJValue(CJFalseType::instance(js))
{
}
