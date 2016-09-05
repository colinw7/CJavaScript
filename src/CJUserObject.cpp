#include <CJUserObject.h>
#include <CJavaScript.h>

CJObjTypeP CJUserType::type_;

CJObjTypeP
CJUserType::
instance(CJavaScript *js, const std::string &name)
{
  if (! type_)
    type_ = CJObjTypeP(new CJUserType(js, name));

  return type_;
}

CJUserType::
CJUserType(CJavaScript *js, const std::string &name) :
 CJObjType(js, CJToken::Type::UserObject, name)
{
}

CJValueP
CJUserType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJUserObject::
CJUserObject(CJavaScript *js, CJObjTypeP userType, CJFunctionP userFn) :
 CJObj(js, CJUserType::instance(js, userType->name())), userType_(userType), userFn_(userFn)
{
}

CJValueP
CJUserObject::
getProperty(CJavaScript *js, const std::string &name) const
{
  return userFn_->getProperty(js, name);
}

void
CJUserObject::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  userFn_->setProperty(js, name, value);
}

CJValueP
CJUserObject::
execNameFn(CJavaScript *js, const std::string &name, const Values &)
{
  js->errorMsg("Invalid user function - " + name);

  return CJValueP();
}
