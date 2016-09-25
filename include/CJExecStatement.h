#ifndef CJExecStatement_H
#define CJExecStatement_H

#include <CJToken.h>

class CJExecStatement : public CJToken {
 public:
  CJExecStatement(CJTokenP token=CJTokenP());

  const CJTokenP &token() const { return token_; }
  void setToken(const CJTokenP &token) { token_ = token; }

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const;

 private:
  CJTokenP token_;
};

#endif
