#ifndef CJExecWhile_H
#define CJExecWhile_H

#include <CJToken.h>
#include <iostream>

// while ( <expression> ) <block>
class CJExecWhile : public CJToken {
 public:
  CJExecWhile();

  void setExprList(CJExecExpressionListP exprList) {
    exprList_ = exprList;
  }

  void setBlock(CJExecBlockP block) {
    block_ = block;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  CJExecExpressionListP exprList_;
  CJExecBlockP          block_;
};

typedef std::shared_ptr<CJExecWhile> CJExecWhileP;

#endif
