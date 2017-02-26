#include <CQJRequestAnimationFrame.h>
#include <CQJWindow.h>

CQJRequestAnimationFrame::
CQJRequestAnimationFrame(CJavaScript *js, CQJWindowP window) :
 CJFunctionBase(js, "requestAnimationFrame"), window_(window)
{
}

CJValueP
CQJRequestAnimationFrame::
exec(CJavaScript *, const Values &values)
{
  if (values.size() != 1)
    return CJValueP();

  CJValueP fnValue = values[0];

  if (fnValue->type() == CJToken::Type::Function) {
    CJFunctionBaseP fn = CJValue::cast<CJFunctionBase>(fnValue);

    window_->addOneShotTimer(fn, 10);
    //fn->exec(js, values);

    //qjs->canvas()->update();
  }

  return CJValueP();
}
