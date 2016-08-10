#ifndef CJArray_H
#define CJArray_H

#include <CJObjectType.h>
#include <CJValue.h>
#include <deque>
#include <vector>
#include <algorithm>

// Array Type
class CJArrayType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CJArrayType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjectTypeP type_;
};

//-------

// Array Value
class CJArray : public CJValue {
 public:
  typedef std::deque<CJValueP> Values;

 public:
  CJArray(CJavaScript *js, int n=0);

  CJArray(CJavaScript *js, const Values &values);
  CJArray(CJavaScript *js, const std::vector<CJValueP> &values);

  CJArray *dup(CJavaScript *js) const override { return new CJArray(js, values_); }

  bool isProtoValue() const override { return true; }

  Values values() const { return values_; }
  void setValues(const Values &values) { values_ = values; }

  void setValues(const std::vector<CJValueP> &values);

  std::string toString() const override { return ""; }

  double toReal() const override { return 0; }

  bool toBoolean() const override { return ! values_.empty(); }

  void addValue(CJValueP value);

  CJValueP removeValue();

  void addFrontValue(CJValueP value);

  CJValueP removeFrontValue();

  bool hasIndex() const override { return true; }

  CJValueP indexValue(int i) const override;

  void setIndexValue(int i, CJValueP value) override;

  int length() const override { return values_.size(); }

  void reverse();

  void sort();

  void print(std::ostream &os) const override;

 private:
  Values values_;
};

typedef std::shared_ptr<CJArray> CJArrayP;

#endif
