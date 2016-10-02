#ifndef CJExecQuestion_H
#define CJExecQuestion_H

#include <CJToken.h>
#include <iostream>

// <expression> ? <expression> : <expression>
class CJExecQuestion : public CJToken {
 public:
  CJExecQuestion(CJExecExpressionP bexpr);

  void setExpr1(CJExecExpressionP expr) {
    expr1_ = expr;
  }

  void setExpr2(CJExecExpressionP expr) {
    expr2_ = expr;
  }

  CJValueP exec(CJavaScript *js) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJExecExpressionP bexpr_;
  CJExecExpressionP expr1_;
  CJExecExpressionP expr2_;
};

#endif
