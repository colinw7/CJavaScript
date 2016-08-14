#ifndef CJArrayFunction_H
#define CJArrayFunction_H

#include <CJTypeFunction.h>

// Array constructor function
class CJArrayFunction : public CJTypeFunction {
 public:
  CJArrayFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJArrayFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "[Function: Array]";
  }
};

typedef std::shared_ptr<CJArrayFunction> CJArrayFunctionP;

#endif
