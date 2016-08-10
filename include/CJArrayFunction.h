#ifndef CJArrayFunction_H
#define CJArrayFunction_H

#include <CJTypeFunction.h>

// array function
class CJArrayFunction : public CJTypeFunction {
 public:
  CJArrayFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJArrayFunction(js); }

  bool hasObjectValue() const override { return true; }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "[Function: Array]";
  }
};

#endif
