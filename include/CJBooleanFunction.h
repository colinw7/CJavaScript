#ifndef CJBooleanFunction_H
#define CJBooleanFunction_H

#include <CJTypeFunction.h>

// boolean function
class CJBooleanFunction : public CJTypeFunction {
 public:
  CJBooleanFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJBooleanFunction(js); }

  bool hasObjectValue() const override { return true; }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "[Function: Boolean]";
  }
};

#endif
