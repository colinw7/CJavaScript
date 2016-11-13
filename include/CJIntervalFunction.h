#ifndef CJIntervalFunction_H
#define CJIntervalFunction_H

#include <CJFunctionBase.h>

class CJSetInterval : public CJFunctionBase {
 public:
  CJSetInterval(CJavaScript *js);

  CJSetInterval(const CJSetInterval &i);

  CJValue *dup(CJavaScript *) const override { return new CJSetInterval(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

class CJClearInterval : public CJFunctionBase {
 public:
  CJClearInterval(CJavaScript *js);

  CJClearInterval(const CJClearInterval &i);

  CJValue *dup(CJavaScript *) const override { return new CJClearInterval(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

#endif
