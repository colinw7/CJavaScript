#ifndef CJExecAssert_H
#define CJExecAssert_H

#include <CJToken.h>

// break
class CJExecAssert : public CJToken {
 public:
  CJExecAssert();

  CJValueP exec(CJavaScript *js) override;

  bool isEquals() const { return equals_; }
  void setEquals(bool b) { equals_ = b; }

  CJExecExpressionListP exprList() const { return exprList_; }
  void setExprList(CJExecExpressionListP exprList) { exprList_ = exprList; }

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJExecExpressionListP exprList_;
  bool                  equals_ { false };
};

#endif
