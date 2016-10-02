#ifndef CJBooleanFunction_H
#define CJBooleanFunction_H

#include <CJObjTypeFunction.h>

// boolean function
class CJBooleanFunction : public CJObjTypeFunction {
 public:
  CJBooleanFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJBooleanFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  CJValueP execNew(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

#endif
