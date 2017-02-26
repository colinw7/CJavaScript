#ifndef CJNameSpace_H
#define CJNameSpace_H

#include <CJTypes.h>
#include <CJPropertyValue.h>
#include <map>
#include <set>
#include <vector>

class CJNameSpace {
 public:
  typedef std::map<std::string,CJPropertyValue> KeyValues;
  typedef std::set<std::string>                 KeyNames;
  typedef std::vector<std::string>              Names;
  typedef std::vector<CJValueP>                 Values;

 public:
  CJNameSpace();

  CJNameSpace(const CJNameSpace &ns);

  virtual ~CJNameSpace() { }

  const KeyValues &keyValues() const { return keyValues_; }

  const KeyNames &keyNames() const { return keyNames_; }

  void setRealProperty   (CJavaScript *js, const std::string &key, double r);
  void setIntegerProperty(CJavaScript *js, const std::string &key, long i);
  void setStringProperty (CJavaScript *js, const std::string &key, const std::string &str);
  void setBoolProperty   (CJavaScript *js, const std::string &key, bool b);

  void setFunctionProperty(CJavaScript *js, const std::string &key, CJFunctionBaseP function);

  void setFunctionProperty(CJavaScript *js, CJFunctionBaseP function,
                           CJObjTypeP objType=CJObjTypeP());

  std::string getStringProperty(CJavaScript *js, const std::string &key,
                                const std::string &def="") const;

  double getRealProperty(CJavaScript *js, const std::string &key, double def=0.0) const;

  bool getPropertyData(CJavaScript *js, const std::string &key, CJPropertyValue &data);
  void setPropertyData(CJavaScript *js, const std::string &key, const CJPropertyValue &data);

  virtual bool hasProperty(CJavaScript *js, const std::string &key, bool inherit=true) const;
  virtual CJValueP getProperty(CJavaScript *js, const std::string &key) const;
  virtual void setProperty(CJavaScript *js, const std::string &key, CJValueP value);

  virtual Names getPropertyNames(bool pseudo=true) const;

  bool deletePropertyIndices(CJavaScript *js, const std::string &key, const Values &values);

  virtual void deleteProperty(CJavaScript *js, const std::string &key);

  void addPseudoProperty(const std::string &key);

  const KeyNames &getPseudoPropertyNames() const { return keyNames_; }

  bool isWritableProperty(const std::string &key) const;
  void setWritableProperty(const std::string &key, bool b=true);

  virtual bool isEnumerableProperty(const std::string &key) const;
  void setEnumerableProperty(const std::string &key, bool b=true);

  bool isConfigurableProperty(const std::string &key) const;
  void setConfigurableProperty(const std::string &key, bool b);

  std::string toString() const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CJNameSpace &ns) {
    ns.print(os);

    return os;
  }

 protected:
  KeyValues keyValues_;
  KeyNames  keyNames_;
};

#endif
