#ifndef CJObjectFunction_H
#define CJObjectFunction_H

#include <CJFunction.h>

// object function
class CJObjectFunction : public CJFunction {
 public:
  CJObjectFunction(CJavaScript *js, const std::string &name) :
   CJFunction(js, name, CJFunction::Type::Object) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJObjectFunction(js, name_); }

  bool hasObjectValue() const override { return true; }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "object fn";
  }
};

#endif
