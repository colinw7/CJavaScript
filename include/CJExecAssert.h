#ifndef CJExecAssert_H
#define CJExecAssert_H

#include <CJToken.h>

// break
class CJExecAssert : public CJToken {
 public:
  CJExecAssert();

  CJValueP exec(CJavaScript *js) override;

  CJExecExpressionListP exprList() const { return exprList_; }
  void setExprList(CJExecExpressionListP exprList) { exprList_ = exprList; }

  void print(std::ostream &os) const override;

 private:
  CJExecExpressionListP exprList_;
};

#endif
