#ifndef CJExecDelete_H
#define CJExecDelete_H

#include <CJToken.h>
#include <iostream>

// delete
class CJExecDelete : public CJToken {
 public:
  CJExecDelete();

  CJValueP exec(CJavaScript *js) override;

  CJExecIdentifiersP identifiers() const { return identifiers_; }
  void setIdentifiers(CJExecIdentifiersP identifiers) { identifiers_ = identifiers; }

  void print(std::ostream &os) const override;

 private:
  CJExecIdentifiersP identifiers_;
};

typedef std::shared_ptr<CJExecDelete> CJExecDeleteP;

#endif
