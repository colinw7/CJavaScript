#include <CQJComboBox.h>
#include <CQJavaScript.h>
#include <CQJWindow.h>
#include <CQJEvent.h>
#include <CQJHtmlObj.h>
#include <CQJFormInputIFace.h>

CQJComboBox::
CQJComboBox(CQJHtmlObj *obj, CQJFormInputIFace *iface) :
 QComboBox(nullptr), obj_(obj), iface_(iface)
{
  setObjectName(iface_->getName().c_str());

  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexSlot(int)));
}

void
CQJComboBox::
currentIndexSlot(int)
{
  CJValueP edit = obj_->shared_from_this();

  CJValueP event(new CQJEvent(CQJavaScriptInst->js(), edit, 0));

  callEventListener("change", "onchange", event);
}

void
CQJComboBox::
callEventListener(const std::string &name, const std::string &prop, CJValueP event)
{
  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  args.push_back(event);

  if (obj_->callEventListener(name, prop, args, nameValues))
    return;
}
