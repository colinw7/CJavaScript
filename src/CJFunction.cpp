#include <CJFunction.h>
#include <CJavaScript.h>

CJObjectTypeP CJFunctionType::type_;

CJObjectTypeP
CJFunctionType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CJFunctionType);

    js->addObjectType("function", type_);
  }

  return type_;
}

CJFunctionType::
CJFunctionType() :
 CJObjectType(CJToken::Type::Function, "function")
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
