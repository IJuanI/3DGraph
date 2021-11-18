#include <graph3d.h>

using namespace graph3d;
using namespace graph3d::entity;
using namespace graph3d::opengl;
using namespace graph3d::util;

class MyApp : public ::Graph3D, graph3d::opengl::drawer {
 private:
  Object *nanosuit, *ball;
  unsigned int VBO, VAO;
  float lastX, lastY;
  bool firstMouse = false;

 public:
  MyApp();

 protected:
  void configure();
  void preinitialize();
  void initialize();

 public:
  void draw(const Context &) override;
  
  //    void closeListener(const Window&);
};
