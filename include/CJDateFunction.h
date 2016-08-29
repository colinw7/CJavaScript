#ifndef CJDateFunction_H
#define CJDateFunction_H

#include <CJTypeFunction.h>

// string function
class CJDateFunction : public CJTypeFunction {
 public:
  CJDateFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJDateFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "[Function: Date]";
  }
};

#endif
