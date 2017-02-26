#ifndef CQJIntervalFunction_H
#define CQJIntervalFunction_H

#include <CJFunctionBase.h>
#include <CQJWindow.h>

class CQJSetInterval : public CJFunctionBase {
 public:
  CQJSetInterval(CJavaScript *js, CQJWindowP window);

  CJValue *dup(CJavaScript *js) const override { return new CQJSetInterval(js, window_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

 private:
  CQJWindowP window_;
};

class CQJClearInterval : public CJFunctionBase {
 public:
  CQJClearInterval(CJavaScript *js, CQJWindowP window);

  CJValue *dup(CJavaScript *js) const override { return new CQJClearInterval(js, window_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

 private:
  CQJWindowP window_;
};

#endif
