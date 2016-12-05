#include <CQJEvent.h>
#include <CJavaScript.h>
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
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJEventType(js));

    js->addObjectType("Event", type_);
  }

  return type_;
}

CQJEventType::
CQJEventType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Event")
{
}

CJValueP
CQJEventType::
construct(CJavaScript *js, const Values &)
{
  return CJValueP(new CQJEvent(js, CJValueP(), 0));
}

//------

CQJEvent::
CQJEvent(CJavaScript *js, CJValueP value, QEvent *e) :
 CQJObject(js, CQJEventType::instance(js)), value_(value), e_(e)
{
  addPseudoProperty("key");
  addPseudoProperty("keyCode");
}

CJValueP
CQJEvent::
getProperty(CJavaScript *js, const std::string &name) const
{
  const QKeyEvent   *ke = dynamic_cast<const QKeyEvent *>(e_);
  const QMouseEvent *me = dynamic_cast<const QMouseEvent *>(e_);

  if      (ke) {
    if       (name == "key") {
      QString text;

      if (ke)
        text = ke->text();

      return js->createStringValue(text.toStdString());
    }
    else if  (name == "keyCode") {
      const QKeyEvent *ke = dynamic_cast<const QKeyEvent *>(e_);

      long keyCode = 0;

      if (ke)
        keyCode = convertKeyCode(ke->key());

      return js->createNumberValue(keyCode);
    }
  }
  else if (me) {
    if      (name == "clientX") {
      return js->createNumberValue(long(me->x()));
    }
    else if (name == "clientY") {
      return js->createNumberValue(long(me->y()));
    }
  }

  if (name == "target")
    return value_;

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
