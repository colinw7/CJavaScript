#ifndef CJObjFunction_H
#define CJObjFunction_H

#include <CJFunctionBase.h>

// object function
class CJObjFunction : public CJFunctionBase {
 public:
  CJObjFunction(CJavaScript *js, const std::string &name) :
   CJFunctionBase(js, name, CJFunctionBase::Type::Object) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJObjFunction(js, name_); }

  bool hasObjectValue() const override { return true; }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

#endif
