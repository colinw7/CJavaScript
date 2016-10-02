#include <CJNull.h>
#include <CJavaScript.h>

CJObjTypeP CJNullType::type_;

CJObjTypeP
CJNullType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJNullType(js));

    js->addObjectType("null", type_);
  }

  return type_;
}

CJNullType::
CJNullType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Null, "null")
{
}

CJValueP
CJNullType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJValueP
CJNull::
value(CJavaScript *js)
{
  static CJValueP nullValue;

  if (! nullValue)
    nullValue = CJValueP(new CJNull(js));

  return nullValue;
}

CJNull::
CJNull(CJavaScript *js) :
 CJValue(CJNullType::instance(js))
{
}

void
CJNull::
print(std::ostream &os) const
{
  os << "null";
}
