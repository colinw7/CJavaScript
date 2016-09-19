#ifndef CJRequireFunction_H
#define CJRequireFunction_H

#include <CJFunctionBase.h>

class CJRequireFunction : public CJFunctionBase {
 public:
  CJRequireFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJRequireFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

#endif
