#include "app.h"

MyApp::MyApp() : Graph3D(800, 600, "Mi Aplicación OpenGL") {}

void MyApp::configure() {
  addShaderFolder("resources/shaders");
  addModelFolder("resources/models");
  addTextureFolder("resources/textures");

  getContext().viewport.operator->().attachDrawer(this);

  // Window* main = getContext().window;
  // Viewport* view = main->createViewport(1);
  // view->resizer = Resizer(glm::vec2(.5f, 0.f), glm::vec2(1.f, 1.f));
  // view->backgroundColor = glm::vec4(.7f, .3f, .2f, 1.f);

  // Window* window = createWindow(1080, 720, "dummy");
  // Viewport* view1 = window->createViewport(1);

  // view1->resizer = Resizer(glm::vec2(.25f, .25f), glm::vec2(.75f, .75f));
  // view1->backgroundColor = glm::vec4(.1f, .3f, .7f, 1.f);

  // registerEvent(G3D_EV_WINDOW_CLOSE, MyApp::closeListener);
}

void MyApp::preinitialize() {
  loadShaders({"simple", "simple_model", "simple_phong"});
  loadModels({"nanosuit/nanosuit.obj", "sphere.obj"});

  // Material* glassMat = createMaterial("glass");
  // glassMat->addDiffuseMap("glass_color");
  // glassMat->addNormalMap("glass_normals");
  // glassMat->addSpecularMap("glass_specular");

  // Material* ballMat = createMaterial("ball");
  // ballMat->addDifuseMap("ball_color");
  // ballMat->addNormalMap("ball_normals");
  // ballMat->addRoughnessMap("ball_roughness");
  // ballMat->addDisplacementMap("ball_height");
}

void MyApp::initialize() {
  // createCamera(G3D_CAMERA_FREE_MOVE);
  Camera* camera = getContext().viewport.operator->().camera;
  glm::vec3 position = camera->position;
  glm::vec3 up = camera->up, right = camera->right, front = camera->front;

  // ------------------------------------------------------------------
  // float vertices[] = {
  //     -0.5f, -0.5f, 0.0f,  // left
  //     0.5f,  -0.5f, 0.0f,  // right
  //     0.0f,  0.5f,  0.0f   // top
  // };

  // glGenVertexArrays(1, &VAO);
  // glGenBuffers(1, &VBO);
  // // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  // glBindVertexArray(VAO);

  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  // glEnableVertexAttribArray(0);

  // glBindBuffer(GL_ARRAY_BUFFER, 0);

  // glBindVertexArray(0);

  // ------------------------------------------------------------------
  // ball = createObject("sphere");
  // // ball->setMaterial("ball");
  // ball->setModel("sphere.obj");
  // ball->position = position + front * 4.f + .5f * right;

  // ------------------------------------------------------------------
  nanosuit = createObject("sphere");
  // nanosuit->setMaterial("glass");
  nanosuit->setModel("nanosuit/nanosuit.obj");
  nanosuit->scale({.2f, .2f, .2f});
  nanosuit->position = position + front * 4.f + .4f * right - 1.3f * up;
}

void MyApp::draw(const Context& context) {
  Viewport* viewport = context.viewport;
  viewport->depthTesting = true;

  useShader("simple_model");

  // glBindVertexArray(VAO);
  // glDrawArrays(GL_TRIANGLES, 0, 3);

  // drawObject(ball);

  drawObject(nanosuit);
}

// void MyApp::closeListener(const Window) {
//  printf("Good Bye !\n");
//}

int main() {
  graph3d_log_level = 3;
  MyApp().start();
}