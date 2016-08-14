#include <CQJObject.h>
#include <CQJavaScript.h>

CQJObject::
CQJObject(CQJavaScript *qjs, const CJObjTypeP &type) :
 CJObj(type), js_(qjs)
{
  CJavaScript *js = js_->js();

  type_->addObjectFunction(js, "addEventListener");

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
callEventListener(const std::string &name, const EventArgs &args, const NameValues &nameValues)
{
  auto p = eventListeners_.find(name);

  if (p == eventListeners_.end())
    return;

  CJValueP value = (*p).second;

  if (value->type() == CJToken::Type::Function) {
    CJFunctionP fn = std::static_pointer_cast<CJFunction>(value);

    if (fn->type() == CJFunction::Type::User) {
      CJUserFunctionP userFn = std::static_pointer_cast<CJUserFunction>(value);

      for (auto &nv : nameValues)
        userFn->setProperty(nv.first, nv.second);
    }

    CJavaScript *js = js_->js();

    CJObjType::Values fnValues;

    fnValues.push_back(shared_from_this());

    for (auto &arg : args)
      fnValues.push_back(arg);

    fn->exec(js, fnValues);
  }
}
