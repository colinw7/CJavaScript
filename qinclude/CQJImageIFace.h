#ifndef CQJImageIFace_H
#define CQJImageIFace_H

#include <string>
#include <vector>

class CQJImage;

class CQJImageIFace {
 public:
  CQJImageIFace() { }

  virtual ~CQJImageIFace() { }

  virtual CImagePtr image() const = 0;
};

#endif
