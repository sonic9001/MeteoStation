#include <stdlib.h>
#include <unistd.h>

#include "clock.h"
#include "winsen.h"
#include "winsys.h"

int main(int argc, char *argv[]) {
  if (gfx_init())
    exit(3);
  CTemperatureSensor<float> *temp =
      new CTemperatureSensor<float>(-10.0, 20.0, "temp", "*C");
  CHumiditySensor<float> *hum =
      new CHumiditySensor<float>(0.0, 100.0, "hum", "%");

  CSensorWindow *window = new CSensorWindow(
      temp, CRect(CPoint(100, 50), CPoint(400, 150)), WHITE, BLACK, 1, 25);
  CSensorWindow *window2 = new CSensorWindow(
      hum, CRect(CPoint(100, 200), CPoint(400, 150)), WHITE, BLACK, 2, 25);

  CDesktop d;
  CGroup *x = new CGroup;
  x->insert(window);
  x->insert(window2);
  d.insert(x);

  d.run();

  return 0;
}
