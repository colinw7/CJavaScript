#ifndef CQJCanvasWidget_H
#define CQJCanvasWidget_H

#include <CJavaScript.h>
#include <CQJObject.h>
#include <QFrame>
#include <QGradient>
#include <QImage>
#include <QFont>
#include <QColor>
#include <QPen>
#include <QPainterPath>

class CQJCanvas;

class CQJCanvasWidget : public QFrame {
  Q_OBJECT

 public:
  CQJCanvasWidget(CQJCanvas *canvas);

  int size() const { return size_; }
  void setSize(int i) { size_ = i; }

  void updateSize(int w, int h);

  void setFillColor   (const QColor &c);
  void setFillGradient(const QGradient &g);
  void setFillPattern (const QImage &i);
  void setFillAlpha   (double a);

  void setStrokeColor   (const QColor &c);
  void setStrokeGradient(const QGradient &g);
  void setStrokeAlpha   (double a);
  void setStrokeWidth   (double w);
  void setStrokeCap     (Qt::PenCapStyle  cap);
  void setStrokeJoin    (Qt::PenJoinStyle join);

  void setOpacity(double a);

  void fillRect (double x, double y, double w, double h);
  void drawRect (double x, double y, double w, double h);
  void clearRect(double x, double y, double w, double h);

  void fillText  (double x, double y, const std::string &text);
  void strokeText(double x, double y, const std::string &text);

  void fillShadowText(double x, double y, const std::string &text, double d, const QColor &c);

  void drawImage(const QImage &image, double dx, double dy);
  void drawImage(const QImage &image, double dx, double dy, double dWidth, double dHeight);
  void drawImage(const QImage &image, double sx, double sy, double sWidth, double sHeight,
                 double dx, double dy, double dWidth, double dHeight);

  QImage getImageData(double x, double y, double w, double h);
  void putImageData(const QImage &image, double x, double y);

  void beginPath();
  void moveTo(double x, double y);
  void lineTo(double x, double y);
  void arc(double x, double y, double r, double a1, double a2, bool counter=false);
  void closePath();
  void stroke();
  void fill();

  void save();
  void restore();

  void rect(double x, double y, double w, double h);
  void clip();

  void setTransform(double a, double b, double c, double d, double e, double f);

  void translate(double dx, double dy);
  void scale(double sx, double sy);
  void rotate(double a);

  const QFont &font() const { return font_; }
  void setFont(const QFont &v) { font_ = v; }

  const Qt::Alignment &fontAlign() const { return fontAlign_; }
  void setFontAlign(const Qt::Alignment &v) { fontAlign_ = v; }

  void paintEvent(QPaintEvent *) override;

  void resizeEvent(QResizeEvent *) override;

  void enterEvent(QEvent *) override;
  void leaveEvent(QEvent *) override;

  void mousePressEvent  (QMouseEvent *e) override;
  void mouseMoveEvent   (QMouseEvent *e) override;
  void mouseReleaseEvent(QMouseEvent *e) override;

  void mouseDoubleClickEvent(QMouseEvent *e) override;

  void contextMenuEvent(QContextMenuEvent *e) override;

  void keyPressEvent  (QKeyEvent *e) override;
  void keyReleaseEvent(QKeyEvent *e) override;

  QSize sizeHint() const override;

 private:
  void setPen();
  void setBrush();

  void callEventListener(const std::string &name, const std::string &prop, CJValueP event,
                         const CQJObject::NameValues &nameValues=CQJObject::NameValues());

  void callEventListener(const std::string &name, const std::string &prop,
                         const CQJObject::EventArgs &args=CQJObject::EventArgs(),
                         const CQJObject::NameValues &nameValues=CQJObject::NameValues());

  CJValueP createEvent(QEvent *e) const;

 private:
  CQJCanvas*       canvas_      { nullptr };
  int              size_        { 600 };
  QPainter*        ip_          { nullptr };
  QImage           qimage_;
  QColor           fillColor_   { QColor(Qt::black) };
  QGradient        fillGradient_;
  QImage           fillPattern_;
  Qt::BrushStyle   fillStyle_   { Qt::SolidPattern };
  double           fillAlpha_   { 1 };
  QColor           strokeColor_ { QColor(Qt::black) };
  QGradient        strokeGradient_;
  Qt::BrushStyle   strokeStyle_ { Qt::SolidPattern };
  double           strokeAlpha_ { 1 };
  double           strokeWidth_ { 0 };
  Qt::PenCapStyle  strokeCap_   { Qt::SquareCap };
  Qt::PenJoinStyle strokeJoin_  { Qt::MiterJoin };
  QPainterPath     path_;
  QPen             pen_;
  QBrush           fillBrush_;
  QBrush           strokeBrush_;
//QRectF           rect_;
  QFont            font_;
  Qt::Alignment    fontAlign_ { Qt::AlignBaseline };
};

#endif
