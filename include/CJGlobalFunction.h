#ifndef CJGlobalFunction_H
#define CJGlobalFunction_H

#include <CJFunctionBase.h>

// function calling API with signature: double result = fn(double r);
class CJGlobalFunction : public CJFunctionBase {
 public:
  CJGlobalFunction(CJavaScript *js, const std::string &name) :
   CJFunctionBase(js, name, CJFunctionBase::Type::Global) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJGlobalFunction(js, name_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << name_;
  }
};

#endif
