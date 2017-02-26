#include <CQJavaScript.h>
#include <CQJWindow.h>
#include <CQJDocument.h>
#include <CQJCanvas.h>
#include <CQJImage.h>
#include <CQJIntervalFunction.h>
#include <CQJRequestAnimationFrame.h>
#include <CJavaScript.h>
#include <CCSS.h>
#include <CQJHtmlObjCSSTagData.h>

CQJavaScript *
CQJavaScript::
instance()
{
  static CQJavaScript *inst;

  if (! inst)
    inst = new CQJavaScript;

  return inst;
}

CQJavaScript::
CQJavaScript()
{
  js_ = new CJavaScript;

  //js_->setParseDebug(true);
  //js_->setExecDebug(true);
}

CQJWindowP
CQJavaScript::
jsWindow() const
{
  return CJValue::cast<CQJWindow>(jsWindow_);
}

CQJDocumentP
CQJavaScript::
jsDocument() const
{
  return CJValue::cast<CQJDocument>(jsDocument_);
}

void
CQJavaScript::
setWindowIFace(CQJWindowIFace *windowIFace)
{
  windowIFace_ = windowIFace;
}

void
CQJavaScript::
init()
{
  js_->defineObjectT<CQJImageFunction, CQJImage>(js_, "Image");

  js_->addObjectType("Canvas", CQJCanvasType::instance(js_));

  //---

  jsWindow_ = CJValueP(new CQJWindow(js_, windowIFace_));

  CQJWindowP window = jsWindow();

  window->init();

  js_->setProperty("window", jsWindow_);

  //---

  jsDocument_ = CJValueP(new CQJDocument(js_, window));

  js_->setProperty("document", jsDocument_);

  //---

  CJValueP setInterval   = CJValueP(new CQJSetInterval  (js_, window));
  CJValueP clearInterval = CJValueP(new CQJClearInterval(js_, window));

  js_->setProperty("setInterval"  , setInterval);
  js_->setProperty("clearInterval", clearInterval);

  CJValueP requestAnimationFrame = CJValueP(new CQJRequestAnimationFrame(js_, window));

  js_->setProperty("requestAnimationFrame", requestAnimationFrame);

  js_->setProperty("location", window->location());
}

void
CQJavaScript::
addObject(CQJObject *obj)
{
  objects_.insert(obj);
}

void
CQJavaScript::
removeObject(CQJObject *obj)
{
  objects_.erase(obj);
}

void
CQJavaScript::
addHtmlObject(CQJHtmlObj *obj)
{
  if (obj->getId() != "")
    idObjMap_[obj->getId()] = obj;
}

CQJHtmlObj *
CQJavaScript::
getObjectById(const std::string &id) const
{
  auto p = idObjMap_.find(id);

  if (p == idObjMap_.end())
    return nullptr;

  return (*p).second;
}

void
CQJavaScript::
getObjectsByClassName(const std::string &name, std::vector<CQJHtmlObj *> &objs) const
{
  for (const auto &obj : objects()) {
    CQJHtmlObj *obj1 = dynamic_cast<CQJHtmlObj *>(obj);
    if (! obj1) continue;

    if (obj1->getClass() == name)
      objs.push_back(obj1);
  }
}

void
CQJavaScript::
getObjectsByTypeName(const std::string &name, std::vector<CQJHtmlObj *> &objs) const
{
  for (const auto &obj : objects()) {
    CQJHtmlObj *obj1 = dynamic_cast<CQJHtmlObj *>(obj);
    if (! obj1) continue;

    if (obj1->getType() == name)
      objs.push_back(obj1);
  }
}

void
CQJavaScript::
getObjectsBySelector(const std::string &id, std::vector<CQJHtmlObj *> &objs) const
{
  CCSS css;

  std::vector<CCSS::StyleData> styles;

  if (! css.parseSelector(id, styles))
    return;

  for (const auto &obj : objects()) {
    CQJHtmlObj *obj1 = dynamic_cast<CQJHtmlObj *>(obj);
    if (! obj1) continue;

    CCSSTagDataP tagData(new CQJHtmlObjCSSTagData(obj1));

    bool match = false;

    for (const auto &style : styles) {
      if (! style.checkMatch(tagData))
        continue;

      match = true;

      break;
    }

    if (match)
      objs.push_back(obj1);
  }
}

void
CQJavaScript::
runScript(const std::string &text)
{
  js_->loadString(text);

  CJValueP value = js_->exec();

  if (value) {
    value->print(std::cout);
    std::cout << std::endl;
  }
}

void
CQJavaScript::
runScriptFile(const std::string &filename)
{
  js_->loadFile(filename);

  CJValueP value = js_->exec();

  if (value) {
    value->print(std::cout);
    std::cout << std::endl;
  }
}

void
CQJavaScript::
onLoad()
{
  callEventListeners("load", "onload");
}

void
CQJavaScript::
callEventListeners(const std::string &name, const std::string &prop, const EventArgs &args)
{
  for (const auto &o : objects_)
    o->callEventListener(name, prop, args);
}
