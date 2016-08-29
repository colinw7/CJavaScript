#include <CJFunctionFunction.h>
#include <CJUserFunction.h>
#include <CJavaScript.h>

CJFunctionFunction::
CJFunctionFunction(CJavaScript *js) :
 CJTypeFunction(js, "Function", CJFunctionType::instance(js, "Function"))
{
}

CJValueP
CJFunctionFunction::
exec(CJavaScript *js, const Values &values)
{
  std::string          name; // anonymous function
  CJUserFunction::Args args;
  CJExecBlockP         block;

  if (values.size() > 1) {
    for (uint i = 0; i < values.size() - 1; ++i)
      args.push_back(values[i]->toString());

    if (values.size() > 2) {
      std::string body = values[values.size() - 1]->toString();

      block = js->interpFunctionBlock(body);
    }
  }

  CJUserFunction *fn = new CJUserFunction(js, name, args, block);

  fn->setScope(js);

  return CJValueP(fn);
}
