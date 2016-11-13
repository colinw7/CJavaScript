#ifndef CJArrayFunction_H
#define CJArrayFunction_H

#include <CJObjTypeFunction.h>

// Array constructor function
class CJArrayFunction : public CJObjTypeFunction {
 public:
  CJArrayFunction(CJavaScript *js);

  CJArrayFunction(const CJArrayFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CJArrayFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  CJValueP execNew(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

typedef std::shared_ptr<CJArrayFunction> CJArrayFunctionP;

#endif
