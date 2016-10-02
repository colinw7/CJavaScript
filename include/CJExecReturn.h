#ifndef CJExecReturn_H
#define CJExecReturn_H

#include <CJToken.h>
#include <iostream>

// return
class CJExecReturn : public CJToken {
 public:
  CJExecReturn();

  void setExpr(CJExecExpressionP expr) { expr_ = expr; }

  CJValueP exec(CJavaScript *js) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJExecExpressionP expr_;
};

#endif
