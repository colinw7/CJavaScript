#ifndef CQJIntervalFunction_H
#define CQJIntervalFunction_H

#include <CJFunctionBase.h>

class CQJavaScript;

class CQJSetInterval : public CJFunctionBase {
 public:
  CQJSetInterval(CQJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CQJSetInterval(qjs_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

 private:
  CQJavaScript *qjs_ { 0 };
};

class CQJClearInterval : public CJFunctionBase {
 public:
  CQJClearInterval(CQJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CQJClearInterval(qjs_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

 private:
  CQJavaScript *qjs_ { 0 };
};

#endif
