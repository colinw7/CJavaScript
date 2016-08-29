#ifndef CJMath_H
#define CJMath_H

#include <CJTypes.h>

class CJMath {
 public:
  CJMath(CJavaScript *js);

 private:
  CJDictionaryP dict_;
};

typedef std::shared_ptr<CJMath> CJMathP;

#endif
