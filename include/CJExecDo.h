#ifndef CJExecDo_H
#define CJExecDo_H

#include <CJToken.h>
#include <iostream>

// do <block> while ( <expression> )
class CJExecDo : public CJToken {
 public:
  CJExecDo();

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

#endif
