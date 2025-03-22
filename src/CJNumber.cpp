#include <CJNumber.h>
#include <CJObjectTypeFunction.h>
#include <CJavaScript.h>
#include <CJUtil.h>
#include <cstring>

CJNumberTypeP CJNumberType::type_;

CJNumberTypeP
CJNumberType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJNumberTypeP(new CJNumberType(js));

    type_->init();

    js->addObjectType(type_->name(), type_);

    js->addTypeObject(CJToken::Type::Number, type_);
  }

  return type_;
}

CJNumberType::
CJNumberType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Number, "Number")
{
}

void
CJNumberType::
init()
{
  addTypeFunction(js_, "isFinite"     , type_);
  addTypeFunction(js_, "isInteger"    , type_);
  addTypeFunction(js_, "isNaN"        , type_);
  addTypeFunction(js_, "isSafeInteger", type_);
  addTypeFunction(js_, "parseFloat"   , type_);
  addTypeFunction(js_, "parseInt"     , type_);
  addTypeFunction(js_, "toString"     , type_);

  addObjectFunction(js_, "toExponential" , type_)->setNumArgs(1);
  addObjectFunction(js_, "toFixed"       , type_)->setNumArgs(1);
  addObjectFunction(js_, "toLocaleString", type_)->setNumArgs(0);
  addObjectFunction(js_, "toPrecision"   , type_)->setNumArgs(1);
  addObjectFunction(js_, "toSource"      , type_)->setNumArgs(1);
  addObjectFunction(js_, "toString"      , type_)->setNumArgs(1);
  addObjectFunction(js_, "valueOf"       , type_)->setNumArgs(0);

  addPseudoProperty("EPSILON");
  addPseudoProperty("MAX_SAFE_INTEGER");
  addPseudoProperty("MAX_VALUE");
  addPseudoProperty("MIN_SAFE_INTEGER");
  addPseudoProperty("MIN_VALUE");
  addPseudoProperty("NaN");
  addPseudoProperty("NEGATIVE_INFINITY");
  addPseudoProperty("POSITIVE_INFINITY");

  addPseudoProperty("MAX_INT");
  addPseudoProperty("MIN_INT");
  addPseudoProperty("MAX_UINT");
}

CJValueP
CJNumberType::
getProperty(CJavaScript *js, const std::string &key) const
{
  if      (key == "EPSILON") {
    return js->createNumberValue(std::numeric_limits<double>::epsilon());
  }
  else if (key == "MAX_SAFE_INTEGER") {
    return js->createNumberValue(CJUtil::maxSafeInteger());
  }
  else if (key == "MAX_VALUE") {
    return js->createNumberValue(CJUtil::maxReal());
  }
  else if (key == "MIN_SAFE_INTEGER") {
    return js->createNumberValue(CJUtil::minSafeInteger());
  }
  else if (key == "MIN_VALUE") {
    return js->createNumberValue(CJUtil::minReal());
  }
  else if (key == "NaN") {
    return js->createNumberValue(CJUtil::getNaN());
  }
  else if (key == "NEGATIVE_INFINITY") {
    return js->createNumberValue(CJUtil::getNegInf());
  }
  else if (key == "POSITIVE_INFINITY") {
    return js->createNumberValue(CJUtil::getPosInf());
  }
  else if (key == "MAX_INT") {
    return js->createNumberValue(CJUtil::maxInteger());
  }
  else if (key == "MAX_UINT") {
    return js->createNumberValue(CJUtil::maxInteger() + 1);
  }

  return CJObjType::getProperty(js, key);
}

CJValueP
CJNumberType::
execType(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  // values[0] is CJNumberFunction

  //---

  // type functions
  if      (name == "isFinite") {
    bool b = false;

    if (values.size() > 1) {
      double r = values[1]->toReal().value_or(0.0);

      b = (! CJUtil::isNaN(r) && ! CJUtil::isPosInf(r) && ! CJUtil::isNegInf(r));
    }

    return js->createBoolValue(b);
  }
  else if (name == "isInteger") {
    bool b = false;

    if (values.size() > 1) {
      double r = values[1]->toReal().value_or(0.0);

      b = CJUtil::isInteger(r);
    }

    return js->createBoolValue(b);
  }
  else if (name == "isNaN") {
    bool b = false;

    if (values.size() > 1) {
      double r = values[1]->toReal().value_or(0.0);

      b = CJUtil::isNaN(r);
    }

    return js->createBoolValue(b);
  }
  else if (name == "isSafeInteger") {
    bool b = false;

    if (values.size() > 1) {
      double r = values[1]->toReal().value_or(0.0);

      b = (r >= CJUtil::minSafeInteger() && r <= CJUtil::maxSafeInteger());
    }

    return js->createBoolValue(b);
  }
  else if (name == "parseFloat") {
    if (values.size() <= 1)
      return js->createNumberValue(CJUtil::getNaN());

    OptReal real = CJString::parseFloat(values[1]->toString(), /*extraChars*/true);

    return js->createNumberValue(real.value_or(CJUtil::getNaN()));
  }
  else if (name == "parseInt") {
    if (values.size() <= 1)
      return js->createNumberValue(CJUtil::getNaN());

    std::string str = (values[1] ? values[1]->toString() : std::string());

    OptInt base;

    if (values.size() > 1 && values[1])
      base = values[1]->toInteger().value_or(0);

    if (base && (base.value() < 2 || base.value() > 36)) {
      js->throwRangeError(values[0], "Invalid base value");
      return js->createNumberValue(CJUtil::getNaN());
    }

    OptLong integer = CJString::parseInt(values[1]->toString(), base, /*extraChars*/true);

    if (integer)
      return js->createNumberValue(integer.value());
    else
      return js->createNumberValue(CJUtil::getNaN());
  }
  else if (name == "toString") {
    return js->createStringValue("function Number() { }");
  }
  else
    return CJValueP();
}

CJValueP
CJNumberType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJNumberP num = CJValue::cast<CJNumber>(values[0]);
  assert(num);

  //---

  // object functions
  if      (name == "toExponential") {
    double r = num->toReal().value_or(0.0);

    if      (CJUtil::isNaN(r))
      return js->createStringValue("NaN");
    else if (CJUtil::isNegInf(r))
      return js->createStringValue("-Infinity");
    else if (CJUtil::isPosInf(r))
      return js->createStringValue("Infinity");

    long n = 0;

    if (values.size() > 1)
      n = values[1]->toInteger().value_or(0);
    else {
      double r1 = std::abs(r);

      if (r1 > 0)
        n = int(log10(r1));

      r1 = r1 - std::floor(r1);

      while (r1 > 1E-6) {
        r1 *= 10;

        ++n;

        r1 = r1 - std::floor(r1);
      }
    }

    if (n < 0 || n > 20) {
      js->throwRangeError(values[0], "Invalid digits value");
      return js->createNumberValue(CJUtil::getNaN());
    }

    if (r == 0) {
      std::string s = "0";

      if (n > 0)
        s += "." + CJUtil::nchars(n, '0');

      return js->createStringValue(s + "e+0");
    }

    bool isNeg = (r < 0);

    std::string str;

    double e  = log10(std::abs(r));
    int    ie = (e > 0 ? int(e) : int(e) - 1);

    bool isNegExp = (ie < 0);

    double r1 = std::abs(r)/pow(10, ie);

    char buffer[256];

    sprintf(buffer, "%.*f", int(n), r1);

    auto p = strchr(buffer, '.');

    int dpos = (p ? p - buffer : -1);

    int blen = strlen(buffer);

    if (dpos > 0) {
      while (blen > dpos && buffer[blen - 1] == '0')
        buffer[--blen] = '\0';
    }

    if (n > 0 ) {
      if (dpos < 0) {
        buffer[blen    ] = '.';
        buffer[blen + 1] = '\0';

        dpos = blen;

        blen += 2;
      }

      for (int i = 0; i < n; ++i) {
        if (buffer[dpos + i + 1] == '\0') {
          buffer[dpos + i + 1] = '0';
          buffer[dpos + i + 2] = '\0';
        }
      }
    }

    str = (isNeg ? "-" : "") + std::string(buffer) + "e" + (isNegExp ? "-" : "+");

    sprintf(buffer, "%d", std::abs(ie));

    str += buffer;

    return js->createStringValue(str);
  }
  else if (name == "toFixed") {
    double x = num->toReal().value_or(0.0);

    if      (CJUtil::isNaN(x))
      return js->createStringValue("NaN");
    else if (CJUtil::isNegInf(x))
      return js->createStringValue("-Infinity");
    else if (CJUtil::isPosInf(x))
      return js->createStringValue("Infinity");

    long f = 0;

    if (values.size() > 1)
      f = values[1]->toInteger().value_or(0);

    if (f < 0 || f > 20) {
      js->throwRangeError(values[0], "Invalid digits value");
      return js->createNumberValue(CJUtil::getNaN());
    }

    std::string s;

    if (x < 0) {
      s = "-";
      x = -x;
    }

    std::string m;

    if (x >= 10e21) {
      m = CJUtil::realToString(x);
    }
    else {
      double n = x;

      double z = x*pow(10, f);

      n = std::round(z);

      if (n == 0)
        m = "0";
      else
        m = CJUtil::integerToString(n);

      if (f != 0) {
        int k = m.size();

        if (k <= f) {
          std::string z1 = CJUtil::nchars(f + 1 - k, '0');

          m = z1 + m;

          k = f + 1;
        }

        std::string a = m.substr(0, k - f);
        std::string b = m.substr(k - f);

        m = a + "." + b;
      }
    }

    return js->createStringValue(s + m);
  }
  else if (name == "toLocaleString") {
    return js->createStringValue(num->toString());
  }
  else if (name == "toPrecision") {
    double x = num->toReal().value_or(0.0);

    if      (CJUtil::isNaN(x))
      return js->createStringValue("NaN");
    else if (CJUtil::isNegInf(x))
      return js->createStringValue("-Infinity");
    else if (CJUtil::isPosInf(x))
      return js->createStringValue("Infinity");

    OptLong precision;

    if (values.size() > 1)
      precision = values[1]->toInteger();

    if (precision) {
      long p = precision.value();

      std::string s;

      if (x < 0) {
        s = "-";
        x = -x;
      }

      if (p < 1 || p > 21) {
        js->throwRangeError(values[0], "Invalid precision value");
        return js->createNumberValue(CJUtil::getNaN());
      }

      std::string m;

      double e = 0;

      if (x == 0) {
        m = CJUtil::nchars(p, '0');
        e = 0;
      }
      else {
        // Let e and n be integers such that 10^(p–1) ≤ n < 10^p and for which the exact
        // mathematical value of n.10^(e–p+1) – x is as close to zero as possible. If there
        // are two such sets of e and n, pick the e and n for which n × 10^(e–p+1) is larger.
        double p1 = pow(10, p - 1);
        double p2 = pow(10, p    );

#if 0
        {
        double minp = 1E50;
        double n    = p1;
        double e    = 0;

        for (double tn = p1; tn < p2; ++tn) {
          for (double te = 0; te < 21; ++te) {
            double p3 = tn*pow(10, te - p + 1) - x;

            if (fabs(p3) < minp) {
              minp = fabs(p3);
              n    = tn;
              e    = te;
            }
          }
        }
        printf("minp=%g n=%g e=%g\n", minp, n, e);
        }
#endif

        double n = x;

        if      (n < p1) {
          while (n < p1) {
            n *= 10;

            --e;
          }
        }
        else if (n > p2) {
          while (n > p2) {
            n /= 10;

            ++e;
          }
        }

        n = std::round(n);

        e += p - 1;

        //---

        m = CJUtil::integerToString(n);

        //---

        if (e < -6 || e >= p) {
          assert(e != 0);

          std::string a = m.substr(0, 1);
          std::string b = m.substr(1);

          if (b != "")
            m = a + "." + b;

          std::string c, d;

          if      (e > 0) {
            c = "+";
          }
          else if (e < 0) {
            c = "-";
            e = -e;
          }

          d = CJUtil::integerToString(e);

          return js->createStringValue(s + m + "e" + c + d);
        }
      }

      if (e == p - 1) {
        return js->createStringValue(s + m);
      }

      if (e >= 0) {
        m = m.substr(0, e + 1) + "." + m.substr(e + 1);
      }
      else {
        m = "0." + CJUtil::nchars(-(e + 1), '0') + m;
      }

      return js->createStringValue(s + m);
    }
    else {
      std::string s = CJUtil::realToString(x);

      return js->createStringValue(s);
    }
  }
  else if (name == "toString") {
    double r = num->toReal().value_or(0.0);

    OptLong base;

    if (values.size() > 1)
      base = values[1]->toInteger();

    std::string str;

    if (base) {
      int ibase = base.value_or(0);

      if (ibase < 2 || ibase > 36) {
        js->throwRangeError(values[0], "Invalid base value");
        return js->createNumberValue(CJUtil::getNaN());
      }

      str = CJUtil::realToBaseString(r, ibase);
    }
    else
      str = CJUtil::realToString(r);

    return js->createStringValue(str);
  }
  else if (name == "valueOf") {
    return values[0];
  }
  else
    return CJValueP();
}

//------

CJNumber::
CJNumber(CJavaScript *js, double real) :
 CJObj(js, CJNumberType::instance(js)), real_(real)
{
}

CJNumber::
CJNumber(const CJNumber &n) :
 CJObj(n), real_(n.real_), primitive_(n.primitive_)
{
}

std::string
CJNumber::
toString() const
{
  if (! isPrimitive())
    return "[Number: " + realString() + "]";
  else
    return realString();
}

OptLong
CJNumber::
toInteger() const
{
  long l = 0;

  if      (CJUtil::isNaN(real_))
    l = 0;
  else if (CJUtil::isPosInf(real_))
    l = CJUtil::maxUInteger();
  else if (CJUtil::isNegInf(real_))
    l = CJUtil::minInteger();
  else if (real_ > CJUtil::maxUInteger())
    l = CJUtil::maxUInteger();
  else if (real_ < CJUtil::minInteger())
    l = CJUtil::minInteger();
  else if (real_ >= 0)
    l = long(real_);
  else
    l = -long(-real_);

  return OptLong(l);
}

std::string
CJNumber::
realString() const
{
  return CJUtil::realToString(real_);
}
