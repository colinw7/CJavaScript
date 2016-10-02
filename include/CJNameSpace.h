#ifndef CJNameSpace_H
#define CJNameSpace_H

#include <CJTypes.h>
#include <COptVal.h>
#include <map>
#include <set>
#include <vector>

class CJNameSpace {
 public:
  struct PropertyData {
    PropertyData(CJValueP v=CJValueP()) : value(v) { }

    PropertyData(CJValueP v, const std::string &attr) :
     value(v) {
      bool b = true;

      for (const auto &c : attr) {
        switch (c) {
          case '-': b            = false; break;
          case '+': b            = true ; break;
          case 'w': writable     = b    ; break;
          case 'e': enumerable   = b    ; break;
          case 'c': configurable = b    ; break;
        }
      }
    }

    CJValueP value;
    COptBool writable;
    COptBool enumerable;
    COptBool configurable;

    bool isEnumerable() const { return enumerable.getValue(true); }
  };

  typedef std::map<std::string,PropertyData> KeyValues;
  typedef std::set<std::string>              KeyNames;
  typedef std::vector<std::string>           Names;
  typedef std::vector<CJValueP>              Values;

 public:
  CJNameSpace(const KeyValues &keyValues=KeyValues()) :
   keyValues_(keyValues) {
  }

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

  bool getPropertyData(CJavaScript *js, const std::string &key, PropertyData &data);
  void setPropertyData(CJavaScript *js, const std::string &key, const PropertyData &data);

  virtual bool hasProperty(CJavaScript *js, const std::string &key, bool inherit=true) const;
  virtual CJValueP getProperty(CJavaScript *js, const std::string &key) const;
  virtual void setProperty(CJavaScript *js, const std::string &key, CJValueP value);

  Names getPropertyNames(bool pseudo=true) const;

  bool deleteProperty(CJavaScript *js, const std::string &key, const Values &values);
  void deleteProperty(const std::string &key);

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
