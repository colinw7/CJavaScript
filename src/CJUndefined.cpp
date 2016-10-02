#include <CJUndefined.h>
#include <CJavaScript.h>

CJObjTypeP CJUndefinedType::type_;

CJObjTypeP
CJUndefinedType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJUndefinedType(js));

    js->addObjectType("undefined", type_);
  }

  return type_;
}

CJUndefinedType::
CJUndefinedType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Undefined, "undefined")
{
}

CJValueP
CJUndefinedType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJValueP
CJUndefined::
value(CJavaScript *js)
{
  static CJValueP undefinedValue;

  if (! undefinedValue)
    undefinedValue = CJValueP(new CJUndefined(js));

  return undefinedValue;
}

CJUndefined::
CJUndefined(CJavaScript *js) :
 CJValue(CJUndefinedType::instance(js))
{
}

void
CJUndefined::
print(std::ostream &os) const
{
  os << "undefined";
}
