#ifndef CJAlertFunction_H
#define CJAlertFunction_H

#include <CJFunction.h>

class CJAlertFunction : public CJFunction {
 public:
  CJAlertFunction(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJAlertFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

#endif
