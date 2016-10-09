#include <CJDate.h>
#include <CJavaScript.h>
#include <CJUtil.h>

#include <sys/time.h>
#include <cstring>

CJDateTypeP CJDateType::type_;

CJDateTypeP
CJDateType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJDateTypeP(new CJDateType(js));

    type_->init();

    js->addObjectType(type_->name(), type_);

    js->addTypeObject(CJToken::Type::Date, type_);
  }

  return type_;
}

CJDateType::
CJDateType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Date, "Date")
{
}

void
CJDateType::
init()
{
  addTypeFunction(js_, "UTC"     , type_);
  addTypeFunction(js_, "now"     , type_);
  addTypeFunction(js_, "parse"   , type_);
  addTypeFunction(js_, "toString", type_);

  addObjectFunction(js_, "getDate"           , type_);
  addObjectFunction(js_, "getDay"            , type_);
  addObjectFunction(js_, "getFullYear"       , type_);
  addObjectFunction(js_, "getHours"          , type_);
  addObjectFunction(js_, "getMilliseconds"   , type_);
  addObjectFunction(js_, "getMinutes"        , type_);
  addObjectFunction(js_, "getMonth"          , type_);
  addObjectFunction(js_, "getSeconds"        , type_);
  addObjectFunction(js_, "getTime"           , type_);
  addObjectFunction(js_, "getTimezoneOffset" , type_);
  addObjectFunction(js_, "getUTCDate"        , type_);
  addObjectFunction(js_, "getUTCDay"         , type_);
  addObjectFunction(js_, "getUTCFullYear"    , type_);
  addObjectFunction(js_, "getUTCHours"       , type_);
  addObjectFunction(js_, "getUTCMilliseconds", type_);
  addObjectFunction(js_, "getUTCMinutes"     , type_);
  addObjectFunction(js_, "getUTCMonth"       , type_);
  addObjectFunction(js_, "getUTCSeconds"     , type_);
  addObjectFunction(js_, "getYear"           , type_);
  addObjectFunction(js_, "setDate"           , type_);
  addObjectFunction(js_, "setFullYear"       , type_);
  addObjectFunction(js_, "setHours"          , type_);
  addObjectFunction(js_, "setMilliseconds"   , type_);
  addObjectFunction(js_, "setMinutes"        , type_);
  addObjectFunction(js_, "setMonth"          , type_);
  addObjectFunction(js_, "setSeconds"        , type_);
  addObjectFunction(js_, "setTime"           , type_);
  addObjectFunction(js_, "setUTCDate"        , type_);
  addObjectFunction(js_, "setUTCFullYear"    , type_);
  addObjectFunction(js_, "setUTCHours"       , type_);
  addObjectFunction(js_, "setUTCMilliseconds", type_);
  addObjectFunction(js_, "setUTCMinutes"     , type_);
  addObjectFunction(js_, "setUTCMonth"       , type_);
  addObjectFunction(js_, "setUTCSeconds"     , type_);
  addObjectFunction(js_, "setYear"           , type_);
  addObjectFunction(js_, "toDateString"      , type_);
  addObjectFunction(js_, "toGMTString"       , type_);
  addObjectFunction(js_, "toISOString"       , type_);
  addObjectFunction(js_, "toJSON"            , type_);
  addObjectFunction(js_, "toLocaleDateString", type_);
  addObjectFunction(js_, "toLocaleString"    , type_);
  addObjectFunction(js_, "toLocaleTimeString", type_);
  addObjectFunction(js_, "toString"          , type_);
  addObjectFunction(js_, "toTimeString"      , type_);
  addObjectFunction(js_, "toUTCString"       , type_);
  addObjectFunction(js_, "valueOf"           , type_);
}

CJValueP
CJDateType::
execType(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  // values[0] is CJDateFunction

  //---

  // type functions
  if      (name == "UTC") {
    long t = CJDate::dateFromValues(values);

    return js->createNumberValue(t);
  }
  else if (name == "now") {
    long t = CJDate::getCurrentTime();

    return js->createNumberValue(t);
  }
  else if (name == "parse") {
    // TODO:
    return js->createNumberValue(long(0));
  }
  else if (name == "toString") {
    return js->createStringValue("function Date() { }");
  }
  else {
    js->errorMsg("Invalid date type function " + name);
  }

  return CJValueP();
}

CJValueP
CJDateType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CJDateP date = CJValue::cast<CJDate>(values[0]);
  assert(date);

  //---

  // object functions
  if      (name == "getDate") {
    int year, month, day, hour, min, sec, mday, yday;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec, &mday, &yday);

    return js->createNumberValue(long(mday));
  }
  else if (name == "getDay") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    return js->createNumberValue(long(day));
  }
  else if (name == "getFullYear") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    return js->createNumberValue(long(year));
  }
  else if (name == "getHours") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    return js->createNumberValue(long(hour));
  }
  else if (name == "getMilliseconds") {
    // TODO
    return js->createNumberValue(long(0));
  }
  else if (name == "getMinutes") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    return js->createNumberValue(long(min));
  }
  else if (name == "getMonth") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    return js->createNumberValue(long(month));
  }
  else if (name == "getSeconds") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    return js->createNumberValue(long(sec));
  }
  else if (name == "getTime") {
    return js->createNumberValue(date->t());
  }
  else if (name == "getTimezoneOffset") {
    // TODO
    return js->createNumberValue(long(0));
  }
  else if (name == "getUTCDate") {
    int year, month, day, hour, min, sec, mday, yday;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec, &mday, &yday);

    return js->createNumberValue(long(mday));
  }
  else if (name == "getUTCDay") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    return js->createNumberValue(long(day));
  }
  else if (name == "getUTCFullYear") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    return js->createNumberValue(long(year));
  }
  else if (name == "getUTCHours") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    return js->createNumberValue(long(hour));
  }
  else if (name == "getUTCMilliseconds") {
    // TODO
    return js->createNumberValue(long(0));
  }
  else if (name == "getUTCMinutes") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    return js->createNumberValue(long(min));
  }
  else if (name == "getUTCMonth") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    return js->createNumberValue(long(month));
  }
  else if (name == "getUTCSeconds") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    return js->createNumberValue(long(sec));
  }
  else if (name == "getYear") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    return js->createNumberValue(long(year - 1900));
  }

  else if (name == "setDate") {
    // TODO
  }
  else if (name == "setFullYear") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    year = (values.size() > 1 ? values[1]->toInteger().getValue(0) - 1900 : year);

    date->setT(CJDate::timeFromValues(year, month, day, hour, min, sec));
  }
  else if (name == "setHours") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    hour = (values.size() > 1 ? values[1]->toInteger().getValue(0) : hour);

    date->setT(CJDate::timeFromValues(year, month, day, hour, min, sec));
  }
  else if (name == "setMilliseconds") {
    // TODO
  }
  else if (name == "setMinutes") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    min = (values.size() > 1 ? values[1]->toInteger().getValue(0) : min);

    date->setT(CJDate::timeFromValues(year, month, day, hour, min, sec));
  }
  else if (name == "setMonth") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    month = (values.size() > 1 ? values[1]->toInteger().getValue(0) : month);

    date->setT(CJDate::timeFromValues(year, month, day, hour, min, sec));
  }
  else if (name == "setSeconds") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    sec = (values.size() > 1 ? values[1]->toInteger().getValue(0) : sec);

    date->setT(CJDate::timeFromValues(year, month, day, hour, min, sec));
  }
  else if (name == "setTime") {
    // TODO
  }
  else if (name == "setUTCDate") {
    // TODO
  }
  else if (name == "setUTCFullYear") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    year = (values.size() > 1 ? values[1]->toInteger().getValue(0) - 1900 : year);

    date->setT(CJDate::timeFromValues(year, month, day, hour, min, sec));
  }
  else if (name == "setUTCHours") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    hour = (values.size() > 1 ? values[1]->toInteger().getValue(0) : hour);

    date->setT(CJDate::timeFromValues(year, month, day, hour, min, sec));
  }
  else if (name == "setUTCMilliseconds") {
    // TODO
  }
  else if (name == "setUTCMinutes") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    min = (values.size() > 1 ? values[1]->toInteger().getValue(0) : min);

    date->setT(CJDate::timeFromValues(year, month, day, hour, min, sec));
  }
  else if (name == "setUTCMonth") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    month = (values.size() > 1 ? values[1]->toInteger().getValue(0) : month);

    date->setT(CJDate::timeFromValues(year, month, day, hour, min, sec));
  }
  else if (name == "setUTCSeconds") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    sec = (values.size() > 1 ? values[1]->toInteger().getValue(0) : sec);

    date->setT(CJDate::timeFromValues(year, month, day, hour, min, sec));
  }
  else if (name == "setYear") {
    int year, month, day, hour, min, sec;

    CJDate::timeToValues(date->t(), &year, &month, &day, &hour, &min, &sec);

    year = (values.size() > 1 ? values[1]->toInteger().getValue(0): year);

    date->setT(CJDate::timeFromValues(year, month, day, hour, min, sec));
  }

  else if (name == "toDateString") {
    return js->createStringValue(CJDate::getTimeString(date->t(), "%a %b %d %Y"));
  }
  else if (name == "toGMTString") {
    return js->createStringValue(CJDate::getTimeString(date->t()));
  }
  else if (name == "toISOString") {
    return js->createStringValue(CJDate::getTimeString(date->t(), "%Y-%m-%dT%h:%M:%s.0Z"));
  }
  else if (name == "toJSON") {
    return js->createStringValue(CJDate::getTimeString(date->t(), "%Y-%m-%dT%h:%M:%s.0Z"));
  }
  else if (name == "toLocaleDateString") {
    return js->createStringValue(CJDate::getTimeString(date->t(), "%a %b %d %Y"));
  }
  else if (name == "toLocaleString") {
    return js->createStringValue(CJDate::getTimeString(date->t()));
  }
  else if (name == "toLocaleTimeString") {
    return js->createStringValue(CJDate::getTimeString(date->t(), "%h:%M:%s %Z"));
  }
  else if (name == "toString") {
    return js->createStringValue(CJDate::getTimeString(date->t()));
  }
  else if (name == "toTimeString") {
    return js->createStringValue(CJDate::getTimeString(date->t(), "%h:%M:%s %Z"));
  }
  else if (name == "toUTCString") {
    return js->createStringValue(CJDate::getTimeString(date->t()));
  }
  else if (name == "valueOf") {
    return js->createNumberValue(date->t());
  }
  else {
    js->errorMsg("Invalid date function " + name);
  }

  return CJValueP();
}

//------

CJDate::
CJDate(CJavaScript *js, long t) :
 CJObj(js, CJDateType::instance(js)), t_(t)
{
}

CJDate::
CJDate(CJavaScript *js, const Values &values) :
 CJObj(js, CJDateType::instance(js)), t_(dateFromValues(values))
{
}

CJValueP
CJDate::
getProperty(CJavaScript *js, const std::string &key) const
{
  return CJObj::getProperty(js, key);
}

void
CJDate::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJObj::setProperty(js, key, value);
}

std::string
CJDate::
toString() const
{
  return getTimeString(t_);
}

long
CJDate::
dateFromValues(const Values &values)
{
  if      (values.size() == 1) {
    if (! values[0])
      return 0;

    if      (values[0]->isNumber()) {
      long t = values[0]->toInteger().getValue(0);

      return t;
    }
    else if (values[0]->type() == CJToken::Type::String) {
      std::string time_string = values[0]->toString();

      long t;

      if (decodeTimeString(time_string, "%c", t))
        return t;
    }
    else
      return 0;
  }
  else if (values.size() >= 2 && values.size() <= 7) {
    int year    = values[0]->toInteger().getValue(0);
    int month   = values[1]->toInteger().getValue(0);
    int day     = (values.size() >= 3 ? values[2]->toInteger().getValue(0) : 0);
    int hour    = (values.size() >= 4 ? values[3]->toInteger().getValue(0) : 0);
    int minutes = (values.size() >= 5 ? values[4]->toInteger().getValue(0) : 0);
    int seconds = (values.size() >= 6 ? values[5]->toInteger().getValue(0) : 0);
  //int msec    = (values.size() >= 7 ? values[6]->toInteger().getValue(0) : 0);

    struct tm tm;

    tm.tm_year  = year - 1900;
    tm.tm_mon   = month;
    tm.tm_mday  = day;
    tm.tm_hour  = hour;
    tm.tm_min   = minutes;
    tm.tm_sec   = seconds;
    tm.tm_wday  = 0;
    tm.tm_yday  = 0;
    tm.tm_isdst = -1; // auto DST

    time_t t = mktime(&tm);

    return t*1000;
  }

  long t = getCurrentTime();

  return t;
}

std::string
CJDate::
getTimeString(const std::string &format)
{
  long t = getCurrentTime();

  return getTimeString(t, format);
}

std::string
CJDate::
getTimeString(long tv, const std::string &format)
{
  static char time_string[256];

  time_t t = tv/1000;

  struct tm *tm = localtime(&t);

  if (! tm)
    tm = gmtime(&t);

  strftime(time_string, 256, format.c_str(), tm);

  return time_string;
}

long
CJDate::
getCurrentTime()
{
  //time_t t = time(0);

  struct timeval tv;

  (void) gettimeofday(&tv, 0);

  long t = tv.tv_sec*1000 + tv.tv_usec;

  return t;
}

bool
CJDate::
decodeTimeString(const std::string &time_string, const std::string &format, long &tv)
{
  struct tm tm;

  memset(&tm, 0, sizeof(tm));

  char *p = strptime(time_string.c_str(), format.c_str(), &tm);

  if (! p)
    return false;

  tm.tm_wday  = 0;
  tm.tm_yday  = 0;
  tm.tm_isdst = -1; // auto DST

  time_t t = mktime(&tm);

  tv = t*1000;

  return true;
}

void
CJDate::
timeToValues(long tv, int *year, int *month, int *day, int *hour, int *min, int *sec,
             int *wday, int *yday)
{
  time_t t = tv/1000;

  struct tm *tm = localtime(&t);

  if (! tm)
    tm = gmtime(&t);

  if (year ) *year  = tm->tm_year + 1900;
  if (month) *month = tm->tm_mon;
  if (day  ) *day   = tm->tm_mday;
  if (hour ) *hour  = tm->tm_hour;
  if (min  ) *min   = tm->tm_min;
  if (sec  ) *sec   = tm->tm_sec;
  if (wday ) *wday  = tm->tm_wday;
  if (yday ) *yday  = tm->tm_yday;
}

time_t
CJDate::
timeFromValues(int year, int month, int day, int hour, int min, int sec)
{
  struct tm tm;

  tm.tm_year  = year - 1900;
  tm.tm_mon   = month;
  tm.tm_mday  = day;
  tm.tm_hour  = hour;
  tm.tm_min   = min;
  tm.tm_sec   = sec;
  tm.tm_wday  = 0;
  tm.tm_yday  = 0;
  tm.tm_isdst = -1; // auto DST

  time_t t = mktime(&tm);

  return t*1000;
}
