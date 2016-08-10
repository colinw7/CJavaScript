#include <CQJavaScript.h>
#include <CQJSCanvas.h>
#include <CQJWindow.h>
#include <CQJDocument.h>
#include <CQJImage.h>
#include <CQJCanvas.h>
#include <CJavaScript.h>
#include <CQApp.h>
#include <CQHistoryLineEdit.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QPainter>
#include <QDir>
#include <iostream>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  CQJavaScript *qjs = new CQJavaScript;

  std::string filename;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-')
      continue;
    else
      filename = argv[i];
  }

  qjs->show();

  if (filename != "") {
    CJavaScript *js = qjs->js();

    js->loadFile(filename);

    js->exec();
  }

  qjs->callEventListeners("load");

  app.exec();

  return 0;
}

CQJavaScript::
CQJavaScript()
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  canvas_ = new CQJSCanvas(this);

  layout->addWidget(canvas_);

  js_ = new CJavaScript;

  //---

  jsWindowType_   = CJObjectTypeP(new CQJWindowType  ());
  jsDocumentType_ = CJObjectTypeP(new CQJDocumentType());
  jsImageType_    = CJObjectTypeP(new CQJImageType   (this));
  jsCanvasType_   = CJObjectTypeP(new CQJCanvasType  ());

  js_->addObjectType("window"  , jsWindowType_);
  js_->addObjectType("document", jsDocumentType_);
  js_->addObjectType("Image"   , jsImageType_);
  js_->addObjectType("Canvas"  , jsCanvasType_);

  jsWindow_          = CJValueP(new CQJWindow         (this));
  jsDocument_        = CJValueP(new CQJDocument       (this));
  jsCanvas_          = CJValueP(new CQJCanvas         (this));
  jsCanvasContext2D_ = CJValueP(new CQJCanvasContext2D(this));

  js_->setProperty("window", jsWindow_);
  js_->setProperty("document", jsDocument_);

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
callEventListeners(const std::string &name, const EventArgs &args)
{
  for (const auto &o : objects_)
    o->callEventListener(name, args);
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
