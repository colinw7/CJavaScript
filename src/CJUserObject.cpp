#include <CJUserObject.h>
#include <CJavaScript.h>

CJUserType::
CJUserType(CJavaScript *js, const std::string &name) :
 CJObjType(js, CJToken::Type::Object, name)
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
CJUserObject(CJavaScript *js, CJObjTypeP userType) :
 CJObj(userType), js_(js)
{
}

CJValueP
CJUserObject::
execNameFn(CJavaScript *js, const std::string &name, const Values &)
{
  js->errorMsg("Invalid user function - " + name);

  return CJValueP();
}
