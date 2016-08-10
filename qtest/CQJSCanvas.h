#ifndef CQJSCanvas_H
#define CQJSCanvas_H

#include <QFrame>
#include <QPen>

class CQJavaScript;

class CQJSCanvas : public QFrame {
  Q_OBJECT

 public:
  CQJSCanvas(CQJavaScript *js);

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

  void fillRect (double x, double y, double w, double h);
  void drawRect (double x, double y, double w, double h);
  void clearRect(double x, double y, double w, double h);

  void fillText  (double x, double y, const std::string &text);
  void strokeText(double x, double y, const std::string &text);

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

  void resizeEvent(QResizeEvent *);

  void paintEvent(QPaintEvent *);

  void enterEvent(QEvent *);
  void leaveEvent(QEvent *);

  void mousePressEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);

  void mouseDoubleClickEvent(QMouseEvent *e);

  void contextMenuEvent(QContextMenuEvent *e);

  QSize sizeHint() const;

 private:
  void setPen();
  void setBrush();

 private:
  CQJavaScript*    js_ { 0 };
  QPainter*        ip_ { 0 };
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
  QRectF           rect_;
  QFont            font_;
};

#endif
