#include <CQJavaScript.h>
#include <CQJSCanvas.h>
#include <CQJWindow.h>
#include <CQJDocument.h>
#include <CQJImage.h>
#include <CQJCanvas.h>
#include <CQJCanvasContext2D.h>
#include <CJavaScript.h>
#include <CQApp.h>
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

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  CQJavaScript *qjs = new CQJavaScript;

  std::string filename;

  bool pdebug = false;
  bool idebug = false;
  bool edebug = false;
  bool xdebug = false;
  int  size   = -1;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if      (strcmp(&argv[i][1], "pdebug") == 0)
        pdebug = true;
      else if (strcmp(&argv[i][1], "idebug") == 0)
        idebug = true;
      else if (strcmp(&argv[i][1], "edebug") == 0)
        edebug = true;
      else if (strcmp(&argv[i][1], "xdebug") == 0)
        xdebug = true;
      else if (strcmp(&argv[i][1], "debug") == 0) {
        pdebug = true;
        idebug = true;
        edebug = true;
        xdebug = true;
      }
      else if (strcmp(&argv[i][1], "size") == 0) {
        ++i;

        if (i < argc)
          size = std::max(1, atoi(argv[i]));
      }
      else
        std::cerr << "Invalid option: " << argv[i] << std::endl;
    }
    else
      filename = argv[i];
  }

  if (size > 0)
    qjs->setSize(size);

  qjs->init();

  if (size > 0)
    qjs->setFixedSize(QSize(size, size));

  qjs->show();

  CJavaScript *js = qjs->js();

  js->setParseDebug (pdebug);
  js->setInterpDebug(idebug);
  js->setExecDebug  (edebug);
  js->setExprDebug  (xdebug);

  if (filename != "") {
    js->loadFile(filename);

    js->exec();
  }

  qjs->callEventListeners("load", "onload");

  app.exec();

  return 0;
}

CQJavaScript::
CQJavaScript()
{
}

void
CQJavaScript::
init()
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  canvas_ = new CQJSCanvas(this, size_);

  layout->addWidget(canvas_);

  js_ = new CJavaScript;

  //---

  js_->addObjectType("Image" , CQJImageType ::instance(this));
  js_->addObjectType("Canvas", CQJCanvasType::instance(js_));

  jsWindow_          = CJValueP(new CQJWindow         (this));
  jsDocument_        = CJValueP(new CQJDocument       (this));
  jsCanvas_          = CJValueP(new CQJCanvas         (this));
  jsCanvasContext2D_ = CJValueP(new CQJCanvasContext2D(this));

  js_->setProperty("window"  , jsWindow_);
  js_->setProperty("document", jsDocument_);

  js_->setProperty("setInterval"  , CJValueP(new CQJSetInterval  (this)));
  js_->setProperty("clearInterval", CJValueP(new CQJClearInterval(this)));

  js_->setProperty("requestAnimationFrame", CJValueP(new CQJRequestAnimationFrame(this)));

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
    dialog_ = new CQJDialog(this);

  dialog_->show();
}

QSize
CQJavaScript::
sizeHint() const
{
  return QSize(size_, size_);
}
