#ifndef CJRegExpFunction_H
#define CJRegExpFunction_H

#include <CJObjTypeFunction.h>

// string function
class CJRegExpFunction : public CJObjTypeFunction {
 public:
  CJRegExpFunction(CJavaScript *js);

  CJRegExpFunction(const CJRegExpFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CJRegExpFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  CJValueP execNew(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;
};

#endif
