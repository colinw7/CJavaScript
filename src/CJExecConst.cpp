#include <CJExecConst.h>
#include <CJExecIdentifiers.h>
#include <CJExecExpression.h>
#include <CJExecArray.h>
#include <CJExecDictionary.h>
#include <CJavaScript.h>

CJExecConst::
CJExecConst() :
 CJToken(CJToken::Type::Const)
{
}

CJValueP
CJExecConst::
exec(CJavaScript *js)
{
  CJValueP value;

  for (const auto &varValue : varValues_) {
    auto identifiers1 = varValue.identifiers->identifiers();

    CJLValueP idValue = js->lookupProperty(identifiers1);

    if (! idValue)
      idValue = js->lookupProperty(identifiers1, /*create*/true);

    if      (varValue.expr)
      value = varValue.expr->exec(js);
    else if (varValue.block)
      value = varValue.block->exec(js);
    else if (varValue.array)
      value = varValue.array->exec(js);
    else if (varValue.dict)
      value = varValue.dict->exec(js);
    else if (varValue.value)
      value = varValue.value;

    if (idValue) {
      if (! value)
        value = js->createUndefinedValue();

      idValue->setValue(value);

      idValue->setWritable(false);
    }
  }

  return value;
}

void
CJExecConst::
print(std::ostream &os) const
{
  os << "const";

  for (const auto &v : varValues_) {
    if (v.identifiers)
      os << " " << v.identifiers;

    if (v.expr || v.block || v.array || v.dict || v.value) {
      os << " = ";

      if      (v.expr ) os << *v.expr;
      else if (v.block) os << *v.block;
      else if (v.array) os << *v.array;
      else if (v.dict ) os << *v.dict;
      else if (v.value) os << *v.value;
    }
  }
}
