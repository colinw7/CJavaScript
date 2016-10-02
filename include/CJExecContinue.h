#ifndef CJExecContinue_H
#define CJExecContinue_H

#include <CJToken.h>

// continue
class CJExecContinue : public CJToken {
 public:
  CJExecContinue();

  CJValueP exec(CJavaScript *js) override;

  CJExecIdentifiersP identifiers() const { return identifiers_; }
  void setIdentifiers(CJExecIdentifiersP identifiers) { identifiers_ = identifiers; }

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJExecIdentifiersP identifiers_;
};

#endif
