#ifndef CJDate_H
#define CJDate_H

#include <CJObj.h>

// Date Type
class CJDateType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CJDateType(CJavaScript *js);

  CJValueP exec(CJavaScript *js, const std::string &name, const Values &values) override;

 private:
  static CJObjTypeP type_;
};

//-------

// Date Value
class CJDate : public CJObj {
 public:
  CJDate(CJavaScript *js, time_t t);
  CJDate(CJavaScript *js, const Values &values);

  CJDate *dup(CJavaScript *js) const override { return new CJDate(js, t_); }

  time_t t() const { return t_; }
  void setT(time_t t) { t_ = t; }

  CJValueP getProperty(CJavaScript *js, const std::string &key) const override;
  void setProperty(CJavaScript *js, const std::string &key, CJValueP value) override;

  void print(std::ostream &os) const override;

  static long dateFromValues(const Values &values);

  static std::string getTimeString(const std::string &format="%c");
  static std::string getTimeString(long t, const std::string &format="%c");

  static long getCurrentTime();

  static bool decodeTimeString(const std::string &time_string,
                               const std::string &format, long &t);

  static void timeToValues(long t, int *year, int *month, int *day,
                           int *hour, int *min, int *sec, int *wday=0, int *yday=0);

  static long timeFromValues(int year, int month, int day, int hour, int min, int sec);

 protected:
  long t_ { 0 };
};

typedef std::shared_ptr<CJDate> CJDateP;

#endif
