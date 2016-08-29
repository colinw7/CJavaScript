#ifndef CJExecThis_H
#define CJExecThis_H

#include <CJToken.h>

// new <type> ( <args> )
class CJExecThis : public CJToken {
 public:
  CJExecThis();

  void setIdentifiers(CJExecIdentifiersP identifiers) { identifiers_ = identifiers; }

  void setIndexExpression(CJExecIndexExpressionP iexpr) { iexpr_ = iexpr; }

  void setAssignExpression(CJExecExpressionP assign) { assign_ = assign; }

  void setIncrDecr(CJOperatorP op, bool postOp=true) { incrDecr_ = op; postOp_ = postOp; }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  CJExecIdentifiersP     identifiers_;
  CJExecIndexExpressionP iexpr_;
  CJExecExpressionP      assign_;
  CJOperatorP            incrDecr_;
  bool                   postOp_ { true };
};

#endif
