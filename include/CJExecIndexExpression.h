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

  CJExecExpressionP expression() const { return expr_; }
  void setExpression(CJExecExpressionP expr) { expr_ = expr; }

  CJExecExpressionP indexExpression() const { return iexpr_; }
  void setIndexExpression(CJExecExpressionP iexpr) { iexpr_ = iexpr; }

  void setEValue(CJValueP evalue) { evalue_ = evalue; }

  // is bound to lhs value
  bool isBound() const { return (identifiers_ || expr_); }

  CJValueP exec(CJavaScript *js) override;

  CJValueP indexValue(CJavaScript *js, CJValueP avalue);
  void setIndexValue(CJavaScript *js, CJValueP avalue, CJValueP rvalue);

  bool decodeExpressions(CJExecIdentifiersP &identifiers,
                         std::vector<CJExecExpressionP> &indices) const;

  CJExecIndexExpressionP lindexExpression() const;

  CJValueP indValue(CJavaScript *js);

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJExecIdentifiersP identifiers_;
  CJExecExpressionP  expr_;
  CJValueP           evalue_;
  CJExecExpressionP  iexpr_;
};

#endif
