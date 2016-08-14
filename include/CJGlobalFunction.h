#ifndef CJGlobalFunction_H
#define CJGlobalFunction_H

#include <CJFunction.h>

// function calling API with signature: double result = fn(double r);
class CJGlobalFunction : public CJFunction {
 public:
  CJGlobalFunction(CJavaScript *js, const std::string &name) :
   CJFunction(js, name, CJFunction::Type::Global) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJGlobalFunction(js, name_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << name_;
  }
};

#endif
