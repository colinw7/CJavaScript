#include <CJUndefined.h>
#include <CJavaScript.h>

CJObjectTypeP CJUndefinedType::type_;

CJObjectTypeP
CJUndefinedType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CJUndefinedType);

    js->addObjectType("undefined", type_);
  }

  return type_;
}

CJUndefinedType::
CJUndefinedType() :
 CJObjectType(CJToken::Type::Undefined, "undefined")
{
}

CJValueP
CJUndefinedType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJUndefined::
CJUndefined(CJavaScript *js) :
 CJValue(CJUndefinedType::instance(js))
{
}
