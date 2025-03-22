#include <CJGlobalFunction.h>
#include <CJavaScript.h>
#include <CJUtil.h>
#include <CUtf8.h>

CJGlobalFunction::
CJGlobalFunction(CJavaScript *js, const std::string &name) :
 CJFunctionBase(js, name, CJFunctionBase::Type::Global)
{
}

CJGlobalFunction::
CJGlobalFunction(const CJGlobalFunction &fn) :
 CJFunctionBase(fn)
{
}

CJValueP
CJGlobalFunction::
exec(CJavaScript *js, const Values &values)
{
  if      (name_ == "escape" || name_ == "encodeURI" || name_ == "encodeURIComponent") {
    static char buffer[16];

    if (values.size() < 1) {
      return js->createStringValue("");
    }

    std::string str = (values[0] ? values[0]->toString() : std::string());

    std::string str1;

    int pos = 0;
    int len = str.size();

    while (pos < len) {
      ulong c = CUtf8::readNextChar(str, pos, len);

      if     (c <= 0x29 || c == 0x2c || (c >= 0x3a && c <= 0x3f) ||
              (c >= 0x5b && c <= 0x5e) || c == 0x60 || (c >= 0x7b && c <= 0x7f)) {
        ::sprintf(buffer, "%%%02X", uint(c));

        str1 += buffer;
      }
      else if (c >= 0x80 && c <= 0xFF) {
        ::sprintf(buffer, "%%%02X", uint(c));

        str1 += buffer;
      }
      else if (c > 0xff) {
        int i1 = (c >> 12) & 0xF;
        int i2 = (c >> 8 ) & 0xF;
        int i3 = (c >> 4 ) & 0xF;
        int i4 =  c        & 0xF;

        std::string str2;

        ::sprintf(buffer, "%X", i1); str2 += buffer;
        ::sprintf(buffer, "%X", i2); str2 += buffer;
        ::sprintf(buffer, "%X", i3); str2 += buffer;
        ::sprintf(buffer, "%X", i4); str2 += buffer;

        str1 += "%u" + str2;
      }
      else {
        int len1;

        CUtf8::encode(c, buffer, len1);

        for (int j = 0; j < len1; ++j)
          str1 += buffer[j];
      }
    }

    return js->createStringValue(str1);
  }
  else if (name_ == "eval") {
    if (values.size() >= 1) {
      if (values[0] && values[0]->type() == CJToken::Type::String) {
        std::string str = values[0]->toString();

        return js->interpString(str);
      }
      else
        return values[0];
    }
    else
      return js->createUndefinedValue();
  }
  else if (name_ == "isFinite") {
    if (values.size() < 1) {
      js->errorMsg("Wrong number of function values");
      return CJValueP();
    }

    double r = (values[0] ? values[0]->toReal().value_or(0.0) : 0.0);

    return js->createBoolValue(! CJUtil::isInf(r) && ! CJUtil::isNaN(r));
  }
  else if (name_ == "isNaN") {
    if (values.size() < 1) {
      js->errorMsg("Wrong number of function values");
      return CJValueP();
    }

    double r = (values[0] ? values[0]->toReal().value_or(0.0) : 0.0);

    return js->createBoolValue(CJUtil::isNaN(r));
  }
  else if (name_ == "parseInt") {
    if (values.size() < 1)
      return js->createNumberValue(CJUtil::getNaN());

    std::string str = (values[0] ? values[0]->toString() : std::string());

    OptInt base;

    if (values.size() > 1 && values[1])
      base = values[1]->toInteger().value_or(0);

    if (base && (base.value() < 2 || base.value() > 36))
      return js->createNumberValue(CJUtil::getNaN());

    OptLong integer = CJString::parseInt(str, base, /*extraChars*/true);

    if (integer)
      return js->createNumberValue(integer.value());
    else
      return js->createNumberValue(CJUtil::getNaN());
  }
  else if (name_ == "parseFloat") {
    if (values.size() < 1)
      return js->createNumberValue(CJUtil::getNaN());

    std::string str = (values[0] ? values[0]->toString() : std::string());

    OptReal real = CJString::parseFloat(str, /*extraChars*/true);

    return js->createNumberValue(real.value_or(CJUtil::getNaN()));
  }
  else if (name_ == "unescape" || name_ == "decodeURI" || name_ == "decodeURIComponent") {
    if (values.size() < 1) {
      return js->createStringValue("undefined");
    }

    std::string str = (values[0] ? values[0]->toString() : std::string());

    std::string str1;

    int pos = 0;
    int len = str.size();

    while (pos < len) {
      ulong c = CUtf8::readNextChar(str, pos, len);

      if (c == '%') {
        long l = 0;

        OptULong c1 = (pos < len ? OptULong(CUtf8::readNextChar(str, pos, len)) : OptULong());

        if (c1 == 'u') {
          OptULong c2 = (pos < len ? OptULong(CUtf8::readNextChar(str, pos, len)) : OptULong());
          OptULong c3 = (pos < len ? OptULong(CUtf8::readNextChar(str, pos, len)) : OptULong());
          OptULong c4 = (pos < len ? OptULong(CUtf8::readNextChar(str, pos, len)) : OptULong());
          OptULong c5 = (pos < len ? OptULong(CUtf8::readNextChar(str, pos, len)) : OptULong());

          if (c2 && c3 && c4 && c5 &&
              isxdigit(c2.value()) && isxdigit(c3.value()) &&
              isxdigit(c4.value()) && isxdigit(c5.value())) {
            l = (CJUtil::hexCharValue(c2.value()) << 12) |
                (CJUtil::hexCharValue(c3.value()) << 8 ) |
                (CJUtil::hexCharValue(c4.value()) << 4 ) |
                 CJUtil::hexCharValue(c5.value());
          }
          else {
            CUtf8::append(str1, c );

            if (c1) CUtf8::append(str1, c1.value());
            if (c2) CUtf8::append(str1, c2.value());
            if (c3) CUtf8::append(str1, c3.value());
            if (c4) CUtf8::append(str1, c4.value());
            if (c5) CUtf8::append(str1, c5.value());

            continue;
          }
        }
        else {
          OptULong c2 = (pos < len ? OptULong(CUtf8::readNextChar(str, pos, len)) : OptULong());

          if (c1 && c2 &&
              isxdigit(c1.value()) && isxdigit(c2.value())) {
            l = (CJUtil::hexCharValue(c1.value()) << 4) |
                 CJUtil::hexCharValue(c2.value());
          }
          else {
            CUtf8::append(str1, c );

            if (c1) CUtf8::append(str1, c1.value());
            if (c2) CUtf8::append(str1, c2.value());

            continue;
          }
        }

        CUtf8::append(str1, l);
      }
      else {
        CUtf8::append(str1, c);
      }
    }

    return js->createStringValue(str1);
  }
  else
    return CJValueP();
}

std::string
CJGlobalFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJGlobalFunction::
print(std::ostream &os) const
{
  os << name_;
}
