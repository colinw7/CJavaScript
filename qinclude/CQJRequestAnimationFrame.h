#ifndef CQJRequestAnimationFrame_H
#define CQJRequestAnimationFrame_H

#include <CJFunctionBase.h>
#include <CQJWindow.h>

class CQJavaScript;

class CQJRequestAnimationFrame : public CJFunctionBase {
 public:
  CQJRequestAnimationFrame(CJavaScript *js, CQJWindowP window);

  CJValue *dup(CJavaScript *js) const override { return new CQJRequestAnimationFrame(js, window_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

 private:
  CQJWindowP window_;
};

#endif
