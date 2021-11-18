#include <exceptions/messages.h>

#include <cstdarg>
#include <vector>

namespace graph3d {
namespace exceptions {

// https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf/26221725
const std::string format(const char* const zcFormat, ...) {
  va_list vaArgs;
  va_start(vaArgs, zcFormat);

  va_list vaArgsCopy;
  va_copy(vaArgsCopy, vaArgs);
  const int iLen = std::vsnprintf(NULL, 0, zcFormat, vaArgsCopy);
  va_end(vaArgsCopy);

  std::vector<char> zc(iLen + 1);
  std::vsnprintf(zc.data(), zc.size(), zcFormat, vaArgs);
  va_end(vaArgs);
  return std::string(zc.data(), iLen);
}
}  // namespace exceptions
}  // namespace graph3d
