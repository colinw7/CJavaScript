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

  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "abs"   , fabs )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "acos"  , acos )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "acosh" , acosh)));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "asin"  , asin )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "asinh" , asinh)));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "atan"  , atan )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJReal2Function(js, "atan2" , atan2)));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "ceil"  , ceil )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "cos"   , cos  )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "cosh"  , cosh )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "exp"   , exp  )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "floor" , floor)));
  dict_->setFunctionProperty(js, CJFunctionP(new CJReal2Function(js, "hypot" , hypot)));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "log"   , log  )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "log10" , log10)));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "log2"  , log2 )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJReal2Function(js, "max"   , CJUtil::max)));
  dict_->setFunctionProperty(js, CJFunctionP(new CJReal2Function(js, "min"   , CJUtil::min)));
  dict_->setFunctionProperty(js, CJFunctionP(new CJReal2Function(js, "pow"   , pow  )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRandFunction (js)));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "round" , round)));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "sign"  , CJUtil::sign)));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "sin"   , sin  )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "sinh"  , sinh )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "sqrt"  , sqrt )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "tan"   , tan  )));
  dict_->setFunctionProperty(js, CJFunctionP(new CJRealFunction (js, "tanh"  , tanh )));
}
