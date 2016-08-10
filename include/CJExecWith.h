#ifndef CJExecWith_H
#define CJExecWith_H

#include <CJToken.h>
#include <iostream>

// with (<identifiers>) { <block> }
class CJExecWith : public CJToken {
 public:
  CJExecWith();

  CJExecIdentifiersP identifiers() const { return identifiers_; }
  void setIdentifiers(CJExecIdentifiersP identifiers) { identifiers_ = identifiers; }

  void setBlock(CJExecBlockP block) {
    block_ = block;
  }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  CJExecIdentifiersP identifiers_;
  CJExecBlockP       block_;
};

typedef std::shared_ptr<CJExecWith> CJExecWithP;

#endif
