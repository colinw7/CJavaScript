#include <CJTrue.h>
#include <CJavaScript.h>

CJTrueTypeP CJTrueType::type_;

CJTrueTypeP
CJTrueType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = std::make_shared<CJTrueType>(js);

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
    return js->createTrueValue();
  else
    return CJValueP();
}

//------

CJTrueP
CJTrue::
value(CJavaScript *js)
{
  static CJTrueP trueValue;

  if (! trueValue)
    trueValue = std::make_shared<CJTrue>(js);

  return trueValue;
}

CJTrue::
CJTrue(CJavaScript *js) :
 CJValue(CJTrueType::instance(js))
{
}

CJTrue::
CJTrue(const CJTrue &t) :
 CJValue(t)
{
}

void
CJTrue::
print(std::ostream &os) const
{
  os << "true";
}
