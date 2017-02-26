#ifndef CQJFormInputIFace_H
#define CQJFormInputIFace_H

class CQJFormInputIFace {
 public:
  CQJFormInputIFace() { }

  virtual ~CQJFormInputIFace() { }

  virtual std::string getName() const = 0;

  virtual CJValueP value(CJavaScript *js) const = 0;

  virtual CJValueP numberValue(CJavaScript *js) const = 0;
};

#endif
