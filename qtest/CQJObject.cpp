#include <CQJObject.h>
#include <CQJavaScript.h>

CQJObject::
CQJObject(CQJavaScript *qjs, const CJObjectTypeP &type) :
 CJObject(type), js_(qjs)
{
  CJavaScript *js = js_->js();

  type_->addFunction(js, "addEventListener");

  js_->addObject(this);
}

CQJObject::
~CQJObject()
{
  js_->removeObject(this);
}

CJValueP
CQJObject::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if      (name == "addEventListener") {
    if (values.size() >= 2) {
      std::string id = values[1]->toString();
      CJValueP    fn = values[2];

      eventListeners_[id] = fn;
    }

    return CJValueP();
  }
  else
    return CJValueP();
}

void
CQJObject::
callEventListener(const std::string &name)
{
  auto p = eventListeners_.find(name);

  if (p == eventListeners_.end())
    return;

  CJValueP value = (*p).second;

  if (value->type() == CJToken::Type::Function) {
    CJFunctionP fn = std::static_pointer_cast<CJFunction>(value);

    CJavaScript *js = js_->js();

    CJObjectType::Values fnValues;

    fnValues.push_back(shared_from_this());

    fn->exec(js, fnValues);
  }
}
