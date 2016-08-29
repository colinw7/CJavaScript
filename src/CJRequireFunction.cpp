#include <CJRequireFunction.h>
#include <CJavaScript.h>

CJRequireFunction::
CJRequireFunction(CJavaScript *js) :
 CJFunction(js, "require")
{
}

CJValueP
CJRequireFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() < 1)
    return CJValueP();

  std::string filename = values[0]->toString();

  bool rc = js->loadSubFile(filename);

  if (! rc) {
    js->errorMsg("Failed to load '" + filename + "'");
    return CJValueP();
  }

  return js->exec();
}
