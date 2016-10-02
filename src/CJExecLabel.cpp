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

std::string
CJExecLabel::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJExecLabel::
print(std::ostream &os) const
{
  os << name_ << ":";
}
