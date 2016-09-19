#ifndef CJArguments_H
#define CJArguments_H

#include <CJObj.h>
#include <deque>
#include <vector>

// Arguments Type
class CJArgumentsType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJArgumentsType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//-------

// Arguments Value
class CJArguments : public CJObj {
 public:
  typedef std::deque<CJValueP> Values;

 public:
  CJArguments(CJavaScript *js, const Values &values=Values());

  CJArguments *dup(CJavaScript *js) const override { return new CJArguments(js, values_); }

  Values values() const { return values_; }
  void setValues(const Values &values) { values_ = values; }

  std::string toString() const override {
    std::ostringstream ss; ss << *this;

    return ss.str();
  }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return ! values_.empty(); }

  bool hasIndex() const override { return true; }
  bool hasIndexValue(int ind) const override;
  CJValueP indexValue(int ind) const override;
  void setIndexValue(int ind, CJValueP value) override;
  void deleteIndexValue(int ind) override;

  void addValue(CJValueP value);

  long length() const override { return values_.size(); }

  CJValueP callee() const { return callee_; }
  void setCallee(CJValueP value) { callee_ = value; }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  void print(std::ostream &os) const override;

 protected:
  Values   values_;
  CJValueP callee_;
};

#endif
