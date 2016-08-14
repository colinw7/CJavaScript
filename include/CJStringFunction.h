#ifndef CJStringFunction_H
#define CJStringFunction_H

#include <CJTypeFunction.h>

// string function
class CJStringFunction : public CJTypeFunction {
 public:
  CJStringFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJStringFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "[Function: String]";
  }
};

#endif
