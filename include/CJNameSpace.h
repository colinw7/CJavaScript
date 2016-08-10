#ifndef CJNameSpace_H
#define CJNameSpace_H

#include <CJTypes.h>
#include <map>
#include <vector>

class CJNameSpace {
 public:
  typedef std::map<std::string,CJValueP> KeyValues;
  typedef std::vector<std::string>       Names;

 public:
  CJNameSpace(const KeyValues &keyValues=KeyValues()) :
   keyValues_(keyValues) {
  }

  virtual ~CJNameSpace() { }

  const KeyValues &keyValues() const { return keyValues_; }

  void setRealProperty   (CJavaScript *js, const std::string &key, double r);
  void setIntegerProperty(CJavaScript *js, const std::string &key, long i);
  void setStringProperty (CJavaScript *js, const std::string &key, const std::string &str);

  void setFunctionProperty(CJavaScript *js, const std::string &key, CJFunctionP function);
  void setFunctionProperty(CJavaScript *js, CJFunctionP function);

  std::string getStringProperty(const std::string &key, const std::string &def="") const;
  double      getRealProperty  (const std::string &key, double def=0.0) const;

  virtual void setProperty(const std::string &key, CJValueP value);

  virtual bool hasProperty(const std::string &key) const;

  virtual CJValueP getProperty(const std::string &key) const;

  Names getPropertyNames() const;

  void deleteProperty(const std::string &key);

 protected:
  KeyValues keyValues_;
};

#endif
