#ifndef CJIdentifier_H
#define CJIdentifier_H

#include <CJToken.h>

class CJIdentifier : public CJToken {
 public:
  CJIdentifier(const std::string &id);

  const std::string &name() const { return id_; }

  void print(std::ostream &os) const override { os << id_; }

 private:
  std::string id_;
};

#endif
