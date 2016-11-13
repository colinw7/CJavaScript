#ifndef CJObjFunction_H
#define CJObjFunction_H

#include <CJFunctionBase.h>

// object function
class CJObjFunction : public CJFunctionBase {
 public:
  CJObjFunction(CJavaScript *js, const std::string &name);

  CJObjFunction(const CJObjFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CJObjFunction(*this); }

  bool hasObjectValue() const override { return true; }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

#endif
