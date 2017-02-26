#include <CQJColorEdit.h>
#include <CQJavaScript.h>
#include <CQJWindow.h>
#include <CQJEvent.h>
#include <CQJHtmlObj.h>
#include <CQJFormInputIFace.h>
#include <QApplication>
#include <QPainter>

CQJColorEdit::
CQJColorEdit(CQJHtmlObj *obj, CQJFormInputIFace *iface) :
 QLineEdit(nullptr), obj_(obj), iface_(iface)
{
  setObjectName(iface->getName().c_str());

  setAutoFillBackground(true);

  connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(changeSlot(const QString &)));

  colorWidget_ = new CQJColorWidget(this);

  connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
          this, SLOT(focusChanged(QWidget*,QWidget*)));

  updateState();
}

void
CQJColorEdit::
setText(const QString &text)
{
  QLineEdit::setText(text);

  color_ = QColor("#" + text);

  updateState();

  emitChange();
}

void
CQJColorEdit::
setColor(const QColor &c)
{
  color_ = c;

  blockSignals(true);

  QLineEdit::setText(color_.name().mid(1).toUpper());

  updateState();

  blockSignals(false);

  emitChange();
}

void
CQJColorEdit::
updateState()
{
  int gray = qGray(color_.rgb());

  QPalette palette = this->palette();

  palette.setColor(backgroundRole(), color_);
  palette.setColor(QPalette::Text, gray < 80 ? Qt::white : Qt::black);

  setPalette(palette);
}

void
CQJColorEdit::
changeSlot(const QString &text)
{
  color_ = QColor("#" + text);

  emitChange();
}

void
CQJColorEdit::
emitChange()
{
  CJValueP edit = obj_->shared_from_this();

  CJValueP event(new CQJEvent(CQJavaScriptInst->js(), edit, 0));

  callEventListener("change", "onchange", event);
}

void
CQJColorEdit::
focusChanged(QWidget *, QWidget *newWidget)
{
  if      (newWidget == this) {
    QPoint pos = mapToGlobal(rect().bottomLeft());

    colorWidget_->move(pos.x(), pos.y() + 4);

    colorWidget_->show();
  }
  else if (newWidget) {
    if (colorWidget_->isVisible())
      colorWidget_->hide();
  }
}

void
CQJColorEdit::
keyPressEvent(QKeyEvent *e)
{
  QLineEdit::keyPressEvent(e);

  CJValueP edit = obj_->shared_from_this();

  CJValueP event(new CQJEvent(CQJavaScriptInst->js(), edit, e));

  callEventListener("keydown", "onkeydown", event);
}

void
CQJColorEdit::
keyReleaseEvent(QKeyEvent *e)
{
  QLineEdit::keyReleaseEvent(e);

  CJValueP edit = obj_->shared_from_this();

  CJValueP event(new CQJEvent(CQJavaScriptInst->js(), edit, e));

  callEventListener("keyup", "onkeyup", event);
}

void
CQJColorEdit::
callEventListener(const std::string &name, const std::string &prop, CJValueP event)
{
  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  args.push_back(event);

  if (obj_->callEventListener(name, prop, args, nameValues))
    return;
}

//------

CQJColorWidget::
CQJColorWidget(CQJColorEdit *edit) :
 edit_(edit)
{
  setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);

  setFixedSize(300, 216);

  setFocusProxy(edit);

  setFocusPolicy(Qt::ClickFocus);
}

void
CQJColorWidget::
paintEvent(QPaintEvent *)
{
  QPainter p(this);

  p.fillRect(rect(), palette().background());

  palRect_ = QRect(8, 8, 200, 200);

  QColor c;

  for (int y = 0; y < palRect_.height(); ++y) {
    double sat = 1.0 - (1.0*y)/palRect_.height();

    for (int x = 0; x < palRect_.width(); ++x) {
      double hue = (1.0*x)/palRect_.width();

      c.setHsvF(hue, sat, 1.0);

      p.setPen(c);

      p.drawPoint(palRect_.left() + x, palRect_.top() + y);
    }
  }

  double hue, sat, value;

  edit_->color().getHsvF(&hue, &sat, &value);

  int x = hue*palRect_.width() + palRect_.left();
  int y = (1.0 - sat)*palRect_.height() + palRect_.top();

  //---

  QPen pen;

  pen.setColor(Qt::white);
  pen.setWidth(3);

  p.setPen(pen);

  p.drawLine(x - 4, y, x + 4, y);
  p.drawLine(x, y - 4, x, y + 4);

  pen.setColor(Qt::black);
  pen.setWidth(1);

  p.setPen(pen);

  p.drawLine(x - 4, y, x + 4, y);
  p.drawLine(x, y - 4, x, y + 4);

  //---

  p.setPen(Qt::black);

  p.drawRect(palRect_);

  //---

  valueRect_ = QRect(240, 8, 32, 200);

  for (int y = 0; y < valueRect_.height(); ++y) {
    double value = 1.0 - (1.0*y)/valueRect_.height();

    c.setHsvF(0.0, 0.0, value);

    p.setPen(c);

    p.drawLine(valueRect_.left()                     , valueRect_.top() + y,
               valueRect_.left() + valueRect_.width(), valueRect_.top() + y);
  }

  y = (1.0 - value)*valueRect_.height() + valueRect_.top();

  p.setPen(Qt::black);

  p.drawLine(valueRect_.left() - 8, y + 4, valueRect_.left()    , y    );
  p.drawLine(valueRect_.left() - 8, y - 4, valueRect_.left() - 8, y + 4);
  p.drawLine(valueRect_.left()    , y    , valueRect_.left() - 8, y - 4);

  p.setPen(Qt::black);

  p.drawRect(valueRect_);
}

void
CQJColorWidget::
mousePressEvent(QMouseEvent *e)
{
  updateColorFromPos(e->pos());

  update();
}

void
CQJColorWidget::
mouseMoveEvent(QMouseEvent *e)
{
  updateColorFromPos(e->pos());

  update();
}

void
CQJColorWidget::
mouseReleaseEvent(QMouseEvent *e)
{
  updateColorFromPos(e->pos());

  update();
}

void
CQJColorWidget::
updateColorFromPos(const QPoint &pos)
{
  QColor c = edit_->color();

  double hue, sat, value;

  c.getHsvF(&hue, &sat, &value);

  if      (palRect_.contains(pos)) {
    int x = pos.x() - palRect_.left();
    int y = pos.y() - palRect_.top();

    hue = (1.0*x)/palRect_.width();
    sat = 1.0 - (1.0*y)/palRect_.height();

    c.setHsvF(hue, sat, value);
  }
  else if (valueRect_.contains(pos)) {
    int y = pos.y() - valueRect_.top();

    value = 1.0 - (1.0*y)/valueRect_.height();

    c.setHsvF(hue, sat, value);
  }

  edit_->setColor(c);
}
