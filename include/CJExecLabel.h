#ifndef CJExecLabel_H
#define CJExecLabel_H

#include <CJToken.h>
#include <iostream>

// label
class CJExecLabel : public CJToken {
 public:
  CJExecLabel();

  const std::string &name() const { return name_; }
  void setName(const std::string &s) { name_ = s; }

  CJValueP exec(CJavaScript *js) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  std::string name_;
};

#endif
