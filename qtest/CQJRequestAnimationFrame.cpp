#include <CQJRequestAnimationFrame.h>
#include <CQJWindow.h>
#include <CQJSCanvas.h>
#include <CQJavaScript.h>

CQJRequestAnimationFrame::
CQJRequestAnimationFrame(CQJavaScript *qjs) :
 CJFunction(qjs->js(), "requestAnimationFrame"), qjs_(qjs)
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
    CJFunctionP fn = std::static_pointer_cast<CJFunction>(fnValue);

    CQJWindowP window = std::static_pointer_cast<CQJWindow>(qjs_->jsWindow());

    window->addOneShotTimer(fn, 10);
    //fn->exec(js, values);

    //qjs_->canvas()->update();
  }

  return CJValueP();
}
