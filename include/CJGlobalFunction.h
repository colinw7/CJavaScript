#ifndef CJGlobalFunction_H
#define CJGlobalFunction_H

#include <CJFunctionBase.h>

// function calling API with signature: double result = fn(double r);
class CJGlobalFunction : public CJFunctionBase {
 public:
  CJGlobalFunction(CJavaScript *js, const std::string &name);

  CJGlobalFunction(const CJGlobalFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CJGlobalFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

#endif
