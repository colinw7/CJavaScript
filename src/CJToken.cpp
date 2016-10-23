#include <CJToken.h>

CJToken::
CJToken(Type type) :
 type_(type)
{
}

CJToken::
CJToken(const CJToken &t) :
 type_(t.type_), lineNum_(t.lineNum_)
{
}
