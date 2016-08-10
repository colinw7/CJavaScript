#ifndef CJExecContinue_H
#define CJExecContinue_H

#include <CJToken.h>
#include <iostream>

// continue
class CJExecContinue : public CJToken {
 public:
  CJExecContinue() :
   CJToken(CJToken::Type::Return) {
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    os << "continue";
  }
};

typedef std::shared_ptr<CJExecContinue> CJExecContinueP;

#endif
