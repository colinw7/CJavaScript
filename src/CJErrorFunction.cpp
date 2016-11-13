#include <CJErrorFunction.h>
#include <CJavaScript.h>
#include <CJError.h>

class CJErrorToStringFunction : public CJFunctionBase {
 public:
  CJErrorToStringFunction(CJErrorFunctionBase *base) :
   CJFunctionBase(base->js(), "toString"), base_(base) {
  }

  CJErrorToStringFunction(const CJErrorToStringFunction &fn) :
   CJFunctionBase(fn), base_(fn.base_) {
  }

  CJValue *dup(CJavaScript *) const override { return new CJErrorToStringFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &) override {
    return js->createStringValue(base_->name());
  }

 private:
  CJErrorFunctionBase *base_ { 0 };
};

//------

CJErrorFunctionBase::
CJErrorFunctionBase(CJavaScript *js, const std::string &name, CJObjTypeP type) :
 CJObjTypeFunction(js, name, type)
{
  prototype()->setStringProperty(js, "name", name);
  prototype()->setStringProperty(js, "message", "");

  prototype()->setProperty(js, "toString", CJValueP(new CJErrorToStringFunction(this)));
}

CJValueP
CJErrorFunctionBase::
execNew(CJavaScript *js, const Values &values)
{
  return exec(js, values);
}

std::string
CJErrorFunctionBase::
toString() const
{
  return "[Function: " + name_ + "]";
}

void
CJErrorFunctionBase::
print(std::ostream &os) const
{
  os << "[Function: " << name_ << "]";
}

//------

CJErrorFunction::
CJErrorFunction(CJavaScript *js) :
 CJErrorFunctionBase(js, "Error", CJErrorType::instance(js))
{
}

CJErrorFunction::
CJErrorFunction(const CJErrorFunction &fn) :
 CJErrorFunctionBase(fn)
{
}

CJValueP
CJErrorFunction::
exec(CJavaScript *js, const Values &values)
{
  CJError *error = new CJError(js);

  if (values.size() > 1)
    error->setMessage(values[1]->toString());

  return CJValueP(error);
}

//------

CJTypeErrorFunction::
CJTypeErrorFunction(CJavaScript *js) :
 CJErrorFunctionBase(js, "TypeError", CJTypeErrorType::instance(js))
{
}

CJTypeErrorFunction::
CJTypeErrorFunction(const CJTypeErrorFunction &fn) :
 CJErrorFunctionBase(fn)
{
}

CJValueP
CJTypeErrorFunction::
exec(CJavaScript *js, const Values &values)
{
  CJTypeError *error = new CJTypeError(js);

  if (values.size() > 1)
    error->setMessage(values[1]->toString());

  return CJValueP(error);
}

//------

CJReferenceErrorFunction::
CJReferenceErrorFunction(CJavaScript *js) :
 CJErrorFunctionBase(js, "ReferenceError", CJReferenceErrorType::instance(js))
{
}

CJReferenceErrorFunction::
CJReferenceErrorFunction(const CJReferenceErrorFunction &fn) :
 CJErrorFunctionBase(fn)
{
}

CJValueP
CJReferenceErrorFunction::
exec(CJavaScript *js, const Values &values)
{
  CJReferenceError *error = new CJReferenceError(js);

  if (values.size() > 1)
    error->setMessage(values[1]->toString());

  return CJValueP(error);
}

//------

CJEvalErrorFunction::
CJEvalErrorFunction(CJavaScript *js) :
 CJErrorFunctionBase(js, "EvalError", CJEvalErrorType::instance(js))
{
}

CJEvalErrorFunction::
CJEvalErrorFunction(const CJEvalErrorFunction &fn) :
 CJErrorFunctionBase(fn)
{
}

CJValueP
CJEvalErrorFunction::
exec(CJavaScript *js, const Values &values)
{
  CJEvalError *error = new CJEvalError(js);

  if (values.size() > 1)
    error->setMessage(values[1]->toString());

  return CJValueP(error);
}

//------

CJRangeErrorFunction::
CJRangeErrorFunction(CJavaScript *js) :
 CJErrorFunctionBase(js, "RangeError", CJRangeErrorType::instance(js))
{
}

CJRangeErrorFunction::
CJRangeErrorFunction(const CJRangeErrorFunction &fn) :
 CJErrorFunctionBase(fn)
{
}

CJValueP
CJRangeErrorFunction::
exec(CJavaScript *js, const Values &values)
{
  CJRangeError *error = new CJRangeError(js);

  if (values.size() > 1)
    error->setMessage(values[1]->toString());

  return CJValueP(error);
}

//------

CJSyntaxErrorFunction::
CJSyntaxErrorFunction(CJavaScript *js) :
 CJErrorFunctionBase(js, "SyntaxError", CJSyntaxErrorType::instance(js))
{
}

CJSyntaxErrorFunction::
CJSyntaxErrorFunction(const CJSyntaxErrorFunction &fn) :
 CJErrorFunctionBase(fn)
{
}

CJValueP
CJSyntaxErrorFunction::
exec(CJavaScript *js, const Values &values)
{
  CJSyntaxError *error = new CJSyntaxError(js);

  if (values.size() > 1)
    error->setMessage(values[1]->toString());

  return CJValueP(error);
}

//------

CJURIErrorFunction::
CJURIErrorFunction(CJavaScript *js) :
 CJErrorFunctionBase(js, "URIError", CJURIErrorType::instance(js))
{
}

CJURIErrorFunction::
CJURIErrorFunction(const CJURIErrorFunction &fn) :
 CJErrorFunctionBase(fn)
{
}

CJValueP
CJURIErrorFunction::
exec(CJavaScript *js, const Values &values)
{
  CJURIError *error = new CJURIError(js);

  if (values.size() > 1)
    error->setMessage(values[1]->toString());

  return CJValueP(error);
}
