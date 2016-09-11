#ifndef CJUserFunction_H
#define CJUserFunction_H

#include <CJFunction.h>

// user defined function
class CJUserFunction : public CJFunction {
 public:
  typedef std::vector<std::string> Args;

 public:
  CJUserFunction(CJavaScript *js, const std::string &name,
                 const Args &args=Args(), CJExecBlockP block=CJExecBlockP());

  CJValue *dup(CJavaScript *js) const override {
    return new CJUserFunction(js, name_, args_, block_);
  }

  bool hasObjectValue() const override { return true; }

  void setScope(CJavaScript *js, CJDictionaryP parentScope=CJDictionaryP());

  const Args &args() const { return args_; }
  void setArgs(const Args &v) { args_ = v; }

  const CJExecBlockP &block() const { return block_; }
  void setBlock(const CJExecBlockP &v) { block_ = v; }

  const CJDictionaryP &scope() const { return scope_; }
  void setScope(const CJDictionaryP &v) { scope_ = v; }

  CJValueP getScopeProperty(const std::string &name) const;
  void setScopeProperty(const std::string &name, CJValueP value);

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override;

  friend std::ostream &operator<<(std::ostream &os, const CJUserFunction &rhs) {
    rhs.print(os);

    return os;
  }

 private:
  Args          args_;
  CJExecBlockP  block_;
  CJDictionaryP scope_;
};

#endif
