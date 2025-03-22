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
  CJJSONFunction(CJavaScript *js, const std::string &name);

  CJJSONFunction(const CJJSONFunction &fn);

  CJValue *dup(CJavaScript *) const override { return new CJJSONFunction(*this); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  std::string toString() const override;

  void print(std::ostream &os) const override;

 private:
  class Indent {
   public:
    Indent() { }

    void setInteger(int i) {
      i_   = i;
      str_ = "";

      while (int(str_.size()) < i)
        str_ += " ";
    }

    void setString(const std::string &s) {
      s_ = s;

      str_ = s;
    }

    bool isValid() const { return i_ || s_; }

    std::string str() const { return str_; }

    std::string depthStr(int depth) const {
      std::string s;

      for (int i = 0; i < depth; ++i)
        s += str();

      return s;
    }

   private:
    OptInt      i_;
    OptString   s_;
    std::string str_;
  };

 private:
  CJValueP parseToValue(CJavaScript *js, CJValueP key, CJson::ValueP value,
                        CJFunctionBaseP func=CJFunctionBaseP());

  CJValueP parseCallFunc(CJavaScript *js, CJValueP key, CJValueP value, CJFunctionBaseP func,
                         CJDictionaryP thisValue=CJDictionaryP());

  bool stringify(CJavaScript *js, CJValueP key, CJValueP value, const Indent &indent,
                 int depth, std::string &str, bool &skip) const;

  std::string encodeString(const std::string &s) const;

 private:
  std::string     name_;
  CJValueP        ivalue_;
  CJFunctionBaseP toJSON_;
  CJFunctionBaseP replacer_;
  CJArrayP        filter_;
};

#endif
