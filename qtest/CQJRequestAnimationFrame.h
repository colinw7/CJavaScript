#ifndef CQJRequestAnimationFrame_H
#define CQJRequestAnimationFrame_H

#include <CJFunctionBase.h>

class CQJavaScript;

class CQJRequestAnimationFrame : public CJFunctionBase {
 public:
  CQJRequestAnimationFrame(CQJavaScript *js);

  CJValue *dup(CJavaScript *) const override { return new CQJRequestAnimationFrame(qjs_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

 private:
  CQJavaScript *qjs_ { 0 };
};

#endif
