#include <CQJLineEdit.h>
#include <CQJavaScript.h>
#include <CQJWindow.h>
#include <CQJEvent.h>
#include <CQJHtmlObj.h>
#include <CQJFormInputIFace.h>

CQJLineEdit::
CQJLineEdit(CQJHtmlObj *obj, CQJFormInputIFace *iface) :
 QLineEdit(nullptr), obj_(obj), iface_(iface)
{
  setObjectName(iface_->getName().c_str());

  connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(changeSlot()));
}

void
CQJLineEdit::
changeSlot()
{
  CJValueP edit = obj_->shared_from_this();

  CJValueP event(new CQJEvent(CQJavaScriptInst->js(), edit, 0));

  callEventListener("change", "onchange", event);
}

void
CQJLineEdit::
keyPressEvent(QKeyEvent *e)
{
  QLineEdit::keyPressEvent(e);

  CJValueP edit = obj_->shared_from_this();

  CJValueP event(new CQJEvent(CQJavaScriptInst->js(), edit, e));

  callEventListener("keydown", "onkeydown", event);
}

void
CQJLineEdit::
keyReleaseEvent(QKeyEvent *e)
{
  QLineEdit::keyReleaseEvent(e);

  CJValueP edit = obj_->shared_from_this();

  CJValueP event(new CQJEvent(CQJavaScriptInst->js(), edit, e));

  callEventListener("keyup", "onkeyup", event);
}

void
CQJLineEdit::
callEventListener(const std::string &name, const std::string &prop, CJValueP event)
{
  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  args.push_back(event);

  if (obj_->callEventListener(name, prop, args, nameValues))
    return;
}
