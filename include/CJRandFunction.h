#ifndef CJRandFunction_H
#define CJRandFunction_H

#include <CJFunctionBase.h>

// function calling API with signature: double result = fn(double r1, double r2);
class CJRandFunction : public CJFunctionBase {
 public:
  explicit CJRandFunction(CJavaScript *js);

  CJRandFunction(const CJRandFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CJRandFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

#endif
