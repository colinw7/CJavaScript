#ifndef CJIntervalFunction_H
#define CJIntervalFunction_H

#include <CJFunctionBase.h>

class CJSetInterval : public CJFunctionBase {
 public:
  CJSetInterval(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJSetInterval(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

class CJClearInterval : public CJFunctionBase {
 public:
  CJClearInterval(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJClearInterval(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

#endif
