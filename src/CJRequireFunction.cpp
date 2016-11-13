#include <CJRequireFunction.h>
#include <CJavaScript.h>

CJRequireFunction::
CJRequireFunction(CJavaScript *js) :
 CJFunctionBase(js, "require")
{
}

CJRequireFunction::
CJRequireFunction(const CJRequireFunction &fn) :
 CJFunctionBase(fn)
{
}

CJValueP
CJRequireFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() <= 1)
    return CJValueP();

  std::string filename = values[1]->toString();

  bool rc = js->loadSubFile(filename);

  if (! rc) {
    js->errorMsg("Failed to load '" + filename + "'");
    return CJValueP();
  }

  return js->exec();
}
