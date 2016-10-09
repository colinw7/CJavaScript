#include <CJUndefined.h>
#include <CJavaScript.h>

CJUndefinedTypeP CJUndefinedType::type_;

CJUndefinedTypeP
CJUndefinedType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = std::make_shared<CJUndefinedType>(js);

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

CJUndefinedP
CJUndefined::
value(CJavaScript *js)
{
  static CJUndefinedP undefinedValue;

  if (! undefinedValue)
    undefinedValue = std::make_shared<CJUndefined>(js);

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
