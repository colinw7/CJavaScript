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

#include <CJFunction.h>
#include <CJson.h>

class CJJSONParseFunction : public CJFunction {
 public:
  CJJSONParseFunction(CJavaScript *js) :
   CJFunction(js, "parse", CJFunction::Type::JSONStringify) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJJSONParseFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "parse";
  }

 private:
  CJValueP toValue(CJavaScript *js, CJValueP key, CJson::Value *value,
                   CJFunctionP func=CJFunctionP());

  CJValueP callFunc(CJavaScript *js, CJValueP key, CJValueP value, CJFunctionP func);
};

//------

class CJJSONStringifyFunction : public CJFunction {
 public:
  CJJSONStringifyFunction(CJavaScript *js) :
   CJFunction(js, "stringify", CJFunction::Type::JSONStringify) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJJSONStringifyFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "stringify";
  }

 private:
  bool stringify(CJavaScript *js, CJValueP key, CJValueP value,
                 std::string &str, bool &skip) const;

  std::string encodeString(const std::string &s) const;

 private:
  CJValueP    ivalue_;
  CJFunctionP toJSON_;
  CJFunctionP replacer_;
  CJArrayP    filter_;
};

#endif
