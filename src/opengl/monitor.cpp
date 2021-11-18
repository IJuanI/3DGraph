#include <opengl/monitor.h>

#include <iostream>

#include <exceptions/exception.h>
#include <exceptions/messages.h>
#include <opengl/window.h>

namespace graph3d {
namespace opengl {

Monitor::Monitor() { init(glfwGetPrimaryMonitor()); }

Monitor::Monitor(int index) {
  int count;
  GLFWmonitor **monitors = glfwGetMonitors(&count);
  if (index < 0 || index >= count)
    throw exceptions::exception("ERR006", exceptions::ERR006, exceptions::format(exceptions::ERR006_2, index, count));
  init(monitors[index]);
}

Monitor::Monitor(const Window &window) {
  int nmonitors, i;
  int mx, my, mw, mh;
  int overlap, bestoverlap;
  GLFWmonitor *bestmonitor;
  GLFWmonitor **monitors;
  const GLFWvidmode *mode;

  bestoverlap = 0;
  bestmonitor = NULL;

  glm::vec2 size = (util::dimension)window.size, pos = window.position;
  monitors = glfwGetMonitors(&nmonitors);

  for (i = 0; i < nmonitors; i++) {
    mode = glfwGetVideoMode(monitors[i]);
    glfwGetMonitorPos(monitors[i], &mx, &my);
    mw = mode->width;
    mh = mode->height;

    overlap = std::max(0, std::min((int)(pos.x + size.x), mx + mw) - std::max((int)pos.x, mx)) *
              std::max(0, std::min((int)(pos.y + size.y), my + mh) - std::max((int)pos.y, my));

    if (bestoverlap < overlap) {
      bestoverlap = overlap;
      bestmonitor = monitors[i];
    }
  }

  init(bestmonitor);
}
}  // namespace opengl
}  // namespace graph3d