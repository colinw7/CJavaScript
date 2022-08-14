#ifndef CJUtil_H
#define CJUtil_H

#include <COSNaN.h>
#include <cmath>

namespace CJUtil {
  template<typename T>
  inline T clamp(T val, T low, T high) {
    if (val < low ) return low;
    if (val > high) return high;
    return val;
  }

  //---

  inline double getNaN() {
    static COptReal nanValue;

    if (! nanValue.isValid()) {
      double r;

      COSNaN::set_nan(&r);

      nanValue = r;
    }

    return nanValue.getValue();
  }

  inline bool isNaN(double r) {
    return COSNaN::is_nan(r) && ! COSNaN::is_inf(r);
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

  //---

  inline bool isBaseChar(char c, int base) {
    int d = -1;

    if      (isdigit(c))
      d = (c - '0');
    else if (isalpha(c))
      d = (tolower(c) - 'a' + 10);

    return (d >= 0 && d < base);
  }

  inline int baseCharValue(char c, int base) {
    int d = -1;

    if      (isdigit(c))
      d = (c - '0');
    else if (isalpha(c))
      d = (tolower(c) - 'a' + 10);

    assert(d >= 0 && d < base);

    return d;
  }

  //---

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

    long factor = int(real1/real2);

    double result = real1 - (real2*double(factor));

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

  inline std::string integerToString(double r, int base=10) {
    static const char ichars[] = "0123456789abcdefghijklmnopqrstuvwxyz";

    std::string str;

    bool isNeg = (r < 0);

    double r1 = std::abs(r);

    if (r1 < 1)
      return "0";

    while (r1 >= 1) {
      auto i1 = long(fmod(r1, base));

      str += ichars[i1];

      r1 = (r1 - double(i1))/double(base);
    }

    int len = int(str.size());

    for (int i = 0; i < len/2; ++i)
      std::swap(str[size_t(i)], str[size_t(len - i - 1)]);

    if (isNeg)
      return "-" + str;
    else
      return str;
  }

  inline std::string realToBaseString(double r, int base) {
    static const char ichars[] = "0123456789abcdefghijklmnopqrstuvwxyz";

    if      (isPosInf(r))
      return "Infinity";
    else if (isNegInf(r))
      return "-Infinity";
    else if (isNaN(r))
      return "NaN";

    //---

    bool isNeg = (r < 0);

    double r1 = std::abs(r);

    //---

    double r2 = std::floor(r1);

    std::string lstr = integerToString(r2, base);

    //---

    //uint digits = std::numeric_limits<double>::max_digits10;
    uint digits = 256;

    std::string rstr;

    r2 = r1 - std::floor(r1);

    while (r2 > 0 && rstr.size() < digits) {
      long i1 = long(r2*base);

      rstr += ichars[i1];

      r2 = r2*double(base) - double(i1);
    }

    //---

    std::string str;

    if (isNeg)
      str += "-";

    if (lstr != "")
      str += lstr;
    else
      str += "0";

    if (rstr != "")
      str += "." + rstr;

    return str;
  }

  inline bool isInteger(double r) {
    return (double(long(r)) == r);
  }

  //---

  inline std::string nchars(int n, char c) {
    std::string str;

    for (int i = 0; i < n; ++i)
      str += c;

    return str;
  }

  //---

  inline double max(double a, double b) {
    if (isNaN(a) || isNaN(b)) return getNaN();

    if (isPosInf(a)) return a;
    if (isPosInf(b)) return b;

    if (isNegInf(a)) return b;
    if (isNegInf(b)) return a;

    return std::max(a, b);
  }

  inline double min(double a, double b) {
    if (isNaN(a) || isNaN(b)) return getNaN();

    if (isNegInf(a)) return a;
    if (isNegInf(b)) return b;

    if (isPosInf(a)) return b;
    if (isPosInf(b)) return a;

    return std::min(a, b);
  }

  inline double sign(double x) {
    if (isNaN   (x)) return getNaN();
    if (isPosInf(x)) return 1;
    if (isNegInf(x)) return -1;

    if      (x < 0) return -1;
    else if (x > 0) return  1;
    else            return  0;
  }

  inline double round(double x) {
    /* static const double kIntegerThreshold = 1LLU << 52;
    double jsround(double x) {
      double absx = fabs(x);
      if (absx > kIntegerThreshold) {
        // x is already integral
        return x;
      } else if (absx < 0.5) {
        // x may suffer precision loss when adding 0.5
        // round to +/- 0
        return copysign(0, x);
      } else {
        // normal rounding.
        // ensure negative values stay negative.
        return copysign(floor(x + 0.5), x);
      }
    }
    */

    if (isNaN   (x)) return getNaN();
    if (isPosInf(x)) return getPosInf();
    if (isNegInf(x)) return getNegInf();

    if (x >= 0.0)
      return std::round(x);
    else
      return std::round(x + 0.00001);
  }

  inline double exp(double x) {
    if (isNaN   (x)) return getNaN();
    if (isPosInf(x)) return getPosInf();
    if (isNegInf(x)) return 0.0;

    return std::exp(x);
  }

  inline double sqrt(double x) {
    if (isNaN(x)) return getNaN();

    if (x < 0.0) return getNaN();

    if (isPosInf(x)) return getPosInf();
    if (isNegInf(x)) return getNaN();

    return std::sqrt(x);
  }

  inline double pow(double x, double y) {
    if (isNaN(y)) return getNaN();

    if (y == 0.0) return 1.0;

    if (isNaN(x)) return getNaN();

    if (isPosInf(y)) {
      if      (x > 1.0) return getPosInf();
      else if (x < 1.0) return 0.0;
      else              return getNaN();
    }

    if (isNegInf(y)) {
      if      (x > 1.0) return 0.0;
      else if (x < 1.0) return getPosInf();
      else              return getNaN();
    }

    if (isPosInf(x)) {
      if (y >= 1.0) return getPosInf();
      else          return 0.0;
    }

    if (isNegInf(x)) {
      // TODO: odd integer
      if (isInteger(y) && long(y) & 1)
        return getNegInf();

      if (y >= 1.0) return getPosInf();
      else          return 0.0;
    }

    if (x == 0) {
      if (y < 0) return getPosInf();
      else       return 0.0;
    }

    if (x < 0 && ! isInteger(y))
      return getNaN();

    double r = std::pow(x, y);

    if (errno != 0)
      return getNaN();

    return r;
  }

  inline double log(double x) {
    if (isNaN   (x)) return getNaN();
    if (isPosInf(x)) return getPosInf();
    if (isNegInf(x)) return getNaN();

    if (x <  0) return getNaN();
    if (x == 0) return getNegInf();

    return std::log(x);
  }

  inline double log10(double x) {
    if (isNaN   (x)) return getNaN();
    if (isPosInf(x)) return getPosInf();
    if (isNegInf(x)) return getNaN();

    if (x <  0) return getNaN();
    if (x == 0) return getNegInf();

    return std::log10(x);
  }

  inline double log2(double x) {
    if (isNaN   (x)) return getNaN();
    if (isPosInf(x)) return getPosInf();
    if (isNegInf(x)) return getNaN();

    if (x <  0) return getNaN();
    if (x == 0) return getNegInf();

    return std::log2(x);
  }

  inline double cos(double x) {
    if (isNaN   (x)) return getNaN();
    if (isPosInf(x)) return getNaN();
    if (isNegInf(x)) return getNaN();

    return std::cos(x);
  }

  inline double sin(double x) {
    if (isNaN   (x)) return getNaN();
    if (isPosInf(x)) return getNaN();
    if (isNegInf(x)) return getNaN();

    return std::sin(x);
  }

  inline double tan(double x) {
    if (isNaN   (x)) return getNaN();
    if (isPosInf(x)) return getNaN();
    if (isNegInf(x)) return getNaN();

    return std::tan(x);
  }

  //---

  inline double maxReal() { return std::numeric_limits<double>::max(); }
  inline double minReal() { return std::numeric_limits<double>::min(); }

  inline long maxInteger() { return  long(std::numeric_limits<unsigned int>::max()); }
  inline long minInteger() { return -long(std::numeric_limits<unsigned int>::max()); }

  inline long maxUInteger() { return long(std::numeric_limits<unsigned int>::max()) + 1; }

  inline long maxSafeInteger() { return  long((1L<<53) - 1); }
  inline long minSafeInteger() { return -maxSafeInteger(); }
}

#endif
