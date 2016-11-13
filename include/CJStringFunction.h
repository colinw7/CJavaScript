#ifndef CJStringFunction_H
#define CJStringFunction_H

#include <CJObjTypeFunction.h>

// string function
class CJStringFunction : public CJObjTypeFunction {
 public:
  CJStringFunction(CJavaScript *js);

  CJStringFunction(const CJStringFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CJStringFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  CJValueP execNew(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

#endif
