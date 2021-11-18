#ifndef GRAPH3D_UTIL_SLEEP_H_
#define GRAPH3D_UTIL_SLEEP_H_

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace graph3d {
namespace util {

inline void sleep(int32_t milliseconds) {
#ifdef _WIN32
  Sleep(milliseconds);
#else
  usleep(milliseconds * 1000);  // usleep takes sleep time in us (1 millionth of a second)
#endif
}

}  // namespace util
}  // namespace graph3d

#endif