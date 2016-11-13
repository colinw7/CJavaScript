#include <CJSystem.h>
#include <CJavaScript.h>
#include <CJUtil.h>

// function calling API with signature: double result = fn(double r);
class CJSystemExecFunction : public CJFunctionBase {
 public:
  CJSystemExecFunction(CJavaScript *js) :
   CJFunctionBase(js, "exec", CJFunctionBase::Type::System) {
  }

  CJSystemExecFunction(const CJSystemExecFunction &fn) :
   CJFunctionBase(fn) {
  }

  CJValue *dup(CJavaScript *) const override { return new CJSystemExecFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "exec()";
  }
};

//---

CJSystem::
CJSystem(CJavaScript *js) :
 CJDictionary(js, "")
{
  setFunctionProperty(js, CJFunctionBaseP(new CJSystemExecFunction(js)));
}

//---

CJValueP
CJSystemExecFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() < 1)
    return CJValueP();

  std::string cmd = values[0]->toString();

  int rc = system(cmd.c_str());

  return js->createNumberValue(long(rc));
}
