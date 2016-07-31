#include <CQJavaScript.h>
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

  jsWindowType_               = CJObjectTypeP(new CQJWindowType              ());
  jsDocumentType_             = CJObjectTypeP(new CQJDocumentType            ());
  jsImageType_                = CJObjectTypeP(new CQJImageType               (this));
  jsCanvasType_               = CJObjectTypeP(new CQJCanvasType              ());
  jsCanvasContext2DType_      = CJObjectTypeP(new CQJCanvasContext2DType     ());
  jsCanvasLinearGradientType_ = CJObjectTypeP(new CQJCanvasLinearGradientType());
  jsCanvasRadialGradientType_ = CJObjectTypeP(new CQJCanvasRadialGradientType());
  jsCanvasPatternType_        = CJObjectTypeP(new CQJCanvasPatternType       ());
  jsCanvasFontMetricsType_    = CJObjectTypeP(new CQJCanvasFontMetricsType   ());

  js_->addObjectType("window"         , jsWindowType_);
  js_->addObjectType("document"       , jsDocumentType_);
  js_->addObjectType("Image"          , jsImageType_);
  js_->addObjectType("Canvas"         , jsCanvasType_);
  js_->addObjectType("CanvasContext2D", jsCanvasContext2DType_);

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
callEventListeners(const std::string &name)
{
  for (const auto &o : objects_)
    o->callEventListener(name);
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

//------

CQJSCanvas::
CQJSCanvas(CQJavaScript *js) :
 js_(js)
{
}

void
CQJSCanvas::
setFillColor(const QColor &c)
{
  fillColor_ = c;
  fillStyle_ = Qt::SolidPattern;
}

void
CQJSCanvas::
setFillGradient(const QGradient &g)
{
  fillGradient_ = g;
  fillStyle_    = Qt::LinearGradientPattern;
}

void
CQJSCanvas::
setFillPattern(const QImage &i)
{
  fillPattern_ = i;
  fillStyle_   = Qt::TexturePattern;
}

void
CQJSCanvas::
setFillAlpha(double a)
{
  fillAlpha_ = a;
}

void
CQJSCanvas::
setStrokeColor(const QColor &c)
{
  strokeColor_ = c;
  strokeStyle_ = Qt::SolidPattern;
}

void
CQJSCanvas::
setStrokeGradient(const QGradient &g)
{
  strokeGradient_ = g;
  strokeStyle_    = Qt::LinearGradientPattern;
}

void
CQJSCanvas::
setStrokeAlpha(double a)
{
  strokeAlpha_ = a;
}

void
CQJSCanvas::
setStrokeWidth(double w)
{
  strokeWidth_ = w;
}

void
CQJSCanvas::
setStrokeCap(Qt::PenCapStyle cap)
{
  strokeCap_ = cap;
}

void
CQJSCanvas::
setStrokeJoin(Qt::PenJoinStyle join)
{
  strokeJoin_ = join;
}

void
CQJSCanvas::
fillRect(double x, double y, double w, double h)
{
  setBrush();

  ip_->fillRect(QRectF(x, y, w, h), fillBrush_);
}

void
CQJSCanvas::
clearRect(double x, double y, double w, double h)
{
  ip_->eraseRect(QRectF(x, y, w, h));
}

void
CQJSCanvas::
drawRect(double x, double y, double w, double h)
{
  setPen();

  ip_->setPen(pen_);

  ip_->drawRect(QRectF(x, y, w, h));
}

void
CQJSCanvas::
fillText(double x, double y, const std::string &text)
{
  setPen();

  ip_->setPen(pen_);

  ip_->drawText(x, y, text.c_str());
}

void
CQJSCanvas::
strokeText(double x, double y, const std::string &text)
{
  setPen();

  ip_->setPen(pen_);

  ip_->drawText(x, y, text.c_str());
}

void
CQJSCanvas::
drawImage(double x, double y, const QImage &image)
{
  ip_->drawImage(x, y, image);
}

void
CQJSCanvas::
beginPath()
{
  path_ = QPainterPath();
}

void
CQJSCanvas::
moveTo(double x, double y)
{
  path_.moveTo(x, y);
}

void
CQJSCanvas::
lineTo(double x, double y)
{
  path_.lineTo(x, y);
}

void
CQJSCanvas::
arc(double x, double y, double r, double a1, double a2, bool counter)
{
  double da1 = 180*a1/M_PI;
  double da2 = 180*a2/M_PI;

  QRectF rect(x - r, y - r, 2*r, 2*r);

  if (! counter) {
    if (! path_.elementCount())
      path_.arcMoveTo(rect, da1);

    path_.arcTo(rect, da1, da2 - da1);
  }
  else {
    if (! path_.elementCount())
      path_.arcMoveTo(rect, da2);

    path_.arcTo(rect, da2, da1 - da2);
  }
}

void
CQJSCanvas::
closePath()
{
  path_.closeSubpath();
}

void
CQJSCanvas::
stroke()
{
  setPen();

  if (strokeStyle_ == Qt::SolidPattern) {
    ip_->strokePath(path_, pen_);
  }
  else {
    QPainterPathStroker stroker;

    stroker.setCapStyle   (pen_.capStyle());
    stroker.setDashOffset (pen_.dashOffset());
    stroker.setDashPattern(pen_.dashPattern());
    stroker.setJoinStyle  (pen_.joinStyle());
    stroker.setMiterLimit (pen_.miterLimit());
    stroker.setWidth      (pen_.widthF());

    ip_->fillPath(stroker.createStroke(path_), strokeBrush_);
  }
}

void
CQJSCanvas::
fill()
{
  setBrush();

  ip_->fillPath(path_, fillBrush_);
}

void
CQJSCanvas::
save()
{
  ip_->save();
}

void
CQJSCanvas::
restore()
{
  ip_->restore();
}

void
CQJSCanvas::
rect(double x, double y, double w, double h)
{
  rect_ = QRectF(x, y, w, h);
}

void
CQJSCanvas::
clip()
{
  ip_->setClipRect(rect_);
}

void
CQJSCanvas::
setTransform(double a, double b, double c, double d, double e, double f)
{
  QTransform m(a, b, c, d, e, f);

  ip_->setWorldTransform(m);
}

void
CQJSCanvas::
translate(double dx, double dy)
{
  ip_->translate(dx, dy);
}

void
CQJSCanvas::
scale(double sx, double sy)
{
  ip_->scale(sx, sy);
}

void
CQJSCanvas::
rotate(double a)
{
  ip_->rotate(180*a/M_PI);
}

void
CQJSCanvas::
setPen()
{
  if      (strokeStyle_ == Qt::SolidPattern) {
    QColor c = strokeColor_;

    c.setAlpha(255*strokeAlpha_);

    pen_.setColor(c);
  }
  else {
    strokeBrush_ = QBrush(strokeGradient_);
  }

  pen_.setCapStyle(strokeCap_);
  pen_.setJoinStyle(strokeJoin_);
  pen_.setWidthF(strokeWidth_);
}

void
CQJSCanvas::
setBrush()
{
  if      (fillStyle_ == Qt::SolidPattern) {
    QColor c = fillColor_;

    c.setAlpha(255*fillAlpha_);

    fillBrush_.setStyle(Qt::SolidPattern);
    fillBrush_.setColor(c);
  }
  else if (fillStyle_ == Qt::LinearGradientPattern) {
    fillBrush_ = QBrush(fillGradient_);
  }
  else if (fillStyle_ == Qt::TexturePattern) {
    fillBrush_ = QBrush(fillPattern_);
  }
}

void
CQJSCanvas::
resizeEvent(QResizeEvent *)
{
  delete ip_;

  qimage_ = QImage(QSize(width(), height()), QImage::Format_ARGB32);

  qimage_.fill(0);

  ip_ = new QPainter(&qimage_);

  ip_->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

  js_->jsCanvas()->cast<CQJCanvas>()->updateSize();
}

void
CQJSCanvas::
paintEvent(QPaintEvent *)
{
  QPainter p(this);

  p.drawImage(0, 0, qimage_);
}

QSize
CQJSCanvas::
sizeHint() const
{
  return QSize(600, 600);
}
