#ifndef CJExecVoid_H
#define CJExecVoid_H

#include <CJToken.h>
#include <iostream>

// void
class CJExecVoid : public CJToken {
 public:
  CJExecVoid();

  CJValueP exec(CJavaScript *js) override;

  CJExecIdentifiersP identifiers() const { return identifiers_; }
  void setIdentifiers(CJExecIdentifiersP identifiers) { identifiers_ = identifiers; }

  void print(std::ostream &os) const override;

 private:
  CJExecIdentifiersP identifiers_;
};

#endif
