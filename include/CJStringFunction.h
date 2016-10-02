#ifndef CJStringFunction_H
#define CJStringFunction_H

#include <CJObjTypeFunction.h>

// string function
class CJStringFunction : public CJObjTypeFunction {
 public:
  CJStringFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJStringFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  CJValueP execNew(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

#endif
