#ifndef CJUtil_H
#define CJUtil_H

#include <COSNaN.h>

namespace CJUtil {
  inline double getNaN() {
    double r;

    COSNaN::set_nan(&r);

    return r;
  }

  inline bool isNaN(double r) {
    return COSNaN::is_nan(r);
  }

  //---

  inline double getPosInf() {
    double r;

    COSNaN::set_pos_inf(r);

    return r;
  }

  inline double getNegInf() {
    double r;

    COSNaN::set_neg_inf(r);

    return r;
  }

  inline bool isInf(double r) {
    return COSNaN::is_inf(r);
  }

  inline bool isPosInf(double r) {
    return COSNaN::is_pos_inf(r);
  }

  inline bool isNegInf(double r) {
    return COSNaN::is_neg_inf(r);
  }

  //---

  inline int hexCharValue(char c) {
    if (isdigit(c)) return (c - '0');

    return (tolower(c) - 'a' + 10);
  }

  inline double realModulus(double real1, double real2, int *error_code=0) {
    if (error_code) *error_code = 0;

    if (isNaN(real1) || isNaN(real2)) {
      //if (error_code) *error_code = int(CExprErrorType::NAN_OPERATION);
      return getNaN();
    }

    if (real2 == 0.0) {
      //if (error_code) *error_code = int(CExprErrorType::DIVIDE_BY_ZERO);
      return getNaN();
    }

    int factor = int(real1/real2);

    double result = real1 - (real2*factor);

    return result;
  }

  inline std::string realToString(double r) {
    if      (isPosInf(r))
      return "Infinity";
    else if (isNegInf(r))
      return "-Infinity";
    else if (isNaN(r))
      return "NaN";

    std::ostringstream ss;

    ss.precision(std::numeric_limits<double>::max_digits10);

    ss << r;

    return ss.str();
  }

  inline double max(double a, double b) { return std::max(a, b); }
  inline double min(double a, double b) { return std::min(a, b); }

  inline double sign(double a) { if (a < 0) return -1; else if (a > 0) return 1; else return 0; }
}

#endif
