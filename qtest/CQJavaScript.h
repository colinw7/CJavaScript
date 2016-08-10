#ifndef CQJavaScript_H
#define CQJavaScript_H

#include <CJavaScript.h>
#include <QFrame>

class CQJObject;
class CQJSCanvas;
class CJavaScript;
class CQHistoryLineEdit;

class CQJavaScript : public QFrame {
  Q_OBJECT

 public:
  typedef std::vector<CJValueP> EventArgs;

 public:
  CQJavaScript();

  CQJSCanvas *canvas() const { return canvas_; }

  CJavaScript *js() const { return js_; }

  CJObjectTypeP jsWindowType  () const { return jsWindowType_  ; }
  CJObjectTypeP jsDocumentType() const { return jsDocumentType_; }
  CJObjectTypeP jsImageType   () const { return jsImageType_   ; }
  CJObjectTypeP jsCanvasType  () const { return jsCanvasType_  ; }

  CJValueP jsWindow         () const { return jsWindow_         ; }
  CJValueP jsDocument       () const { return jsDocument_       ; }
  CJValueP jsCanvas         () const { return jsCanvas_         ; }
  CJValueP jsCanvasContext2D() const { return jsCanvasContext2D_; }

  void addObject   (CQJObject *obj);
  void removeObject(CQJObject *obj);

  void callEventListeners(const std::string &name, const EventArgs &args=EventArgs());

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
  CJValueP           jsCanvasContext2D_;
  Objects            objects_;
  CQHistoryLineEdit* input_ { 0 };
};

#endif
