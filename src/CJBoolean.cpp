#include <CJBoolean.h>
#include <CJavaScript.h>
#include <CJUtil.h>

CJBooleanTypeP CJBooleanType::type_;

CJBooleanTypeP
CJBooleanType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJBooleanTypeP(new CJBooleanType(js));

    type_->init();

    js->addObjectType(type_->name(), type_);

    js->addTypeObject(CJToken::Type::Boolean, type_);
  }

  return type_;
}

CJBooleanType::
CJBooleanType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Boolean, "Boolean")
{
}

void
CJBooleanType::
init()
{
  addTypeFunction(js_, "toString", type_);

  addObjectFunction(js_, "toString", type_);
  addObjectFunction(js_, "valueOf" , type_);
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

  CJBooleanP num = CJValue::cast<CJBoolean>(values[0]);
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
  if (isPrimitive())
    return std::string(b_ ? "true" : "false");
  else
    return "[Boolean: " + std::string(b_ ? "true" : "false") + "]";
}
