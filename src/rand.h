#pragma once

#include <stdlib.h>

inline float getRandomNumber(float lowLimit, float highLimit) {
  return ((float)random() / RAND_MAX * (highLimit - lowLimit)) + lowLimit;
}
