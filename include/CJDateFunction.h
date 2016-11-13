#ifndef CJDateFunction_H
#define CJDateFunction_H

#include <CJObjTypeFunction.h>

// string function
class CJDateFunction : public CJObjTypeFunction {
 public:
  CJDateFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CJDateFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  CJValueP execNew(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

#endif
