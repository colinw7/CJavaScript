#ifndef CJExecIdentifiers_H
#define CJExecIdentifiers_H

#include <CJToken.h>
#include <CJIdentifier.h>
#include <vector>
#include <iostream>

// <identifer> [.<identifier]*
class CJExecIdentifiers : public CJToken, public std::enable_shared_from_this<CJExecIdentifiers> {
 public:
  typedef std::vector<CJIdentifier *> Identifiers;

 public:
  CJExecIdentifiers();

  bool isThis() const { return isThis_; }
  void setIsThis(bool b) { isThis_ = b; }

  void addIdentifier(CJIdentifier *identifier) {
    identifiers_.push_back(identifier);
  }

  const Identifiers &identifiers() const { return identifiers_; }

  int numIdentifiers() const { return identifiers_.size(); }

  void setEValue(CJValueP evalue) { evalue_ = evalue; }

  CJValueP exec(CJavaScript *js) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  bool        isThis_ { false };
  Identifiers identifiers_;
  CJValueP    evalue_;
};

#endif
