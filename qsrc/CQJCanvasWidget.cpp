#include <CQJCanvasWidget.h>
#include <CQJCanvas.h>
#include <CQJEvent.h>
#include <CQJHtmlObj.h>
#include <CQImageGaussianBlur.h>
#include <QPainter>

CQJCanvasWidget::
CQJCanvasWidget(CQJCanvas *canvas) :
 QFrame(0), canvas_(canvas)
{
  //setFocusPolicy(Qt::StrongFocus);

  setMouseTracking(true);
}

void
CQJCanvasWidget::
updateSize(int w, int h)
{
  if (qimage_.width() == w && qimage_.height() == h)
    return;

  delete ip_;

  qimage_ = QImage(QSize(w, h), QImage::Format_ARGB32);

  qimage_.fill(0);

  ip_ = new QPainter(&qimage_);

  //ip_->setCompositionMode(QPainter::CompositionMode_Plus);

  ip_->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
}

void
CQJCanvasWidget::
setFillColor(const QColor &c)
{
  fillColor_ = c;
  fillStyle_ = Qt::SolidPattern;
}

void
CQJCanvasWidget::
setFillGradient(const QGradient &g)
{
  fillGradient_ = g;
  fillStyle_    = Qt::LinearGradientPattern;
}

void
CQJCanvasWidget::
setFillPattern(const QImage &i)
{
  fillPattern_ = i;
  fillStyle_   = Qt::TexturePattern;
}

void
CQJCanvasWidget::
setFillAlpha(double a)
{
  fillAlpha_ = a;
}

void
CQJCanvasWidget::
setStrokeColor(const QColor &c)
{
  strokeColor_ = c;
  strokeStyle_ = Qt::SolidPattern;
}

void
CQJCanvasWidget::
setStrokeGradient(const QGradient &g)
{
  strokeGradient_ = g;
  strokeStyle_    = Qt::LinearGradientPattern;
}

void
CQJCanvasWidget::
setStrokeAlpha(double a)
{
  strokeAlpha_ = a;
}

void
CQJCanvasWidget::
setStrokeWidth(double w)
{
  strokeWidth_ = w;
}

void
CQJCanvasWidget::
setStrokeCap(Qt::PenCapStyle cap)
{
  strokeCap_ = cap;
}

void
CQJCanvasWidget::
setStrokeJoin(Qt::PenJoinStyle join)
{
  strokeJoin_ = join;
}

void
CQJCanvasWidget::
setOpacity(double a)
{
  ip_->setOpacity(a);
}

//------

void
CQJCanvasWidget::
fillRect(double x, double y, double w, double h)
{
  setBrush();

  ip_->fillRect(QRectF(x, y, w, h), fillBrush_);
}

void
CQJCanvasWidget::
clearRect(double x, double y, double w, double h)
{
  qimage_.fill(0);

  //ip_->setCompositionMode(QPainter::CompositionMode_SourceOver);

  ip_->fillRect(QRectF(x, y, w, h), QColor(0,0,0,0));

  //ip_->eraseRect(QRectF(x, y, w, h));

  //ip_->setCompositionMode(QPainter::CompositionMode_Plus);
}

void
CQJCanvasWidget::
drawRect(double x, double y, double w, double h)
{
  setPen();

  ip_->setPen(pen_);

  ip_->drawRect(QRectF(x, y, w, h));
}

void
CQJCanvasWidget::
fillShadowText(double x, double y, const std::string &text, double d, const QColor &c)
{
  setBrush();

  QFontMetrics fm(font_);

  int fw = fm.width(text.c_str());
  int fa = fm.ascent();
  int fd = fm.descent();

  QImage qimage = QImage(QSize(fw, fa + fd), QImage::Format_ARGB32);

  qimage.fill(0);

  QPainter ip(&qimage);

  QPainterPath path;

  path.addText(QPointF(0, fa), font_, text.c_str());

  ip.fillPath(path, QBrush(c));

  CQImageGaussianBlur blur(qimage);

  QImage qimageBlur;

  blur.blur(qimageBlur, d, d, 0, 0);

  //---

  if      (fontAlign_ & Qt::AlignBottom)
    y -= fa + fd;
  else if (fontAlign_ & Qt::AlignVCenter)
    y -= fa + (fa - fd)/2;

  if      (fontAlign_ & Qt::AlignRight)
    x -= fw;
  else if (fontAlign_ & Qt::AlignHCenter)
    x -= fw/2;

  ip_->drawImage(x, y, qimageBlur);
}

void
CQJCanvasWidget::
fillText(double x, double y, const std::string &text)
{
  setBrush();

  QFontMetrics fm(font_);

  int fw = fm.width(text.c_str());
  int fa = fm.ascent();
  int fd = fm.descent();

  if      (fontAlign_ & Qt::AlignTop)
    y += fa;
  else if (fontAlign_ & Qt::AlignBottom)
    y -= fd;
  else if (fontAlign_ & Qt::AlignVCenter)
    y -= (fa - fd)/2;

  if      (fontAlign_ & Qt::AlignRight)
    x -= fw;
  else if (fontAlign_ & Qt::AlignHCenter)
    x -= fw/2;

  path_ = QPainterPath();

  path_.addText(QPointF(x, y), font_, text.c_str());

  ip_->fillPath(path_, fillBrush_);
}

void
CQJCanvasWidget::
strokeText(double x, double y, const std::string &text)
{
  setPen();

  QFontMetrics fm(font_);

  if      (fontAlign_ & Qt::AlignTop)
    y += fm.ascent();
  else if (fontAlign_ & Qt::AlignBottom)
    y -= fm.descent();
  else if (fontAlign_ & Qt::AlignVCenter)
    y -= fm.ascent() - fm.height()/2;

  if      (fontAlign_ & Qt::AlignRight)
    x -= fm.width(text.c_str());
  else if (fontAlign_ & Qt::AlignHCenter)
    x -= fm.width(text.c_str())/2;

  path_ = QPainterPath();

  path_.addText(QPointF(x, y), font_, text.c_str());

  ip_->strokePath(path_, pen_);
}

void
CQJCanvasWidget::
drawImage(const QImage &image, double dx, double dy)
{
  ip_->drawImage(dx, dy, image);
}

void
CQJCanvasWidget::
drawImage(const QImage &image, double dx, double dy, double dWidth, double dHeight)
{
  ip_->drawImage(QRectF(dx, dy, dWidth, dHeight), image);
}

void
CQJCanvasWidget::
drawImage(const QImage &image, double sx, double sy, double sWidth, double sHeight,
          double dx, double dy, double dWidth, double dHeight)
{
  ip_->drawImage(QRectF(dx, dy, dWidth, dHeight), image, QRectF(sx, sy, sWidth, sHeight));
}

QImage
CQJCanvasWidget::
getImageData(double x, double y, double w, double h)
{
  return qimage_.copy(QRect(x, y, w, h));
}

void
CQJCanvasWidget::
putImageData(const QImage &image, double x, double y)
{
  ip_->drawImage(x, y, image);
}

void
CQJCanvasWidget::
beginPath()
{
  path_ = QPainterPath();
}

void
CQJCanvasWidget::
moveTo(double x, double y)
{
  path_.moveTo(x, y);
}

void
CQJCanvasWidget::
lineTo(double x, double y)
{
  path_.lineTo(x, y);
}

void
CQJCanvasWidget::
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
CQJCanvasWidget::
closePath()
{
  path_.closeSubpath();
}

void
CQJCanvasWidget::
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
CQJCanvasWidget::
fill()
{
  setBrush();

  ip_->fillPath(path_, fillBrush_);
}

void
CQJCanvasWidget::
save()
{
  ip_->save();
}

void
CQJCanvasWidget::
restore()
{
  ip_->restore();
}

void
CQJCanvasWidget::
rect(double x, double y, double w, double h)
{
  //rect_ = QRectF(x, y, w, h);

  path_.moveTo(x    , y    );
  path_.lineTo(x + w, y    );
  path_.lineTo(x + w, y + h);
  path_.lineTo(x    , y + h);
}

void
CQJCanvasWidget::
clip()
{
  // clip from path ?
  QRectF rect = path_.boundingRect();

  //ip_->setClipRect(rect_);
  ip_->setClipRect(rect);
}

void
CQJCanvasWidget::
setTransform(double a, double b, double c, double d, double e, double f)
{
  QTransform m(a, b, c, d, e, f);

  ip_->setWorldTransform(m);
}

void
CQJCanvasWidget::
translate(double dx, double dy)
{
  ip_->translate(dx, dy);
}

void
CQJCanvasWidget::
scale(double sx, double sy)
{
  ip_->scale(sx, sy);
}

void
CQJCanvasWidget::
rotate(double a)
{
  ip_->rotate(180*a/M_PI);
}

void
CQJCanvasWidget::
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
CQJCanvasWidget::
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
CQJCanvasWidget::
paintEvent(QPaintEvent *)
{
  QPainter p(this);

  //p.fillRect(QWidget::rect(), Qt::black);

  p.drawImage(0, 0, qimage_);
}

void
CQJCanvasWidget::
resizeEvent(QResizeEvent *)
{
  updateSize(width(), height());
}

void
CQJCanvasWidget::
enterEvent(QEvent *)
{
  CQJObject::EventArgs args;

  callEventListener("mouseover", "onmouseover", args);
}

void
CQJCanvasWidget::
leaveEvent(QEvent *)
{
  CQJObject::EventArgs args;

  callEventListener("mouseout", "onmouseout", args);
}

void
CQJCanvasWidget::
mousePressEvent(QMouseEvent *e)
{
  CJValueP event = createEvent(e);

  callEventListener("mousedown", "onmousedown", event);
}

void
CQJCanvasWidget::
mouseMoveEvent(QMouseEvent *e)
{
  CJValueP event = createEvent(e);

  callEventListener("mousemove", "onmousemove", event);
}

void
CQJCanvasWidget::
mouseReleaseEvent(QMouseEvent *e)
{
  CJValueP event = createEvent(e);

  callEventListener("mouseup", "onmouseup", event);

  callEventListener("click", "onclick", event);
}

void
CQJCanvasWidget::
mouseDoubleClickEvent(QMouseEvent *e)
{
  CJValueP event = createEvent(e);

  callEventListener("dblclick", "ondblclick", event);
}

void
CQJCanvasWidget::
contextMenuEvent(QContextMenuEvent *e)
{
  CJValueP event = createEvent(e);

  callEventListener("contextmenu", "oncontextmenu", event);
}

void
CQJCanvasWidget::
keyPressEvent(QKeyEvent *e)
{
  CJValueP event = createEvent(e);

  callEventListener("keydown", "onkeydown", event);
}

void
CQJCanvasWidget::
keyReleaseEvent(QKeyEvent *e)
{
  CJValueP event = createEvent(e);

  callEventListener("keyup", "onkeyup", event);
}

void
CQJCanvasWidget::
callEventListener(const std::string &name, const std::string &prop,
                  CJValueP event, const CQJObject::NameValues &nameValues)
{
  CQJObject::EventArgs args;

  args.push_back(event);

  callEventListener(name, prop, args, nameValues);
}

void
CQJCanvasWidget::
callEventListener(const std::string &name, const std::string &prop,
                  const CQJObject::EventArgs &args, const CQJObject::NameValues &nameValues)
{
  if (canvas_->callEventListener(name, prop, args, nameValues))
    return;
}

CJValueP
CQJCanvasWidget::
createEvent(QEvent *e) const
{
  CJavaScript *js = canvas_->js();

  CJValueP value = canvas_->shared_from_this();

  return CJValueP(new CQJEvent(js, value, e));
}

QSize
CQJCanvasWidget::
sizeHint() const
{
  return QSize(size_, size_);
}
