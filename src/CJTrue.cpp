#include <CJTrue.h>
#include <CJavaScript.h>

CJObjTypeP CJTrueType::type_;

CJObjTypeP
CJTrueType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJTrueType(js));

    js->addObjectType("true", type_);
  }

  return type_;
}

CJTrueType::
CJTrueType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::True, "true")
{
  addObjectFunction(js, "toString");
  addObjectFunction(js, "valueOf");
}

CJValueP
CJTrueType::
exec(CJavaScript *js, const std::string &name, const Values &)
{
  if      (name == "toString")
    return js->createStringValue("true");
  else if (name == "valueOf")
    return CJValueP(js->createTrueValue());
  else
    return CJValueP();
}

//------

CJValueP
CJTrue::
value(CJavaScript *js)
{
  static CJValueP trueValue;

  if (! trueValue)
    trueValue = CJValueP(new CJTrue(js));

  return trueValue;
}

CJTrue::
CJTrue(CJavaScript *js) :
 CJValue(CJTrueType::instance(js))
{
}

void
CJTrue::
print(std::ostream &os) const
{
  os << "true";
}
