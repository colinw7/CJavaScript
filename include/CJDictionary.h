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

  std::string toString() const override {
    std::ostringstream ss; ss << *this;

    return ss.str();
  }

  bool toBoolean() const override { return ! keyValues_.empty(); }

  double toReal() const override { return toBoolean(); }

  //---

  bool hasProperty() const override { return true; }

  CJValueP propertyValue(const std::string &key) const override {
    return CJNameSpace::getProperty(js_, key);
  }

  void setPropertyValue(const std::string &key, CJValueP value) override {
    CJNameSpace::setProperty(js_, key, value);
  }

  bool hasPropertyValue(const std::string &key) const override {
    return CJNameSpace::hasProperty(js_, key);
  }

  CJValue::KeyNames propertyNames() const override {
    CJValue::KeyNames names;
    for (const auto &n : CJNameSpace::keyNames())
      names.push_back(n);
    return names;
  }

#if 0
  void setIndexValue(CJValueP ivalue, CJValueP value) {
    std::string key = ivalue->toString();

    for (auto &kv : keyValues_) {
      if (kv.first == key) {
        kv.second = value;
        return;
      }
    }
  }

  bool hasIndexValue(CJavaScript *js, const std::string &key) const {
    return CJNameSpace::hasProperty(js, key);
  }
#endif

  std::vector<std::string> getFunctionNames() const;
  std::vector<std::string> getVariableNames() const;

  void print(std::ostream &os) const override {
    int i = 0;

    if (name_ != "")
      os << name_ << " ";

    os << "{";

    for (auto &kv : keyValues()) {
      if (i > 0)
        os << ",";

      os << " " << kv.first << ": ";

      if (kv.second)
        os << *kv.second;
      else
        os << "<null>";

      ++i;
    }

    if (i > 0)
      os << " ";

    os << "}";
  }

  friend std::ostream &operator<<(std::ostream &os, const CJDictionary &dict) {
    dict.print(os);

    return os;
  }

 protected:
  typedef std::map<std::string, CJValueP> Properties;

  CJavaScript*  js_ { 0 };
  std::string   name_;
  CJDictionaryP parent_;
};

#endif
