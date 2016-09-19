#include <CJMath.h>
#include <CJDictionary.h>
#include <CJRealFunction.h>
#include <CJRandFunction.h>
#include <CJavaScript.h>
#include <CJUtil.h>

CJMath::
CJMath(CJavaScript *js)
{
  // Math Dictionary
  // TODO: mark as internal ? different behavior than user dictionary
  dict_ = CJDictionaryP(new CJDictionary(js));

  js->setProperty("Math", std::static_pointer_cast<CJValue>(dict_));

  double Eval = exp(1);

  dict_->setRealProperty(js, "E"      , Eval);
  dict_->setRealProperty(js, "LN10"   , log(10));
  dict_->setRealProperty(js, "LN2"    , log(2));
  dict_->setRealProperty(js, "LN10E"  , log10(Eval));
  dict_->setRealProperty(js, "LN2E"   , log2(Eval));
  dict_->setRealProperty(js, "PI"     , M_PI);
  dict_->setRealProperty(js, "SQRT1_2", sqrt(1.0/2.0));
  dict_->setRealProperty(js, "SQRT2"  , sqrt(2));

  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "abs"   , fabs )));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "acos"  , acos )));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "acosh" , acosh)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "asin"  , asin )));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "asinh" , asinh)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "atan"  , atan )));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJReal2Function(js, "atan2" , atan2)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "ceil"  , ceil )));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "cos"   , CJUtil::cos)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "cosh"  , cosh )));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "exp"   , CJUtil::exp)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "floor" , floor)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJReal2Function(js, "hypot" , hypot)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "log"   , CJUtil::log)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "log10" , CJUtil::log10)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "log2"  , CJUtil::log2)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMaxFunction  (js)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMinFunction  (js)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJReal2Function(js, "pow"   , CJUtil::pow)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJRandFunction (js)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "round" , CJUtil::round)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "sign"  , CJUtil::sign)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "sin"   , CJUtil::sin)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "sinh"  , sinh )));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "sqrt"  , CJUtil::sqrt)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "tan"   , CJUtil::tan)));
  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction (js, "tanh"  , tanh )));

  dict_->setFunctionProperty(js, CJFunctionBaseP(new CJMathFunction(js, "toString")));
}
