#include <CQJSCanvas.h>
#include <CQJCanvas.h>
#include <CQJDocument.h>
#include <CQJEvent.h>
#include <CQJavaScript.h>
#include <QPainter>
#include <QMouseEvent>

CQJSCanvas::
CQJSCanvas(CQJavaScript *qjs, int size) :
 qjs_(qjs), size_(size)
{
  setFocusPolicy(Qt::StrongFocus);
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
  setBrush();

  QFontMetrics fm(font_);

  if      (fontAlign_ == Qt::AlignTop)
    y += fm.ascent();
  else if (fontAlign_ == Qt::AlignBottom)
    y -= fm.descent();

  path_ = QPainterPath();

  path_.addText(QPointF(x, y), font_, text.c_str());

  ip_->fillPath(path_, fillBrush_);
}

void
CQJSCanvas::
strokeText(double x, double y, const std::string &text)
{
  setPen();

  path_ = QPainterPath();

  path_.addText(QPointF(x, y), font_, text.c_str());

  ip_->strokePath(path_, pen_);
}

void
CQJSCanvas::
drawImage(const QImage &image, double dx, double dy)
{
  ip_->drawImage(dx, dy, image);
}

void
CQJSCanvas::
drawImage(const QImage &image, double dx, double dy, double dWidth, double dHeight)
{
  ip_->drawImage(QRectF(dx, dy, dWidth, dHeight), image);
}

void
CQJSCanvas::
drawImage(const QImage &image, double sx, double sy, double sWidth, double sHeight,
          double dx, double dy, double dWidth, double dHeight)
{
  ip_->drawImage(QRectF(dx, dy, dWidth, dHeight), image, QRectF(sx, sy, sWidth, sHeight));
}

QImage
CQJSCanvas::
getImageData(double x, double y, double w, double h)
{
  return qimage_.copy(QRect(x, y, w, h));
}

void
CQJSCanvas::
putImageData(const QImage &image, double x, double y)
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
  //rect_ = QRectF(x, y, w, h);

  path_.moveTo(x    , y    );
  path_.lineTo(x + w, y    );
  path_.lineTo(x + w, y + h);
  path_.lineTo(x    , y + h);
}

void
CQJSCanvas::
clip()
{
  // clip from path ?
  QRectF rect = path_.boundingRect();

  //ip_->setClipRect(rect_);
  ip_->setClipRect(rect);
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

  qjs_->jsCanvas()->cast<CQJCanvas>()->updateSize();
}

void
CQJSCanvas::
paintEvent(QPaintEvent *)
{
  QPainter p(this);

  p.drawImage(0, 0, qimage_);
}

void
CQJSCanvas::
enterEvent(QEvent *)
{
  CQJObject::EventArgs args;

  CQJCanvas *canvas = qjs_->jsCanvas()->cast<CQJCanvas>();

  canvas->callEventListener("mouseover", "onmouseover", args);
}

void
CQJSCanvas::
leaveEvent(QEvent *)
{
  CQJObject::EventArgs args;

  CQJCanvas *canvas = qjs_->jsCanvas()->cast<CQJCanvas>();

  canvas->callEventListener("mouseout", "onmouseout", args);
}

void
CQJSCanvas::
mousePressEvent(QMouseEvent *e)
{
  CJavaScript *js = qjs_->js();

  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  nameValues.push_back(CQJObject::NameValue("mouseX", js->createNumberValue(long(e->x()))));
  nameValues.push_back(CQJObject::NameValue("mouseY", js->createNumberValue(long(e->y()))));

  CQJCanvas *canvas = qjs_->jsCanvas()->cast<CQJCanvas>();

  canvas->callEventListener("mousedown", "onmousedown", args, nameValues);
}

void
CQJSCanvas::
mouseMoveEvent(QMouseEvent *e)
{
  CJavaScript *js = qjs_->js();

  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  nameValues.push_back(CQJObject::NameValue("mouseX", js->createNumberValue(long(e->x()))));
  nameValues.push_back(CQJObject::NameValue("mouseY", js->createNumberValue(long(e->y()))));

  CQJCanvas *canvas = qjs_->jsCanvas()->cast<CQJCanvas>();

  canvas->callEventListener("mousemove", "onmousemove", args, nameValues);
}

void
CQJSCanvas::
mouseReleaseEvent(QMouseEvent *e)
{
  CJavaScript *js = qjs_->js();

  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  nameValues.push_back(CQJObject::NameValue("mouseX", js->createNumberValue(long(e->x()))));
  nameValues.push_back(CQJObject::NameValue("mouseY", js->createNumberValue(long(e->y()))));

  CQJCanvas *canvas = qjs_->jsCanvas()->cast<CQJCanvas>();

  canvas->callEventListener("mouseup", "onmouseup", args, nameValues);

  canvas->callEventListener("click", "onclick", args, nameValues);
}

void
CQJSCanvas::
mouseDoubleClickEvent(QMouseEvent *e)
{
  CJavaScript *js = qjs_->js();

  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  nameValues.push_back(CQJObject::NameValue("mouseX", js->createNumberValue(long(e->x()))));
  nameValues.push_back(CQJObject::NameValue("mouseY", js->createNumberValue(long(e->y()))));

  CQJCanvas *canvas = qjs_->jsCanvas()->cast<CQJCanvas>();

  canvas->callEventListener("dblclick", "ondblclick", args, nameValues);
}

void
CQJSCanvas::
contextMenuEvent(QContextMenuEvent *e)
{
  CJavaScript *js = qjs_->js();

  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  nameValues.push_back(CQJObject::NameValue("mouseX", js->createNumberValue(long(e->x()))));
  nameValues.push_back(CQJObject::NameValue("mouseY", js->createNumberValue(long(e->y()))));

  CQJCanvas *canvas = qjs_->jsCanvas()->cast<CQJCanvas>();

  canvas->callEventListener("contextmenu", "oncontextmenu", args, nameValues);
}

void
CQJSCanvas::
keyPressEvent(QKeyEvent *e)
{
  CJValueP event(new CQJEvent(qjs_, e));

  callEventListener("keydown", "onkeydown", event);
}

void
CQJSCanvas::
keyReleaseEvent(QKeyEvent *e)
{
  CJValueP event(new CQJEvent(qjs_, e));

  callEventListener("keyup", "onkeyup", event);
}

void
CQJSCanvas::
callEventListener(const std::string &name, const std::string &prop, CJValueP event)
{
  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  args.push_back(event);

  CQJCanvas *canvas = qjs_->jsCanvas()->cast<CQJCanvas>();

  if (canvas->callEventListener(name, prop, args, nameValues))
    return;

  CQJDocument *document = qjs_->jsDocument()->cast<CQJDocument>();

  document->callEventListener(name, prop, args, nameValues);
}

QSize
CQJSCanvas::
sizeHint() const
{
  return QSize(size_, size_);
}
