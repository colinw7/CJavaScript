#ifndef CJRandFunction_H
#define CJRandFunction_H

// function calling API with signature: double result = fn(double r1, double r2);
class CJRandFunction : public CJFunction {
 public:
  CJRandFunction(CJavaScript *js) :
   CJFunction(js, "random", CJFunction::Type::Random) {
  }

  CJValue *dup(CJavaScript *js) const override { return new CJRandFunction(js); }

  CJValueP exec(CJavaScript *js, const Values &values) override;

  void print(std::ostream &os) const override {
    os << "double random()";
  }
};

#endif
