#include <CJFunction.h>
#include <CJavaScript.h>

CJObjTypeP CJFunctionType::type_;

CJObjTypeP
CJFunctionType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CJFunctionType(js));

    js->addObjectType("function", type_);
  }

  return type_;
}

CJFunctionType::
CJFunctionType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Function, "function")
{
}

CJValueP
CJFunctionType::
exec(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

//------

CJFunction::
CJFunction(CJavaScript *js, const std::string &name, Type type) :
 CJValue(CJFunctionType::instance(js)), name_(name), type_(type)
{
}
