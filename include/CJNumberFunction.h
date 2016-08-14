#ifndef CJNumberFunction_H
#define CJNumberFunction_H

#include <CJTypeFunction.h>

// number function
class CJNumberFunction : public CJTypeFunction {
 public:
  CJNumberFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJNumberFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "[Function: Number]";
  }
};

#endif
