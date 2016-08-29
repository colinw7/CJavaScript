#include <CJTypeObjectFunction.h>

CJTypeObjectFunction::
CJTypeObjectFunction(CJavaScript *js, CJObjTypeP objType, const std::string &name) :
 CJFunction(js, name, Type::Normal), objType_(std::move(objType)), name_(name)
{
}

CJValueP
CJTypeObjectFunction::
exec(CJavaScript *js, const Values &values)
{
  return objType_->exec(js, name_, values);
}
