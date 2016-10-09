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
  dict_ = js->createDictValue();

  js->setProperty("Math", dict_);

  double Eval = exp(1);

  dict_->setRealProperty(js, "E"      , Eval);
  dict_->setRealProperty(js, "LN10"   , log(10));
  dict_->setRealProperty(js, "LN2"    , log(2));
  dict_->setRealProperty(js, "LN10E"  , log10(Eval));
  dict_->setRealProperty(js, "LN2E"   , log2(Eval));
  dict_->setRealProperty(js, "PI"     , M_PI);
  dict_->setRealProperty(js, "SQRT1_2", sqrt(1.0/2.0));
  dict_->setRealProperty(js, "SQRT2"  , sqrt(2));

  dict_->setFunctionProperty(js, js->createMathFunction ("abs"   , fabs ));
  dict_->setFunctionProperty(js, js->createMathFunction ("acos"  , acos ));
  dict_->setFunctionProperty(js, js->createMathFunction ("acosh" , acosh));
  dict_->setFunctionProperty(js, js->createMathFunction ("asin"  , asin ));
  dict_->setFunctionProperty(js, js->createMathFunction ("asinh" , asinh));
  dict_->setFunctionProperty(js, js->createMathFunction ("atan"  , atan ));
  dict_->setFunctionProperty(js, js->createReal2Function("atan2" , atan2));
  dict_->setFunctionProperty(js, js->createMathFunction ("ceil"  , ceil ));
  dict_->setFunctionProperty(js, js->createMathFunction ("cos"   , CJUtil::cos));
  dict_->setFunctionProperty(js, js->createMathFunction ("cosh"  , cosh ));
  dict_->setFunctionProperty(js, js->createMathFunction ("exp"   , CJUtil::exp));
  dict_->setFunctionProperty(js, js->createMathFunction ("floor" , floor));
  dict_->setFunctionProperty(js, js->createReal2Function("hypot" , hypot));
  dict_->setFunctionProperty(js, js->createMathFunction ("log"   , CJUtil::log));
  dict_->setFunctionProperty(js, js->createMathFunction ("log10" , CJUtil::log10));
  dict_->setFunctionProperty(js, js->createMathFunction ("log2"  , CJUtil::log2));
  dict_->setFunctionProperty(js, js->createMaxFunction  ());
  dict_->setFunctionProperty(js, js->createMinFunction  ());
  dict_->setFunctionProperty(js, js->createReal2Function("pow"   , CJUtil::pow));
  dict_->setFunctionProperty(js, js->createRandFunction ());
  dict_->setFunctionProperty(js, js->createMathFunction ("round" , CJUtil::round));
  dict_->setFunctionProperty(js, js->createMathFunction ("sign"  , CJUtil::sign));
  dict_->setFunctionProperty(js, js->createMathFunction ("sin"   , CJUtil::sin));
  dict_->setFunctionProperty(js, js->createMathFunction ("sinh"  , sinh ));
  dict_->setFunctionProperty(js, js->createMathFunction ("sqrt"  , CJUtil::sqrt));
  dict_->setFunctionProperty(js, js->createMathFunction ("tan"   , CJUtil::tan));
  dict_->setFunctionProperty(js, js->createMathFunction ("tanh"  , tanh ));

  dict_->setFunctionProperty(js, js->createMathFunction("toString"));
}
