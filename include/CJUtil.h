#ifndef CJUtil_H
#define CJUtil_H

#include <COSNaN.h>

namespace CJUtil {
  inline double getNaN() {
    double r;

    COSNaN::set_nan(&r);

    return r;
  }

  inline int hexCharValue(char c) {
    if (isdigit(c)) return (c - '0');

    return (tolower(c) - 'a' + 10);
  }

  inline double realModulus(double real1, double real2, int *error_code=0) {
    if (error_code) *error_code = 0;

    if (COSNaN::is_nan(real1) || COSNaN::is_nan(real2)) {
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

  inline double max(double a, double b) { return std::max(a, b); }
  inline double min(double a, double b) { return std::min(a, b); }

  inline double sign(double a) { if (a < 0) return -1; else if (a > 0) return 1; else return 0; }
}

#endif
