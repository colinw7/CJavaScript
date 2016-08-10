#ifndef CJDictionary_H
#define CJDictionary_H

#include <CJObjectType.h>
#include <CJValue.h>
#include <CJNameSpace.h>
#include <sstream>

// Dictionary Type
class CJDictionaryType : public CJObjectType {
 public:
  static CJObjectTypeP instance(CJavaScript *js);

  CJDictionaryType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjectTypeP type_;
};

//------

// Dictionary Value
// { <expression_pair> [, <expression_pair>]* }
class CJDictionary : public CJValue, public CJNameSpace {
 public:
  CJDictionary(CJavaScript *js, const std::string &name="",
               const KeyValues &keyValues=KeyValues());

  CJValue *dup(CJavaScript *js) const override {
    return new CJDictionary(js, name_, keyValues_);
  }

  const std::string &name() const { return name_; }

  CJDictionaryP getParent() const { return parent_; }
  void setParent(CJDictionaryP s) { assert(s.get() != this); parent_ = s; }

//void addScope   (CJDictionaryP scope);
//void removeScope(const std::string &name);

  //CJDictionaryP lookupScope(const std::string &name);

  std::string toString() const override {
    std::ostringstream ss; ss << *this;

    return ss.str();
  }

  bool toBoolean() const override { return ! keyValues_.empty(); }

  double toReal() const override { return toBoolean(); }

  CJValueP indexValue(const std::string &key) const { return CJNameSpace::getProperty(key); }

  void setIndexValue(CJavaScript *, const std::string &key, CJValueP value) {
    setProperty(key, value);
  }

  void setIndexValue(CJValueP ivalue, CJValueP value) {
    std::string key = ivalue->toString();

    for (auto &kv : keyValues_) {
      if (kv.first == key) {
        kv.second = value;
        return;
      }
    }
  }

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

 private:
  typedef std::map<std::string, CJValueP> Properties;

  std::string   name_;
  CJDictionaryP parent_;
};

#endif
