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

  //---

  bool toBoolean() const override { return ! values_.empty(); }

  COptReal toReal() const override { return COptReal(); }
  COptLong toInteger() const override { return COptLong(); }

  //---

  bool hasIndex() const override { return true; }
  bool hasIndexValue(long ind) const override;
  CJValueP indexValue(long ind) const override;
  void setIndexValue(long ind, CJValueP value) override;
  void deleteIndexValue(long ind) override;

  void addValue(CJValueP value);

  COptLong length() const override { return COptLong(values_.size()); }

  CJValueP callee() const { return callee_; }
  void setCallee(CJValueP value) { callee_ = value; }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 protected:
  Values   values_;
  CJValueP callee_;
};

#endif
