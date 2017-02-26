#include <CQJFrameWidget.h>
#include <CQJCanvasWidget.h>
#include <CQHistoryLineEdit.h>
#include <CQJDialog.h>
#include <CQJCanvas.h>
#include <CQJCanvasIFace.h>
#include <CQJHtmlObjIFace.h>
#include <CQJavaScript.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QFileDialog>
#include <QPainter>
#include <QDir>
#include <iostream>

#include <svg/javascript_svg.h>

//---

class CanvasIFace : public CQJCanvasIFace {
 public:
  CanvasIFace(CQJCanvasWidget *canvas) :
   canvas_(canvas) {
  }

  CQJCanvasWidget *canvasWidget() const override {
    return canvas_;
  }

  int width() const override {
    return canvas_->width();
  }

  void setWidth(int w) override {
    canvas_->resize(w, canvas_->height());
  }

  int height() const override {
    return canvas_->height();
  }

  void setHeight(int h) override {
    canvas_->resize(canvas_->width(), h);
  }

 private:
  CQJCanvasWidget *canvas_ { nullptr };
};

//---

class CanvasObjIFace : public CQJHtmlObjIFace {
 public:
  CanvasObjIFace(CQJCanvas *canvas) :
   canvas_(canvas) {
  }

  std::string getName () const { return "canvas"; }
  std::string getClass() const { return "canvas"; }
  std::string getType () const { return "canvas"; }
  std::string getId   () const { return "canvas"; }

  CQJHtmlObjIFace *parent() const { return nullptr; }

  CQJHtmlObj *obj() const { return canvas_; }

  Children children() const { return Children(); }

 private:
  CQJCanvas *canvas_ { nullptr };
};

//------

CQJFrameWidget *CQJFrameWidget::instance_;

CQJFrameWidget::
CQJFrameWidget()
{
  instance_ = this;
}

void
CQJFrameWidget::
init()
{
  CQJavaScript *qjs = CQJavaScriptInst;

  CJavaScript *js = qjs->js();

  jsCanvas_ = new CQJCanvas(js, nullptr, nullptr);

  jsCanvasValue_ = CJValueP(jsCanvas_);

  //---

  QVBoxLayout *layout = new QVBoxLayout(this);

  canvas_ = new CQJCanvasWidget(jsCanvas_);

  canvas_->setFocusPolicy(Qt::StrongFocus);

  canvas_->setSize(size_);

  layout->addWidget(canvas_);

  //---

  canvasIFace_    = new CanvasIFace(canvas_);
  canvasObjIFace_ = new CanvasObjIFace(jsCanvas_);

  jsCanvas_->setIFace(canvasIFace_);

  jsCanvas_->CQJHtmlObj::setIFace(canvasObjIFace_);

  qjs->addHtmlObject(jsCanvas_);

  //---

  QFrame *entryFrame = new QFrame;

  layout->addWidget(entryFrame);

  QHBoxLayout *entryLayout = new QHBoxLayout(entryFrame);
  entryLayout->setMargin(0); entryLayout->setSpacing(2);

  //---

  input_ = new CQHistoryLineEdit;

  entryLayout->addWidget(input_);

  connect(input_, SIGNAL(exec(const QString &)), this, SLOT(execCmd(const QString &)));

  //---

  QPushButton *load = new QPushButton("Load");

  entryLayout->addWidget(load);

  connect(load, SIGNAL(clicked()), this, SLOT(loadFileSlot()));

  //---

  QToolButton *jbutton = new QToolButton;

  jbutton->setIcon(CQPixmapCacheInst->getIcon("JAVASCRIPT"));

  entryLayout->addWidget(jbutton);

  connect(jbutton, SIGNAL(clicked()), this, SLOT(showDialogSlot()));
}

void
CQJFrameWidget::
execCmd(const QString &cmd)
{
  if (! cmd.length())
    return;

  //---

  CJavaScript *js = CQJavaScript::instance()->js();

  js->loadString(cmd.toStdString());

  CJValueP value = js->exec();

  if (value) {
    std::string str = value->toString();

    std::cerr << str << std::endl;
  }
}

void
CQJFrameWidget::
loadFileSlot()
{
  QString title  = "Load File";
  QString cwd    = QDir::currentPath();
  QString filter = "JavaScript Files (*.js)";

  QString filename = QFileDialog::getOpenFileName(this, title, cwd, filter);

  if (filename == "")
    return;

  CJavaScript *js = CQJavaScript::instance()->js();

  js->loadFile(filename.toStdString());

  js->exec();
}

void
CQJFrameWidget::
showDialogSlot()
{
  if (! dialog_) {
    CJavaScript *js = CQJavaScript::instance()->js();

    dialog_ = new CQJDialog(js);
  }

  dialog_->show();
}

QSize
CQJFrameWidget::
sizeHint() const
{
  return QSize(size_, size_);
}
