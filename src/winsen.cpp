#include "winsen.h"

static void updatewrapper(CSensorWindow *slider) { slider->updateData(); }

static Uint32 timer_callback(Uint32 interval, void *param) {

  CSensorWindow *pSensorWindow = static_cast<CSensorWindow *>(param);

  if (pSensorWindow->isTimerStopping()) {
    pSensorWindow->setTimerStopped();
    return interval;
  }

  SDL_Event event;
  SDL_UserEvent userevent;

  /* In this example, the callback pushes an SDL_USEREVENT event
  into the queue and causes the callback to be called again at the
  same interval: */

  updatewrapper(pSensorWindow);

  userevent.type = SDL_USEREVENT;
  userevent.code = 0;
  userevent.data1 = NULL;
  userevent.data2 = NULL;

  event.type = SDL_USEREVENT;
  event.user = userevent;

  SDL_PushEvent(&event);

  return interval;
}

CSensorWindow::CSensorWindow(CSensor<float> *ptr_, CRect r, color wc, color fc,
                             uint32_t f, uint32_t data_cap)
    : CFramedWindow(r, wc, fc), ptr(ptr_), text(ptr_->getName()),
      freq(f * 1000), data_capacity(data_cap) {
  t_status = TIMER_RUNNING;
  data.assign(data_capacity, ptr->getlowLimit());
  highLimit = ptr->gethighLimit();
  lowLimit = ptr->getlowLimit();
  timer_id = SDL_AddTimer(freq, timer_callback, this);
}

CSensorWindow::~CSensorWindow() {
  setTimerStopping();
  while (!isTimerStopped())
    ;
}

void CSensorWindow::setTimerStopped() {
  timer_mutex.lock();
  t_status = TIMER_STOPPED;
  timer_mutex.unlock();

  SDL_RemoveTimer(timer_id);
}

void CSensorWindow::setTimerStopping() {
  timer_mutex.lock();
  t_status = TIMER_STOPPING;
  timer_mutex.unlock();
}

bool CSensorWindow::isTimerStopped() {
  timer_mutex.lock();
  bool result = (t_status == TIMER_STOPPED);
  timer_mutex.unlock();
  return result;
}

bool CSensorWindow::isTimerStopping() {
  timer_mutex.lock();
  bool result = (t_status == TIMER_STOPPING);
  timer_mutex.unlock();
  return result;
}

void CSensorWindow::paint() {
  CFramedWindow::paint();

  uint32_t dy = geom.size.y / 22;

  gfx_textout(geom.topleft.x + 1, geom.topleft.y + 1, text.c_str(), BLACK);
  gfx_textout(geom.topleft.x + geom.size.x - 30, geom.topleft.y + 2,
              ptr->getUnit().c_str(), BLACK);

  for (uint8_t i = 0; i < 10; ++i) {
    std::string value =
        std::to_string(highLimit - (highLimit - lowLimit) / 9 * i)
            .substr(0,
                    std::to_string(highLimit - (highLimit - lowLimit) / 9 * i)
                            .find(".") +
                        2);
    gfx_textout(geom.topleft.x + geom.size.x - 40,
                geom.topleft.y + 2.25 * i * dy + 12, value.c_str(), BLACK);
  }

  std::deque<float> data_to_print;
  result_mutex.lock();
  data_to_print = data;
  result_mutex.unlock();

  float x1, y1;
  float x2, y2;

  for (uint32_t i = 0; i < data_to_print.size() - 1; ++i) {
    x1 = geom.topleft.x + geom.size.x - 50 -
         i * (geom.size.x - 50) / data_to_print.size();
    y1 = geom.topleft.y + dy +
         ((highLimit - data_to_print[i]) / (highLimit - lowLimit)) *
             (geom.size.y - 3 * dy);
    y2 = geom.topleft.y + dy +
         ((highLimit - data_to_print[i + 1]) / (highLimit - lowLimit)) *
             (geom.size.y - 3 * dy);
    x2 = geom.topleft.x + geom.size.x - 50 -
         (i + 1) * (geom.size.x - 50) / data_to_print.size();

    gfx_line(x1, y1, x2, y2, BLACK);
    gfx_filledCircle(x1, y1, 3, RED);
  }
}

void CSensorWindow::updateData() {
  float data_value = ptr->getMeasurement();
  result_mutex.lock();
  data.pop_back();
  data.push_front(data_value);
  result_mutex.unlock();
}
