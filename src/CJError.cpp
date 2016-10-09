#include <CJError.h>
#include <CJavaScript.h>
#include <CJUtil.h>

CJObjectTypeFunctionP CJErrorTypeBase::toStringFn_;

CJErrorTypeBase::
CJErrorTypeBase(CJavaScript *js, CJToken::Type type, const std::string &typeName) :
 CJObjType(js, type, typeName)
{
}

void
CJErrorTypeBase::
init(CJObjTypeP type)
{
  if (! toStringFn_)
    toStringFn_ = addObjectFunction(js_, "toString", type);
  else
    addObjectFunction(toStringFn_);
}

CJValueP
CJErrorTypeBase::
getProperty(CJavaScript *js, const std::string &key) const
{
  return CJObjType::getProperty(js, key);
}

CJValueP
CJErrorTypeBase::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJErrorBaseP error = CJValue::cast<CJErrorBase>(values[0]);
  assert(error);

  //---

  if (name == "toString") {
    std::string str;

    if (error->name() != "")
      str = error->name();

    if (error->message() != "") {
      if (str != "")
        str += ": ";

      str += error->message();
    }

    return js->createStringValue(str);
  }

  return CJObjType::exec(js, name, values);
}

//---

CJErrorBase::
CJErrorBase(CJavaScript *js, CJObjTypeP type, const std::string &name) :
 CJObj(js, type), name_(name)
{
}

CJValueP
CJErrorBase::
getProperty(CJavaScript *js, const std::string &key) const
{
  if      (key == "name")
    return js->createStringValue(name_);
  else if (key == "message")
    return js->createStringValue(message_);
  else if (key == "code")
    return js->createNumberValue(long(exceptionType_));
  else
    return CJObj::getProperty(js, key);
}

void
CJErrorBase::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  if      (key == "name")
    name_ = value->toString();
  else if (key == "message")
    message_ = value->toString();
  else
    CJObj::setProperty(js, key, value);
}

std::string
CJErrorBase::
toString() const
{
  std::string str;

  if (name_ != "")
    str += name_;

  if (message_ != "")
    str += ": " + message_;

  return str;
}

void
CJErrorBase::
print(std::ostream &os) const
{
  os << "[" << name_ << "]";
}

//------

CJErrorTypeP CJErrorType::type_;

CJErrorTypeP
CJErrorType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJErrorTypeP(new CJErrorType(js));

    type_->init(type_);

    js->addObjectType("Error", type_);

    js->addTypeObject(CJToken::Type::Error, type_);
  }

  return type_;
}

CJErrorType::
CJErrorType(CJavaScript *js) :
 CJErrorTypeBase(js, CJToken::Type::Error, "Error")
{
  parentType_ = CJObjectType::instance(js);
}

CJValueP
CJErrorType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  return CJErrorTypeBase::exec(js, name, values);
}

//------

CJError::
CJError(CJavaScript *js) :
 CJErrorBase(js, CJErrorType::instance(js), "Error")
{
}

CJValueP
CJError::
getProperty(CJavaScript *js, const std::string &key) const
{
  return CJErrorBase::getProperty(js, key);
}

void
CJError::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJErrorBase::setProperty(js, key, value);
}

//------

CJTypeErrorTypeP CJTypeErrorType::type_;

CJTypeErrorTypeP
CJTypeErrorType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJTypeErrorTypeP(new CJTypeErrorType(js));

    type_->init(type_);

    js->addObjectType("TypeError", type_);

    js->addTypeObject(CJToken::Type::TypeError, type_);
  }

  return type_;
}

CJTypeErrorType::
CJTypeErrorType(CJavaScript *js) :
 CJErrorTypeBase(js, CJToken::Type::TypeError, "TypeError")
{
  parentType_ = CJErrorType::instance(js);
}

CJValueP
CJTypeErrorType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  return CJErrorTypeBase::exec(js, name, values);
}

//------

CJTypeError::
CJTypeError(CJavaScript *js) :
 CJErrorBase(js, CJTypeErrorType::instance(js), "TypeError")
{
}

CJValueP
CJTypeError::
getProperty(CJavaScript *js, const std::string &key) const
{
  return CJErrorBase::getProperty(js, key);
}

void
CJTypeError::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJErrorBase::setProperty(js, key, value);
}

//------

CJReferenceErrorTypeP CJReferenceErrorType::type_;

CJReferenceErrorTypeP
CJReferenceErrorType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJReferenceErrorTypeP(new CJReferenceErrorType(js));

    type_->init(type_);

    js->addObjectType("ReferenceError", type_);

    js->addTypeObject(CJToken::Type::ReferenceError, type_);
  }

  return type_;
}

CJReferenceErrorType::
CJReferenceErrorType(CJavaScript *js) :
 CJErrorTypeBase(js, CJToken::Type::ReferenceError, "ReferenceError")
{
  parentType_ = CJErrorType::instance(js);
}

CJValueP
CJReferenceErrorType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  return CJErrorTypeBase::exec(js, name, values);
}

//------

CJReferenceError::
CJReferenceError(CJavaScript *js) :
 CJErrorBase(js, CJReferenceErrorType::instance(js), "ReferenceError")
{
}

CJValueP
CJReferenceError::
getProperty(CJavaScript *js, const std::string &key) const
{
  return CJErrorBase::getProperty(js, key);
}

void
CJReferenceError::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJErrorBase::setProperty(js, key, value);
}

//------

CJEvalErrorTypeP CJEvalErrorType::type_;

CJEvalErrorTypeP
CJEvalErrorType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJEvalErrorTypeP(new CJEvalErrorType(js));

    type_->init(type_);

    js->addObjectType("EvalError", type_);

    js->addTypeObject(CJToken::Type::EvalError, type_);
  }

  return type_;
}

CJEvalErrorType::
CJEvalErrorType(CJavaScript *js) :
 CJErrorTypeBase(js, CJToken::Type::EvalError, "EvalError")
{
  parentType_ = CJErrorType::instance(js);
}

CJValueP
CJEvalErrorType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  return CJErrorTypeBase::exec(js, name, values);
}

//------

CJEvalError::
CJEvalError(CJavaScript *js) :
 CJErrorBase(js, CJEvalErrorType::instance(js), "EvalError")
{
}

CJValueP
CJEvalError::
getProperty(CJavaScript *js, const std::string &key) const
{
  return CJErrorBase::getProperty(js, key);
}

void
CJEvalError::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJErrorBase::setProperty(js, key, value);
}

//------

CJRangeErrorTypeP CJRangeErrorType::type_;

CJRangeErrorTypeP
CJRangeErrorType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJRangeErrorTypeP(new CJRangeErrorType(js));

    type_->init(type_);

    js->addObjectType("RangeError", type_);

    js->addTypeObject(CJToken::Type::RangeError, type_);
  }

  return type_;
}

CJRangeErrorType::
CJRangeErrorType(CJavaScript *js) :
 CJErrorTypeBase(js, CJToken::Type::RangeError, "RangeError")
{
  parentType_ = CJErrorType::instance(js);
}

CJValueP
CJRangeErrorType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  return CJErrorTypeBase::exec(js, name, values);
}

//------

CJRangeError::
CJRangeError(CJavaScript *js) :
 CJErrorBase(js, CJRangeErrorType::instance(js), "RangeError")
{
}

CJValueP
CJRangeError::
getProperty(CJavaScript *js, const std::string &key) const
{
  return CJErrorBase::getProperty(js, key);
}

void
CJRangeError::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJErrorBase::setProperty(js, key, value);
}

//------

CJSyntaxErrorTypeP CJSyntaxErrorType::type_;

CJSyntaxErrorTypeP
CJSyntaxErrorType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJSyntaxErrorTypeP(new CJSyntaxErrorType(js));

    type_->init(type_);

    js->addObjectType("SyntaxError", type_);

    js->addTypeObject(CJToken::Type::SyntaxError, type_);
  }

  return type_;
}

CJSyntaxErrorType::
CJSyntaxErrorType(CJavaScript *js) :
 CJErrorTypeBase(js, CJToken::Type::SyntaxError, "SyntaxError")
{
  parentType_ = CJErrorType::instance(js);
}

CJValueP
CJSyntaxErrorType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  return CJErrorTypeBase::exec(js, name, values);
}

//------

CJSyntaxError::
CJSyntaxError(CJavaScript *js) :
 CJErrorBase(js, CJSyntaxErrorType::instance(js), "SyntaxError")
{
}

CJValueP
CJSyntaxError::
getProperty(CJavaScript *js, const std::string &key) const
{
  return CJErrorBase::getProperty(js, key);
}

void
CJSyntaxError::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJErrorBase::setProperty(js, key, value);
}

//------

CJURIErrorTypeP CJURIErrorType::type_;

CJURIErrorTypeP
CJURIErrorType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJURIErrorTypeP(new CJURIErrorType(js));

    type_->init(type_);

    js->addObjectType("URIError", type_);

    js->addTypeObject(CJToken::Type::URIError, type_);
  }

  return type_;
}

CJURIErrorType::
CJURIErrorType(CJavaScript *js) :
 CJErrorTypeBase(js, CJToken::Type::URIError, "URIError")
{
  parentType_ = CJErrorType::instance(js);
}

CJValueP
CJURIErrorType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  return CJErrorTypeBase::exec(js, name, values);
}

//------

CJURIError::
CJURIError(CJavaScript *js) :
 CJErrorBase(js, CJURIErrorType::instance(js), "URIError")
{
}

CJValueP
CJURIError::
getProperty(CJavaScript *js, const std::string &key) const
{
  return CJErrorBase::getProperty(js, key);
}

void
CJURIError::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJErrorBase::setProperty(js, key, value);
}
