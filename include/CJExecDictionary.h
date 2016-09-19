#ifndef CJExecDictionary_H
#define CJExecDictionary_H

#include <CJToken.h>
#include <vector>
#include <map>
#include <iostream>

// var <identifiers> [= <value>] [, <identifiers> [= <value>], ...]
class CJExecDictionary : public CJToken {
 public:
  CJExecDictionary();

  void addDictionaryValue(const CJValueP &key, CJExecExpressionP expr);

  CJGetterSetterP getGetterSetter(const std::string &name);
  void setGetterSetter(const std::string &name, CJGetterSetterP gs);

  CJValueP exec(CJavaScript *js) override;

  void print(std::ostream &os) const override;

 private:
  struct DictionaryValue {
    CJValueP          key;
    CJExecExpressionP expr;

    DictionaryValue(const CJValueP &key1, CJExecExpressionP expr1) :
     key(key1), expr(expr1) {
    }
  };

  typedef std::vector<DictionaryValue>          DictionaryValues;
  typedef std::map<std::string,CJGetterSetterP> GSMap;

  DictionaryValues values_;
  GSMap            gsMap_;
};

#endif
