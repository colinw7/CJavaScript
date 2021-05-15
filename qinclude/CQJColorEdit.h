#ifndef CQJColorEdit_H
#define CQJColorEdit_H

#include <CJavaScript.h>
#include <QLineEdit>

class CQJHtmlObj;
class CQJFormInputIFace;
class CQJColorWidget;

class CQJColorEdit : public QLineEdit {
  Q_OBJECT

  Q_PROPERTY(QColor color READ color WRITE setColor)

 public:
  CQJColorEdit(CQJHtmlObj *obj, CQJFormInputIFace *iface);

  void setText(const QString &text);

  const QColor &color() const { return color_; }
  void setColor(const QColor &v);

 private:
  void updateState();

  void emitChange();

  void keyPressEvent  (QKeyEvent *e) override;
  void keyReleaseEvent(QKeyEvent *e) override;

  void callEventListener(const std::string &name, const std::string &prop, CJValueP event);

 private slots:
  void focusChanged(QWidget *, QWidget *);
  void changeSlot(const QString &);

 private:
  CQJHtmlObj        *obj_         { nullptr };
  CQJFormInputIFace *iface_       { nullptr };
  QColor             color_;
  CQJColorWidget    *colorWidget_ { nullptr };
};

//------

class CQJColorWidget : public QFrame {
 public:
  explicit CQJColorWidget(CQJColorEdit *edit);

  void paintEvent(QPaintEvent *) override;

  void mousePressEvent(QMouseEvent *e) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;

  void updateColorFromPos(const QPoint &pos);

 private:
  CQJColorEdit *edit_;
  QRect         palRect_;
  QRect         valueRect_;
};

#endif
