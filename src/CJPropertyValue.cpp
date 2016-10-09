#include <CJPropertyValue.h>
#include <CJValue.h>
#include <CJGetterSetter.h>
#include <CJavaScript.h>

CJValueP
CJPropertyValue::
calcValue(CJavaScript *js) const
{
  CJValueP propVal = value;

  if (propVal && propVal->type() == CJToken::Type::GetterSetter) {
    CJGetterSetterP gs = CJValue::cast<CJGetterSetter>(propVal);

    CJObjectP obj = CJValue::cast<CJObject>(gs->parent());

    js->pushThis(obj);

    propVal = gs->getValue();

    js->popThis();
  }

  return propVal;
}
