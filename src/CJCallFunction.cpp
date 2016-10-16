#include <CJCallFunction.h>
#include <CJBindFunction.h>
#include <CJObjTypeFunction.h>
#include <CJObj.h>
#include <CJavaScript.h>

CJCallFunction::
CJCallFunction(CJavaScript *js, CJFunctionBaseP function, Type type, CJObjTypeP objType) :
 CJFunctionBase(js, "call", CJFunctionBase::Type::Call),
 function_(function), type_(type), objType_(objType)
{
}

CJValueP
CJCallFunction::
getProperty(CJavaScript *js, const std::string &name) const
{
  return CJFunctionBase::getProperty(js, name);
}

void
CJCallFunction::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJFunctionBase::setProperty(js, name, value);
}

CJValueP
CJCallFunction::
exec(CJavaScript *js, const Values &values)
{
  if (values.size() < 1) {
    js->throwTypeError(this, "No this value for call/apply");
    return CJValueP();
  }

  CJValueP thisValue = values[0];

  //---

  // convert value to function (this) object type if possible
  // (ignore if function has no object type)
  if (objType_) {
    CJObjP obj;

    if      (thisValue->isObject())
      obj = CJValue::cast<CJObj>(thisValue);
    else if (thisValue->isPrimitive())
      obj = CJValue::cast<CJObj>(js->valueToObject(thisValue));

    //---

    // get object value type
    CJObjTypeP valType;

    if (obj)
      valType = obj->objType();

    if (! valType) {
      js->throwTypeError(thisValue, "No type for first argument to call/apply");
      return CJValueP();
    }

    //---

    // if value type doesn't match object type then create a dummy value
    if (valType != objType_) {
      // create dummy value for type
      CJObjTypeFunctionP typeFn = objType_->typeFunction();

      Values values1;

      values1.push_back(typeFn);
      values1.push_back(obj);

      CJValueP typeVal = typeFn->exec(js, values1);

      //---

      // set new this (first value)
      thisValue = typeVal;
    }
  }

  //---

  // set call values (expand second argument for apply)
  Values callValues;

  if      (type_ == Type::Call) {
    callValues = values;

    callValues[0] = thisValue;
  }
  else if (type_ == Type::Apply) {
    callValues.push_back(thisValue);

    CJArrayP array;

    if (values.size() > 1) {
      if (values[1]->type() != CJToken::Type::Array)
        array = CJArrayType::createArrayFromValue(js, values[1]);
      else
        array = CJValue::cast<CJArray>(values[1]);
    }

    for (int i = 0; i < array->length(); ++i)
      callValues.push_back(array->indexValue(i));
  }
  else if (type_ == Type::Bind) {
    callValues = values;

    callValues[0] = thisValue;
  }

  //---

  CJValueP res;

  if (type_ == Type::Call || type_ == Type::Apply) {
    // run function
    // TODO: set this ?
    res = function_->exec(js, callValues);
  }
  else {
    res = CJValueP(new CJBindFunction(js, function_, thisValue, callValues));
  }

  return res;
}

std::string
CJCallFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJCallFunction::
print(std::ostream &os) const
{
  os << "call " << function_->name();
}
