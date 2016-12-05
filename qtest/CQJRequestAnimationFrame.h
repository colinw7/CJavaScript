#ifndef CQJRequestAnimationFrame_H
#define CQJRequestAnimationFrame_H

#include <CJFunctionBase.h>

class CQJavaScript;

class CQJRequestAnimationFrame : public CJFunctionBase {
 public:
  CQJRequestAnimationFrame(CJavaScript *js);

  CJValue *dup(CJavaScript *js) const override { return new CQJRequestAnimationFrame(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;
};

#endif
