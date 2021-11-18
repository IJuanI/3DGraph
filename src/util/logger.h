#ifndef GRAPH3D_UTIL_LOGGER_H_
#define GRAPH3D_UTIL_LOGGER_H_

#include <iostream>
#include <string>

extern int graph3d_log_level;

namespace graph3d {
namespace util {

inline void log(const std::string info, int level = 4) {
  if (graph3d_log_level >= level) std::clog << info << std::endl;
}

}  // namespace util
}  // namespace graph3d

#endif