#ifndef CJExecIndexExpression_H
#define CJExecIndexExpression_H

#include <CJExecExpression.h>
#include <iostream>

// <expression> [ <expression> ]
class CJExecIndexExpression : public CJExecExpression {
 public:
  CJExecIndexExpression();

  CJExecIdentifiersP identifiers() const { return identifiers_; }
  void setIdentifiers(CJExecIdentifiersP identifiers) { identifiers_ = identifiers; }

  void setExpression(CJExecExpressionP expr) { expr_ = expr; }

  CJExecExpressionP indexExpression() const  { return iexpr_; }
  void setIndexExpression(CJExecExpressionP iexpr) { iexpr_ = iexpr; }

  void setEValue(CJValueP evalue) { evalue_ = evalue; }

  // is bound to lhs value
  bool isBound() const { return (identifiers_ || expr_); }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  CJExecIdentifiersP identifiers_;
  CJExecExpressionP  expr_;
  CJValueP           evalue_;
  CJExecExpressionP  iexpr_;
};

typedef std::shared_ptr<CJExecIndexExpression> CJExecIndexExpressionP;

#endif
