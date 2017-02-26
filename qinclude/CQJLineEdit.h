#ifndef CQJLineEdit_H
#define CQJLineEdit_H

#include <CJavaScript.h>
#include <QLineEdit>

class CQJHtmlObj;
class CQJFormInputIFace;

class CQJLineEdit : public QLineEdit {
  Q_OBJECT

 public:
  CQJLineEdit(CQJHtmlObj *obj, CQJFormInputIFace *iface);

 private:
  void keyPressEvent(QKeyEvent *e);
  void keyReleaseEvent(QKeyEvent *e);

  void callEventListener(const std::string &name, const std::string &prop, CJValueP event);

 private slots:
  void changeSlot();

 private:
  CQJHtmlObj        *obj_   { nullptr };
  CQJFormInputIFace *iface_ { nullptr };
};

#endif
