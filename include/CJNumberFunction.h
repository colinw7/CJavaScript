#ifndef CJNumberFunction_H
#define CJNumberFunction_H

#include <CJObjTypeFunction.h>

// number function
class CJNumberFunction : public CJObjTypeFunction {
 public:
  CJNumberFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJNumberFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  CJValueP execNew(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

#endif
