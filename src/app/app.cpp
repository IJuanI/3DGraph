#include "app.h"

MyApp::MyApp() : Graph3D(800, 600, "Mi Aplicación OpenGL") {
  loadShaders({"simple_phong"});
  loadModels({"sphere", "cube"});
}

void MyApp::configure() {
  addShaderFolder("resources/shaders");
  addModelFolder("resources/models");
  addTextureFolder("resources/textures");

  // registerEvent(G3D_EV_WINDOW_CLOSE, MyApp::closeListener);
}

void MyApp::preinitialize() {
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

  // glass = createObject("sphere");
  // glass->setMaterial("glass");
  // glass->setPosition(camera.pos + camera.front * 2 + Vector.LEFT);

  // ball = createObject("sphere");
  // ball->setMaterial("ball");
  // ball->setPosition(camera.pos + camera.front * 2 + Vector.RIGHT);
}

void MyApp::run() {
  // useShader("simple_phong");
  // draw(glass);
  // draw(ball);
}

void MyApp::closeListener(const Window) {
  printf("Good Bye !\n");
}