#ifndef CJNameSpace_H
#define CJNameSpace_H

#include <CJTypes.h>
#include <map>
#include <set>
#include <vector>

class CJNameSpace {
 public:
  typedef std::map<std::string,CJValueP> KeyValues;
  typedef std::set<std::string>          KeyNames;
  typedef std::vector<std::string>       Names;
  typedef std::map<std::string,bool>     ReadOnly;

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

  void setFunctionProperty(CJavaScript *js, const std::string &key, CJFunctionP function);
  void setFunctionProperty(CJavaScript *js, CJFunctionP function);

  std::string getStringProperty(CJavaScript *js, const std::string &key,
                                const std::string &def="") const;

  double getRealProperty(CJavaScript *js, const std::string &key, double def=0.0) const;

  virtual void setProperty(CJavaScript *js, const std::string &key, CJValueP value);
  virtual bool hasProperty(CJavaScript *js, const std::string &key) const;
  virtual CJValueP getProperty(CJavaScript *js, const std::string &key) const;

  Names getPropertyNames(bool pseudo=true) const;

  void deleteProperty(const std::string &key);

  void addPseudoProperty(const std::string &key);

  const KeyNames &getPseudoPropertyNames() const { return keyNames_; }

  bool isReadOnlyProperty(const std::string &key) const;
  void setReadOnlyProperty(const std::string &key, bool b=true);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CJNameSpace &ns) {
    ns.print(os);

    return os;
  }

 protected:
  KeyValues keyValues_;
  KeyNames  keyNames_;
  ReadOnly  readOnly_;
};

#endif
