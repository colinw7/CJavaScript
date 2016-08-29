#ifndef CJRegExpFunction_H
#define CJRegExpFunction_H

#include <CJTypeFunction.h>

// string function
class CJRegExpFunction : public CJTypeFunction {
 public:
  CJRegExpFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJRegExpFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "[Function: RegExp]";
  }
};

#endif
