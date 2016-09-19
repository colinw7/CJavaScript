#include <CJBooleanFunction.h>
#include <CJavaScript.h>
#include <CJTrue.h>

CJBooleanFunction::
CJBooleanFunction(CJavaScript *js) :
 CJObjTypeFunction(js, "Boolean", CJBooleanType::instance(js))
{
}

CJValueP
CJBooleanFunction::
exec(CJavaScript *js, const Values &values)
{
  CJBoolean *bobj;

  if (values.size() <= 1)
    bobj = new CJBoolean(js);
  else {
    bool b = values[1]->toBoolean();

    bobj = new CJBoolean(js, b);
  }

  return CJValueP(bobj);
}
