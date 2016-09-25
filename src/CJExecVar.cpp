#include <CJExecVar.h>
#include <CJExecIdentifiers.h>
#include <CJExecExpression.h>
#include <CJExecArray.h>
#include <CJExecDictionary.h>
#include <CJavaScript.h>

CJExecVar::
CJExecVar() :
 CJToken(CJToken::Type::Var)
{
}

CJValueP
CJExecVar::
exec(CJavaScript *js)
{
  for (const auto &varValue : varValues_) {
    auto identifiers1 = varValue.identifiers->identifiers();

    CJPropertyData data(js);

    data.setModifiable(true);
    data.setCreate    (true); // always create in local scope

    if (! js->lookupPropertyData(identifiers1, data))
      return CJLValueP();

    CJValueP value;

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

    if (! value)
      value = js->createUndefinedValue();

    CJLValueP lvalue = data.lvalue();

    if (lvalue) {
      lvalue->setValue(value);

      if (data.scope())
        data.scope()->setCanDeleteProperty(data.name(), false);
    }
  }

  return js->createUndefinedValue();
}

void
CJExecVar::
print(std::ostream &os) const
{
  os << "var";

  for (const auto &v : varValues_) {
    if (v.identifiers)
      os << " " << *v.identifiers;

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
