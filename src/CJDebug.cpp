#include <CJDebug.h>
#include <CJObjFunction.h>
#include <CJTypes.h>
#include <CJavaScript.h>

CJObjTypeP CJDebugType::type_;

CJObjTypeP
CJDebugType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJDebugType(js));

    js->addObjectType("debug", type_);
  }

  return type_;
}

CJDebugType::
CJDebugType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Debug")
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
 CJObj(CJDebugType::instance(js)), js_(js)
{
  setFunctionProperty(js, CJFunctionP(new CJObjFunction(js, "printScopeStack")));
  setFunctionProperty(js, CJFunctionP(new CJObjFunction(js, "printScopeChain")));
  setFunctionProperty(js, CJFunctionP(new CJObjFunction(js, "printUserFunctions")));
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
