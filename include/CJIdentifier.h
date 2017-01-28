#ifndef CJIdentifier_H
#define CJIdentifier_H

#include <CJToken.h>

class CJIdentifier : public CJToken {
 public:
  explicit CJIdentifier(const std::string &id);

  const std::string &name() const { return id_; }

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  std::string id_;
};

#endif
