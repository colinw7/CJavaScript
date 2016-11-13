#include <CQJEvent.h>
#include <CQJavaScript.h>
#include <iostream>

namespace {
  long convertKeyCode(int key) {
    if (key <= 0xFF) return key;

    switch (key) {
      case Qt::Key_Left : return 37;
      case Qt::Key_Up   : return 38;
      case Qt::Key_Right: return 39;
      case Qt::Key_Down : return 40;

      default: std::cerr << "Unsupported key: " << key << std::endl; break;
    }

    return 0;
  }
}

//------

CJObjTypeP CQJEventType::type_;

CJObjTypeP
CQJEventType::
instance(CQJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJEventType(js));

    js->js()->addObjectType("Event", type_);
  }

  return type_;
}

CQJEventType::
CQJEventType(CQJavaScript *qjs) :
 CJObjType(qjs->js(), CJToken::Type::Object, "Event"), qjs_(qjs)
{
}

CJValueP
CQJEventType::
construct(CJavaScript *, const Values &)
{
  return CJValueP(new CQJEvent(qjs_, 0));
}

//------

CQJEvent::
CQJEvent(CQJavaScript *qjs, QEvent *e) :
 CQJObject(qjs, CQJEventType::instance(qjs)), e_(e)
{
  addPseudoProperty("key");
  addPseudoProperty("keyCode");
}

CJValueP
CQJEvent::
getProperty(CJavaScript *js, const std::string &name) const
{
  if       (name == "key") {
    const QKeyEvent *ke = dynamic_cast<const QKeyEvent *>(e_);

    QString text;

    if (ke)
      text = ke->text();

    return qjs_->js()->createStringValue(text.toStdString());
  }
  else if  (name == "keyCode") {
    const QKeyEvent *ke = dynamic_cast<const QKeyEvent *>(e_);

    long keyCode = 0;

    if (ke)
      keyCode = convertKeyCode(ke->key());

    return qjs_->js()->createNumberValue(keyCode);
  }
  else
    return CJObj::getProperty(js, name);
}

void
CQJEvent::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJObj::setProperty(js, name, value);
}

CJValueP
CQJEvent::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  CJValueP obj = CQJObject::execNameFn(js, name, values);

  return obj;
}
