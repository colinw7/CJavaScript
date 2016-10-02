#include <CJIdentifier.h>

CJIdentifier::
CJIdentifier(const std::string &id) :
 CJToken(Type::Identifier), id_(id)
{
}

std::string
CJIdentifier::
toString() const
{
  return id_;
}

void
CJIdentifier::
print(std::ostream &os) const
{
  os << id_;
}
