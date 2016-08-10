#include <CJUserObject.h>
#include <CJavaScript.h>

CJValueP
CJUserType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJUserObject::
CJUserObject(CJavaScript *js, CJObjectTypeP userType) :
 CJObject(userType), js_(js)
{
}

CJValueP
CJUserObject::
execNameFn(CJavaScript *js, const std::string &name, const Values &)
{
  js->errorMsg("Invalid user function - " + name);

  return CJValueP();
}
