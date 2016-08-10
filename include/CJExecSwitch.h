#ifndef CJExecSwitch_H
#define CJExecSwitch_H

#include <CJToken.h>
#include <vector>
#include <iostream>

// switch ( <expression> ) { [case <expr> : <exprList> ; break; ]* }
class CJExecSwitch : public CJToken {
 public:
  struct CaseBlock {
    CaseBlock(CJExecExpressionP expr1, CJExecBlockP block1) :
     expr(expr1), block(block1) {
    }

    CJExecExpressionP expr;
    CJExecBlockP      block;
  };

  struct DefaultBlock {
    CJExecBlockP block;
  };

 public:
  CJExecSwitch();

  void setExprList(CJExecExpressionListP exprList) {
    exprList_ = exprList;
  }

  void addCase(CJExecExpressionP expr, CJExecBlockP block) {
    caseBlocks_.push_back(CaseBlock(expr, block));
  }

  void setDefault(CJExecBlockP block) {
    defaultBlock_.block = block;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  typedef std::vector<CaseBlock> CaseBlocks;

  CJExecExpressionListP exprList_;
  CaseBlocks            caseBlocks_;
  DefaultBlock          defaultBlock_;
};

typedef std::shared_ptr<CJExecSwitch> CJExecSwitchP;

#endif
