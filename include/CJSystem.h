#ifndef CJSystem_H
#define CJSystem_H

#include <CJDictionary.h>

class CJSystem : public CJDictionary {
 public:
  CJSystem(CJavaScript *js);
};

typedef std::shared_ptr<CJSystem> CJSystemP;

#endif
