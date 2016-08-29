#include <CJAlertFunction.h>

CJAlertFunction::
CJAlertFunction(CJavaScript *js) :
 CJFunction(js, "alert")
{
}

CJValueP
CJAlertFunction::
exec(CJavaScript *, const Values &values)
{
  int i = 0;

  for (auto &v : values) {
    if (i > 0) std::cout << " ";

    std::cerr << *v;

    ++i;
  }

  std::cerr << std::endl;

  return CJValueP();
}
