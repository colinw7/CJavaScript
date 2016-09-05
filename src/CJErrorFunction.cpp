#include <CJErrorFunction.h>
#include <CJavaScript.h>
#include <CJError.h>

class CJErrorToStringFunction : public CJFunction {
 public:
  CJErrorToStringFunction(CJErrorFunctionBase *base) :
   CJFunction(base->js(), "toString"), base_(base) {
  }

  CJValue *dup(CJavaScript *) const override { return new CJErrorToStringFunction(base_); }

  CJValueP exec(CJavaScript *js, const Values &) override {
    return js->createStringValue(base_->name());
  }

 private:
  CJErrorFunctionBase *base_ { 0 };
};

//------

CJErrorFunctionBase::
CJErrorFunctionBase(CJavaScript *js, const std::string &name, CJObjTypeP type) :
 CJTypeFunction(js, name, type)
{
  prototype()->setStringProperty(js, "name", name);
  prototype()->setStringProperty(js, "message", "");

  prototype()->setProperty(js, "toString", CJValueP(new CJErrorToStringFunction(this)));
}

//------

CJErrorFunction::
CJErrorFunction(CJavaScript *js) :
 CJErrorFunctionBase(js, "Error", CJErrorType::instance(js))
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

CJValueP
CJURIErrorFunction::
exec(CJavaScript *js, const Values &values)
{
  CJURIError *error = new CJURIError(js);

  if (values.size() > 1)
    error->setMessage(values[1]->toString());

  return CJValueP(error);
}
