#include <CJConsole.h>
#include <CJavaScript.h>

CJObjectTypeP CJConsoleType::type_;

CJObjectTypeP
CJConsoleType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CJConsoleType);

    js->addObjectType("console", type_);
  }

  return type_;
}

CJConsoleType::
CJConsoleType() :
 CJObjectType(CJToken::Type::Object, "Console")
{
}

CJValueP
CJConsoleType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//---

CJConsoleObject::
CJConsoleObject(CJavaScript *js) :
 CJObject(CJConsoleType::instance(js)), js_(js)
{
  setFunctionProperty(js, CJFunctionP(new CJObjectFunction(js, "log")));
}

CJValueP
CJConsoleObject::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if (name == "log") {
    int n = values.size();

    for (int i = 1; i < n; ++i) {
      if (i > 1) std::cout << " ";

      if (values[i]) {
        if (values[i]->type() == CJToken::Type::String)
          std::cout << values[i]->cast<CJString>()->text();
        else
          std::cout << *values[i];
      }
    }

    std::cout << std::endl;
  }
  else
    js->errorMsg("Invalid console function - " + name);

  return CJValueP();
}
