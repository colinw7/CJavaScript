#include <CQJRequestAnimationFrame.h>
#include <CQJWindow.h>
#include <CQJCanvasWidget.h>
#include <CQJavaScript.h>

CQJRequestAnimationFrame::
CQJRequestAnimationFrame(CJavaScript *js) :
 CJFunctionBase(js, "requestAnimationFrame")
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

    CQJavaScript *qjs = CQJavaScript::instance();

    CQJWindowP window = CJValue::cast<CQJWindow>(qjs->jsWindow());

    window->addOneShotTimer(fn, 10);
    //fn->exec(js, values);

    //qjs->canvas()->update();
  }

  return CJValueP();
}
