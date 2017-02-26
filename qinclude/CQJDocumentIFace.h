#ifndef CQJDocumentIFace_H
#define CQJDocumentIFace_H

#include <string>
#include <vector>

class CQJDocument;

class CQJDocumentIFace {
 public:
  CQJDocumentIFace() { }

  virtual ~CQJDocumentIFace() { }

  virtual CJValueP createElement(const std::string &id) const = 0;
};

#endif
