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

  explicit CJDictionaryType(CJavaScript *js);

  CJValueP execType(CJavaScript *js, const std::string &name, const Values &values) override;

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

  CJDictionary(CJavaScript *js, const std::string &name);

  CJDictionary(const CJDictionary &dict);

  CJValue *dup(CJavaScript *) const override { return new CJDictionary(*this); }

  void makeUnique();

  CJavaScript *js() const { return js_; }

  const std::string &name() const { return name_; }

  CJDictionaryP getParent() const { return parent_; }
  void setParent(CJDictionaryP s) { assert(s.get() != this); parent_ = s; }

  bool isDictionary() const override { return true; }

  //---

  bool toBoolean() const override { return true; }

  OptReal toReal() const override { return OptReal(); }

  OptLong toInteger() const override { return OptLong(); }

  //---

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  //---

  bool hasProperty() const override { return true; }

  using CJNameSpace::hasProperty;

  bool hasPropertyValue(const std::string &key, bool inherit=true) const override;
  CJValueP propertyValue(const std::string &key) const override;
  void setPropertyValue(const std::string &key, CJValueP value) override;
  void configPropertyValue(const std::string &key, CJValueP value) override;
  void deletePropertyValue(const std::string &key) override;

  CJValue::KeyNames propertyNames() const override;

  //---

  std::vector<std::string> getFunctionNames() const;
  std::vector<std::string> getVariableNames() const;

  //---

  std::string toString() const override;

  void print(std::ostream &os) const override;

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
