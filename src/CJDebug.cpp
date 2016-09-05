#include <CJDebug.h>
#include <CJObjFunction.h>
#include <CJUserFunction.h>
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
 CJObj(js, CJDebugType::instance(js))
{
  setFunctionProperty(js, CJFunctionP(new CJObjFunction(js, "print")));
  setFunctionProperty(js, CJFunctionP(new CJObjFunction(js, "printScopeStack")));
  setFunctionProperty(js, CJFunctionP(new CJObjFunction(js, "printScopeChain")));
  setFunctionProperty(js, CJFunctionP(new CJObjFunction(js, "printUserFunctions")));
}

CJValueP
CJDebugObject::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if      (name == "print") {
    for (uint i = 1; i < values.size(); ++i) {
      if (i > 1) std::cerr << " ";

      if (values[i])
        values[i]->print(std::cerr);
      else
        std::cerr << "null";
    }

    std::cerr << std::endl;
  }
  else if (name == "printScopeStack") {
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
