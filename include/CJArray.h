#ifndef CJArray_H
#define CJArray_H

#include <CJObj.h>
#include <deque>
#include <vector>

// Array Type
class CJArrayType : public CJObjType {
 public:
  static CJArrayTypeP instance(CJavaScript *js);

  CJArrayType(CJavaScript *js);

  CJValueP execType(CJavaScript *js, const std::string &name, const Values &values) override;

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

  static bool canCreateArrayFromValue(CJValueP value1);
  static CJArrayP createArrayFromValue(CJavaScript *js, CJValueP value1);

 private:
  void init();

 private:
  static CJArrayTypeP type_;
};

//-------

// Array Value
class CJArray : public CJObj {
 public:
  typedef std::vector<CJPropertyValue>   Values;
  typedef std::map<long,CJPropertyValue> IndValues;

 public:
  CJArray(CJavaScript *js, long n=0);
  CJArray(CJavaScript *js, const std::vector<CJValueP> &values);
  CJArray(CJavaScript *js, const IndValues &values, long len);

  CJArray(const CJArray &array);

  CJArray *dup(CJavaScript *) const override { return new CJArray(*this); }

  bool isArray() const override { return true; }

  const IndValues &indValues() const { return values_; }
  void setIndValues(const IndValues &values) { values_ = values; }

  void setValues(const std::vector<CJValueP> &values);
  void setValues(const IndValues &values, long len);

  //---

  COptReal toReal() const override;

  COptLong toInteger() const override;

  bool toBoolean() const override { return true; }

  //---

  void addValue(CJValueP value);

  CJValueP removeValue();

  void addFrontValue(CJValueP value);

  CJValueP removeFrontValue();

  bool hasIndex() const override { return true; }
  bool hasIndexValue(long ind) const override;
  CJValueP indexValue(long ind) const override;
  void setIndexValue(long ind, CJValueP value) override;
  void deleteIndexValue(long ind) override;

  bool isWritableIndex(long ind) const override;
  void setWritableIndex(long ind, bool b) override;

  bool isEnumerableIndex(long ind) const;
  void setEnumerableIndex(long ind, bool b=true);

  bool isConfigurableIndex(long ind) const;
  void setConfigurableIndex(long ind, bool b);

  bool hasValue(CJValueP value) const;

  COptLong length() const override;
  void setLength(long n);

  void reverse();

  void sort(CJFunctionP fn);

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 protected:
  IndValues values_;
  long      len_ { 0 };
};

#endif
