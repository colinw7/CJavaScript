#include <CQJavaScript.h>
#include <CQJCanvasWidget.h>
#include <CQJWindow.h>
#include <CQJDocument.h>
#include <CQJImage.h>
#include <CQJCanvas.h>
#include <CQJCanvasContext2D.h>
#include <CJavaScript.h>
#include <CQHistoryLineEdit.h>
#include <CQJDialog.h>
#include <CQJIntervalFunction.h>
#include <CQJRequestAnimationFrame.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QFileDialog>
#include <QPainter>
#include <QDir>
#include <iostream>

#include <svg/javascript_svg.h>

CQJavaScript *CQJavaScript::instance_;

CQJavaScript::
CQJavaScript()
{
  instance_ = this;
}

void
CQJavaScript::
init()
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  canvas_ = new CQJCanvasWidget(this, size_);

  layout->addWidget(canvas_);

  js_ = new CJavaScript;

  //---

  js_->defineObjectT<CQJImageFunction, CQJImage>(js_, "Image");

  js_->addObjectType("Canvas", CQJCanvasType::instance(js_));

  jsWindow_ = CJValueP(new CQJWindow(js_));

  CQJWindowP window = CJValue::cast<CQJWindow>(jsWindow_);

  window->init();

  jsDocument_ = CJValueP(new CQJDocument(js_));
  jsCanvas_   = CJValueP(new CQJCanvas  (js_, canvas_));

  js_->setProperty("window"  , jsWindow_);
  js_->setProperty("document", jsDocument_);

  CJValueP setInterval   = CJValueP(new CQJSetInterval  (js_, window));
  CJValueP clearInterval = CJValueP(new CQJClearInterval(js_, window));

  js_->setProperty("setInterval"  , setInterval);
  js_->setProperty("clearInterval", clearInterval);

  CJValueP requestAnimationFrame = CJValueP(new CQJRequestAnimationFrame(js_, window));

  js_->setProperty("requestAnimationFrame", requestAnimationFrame);

  //---

  QFrame *entryFrame = new QFrame;

  layout->addWidget(entryFrame);

  QHBoxLayout *entryLayout = new QHBoxLayout(entryFrame);
  entryLayout->setMargin(0); entryLayout->setSpacing(2);

  input_ = new CQHistoryLineEdit;

  entryLayout->addWidget(input_);

  connect(input_, SIGNAL(exec(const QString &)), this, SLOT(execCmd(const QString &)));

  QPushButton *load = new QPushButton("Load");

  entryLayout->addWidget(load);

  connect(load, SIGNAL(clicked()), this, SLOT(loadFileSlot()));

  QToolButton *jbutton = new QToolButton;

  jbutton->setIcon(CQPixmapCacheInst->getIcon("JAVASCRIPT"));

  entryLayout->addWidget(jbutton);

  connect(jbutton, SIGNAL(clicked()), this, SLOT(showDialogSlot()));
}

void
CQJavaScript::
execCmd(const QString &cmd)
{
  if (! cmd.length())
    return;

  //---

  js_->loadString(cmd.toStdString());

  CJValueP value = js_->exec();

  if (value) {
    std::string str = value->toString();

    std::cerr << str << std::endl;
  }
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
callEventListeners(const std::string &name, const std::string &prop, const EventArgs &args)
{
  for (const auto &o : objects_)
    o->callEventListener(name, prop, args);
}

void
CQJavaScript::
loadFileSlot()
{
  QString title  = "Load File";
  QString cwd    = QDir::currentPath();
  QString filter = "JavaScript Files (*.js)";

  QString filename = QFileDialog::getOpenFileName(this, title, cwd, filter);

  if (filename == "")
    return;

  js_->loadFile(filename.toStdString());

  js_->exec();
}

void
CQJavaScript::
showDialogSlot()
{
  if (! dialog_)
    dialog_ = new CQJDialog(js_);

  dialog_->show();
}

QSize
CQJavaScript::
sizeHint() const
{
  return QSize(size_, size_);
}
