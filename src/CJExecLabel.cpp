#include <CJExecLabel.h>
#include <CJExecIdentifiers.h>
#include <CJavaScript.h>

CJExecLabel::
CJExecLabel() :
 CJToken(CJToken::Type::ExecLabel)
{
}

CJValueP
CJExecLabel::
exec(CJavaScript *)
{
  return CJValueP();
}

void
CJExecLabel::
print(std::ostream &os) const
{
  os << name_ << ":";
}
