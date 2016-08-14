#include <CJValue.h>
#include <CJObj.h>

CJValue::
CJValue(CJObjTypeP valueType) :
 CJToken(valueType->type()), valueType_(valueType)
{
}
