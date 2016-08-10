#include <CJNumber.h>
#include <CJavaScript.h>

CJObjectTypeP CJNumberType::type_;

CJObjectTypeP
CJNumberType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CJNumberType(js));

    js->addObjectType("number", type_);
  }

  return type_;
}

CJNumberType::
CJNumberType(CJavaScript *js) :
 CJObjectType(CJToken::Type::Number, "number")
{
  addFunction(js, "toFixed");
}

CJValueP
CJNumberType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (name == "toFixed") {
    if (values.size() > 1) {
      double r = values[0]->toReal();
      long   n = std::max(values[1]->toInteger(), 0L);

      char buffer[256];

      sprintf(buffer, "%.*f", int(n), r);

      return CJValueP(js->createStringValue(buffer));
    }
    else
      return CJValueP();
  }
  else
    return CJValueP();
}

//------

CJNumber::
CJNumber(CJavaScript *js, double real) :
 CJValue(CJNumberType::instance(js)), real_(real)
{
}
