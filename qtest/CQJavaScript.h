#ifndef CQJavaScript_H
#define CQJavaScript_H

#include <CJavaScript.h>
#include <QFrame>
#include <QImage>
#include <QPen>
#include <QBrush>

class CQJObject;
class CQJSCanvas;
class CJavaScript;
class CQHistoryLineEdit;

class CQJavaScript : public QFrame {
  Q_OBJECT

 public:
  CQJavaScript();

  CQJSCanvas *canvas() const { return canvas_; }

  CJavaScript *js() const { return js_; }

  CJObjectTypeP jsWindowType              () const { return jsWindowType_              ; }
  CJObjectTypeP jsDocumentType            () const { return jsDocumentType_            ; }
  CJObjectTypeP jsImageType               () const { return jsImageType_               ; }
  CJObjectTypeP jsCanvasType              () const { return jsCanvasType_              ; }
  CJObjectTypeP jsCanvasContext2DType     () const { return jsCanvasContext2DType_     ; }
  CJObjectTypeP jsCanvasLinearGradientType() const { return jsCanvasLinearGradientType_; }
  CJObjectTypeP jsCanvasRadialGradientType() const { return jsCanvasRadialGradientType_; }
  CJObjectTypeP jsCanvasPatternType       () const { return jsCanvasPatternType_       ; }
  CJObjectTypeP jsCanvasFontMetricsType   () const { return jsCanvasFontMetricsType_   ; }

  CJValueP jsWindow         () const { return jsWindow_         ; }
  CJValueP jsDocument       () const { return jsDocument_       ; }
  CJValueP jsCanvas         () const { return jsCanvas_         ; }
  CJValueP jsCanvasContext2D() const { return jsCanvasContext2D_; }

  void addObject   (CQJObject *obj);
  void removeObject(CQJObject *obj);

  void callEventListeners(const std::string &name);

 private slots:
  void execCmd(const QString &cmd);

  void loadFileSlot();

 private:
  typedef std::set<CQJObject *> Objects;

  CQJSCanvas*        canvas_ { 0 };
  CJavaScript*       js_ { 0 };
  CJObjectTypeP      jsWindowType_;
  CJValueP           jsWindow_;
  CJObjectTypeP      jsDocumentType_;
  CJValueP           jsDocument_;
  CJObjectTypeP      jsImageType_;
  CJObjectTypeP      jsCanvasType_;
  CJValueP           jsCanvas_;
  CJObjectTypeP      jsCanvasContext2DType_;
  CJValueP           jsCanvasContext2D_;
  CJObjectTypeP      jsCanvasLinearGradientType_;
  CJObjectTypeP      jsCanvasRadialGradientType_;
  CJObjectTypeP      jsCanvasPatternType_;
  CJObjectTypeP      jsCanvasFontMetricsType_;
  Objects            objects_;
  CQHistoryLineEdit* input_ { 0 };
};

//---

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

  void drawImage(double x, double y, const QImage &image);

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
