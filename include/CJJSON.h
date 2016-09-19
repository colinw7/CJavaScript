#ifndef CJJSON_H
#define CJJSON_H

#include <CJTypes.h>

class CJJSON {
 public:
  CJJSON(CJavaScript *js);

 private:
  CJDictionaryP dict_;
};

//------

#include <CJFunctionBase.h>
#include <CJson.h>

class CJJSONFunction : public CJFunctionBase {
 public:
  CJJSONFunction(CJavaScript *js, const std::string &name) :
   CJFunctionBase(js, name, CJFunctionBase::Type::JSON), name_(name) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJJSONFunction(js, name_); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "parse";
  }

 private:
  CJValueP parseToValue(CJavaScript *js, CJValueP key, CJson::Value *value,
                        CJFunctionBaseP func=CJFunctionBaseP());

  CJValueP parseCallFunc(CJavaScript *js, CJValueP key, CJValueP value, CJFunctionBaseP func);

  bool stringify(CJavaScript *js, CJValueP key, CJValueP value,
                 std::string &str, bool &skip) const;

  std::string encodeString(const std::string &s) const;

 private:
  std::string     name_;
  CJValueP        ivalue_;
  CJFunctionBaseP toJSON_;
  CJFunctionBaseP replacer_;
  CJArrayP        filter_;
};

#endif
