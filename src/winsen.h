#include <algorithm>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>

#include "sensor.h"
#include "winsys.h"

enum TimerStatus { TIMER_RUNNING, TIMER_STOPPING, TIMER_STOPPED };

class CSensorWindow : public CFramedWindow {
private:
  CSensor<float> *ptr;
  std::deque<float> data;
  SDL_TimerID timer_id;
  string text;
  uint32_t freq;
  uint32_t data_capacity;
  std::mutex result_mutex;
  std::mutex timer_mutex;
  TimerStatus t_status;
  float lowLimit;
  float highLimit;

public:
  CSensorWindow(CSensor<float> *ptr_, CRect r, color wc, color fc, uint32_t f,
                uint32_t data_cap);
  ~CSensorWindow();
  void setTimerStopped();
  void setTimerStopping();
  bool isTimerStopping();
  bool isTimerStopped();
  void paint();
  void updateData();
};
