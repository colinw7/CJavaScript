#ifndef CJExecTry_H
#define CJExecTry_H

#include <CJToken.h>
#include <iostream>

// try <block> catch ( <identifiers> ) <block>
class CJExecTry : public CJToken {
 public:
  CJExecTry();

  void setTryBlock(CJExecBlockP block) {
    tryBlock_ = block;
  }

  void setCatchIdentifiers(CJExecIdentifiersP identifiers) {
    catchIdentifiers_ = identifiers;
  }

  void setCatchBlock(CJExecBlockP block) {
    catchBlock_ = block;
  }

  void setFinallyBlock(CJExecBlockP block) {
    finallyBlock_ = block;
  }

  CJValueP exec(CJavaScript *js) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJExecBlockP       tryBlock_;
  CJExecIdentifiersP catchIdentifiers_;
  CJExecBlockP       catchBlock_;
  CJExecBlockP       finallyBlock_;
};

#endif
