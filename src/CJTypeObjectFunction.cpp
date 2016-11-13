#include <CJTypeObjectFunction.h>

CJTypeObjectFunction::
CJTypeObjectFunction(CJavaScript *js, CJObjTypeP objType, const std::string &name) :
 CJFunctionBase(js, name, Type::Normal), objType_(objType), name_(name)
{
}

CJTypeObjectFunction::
CJTypeObjectFunction(const CJTypeObjectFunction &fn) :
 CJFunctionBase(fn), objType_(fn.objType_), name_(fn.name_), hasObjValue_(fn.hasObjValue_)
{
}

CJValueP
CJTypeObjectFunction::
exec(CJavaScript *js, const Values &values)
{
  return objType_->exec(js, name_, values);
}
