#ifndef CQJIntervalFunction_H
#define CQJIntervalFunction_H

#include <CJFunction.h>

class CQJavaScript;

class CQJSetInterval : public CJFunction {
 public:
  CQJSetInterval(CQJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CQJSetInterval(qjs_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

 private:
  CQJavaScript *qjs_ { 0 };
};

class CQJClearInterval : public CJFunction {
 public:
  CQJClearInterval(CQJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CQJClearInterval(qjs_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

 private:
  CQJavaScript *qjs_ { 0 };
};

#endif
