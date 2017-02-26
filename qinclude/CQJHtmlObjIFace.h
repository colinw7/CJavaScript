#ifndef CQJHtmlObjIFace_H
#define CQJHtmlObjIFace_H

#include <string>
#include <vector>

class CQJHtmlObj;

class CQJHtmlObjIFace {
 public:
  typedef std::vector<CQJHtmlObj *> Children;

 public:
  CQJHtmlObjIFace() { }

  virtual ~CQJHtmlObjIFace() { }

  virtual std::string getName () const = 0;
  virtual std::string getClass() const = 0;
  virtual std::string getType () const = 0;
  virtual std::string getId   () const = 0;

  virtual CQJHtmlObjIFace *parent() const = 0;

  virtual CQJHtmlObj *obj() const = 0;

  virtual Children children() const = 0;
};

#endif
