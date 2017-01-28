#ifndef CJExecStatement_H
#define CJExecStatement_H

#include <CJToken.h>

class CJExecStatement : public CJToken {
 public:
  explicit CJExecStatement(CJTokenP token=CJTokenP());

  const CJTokenP &token() const { return token_; }
  void setToken(const CJTokenP &token) { token_ = token; }

  CJValueP exec(CJavaScript *js) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  CJTokenP token_;
};

#endif
