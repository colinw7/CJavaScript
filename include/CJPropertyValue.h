#ifndef CJPropertyValue_H
#define CJPropertyValue_H

#include <CJTypes.h>
#include <COptVal.h>

struct CJPropertyValue {
  CJPropertyValue(CJValueP v=CJValueP()) : value(v) { }

  CJPropertyValue(CJValueP v, const std::string &attr) :
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

  CJValueP calcValue(CJavaScript *js) const;
};

#endif
