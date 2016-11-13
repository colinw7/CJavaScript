#ifndef CJRequireFunction_H
#define CJRequireFunction_H

#include <CJFunctionBase.h>

class CJRequireFunction : public CJFunctionBase {
 public:
  CJRequireFunction(CJavaScript *js);

  CJRequireFunction(const CJRequireFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CJRequireFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

#endif
