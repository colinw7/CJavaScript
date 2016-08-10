#include <CJDebug.h>
#include <CJavaScript.h>

CJObjectTypeP CJDebugType::type_;

CJObjectTypeP
CJDebugType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CJDebugType);

    js->addObjectType("debug", type_);
  }

  return type_;
}

CJDebugType::
CJDebugType() :
 CJObjectType(CJToken::Type::Object, "Debug")
{
}

CJValueP
CJDebugType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//---

CJDebugObject::
CJDebugObject(CJavaScript *js) :
 CJObject(CJDebugType::instance(js)), js_(js)
{
  setFunctionProperty(js, CJFunctionP(new CJObjectFunction(js, "printScopeStack")));
  setFunctionProperty(js, CJFunctionP(new CJObjectFunction(js, "printScopeChain")));
  setFunctionProperty(js, CJFunctionP(new CJObjectFunction(js, "printUserFunctions")));
}

CJValueP
CJDebugObject::
execNameFn(CJavaScript *js, const std::string &name, const Values &)
{
  if      (name == "printScopeStack") {
    js->printScopeStack();
  }
  else if (name == "printScopeChain") {
    CJUserFunctionP fn = js->currentFunction();

    if (fn)
      js->printScopeChain(fn->scope());
  }
  else if (name == "printUserFunctions") {
    js->printUserFunctions();
  }
  else
    js->errorMsg("Invalid debug function - " + name);

  return CJValueP();
}
