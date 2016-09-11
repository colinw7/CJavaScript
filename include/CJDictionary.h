#ifndef CJDictionary_H
#define CJDictionary_H

#include <CJObjType.h>
#include <CJValue.h>
#include <CJNameSpace.h>
#include <sstream>

// Dictionary Type
class CJDictionaryType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJDictionaryType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//------

// Dictionary Value
// { <expression_pair> [, <expression_pair>]* }
class CJDictionary : public CJValue, public CJNameSpace {
 public:
  CJDictionary(CJavaScript *js, CJObjTypeP type);

  CJDictionary(CJavaScript *js, const std::string &name="",
               const KeyValues &keyValues=KeyValues());

  CJValue *dup(CJavaScript *js) const override {
    return new CJDictionary(js, name_, keyValues_);
  }

  CJavaScript *js() const { return js_; }

  const std::string &name() const { return name_; }

  CJDictionaryP getParent() const { return parent_; }
  void setParent(CJDictionaryP s) { assert(s.get() != this); parent_ = s; }

  std::string toString() const override;

  bool toBoolean() const override { return ! keyValues_.empty(); }

  double toReal() const override { return toBoolean(); }

  //---

  bool hasProperty() const override { return true; }

  CJValueP propertyValue(const std::string &key) const override;

  void setPropertyValue(const std::string &key, CJValueP value) override;

  bool hasPropertyValue(const std::string &key) const override;

  bool isReadOnlyProperty(const std::string &ind) const override;
  void setReadOnlyProperty(const std::string &ind, bool b) override;

  CJValue::KeyNames propertyNames() const override;

  std::vector<std::string> getFunctionNames() const;
  std::vector<std::string> getVariableNames() const;

  void print(std::ostream &os) const override;

  friend std::ostream &operator<<(std::ostream &os, const CJDictionary &dict) {
    dict.print(os);

    return os;
  }

 protected:
  typedef std::map<std::string, CJValueP> Properties;
  typedef std::set<std::string>           ReadOnly;

  CJavaScript*  js_ { 0 };
  std::string   name_;
  CJDictionaryP parent_;
  ReadOnly      readOnly_;
};

#endif
