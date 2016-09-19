#ifndef CJAlertFunction_H
#define CJAlertFunction_H

#include <CJFunctionBase.h>

class CJAlertFunction : public CJFunctionBase {
 public:
  CJAlertFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJAlertFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

#endif
