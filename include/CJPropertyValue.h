#ifndef CJPropertyValue_H
#define CJPropertyValue_H

#include <CJTypes.h>
#include <COptVal.h>

class CJPropertyValue {
 public:
  explicit CJPropertyValue(CJValueP v=CJValueP()) : value_(v) { }

  CJPropertyValue(CJValueP v, const std::string &attr) :
   value_(v) {
    bool b = true;

    for (const auto &c : attr) {
      switch (c) {
        case '-': b             = false; break;
        case '+': b             = true ; break;
        case 'w': writable_     = b    ; break;
        case 'e': enumerable_   = b    ; break;
        case 'c': configurable_ = b    ; break;
      }
    }
  }

  CJValueP value() const { return value_; }
  void setValue(CJValueP v) { value_ = v; }

  bool isWriteable() const { return writable_.getValue(true); }
  void setWriteable(bool b) { writable_ = b; }

  bool isEnumerable() const { return enumerable_.getValue(true); }
  void setEnumerable(bool b) { enumerable_ = b; }

  bool isConfigurable() const { return configurable_.getValue(true); }
  void setConfigurable(bool b) { configurable_ = b; }

  CJValueP calcValue(CJavaScript *js) const;

 private:
  CJValueP value_;        // any value
  COptBool writable_;     // is writable
  COptBool enumerable_;   // is enumerable
  COptBool configurable_; // is configurable
};

#endif
