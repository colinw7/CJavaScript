#ifndef CQJFormIFace_H
#define CQJFormIFace_H

class CQJFormIFace {
 public:
  typedef std::vector<CQJHtmlObj *> Inputs;

 public:
  CQJFormIFace() { }

  virtual ~CQJFormIFace() { }

  virtual Inputs inputs() const = 0;
};

#endif
