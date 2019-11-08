# 3DGraph

Este proyecto es desarrollado como proyecto final de Computación Gráfica, para la carrera Ingeniería en Informática, FICH - UNL, Argentina.

## Objetivos
Desarrollar un API gráfica para OpenGL 4.3, que permita al usuario desentenderse de los aspectos técnicos, ajenos a la computación Gráfica, que rondan a OpenGL. Además de resolver situaciones de uso común en la computación gráfica de manera amigable.

La misma estará centrada exclusivamente en entornos 3D.

### Objetivos Secundarios
Crear shaders por defecto y aplicaciones de prueba, que utilicen la api

#### Escena Simple
Un par de objetos de geometría simple, con iluminación de phong y texturas

#### Shader PBR
Similar al anterior, pero utilizando un shader pbr, el cual idealmente estaría incluído en el api.

#### Escena de interiores
Una escena de interiores, sin mucha complejidad, con múltiples luces y algún que otro espejo.

#### Explorar la representación de funciones a través de integrales de polinomios de Bernstein de manera Gráfica
Al derivar un polinomio de Bernstein, se obtiene una nuevo polinomio de Bernstein, pero ahora de un grado menos que el anterior.

El recíproco también es cierto, con la salvedad de que en este caso no se obtiene un único polinomio, sino una familia de polinomios. Puntualmente, se obtiene que la integral de un polinomio de Bernstein es la combinación afín de dos polinomios de Bernstein, de un grado menos que el original.

Dado un polinomio `X` de grado `n`, su integral va a ser un polinomio de Bernstein con `n` grados de libertad. Si se reduce estos grados de libertad a `1`, asumiendo por ejemplo que todas las incógnitas son iguales, entonces se puede graficar la integral como un plano en el espacio, tomando una función directriz arbitraria `d(z)` para variar la incógnita en el nuevo eje.

Ahora, si se toma un parámetro `t` cualquiera del polinomio original y se lo calcula sobre la nueva superficie, se obtiene una nueva función `f(z)`.

El plano queda completamente definido por una función `f(z)` y un polinomio `X`. Al expresar tanto `f(z)` como `X` en base a puntos de control de curvas de Bezier, se puede dibujar una superficie con `n+1+m` puntos de control, en dónde `m` es el orden de una curva de Bezier que describa a `f(z)`.

OJO: Puede ser que `f(z)` no sea única, no tengo forma de probar lo contrario.

No creo que esto tenga implicancia alguna sobre nada, pero es un ejercicio entretenido.


## Ejemplo de uso

> myapp.h
>  ```cpp
>  class MyApp : Graph3D {
>    private:
>      Entity *glass, *ball;
>    public:
>      MyApp();
>      closeListener(const Window);
>  }
>  ```


> myapp.cpp
>  ```cpp
>  MyApp::MyApp() : Graph3D(800, 600, "Mi Aplicación OpenGL") {
>    loadShaders({"simple_phong"});
>    loadModels({"sphere", "cube"});
>  }
>
>  MyApp::configure() {
>    addShaderFolder("resources/shaders");
>    addModelFolder("resources/models");
>    addTextureFolder("resources/textures");
>
>    registerEvent(G3D_EV_WINDOW_CLOSE, MyApp::closeListener);
>  }
>
>  MyApp::preinitialize() {
>    Material* glassMat = createMaterial("glass");
>    glassMat->addDiffuseMap("glass_color");
>    glassMat->addNormalMap("glass_normals");
>    glassMat->addSpecularMap("glass_specular");
>
>    Material* ballMat = createMaterial("ball");
>    ballMat->addDifuseMap("ball_color");
>    ballMat->addNormalMap("ball_normals");
>    ballMat->addRoughnessMap("ball_roughness");
>    ballMat->addDisplacementMap("ball_height");
>  }
>
>  MyApp::initialize() {
>    createCamera(G3D_CAMERA_FREE_MOVE);
>
>    glass = createObject("sphere");
>    glass->setMaterial("glass");
>    glass->setPosition(camera.pos + camera.front * 2 + Vector.LEFT);
>
>    ball = createObject("sphere");
>    ball->setMaterial("ball");
>    ball->setPosition(camera.pos + camera.front * 2 + Vector.RIGHT);
>  }
>
>  MyApp::run() {
>    useShader("simple_phong");
>    draw(glass);
>    draw(ball);
>  }
>
>  MyApp::closeListener(const Window) {
>    printf("Good Bye !\n");
>  }
>  ```
