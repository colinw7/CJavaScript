#include <CJBoolean.h>
#include <CJavaScript.h>
#include <CJUtil.h>

CJObjTypeP CJBooleanType::type_;

CJObjTypeP
CJBooleanType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJBooleanType(js));

    js->addObjectType("boolean", type_);

    js->addTypeObject(CJToken::Type::Boolean, type_);
  }

  return type_;
}

CJBooleanType::
CJBooleanType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Boolean, "boolean")
{
  addTypeFunction(js, "toString");

  addObjectFunction(js, "toString");
  addObjectFunction(js, "valueOf");
}

CJValueP
CJBooleanType::
getProperty(CJavaScript *js, const std::string &key) const
{
  return CJObjType::getProperty(js, key);
}

CJValueP
CJBooleanType::
execType(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  // values[0] is CJBooleanFunction

  //---

  if (name == "toString")
    return js->createStringValue("function Boolean() { }");
  else
    return CJValueP();
}

CJValueP
CJBooleanType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJBoolean *num = values[0]->cast<CJBoolean>();
  assert(num);

  //---

  if      (name == "toString")
    return js->createStringValue(num->toBoolean() ? "true" : "false");
  else if (name == "valueOf")
    return js->createBoolValue(num->toBoolean());
  else
    return CJValueP();
}

//------

CJBoolean::
CJBoolean(CJavaScript *js, bool b) :
 CJObj(js, CJBooleanType::instance(js)), b_(b)
{
}

std::string
CJBoolean::
toString() const
{
  return "[Boolean: " + std::string(b_ ? "true" : "false") + "]";
}
