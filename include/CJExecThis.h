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

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  CJExecIdentifiersP     identifiers_;
  CJExecIndexExpressionP iexpr_;
  CJExecExpressionP      assign_;
};

typedef std::shared_ptr<CJExecThis> CJExecThisP;

#endif
