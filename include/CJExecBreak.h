#ifndef CJExecBreak_H
#define CJExecBreak_H

#include <CJToken.h>
#include <iostream>

// break
class CJExecBreak : public CJToken {
 public:
  CJExecBreak();

  CJValueP exec(CJavaScript *js) override;

  CJExecIdentifiersP identifiers() const { return identifiers_; }
  void setIdentifiers(CJExecIdentifiersP identifiers) { identifiers_ = identifiers; }

  void print(std::ostream &os) const override;

 private:
  CJExecIdentifiersP identifiers_;
};

#endif
