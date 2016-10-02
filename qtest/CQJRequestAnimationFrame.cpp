#include <CQJRequestAnimationFrame.h>
#include <CQJWindow.h>
#include <CQJSCanvas.h>
#include <CQJavaScript.h>

CQJRequestAnimationFrame::
CQJRequestAnimationFrame(CQJavaScript *qjs) :
 CJFunctionBase(qjs->js(), "requestAnimationFrame"), qjs_(qjs)
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

    CQJWindowP window = CJValue::cast<CQJWindow>(qjs_->jsWindow());

    window->addOneShotTimer(fn, 10);
    //fn->exec(js, values);

    //qjs_->canvas()->update();
  }

  return CJValueP();
}
