#ifndef CQJCanvasIFace_H
#define CQJCanvasIFace_H

#include <string>
#include <vector>

class CQJCanvas;

class CQJCanvasIFace {
 public:
  CQJCanvasIFace() { }

  virtual ~CQJCanvasIFace() { }

  virtual CQJCanvasWidget *canvasWidget() const = 0;

  virtual int width() const = 0;
  virtual void setWidth(int w) = 0;

  virtual int height() const = 0;
  virtual void setHeight(int h) = 0;
};

#endif
