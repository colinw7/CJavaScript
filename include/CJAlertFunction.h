#ifndef CJAlertFunction_H
#define CJAlertFunction_H

#include <CJFunctionBase.h>

class CJAlertFunction : public CJFunctionBase {
 public:
  CJAlertFunction(CJavaScript *js);

  CJAlertFunction(const CJAlertFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CJAlertFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

#endif
