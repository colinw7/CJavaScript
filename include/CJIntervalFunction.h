#ifndef CJIntervalFunction_H
#define CJIntervalFunction_H

#include <CJFunction.h>

class CJSetInterval : public CJFunction {
 public:
  CJSetInterval(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJSetInterval(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

class CJClearInterval : public CJFunction {
 public:
  CJClearInterval(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CJClearInterval(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

#endif
