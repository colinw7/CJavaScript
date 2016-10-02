#ifndef CJObjectFunction_H
#define CJObjectFunction_H

#include <CJObjTypeFunction.h>

// Object constructor function
class CJObjectFunction : public CJObjTypeFunction {
 public:
  CJObjectFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJObjectFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  CJValueP execNew(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

typedef std::shared_ptr<CJObjectFunction> CJObjectFunctionP;

#endif
