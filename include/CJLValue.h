#ifndef CJLValue_H
#define CJLValue_H

#include <CJValue.h>

class CJLValue : public CJValue {
 public:
  CJLValue(CJObjTypeP valueType);

  virtual ~CJLValue() { }

  virtual CJValueP value() const = 0;
  virtual void setValue(CJValueP value) = 0;

  virtual bool isWritable() const { return true; }
  virtual void setWritable(bool) { }
};

#endif
