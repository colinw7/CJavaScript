#include <CJValue.h>
#include <CJObjectType.h>

CJValue::
CJValue(CJObjectTypeP valueType) :
 CJToken(valueType->type()), valueType_(valueType)
{
}
