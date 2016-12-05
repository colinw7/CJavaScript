#ifndef CQJIntervalFunction_H
#define CQJIntervalFunction_H

#include <CJFunctionBase.h>

class CQJSetInterval : public CJFunctionBase {
 public:
  CQJSetInterval(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CQJSetInterval(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

class CQJClearInterval : public CJFunctionBase {
 public:
  CQJClearInterval(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CQJClearInterval(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

#endif
