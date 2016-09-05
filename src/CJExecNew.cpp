#include <CJExecNew.h>
#include <CJExecExpressionList.h>
#include <CJavaScript.h>

CJExecNew::
CJExecNew() :
 CJToken(CJToken::Type::New)
{
}

CJValueP
CJExecNew::
exec(CJavaScript *js)
{
  CJValueP value;

  CJExecExpressionList::Values values;

  if (exprList_)
    values = exprList_->getValues(js);

  // TODO: use named functions
  if      (typeName_ == "String") {
    CJStringP str(new CJString(js));

    CJValueP value1 = exprList_->exec(js);

    if (value1)
      str->setText(value1->toString());

    value = std::static_pointer_cast<CJValue>(str);
  }
  else if (typeName_ == "Number") {
    CJNumberP num(new CJNumber(js));

    CJValueP value1 = exprList_->exec(js);

    if (value1)
      num->setReal(value1->toReal());

    value = std::static_pointer_cast<CJValue>(num);
  }
  else if (typeName_ == "Boolean") {
    CJValueP value1 = exprList_->exec(js);

    bool b = (value1 && value1->toBoolean());

    value = js->createBoolValue(b);
  }
  else if (typeName_ == "Array") {
    CJArrayP array;

    if (values.size() == 1 && values[0] && values[0]->type() == CJValue::Type::Number) {
      long n = values[0]->toInteger();

      array = CJArrayP(new CJArray(js, n));
    }
    else
      array = CJArrayP(new CJArray(js, values));

    value = std::static_pointer_cast<CJValue>(array);
  }
  else if (typeName_ == "Date") {
    value = js->createDateValue(values);
  }
  else {
    CJDictionaryP scope = js->currentScope();

    CJValueP typeValue = scope->getProperty(js, typeName_);

    while (! typeValue && scope->getParent()) {
      scope = scope->getParent();

      typeValue = scope->getProperty(js, typeName_);
    }

    if (typeValue && typeValue->type() == CJToken::Type::Function) {
      CJFunctionP fn = std::static_pointer_cast<CJFunction>(typeValue);

      CJObjTypeP userType = js->getObjectType(typeName_);

      if (! userType)
        userType = js->addObjectType(typeName_, CJObjTypeP(new CJUserType(js, typeName_)));

      CJUserObjectP userObj(new CJUserObject(js, userType, fn));

      CJValueP objValue = std::static_pointer_cast<CJValue>(userObj);

      CJObjType::Values fnValues;

      fnValues.push_back(objValue);

      for (auto &v : values)
        fnValues.push_back(v);

      js->pushThis(fn);

      CJValueP fnValue = fn->exec(js, fnValues);

      js->popThis();

      //value = objValue;
      value = fnValue;
    }
    else {
      CJObjTypeP userType = js->getObjectType(typeName_);

      if (! userType) {
        js->errorMsg("Invalid type name '" + typeName_ + "'");
        return CJValueP();
      }

      if (! userType->hasConstructor()) {
        js->errorMsg("No constructor for type name '" + typeName_ + "'");
        return CJValueP();
      }

      CJObjType::Values fnValues;

      for (auto &v : values)
        fnValues.push_back(v);

      value = userType->construct(js, fnValues);
    }
  }

  return value;
}

void
CJExecNew::
print(std::ostream &os) const
{
  os << "new " << typeName_;

  if (exprList_) {
    os << "(" << *exprList_ << ")";
  }
}
