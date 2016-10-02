#include <CJGlobalFunction.h>
#include <CJavaScript.h>
#include <CJUtil.h>
#include <CUtf8.h>

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

    double r = (values[0] ? values[0]->toReal() : 0.0);

    return js->createBoolValue(! CJUtil::isInf(r) && ! CJUtil::isNaN(r));
  }
  else if (name_ == "isNaN") {
    if (values.size() < 1) {
      js->errorMsg("Wrong number of function values");
      return CJValueP();
    }

    double r = (values[0] ? values[0]->toReal() : 0.0);

    return js->createBoolValue(CJUtil::isNaN(r));
  }
  else if (name_ == "parseInt") {
    if (values.size() < 1)
      return js->createNumberValue(CJUtil::getNaN());

    std::string str = (values[0] ? values[0]->toString() : std::string());

    COptInt base;

    if (values.size() > 1 && values[1])
      base = values[1]->toInteger();

    if (base.isValid() && (base.getValue() < 2 || base.getValue() > 36))
      return js->createNumberValue(CJUtil::getNaN());

    COptLong integer = CJString::parseInt(str, base, /*extraChars*/true);

    if (integer.isValid())
      return js->createNumberValue(integer.getValue());
    else
      return js->createNumberValue(CJUtil::getNaN());
  }
  else if (name_ == "parseFloat") {
    if (values.size() < 1)
      return js->createNumberValue(CJUtil::getNaN());

    std::string str = (values[0] ? values[0]->toString() : std::string());

    COptReal real = CJString::parseFloat(str, /*extraChars*/true);

    return js->createNumberValue(real.getValue(CJUtil::getNaN()));
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

        COptULong c1 = (pos < len ? COptULong(CUtf8::readNextChar(str, pos, len)) : COptULong());

        if (c1 == 'u') {
          COptULong c2 = (pos < len ? COptULong(CUtf8::readNextChar(str, pos, len)) : COptULong());
          COptULong c3 = (pos < len ? COptULong(CUtf8::readNextChar(str, pos, len)) : COptULong());
          COptULong c4 = (pos < len ? COptULong(CUtf8::readNextChar(str, pos, len)) : COptULong());
          COptULong c5 = (pos < len ? COptULong(CUtf8::readNextChar(str, pos, len)) : COptULong());

          if (c2.isValid() && c3.isValid() && c4.isValid() && c5.isValid() &&
              isxdigit(c2.getValue()) && isxdigit(c3.getValue()) &&
              isxdigit(c4.getValue()) && isxdigit(c5.getValue())) {
            l = (CJUtil::hexCharValue(c2.getValue()) << 12) |
                (CJUtil::hexCharValue(c3.getValue()) << 8 ) |
                (CJUtil::hexCharValue(c4.getValue()) << 4 ) |
                 CJUtil::hexCharValue(c5.getValue());
          }
          else {
            CUtf8::append(str1, c );

            if (c1.isValid()) CUtf8::append(str1, c1.getValue());
            if (c2.isValid()) CUtf8::append(str1, c2.getValue());
            if (c3.isValid()) CUtf8::append(str1, c3.getValue());
            if (c4.isValid()) CUtf8::append(str1, c4.getValue());
            if (c5.isValid()) CUtf8::append(str1, c5.getValue());

            continue;
          }
        }
        else {
          COptULong c2 = (pos < len ? COptULong(CUtf8::readNextChar(str, pos, len)) : COptULong());

          if (c1.isValid() && c2.isValid() &&
              isxdigit(c1.getValue()) && isxdigit(c2.getValue())) {
            l = (CJUtil::hexCharValue(c1.getValue()) << 4) |
                 CJUtil::hexCharValue(c2.getValue());
          }
          else {
            CUtf8::append(str1, c );

            if (c1.isValid()) CUtf8::append(str1, c1.getValue());
            if (c2.isValid()) CUtf8::append(str1, c2.getValue());

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
