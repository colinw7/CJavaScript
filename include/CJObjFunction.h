#ifndef CJObjFunction_H
#define CJObjFunction_H

#include <CJFunction.h>

// object function
class CJObjFunction : public CJFunction {
 public:
  CJObjFunction(CJavaScript *js, const std::string &name) :
   CJFunction(js, name, CJFunction::Type::Object) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJObjFunction(js, name_); }

  bool hasObjectValue() const override { return true; }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "object fn";
  }
};

#endif
