#include <CJTrue.h>
#include <CJavaScript.h>

CJObjectTypeP CJTrueType::type_;

CJObjectTypeP
CJTrueType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CJTrueType(js));

    js->addObjectType("true", type_);
  }

  return type_;
}

CJTrueType::
CJTrueType(CJavaScript *js) :
 CJObjectType(CJToken::Type::True, "true")
{
  addFunction(js, "toString");
  addFunction(js, "valueOf");
}

CJValueP
CJTrueType::
exec(CJavaScript *js, const std::string &name, const Values &)
{
  if      (name == "toString")
    return js->createStringValue("true");
  else if (name == "valueOf")
    return CJValueP(new CJTrue(js));
  else
    return CJValueP();
}

//------

CJTrue::
CJTrue(CJavaScript *js) :
 CJValue(CJTrueType::instance(js))
{
}
