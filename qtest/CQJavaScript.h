#ifndef CQJavaScript_H
#define CQJavaScript_H

#include <CJavaScript.h>
#include <QFrame>

class CQJObject;
class CQJSCanvas;
class CJavaScript;
class CQHistoryLineEdit;
class CQJDialog;

class CQJavaScript : public QFrame {
  Q_OBJECT

 public:
  typedef std::vector<CJValueP> EventArgs;

 public:
  CQJavaScript();

  void init();

  void setSize(int s) { size_ = s; }

  CQJSCanvas *canvas() const { return canvas_; }

  CJavaScript *js() const { return js_; }

  CJValueP jsWindow         () const { return jsWindow_         ; }
  CJValueP jsDocument       () const { return jsDocument_       ; }
  CJValueP jsCanvas         () const { return jsCanvas_         ; }
  CJValueP jsCanvasContext2D() const { return jsCanvasContext2D_; }

  void addObject   (CQJObject *obj);
  void removeObject(CQJObject *obj);

  void callEventListeners(const std::string &name, const std::string &prop,
                          const EventArgs &args=EventArgs());

  QSize sizeHint() const;

 private slots:
  void execCmd(const QString &cmd);

  void loadFileSlot();

  void showDialogSlot();

 private:
  typedef std::set<CQJObject *> Objects;

  int                size_ { 600 };
  CQJSCanvas*        canvas_ { 0 };
  CJavaScript*       js_ { 0 };
  CJValueP           jsWindow_;
  CJValueP           jsDocument_;
  CJValueP           jsCanvas_;
  CJValueP           jsCanvasContext2D_;
  Objects            objects_;
  CQHistoryLineEdit* input_ { 0 };
  CQJDialog*         dialog_ { 0 };
};

#endif
