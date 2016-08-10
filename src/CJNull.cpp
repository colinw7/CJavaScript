#include <CJNull.h>
#include <CJavaScript.h>

CJObjectTypeP CJNullType::type_;

CJObjectTypeP
CJNullType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CJNullType(js));

    js->addObjectType("null", type_);
  }

  return type_;
}

CJNullType::
CJNullType(CJavaScript *) :
 CJObjectType(CJToken::Type::Null, "null")
{
}

CJValueP
CJNullType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJNull::
CJNull(CJavaScript *js) :
 CJValue(CJNullType::instance(js))
{
}
