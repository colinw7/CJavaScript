#include <CJExecData.h>

void
CJExecData::
print(std::iostream &os) const
{
  for (int i = 0; i < pos_; ++i)
    os << *tokens_[i] << " ";

  os << "[33m";

  if (pos_ < len_)
    os << *tokens_[pos_] << " ";

  os << "[0m";

  for (int i = pos_ + 1; i < len_; ++i)
    os << *tokens_[i] << " ";

  os << std::endl;
}
