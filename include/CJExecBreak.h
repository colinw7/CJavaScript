#ifndef CJExecBreak_H
#define CJExecBreak_H

#include <CJToken.h>
#include <iostream>

// break
class CJExecBreak : public CJToken {
 public:
  CJExecBreak() :
   CJToken(CJToken::Type::Break) {
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override {
    os << "break";
  }
};

typedef std::shared_ptr<CJExecBreak> CJExecBreakP;

#endif
