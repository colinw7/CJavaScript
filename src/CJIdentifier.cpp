#include <CJIdentifier.h>

CJIdentifier::
CJIdentifier(const std::string &id) :
 CJToken(Type::Identifier), id_(id)
{
}
