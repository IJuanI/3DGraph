#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <opengl/viewport.h>
#include <util/logger.h>

int graph3d_log_level = -1;

namespace graph3d {

glm::vec3 G3D_ZERO(.0f, .0f, .0f);
glm::vec3 G3D_FORWARD(.0f, .0f, 1.0f);
glm::vec3 G3D_BACKWARD(.0f, .0f, -1.0f);
glm::vec3 G3D_UP(.0f, 1.0f, .0f);
glm::vec3 G3D_DOWN(.0f, -1.0f, .0f);
glm::vec3 G3D_LEFT(1.0f, .0f, .0f);
glm::vec3 G3D_RIGHT(-1.0f, .0f, .0f);

namespace opengl {
uint32_t Viewport::lastId = 0;
}
}  // namespace graph3d