#ifndef CQJFrameWidget_H
#define CQJFrameWidget_H

#include <CJavaScript.h>
#include <QFrame>

class CQJCanvasWidget;
class CQHistoryLineEdit;
class CQJDialog;
class CQJCanvas;
class CanvasIFace;
class CanvasObjIFace;

class CQJFrameWidget : public QFrame {
  Q_OBJECT

 public:
  static CQJFrameWidget *instance() { return instance_; }

  CQJFrameWidget();

  void init();

  void setSize(int s) { size_ = s; }

  CQJCanvasWidget *canvas() const { return canvas_; }

  QSize sizeHint() const;

 private slots:
  void execCmd(const QString &cmd);

  void loadFileSlot();

  void showDialogSlot();

 private:
  static CQJFrameWidget *instance_;

  int                size_           { 600 };
  CQJCanvasWidget*   canvas_         { nullptr };
  CQHistoryLineEdit* input_          { nullptr };
  CQJDialog*         dialog_         { nullptr };
  CQJCanvas*         jsCanvas_       { nullptr };
  CJValueP           jsCanvasValue_;
  CanvasIFace*       canvasIFace_    { nullptr };
  CanvasObjIFace*    canvasObjIFace_ { nullptr };
};

#endif
