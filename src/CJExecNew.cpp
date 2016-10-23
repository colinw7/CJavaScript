#include <CJExecNew.h>
#include <CJExecExpressionList.h>
#include <CJObjTypeFunction.h>
#include <CJFunction.h>
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
  // get call arguments
  CJExecExpressionList::Values values;

  if (exprList_)
    values = exprList_->getValues(js);

  //---

  // get type value for type name
  CJDictionaryP scope = js->currentScope();

  CJValueP typeValue = scope->getProperty(js, typeName_);

  while (! typeValue && scope->getParent()) {
    scope = scope->getParent();

    typeValue = scope->getProperty(js, typeName_);
  }

  //---

  if (! typeValue) {
    js->errorMsg("No type name '" + typeName_ + "'");
    return CJValueP();
  }

  CJFunctionBaseP fn, typeFn;
  CJDictionaryP   protoDict;
  CJObjectP       protoObj;

  if (typeValue->isFunction()) {
    typeFn = CJValue::cast<CJFunctionBase>(typeValue);

    if (typeFn->type() == CJFunctionBase::Type::ObjType) {
      CJValueP protoValue = typeFn->getProperty(js, "prototype");

      if (! protoValue) {
        js->errorMsg("No prototype for type name '" + typeName_ + "'");
        return CJValueP();
      }

      if      (protoValue->type() == CJToken::Type::Object) {
        protoObj = CJValue::cast<CJObject>(protoValue);

        CJValueP constructor = protoObj->getProperty(js, "constructor");

        if (constructor && constructor->isFunction())
          fn = CJValue::cast<CJFunctionBase>(constructor);
      }
      else if (protoValue->type() == CJToken::Type::Dictionary) {
        protoDict = CJValue::cast<CJDictionary>(protoValue);

        CJValueP constructor = protoDict->getProperty(js, "constructor");

        if (constructor && constructor->isFunction())
          fn = CJValue::cast<CJFunctionBase>(constructor);
      }
    }
    else {
      CJValueP protoValue = typeFn->getProperty(js, "prototype");

      if (! protoValue) {
        js->errorMsg("No prototype for type name '" + typeName_ + "'");
        return CJValueP();
      }

      if      (protoValue->type() == CJToken::Type::Object) {
        protoObj = CJValue::cast<CJObject>(protoValue);
      }
      else if (protoValue->type() == CJToken::Type::Dictionary) {
        protoDict = CJValue::cast<CJDictionary>(protoValue);
      }
    }
  }

  if (! fn)
    fn = typeFn;

  if (! fn) {
    js->errorMsg("No constructor for type name '" + typeName_ + "'");
    return CJValueP();
  }

  //---

  CJObjTypeP userType = js->getObjectType(typeName_);

  if (! userType)
    userType = js->addObjectType(typeName_, CJObjTypeP(new CJUserType(js, typeName_)));

  //---

  CJValueP newValue;

  if (typeFn && typeFn->type() == CJFunctionBase::Type::ObjType) {
    CJObjTypeFunctionP objTypeFn = CJValue::cast<CJObjTypeFunction>(typeFn);

    CJFunction::Values fnValues;

    //CJUserObjectP userObj(new CJUserObject(js, userType, typeFn));

    fnValues.push_back(0);

    for (auto &v : values)
      fnValues.push_back(v);

    newValue = objTypeFn->execNew(js, fnValues);

    return newValue;
  }
  else {
    CJObjectP obj = js->createObject();

    obj->setTypeName(typeName_);

    if      (protoDict) {
      for (const auto &name : protoDict->propertyNames()) {
        if (! protoDict->isEnumerableProperty(name))
          continue;

        CJValueP value = protoDict->getProperty(js, name);

        obj->setProperty(js, name, value);
      }

      obj->setProtoValue(protoDict);
    }
    else if (protoObj) {
      for (const auto &name : protoObj->propertyNames()) {
        if (! protoObj->isEnumerableProperty(name))
          continue;

        CJValueP value = protoObj->getProperty(js, name);

        obj->setProperty(js, name, value);
      }

      obj->setProtoValue(protoObj);
    }

    newValue = obj;
  }

  assert(newValue->isDictionary());

  CJDictionaryP newObj = CJValue::cast<CJDictionary>(newValue);

  //---

  // set user object (argv[0])
  //CJUserObjectP userObj(new CJUserObject(js, userType, fn));

  //CJValueP objValue = userObj;

  //---

  // set function values
  CJObjType::Values fnValues;

  //fnValues.push_back(objValue);
  fnValues.push_back(newValue);

  for (auto &v : values)
    fnValues.push_back(v);

  // call function
  js->pushThis(newObj);

  CJValueP fnValue = fn->exec(js, fnValues);

  js->popThis();

  if (js->isUndefinedValue(fnValue))
    fnValue = newObj;

  return fnValue;
}

std::string
CJExecNew::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
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
