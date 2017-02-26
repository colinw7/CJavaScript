#ifndef CQJComboBox_H
#define CQJComboBox_H

#include <CJavaScript.h>
#include <QComboBox>

class CQJHtmlObj;
class CQJFormInputIFace;

class CQJComboBox : public QComboBox {
  Q_OBJECT

 public:
  CQJComboBox(CQJHtmlObj *obj, CQJFormInputIFace *iface);

 private slots:
  void currentIndexSlot(int);

 private:
  void callEventListener(const std::string &name, const std::string &prop, CJValueP event);

 private:
  CQJHtmlObj        *obj_   { nullptr };
  CQJFormInputIFace *iface_ { nullptr };
};

#endif
