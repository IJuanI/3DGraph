#include <graph3d.h>

class MyApp : Graph3D {
  private:
    int *glass, *ball;
  public:
    MyApp();

    void configure();
    void preinitialize();
    void initialize();
    void run();

    void closeListener(const int);
}