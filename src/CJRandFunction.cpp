#include <CJRandFunction.h>
#include <CJavaScript.h>

CJValueP
CJRandFunction::
exec(CJavaScript *js, const Values &)
{
  double res = (1.0*rand())/RAND_MAX;

  return js->createNumberValue(res);
}

std::string
CJRandFunction::
toString() const
{
  std::ostringstream ss; ss << *this;

  return ss.str();
}

void
CJRandFunction::
print(std::ostream &os) const
{
  os << "double random()";
}
