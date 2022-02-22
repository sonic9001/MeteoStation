#pragma once

#include <ostream>
#include <string>

using namespace std;

#include "rand.h"

template <class T> class CSensor {
private:
  T lowLimit;
  T highLimit;
  mutable T Measurement;
  T makeMeasurement() const { return getRandomNumber(lowLimit, highLimit); }

protected:
  void checkLimit(T lower, T upper) {
    if (lowLimit > highLimit) {
      T tmp;
      tmp = lowLimit;
      lowLimit = highLimit;
      highLimit = tmp;
    }
    if (lowLimit < lower || lowLimit > upper)
      lowLimit = lower;
    if (highLimit > upper || highLimit < lower)
      highLimit = upper;
  };

public:
  CSensor(T lowLimit, T highLimit) : lowLimit(lowLimit), highLimit(highLimit) {}

  T getMeasurement() const {
    Measurement = makeMeasurement();
    return Measurement;
  }

  T gethighLimit() const { return highLimit; }
  T getlowLimit() const { return lowLimit; }

  virtual string getName() const = 0;
  virtual string getUnit() const = 0;
  friend ostream &operator<<(ostream &os, const CSensor &sensor) {
    os << "Current " << sensor.getName() << ": " << sensor.getMeasurement()
       << " " << sensor.getUnit() << std::endl;
    return os;
  }
};

template <class T> class CTemperatureSensor : public CSensor<T> {
private:
  string name;
  string unit;

public:
  CTemperatureSensor(T lowLimit, T highLimit, string name_ = "Temperature",
                     string unit_ = "*C")
      : CSensor<T>(lowLimit, highLimit) {
    this->checkLimit(-88.0f, 88.0f);
    name = name_;
    unit = unit_;
  };

  string getName() const { return name; }

  string getUnit() const { return unit; }
};

template <class T> class CHumiditySensor : public CSensor<T> {
private:
  string name;
  string unit;

public:
  CHumiditySensor(T lowLimit, T highLimit, string name_ = "Humidity",
                  string unit_ = "%")
      : CSensor<T>(lowLimit, highLimit) {
    this->checkLimit(0.0f, 100.0f);
    name = name_;
    unit = unit_;
  };

  string getName() const { return name; }

  string getUnit() const { return unit; }
};

template <class T> class CWindSensor : public CSensor<T> {
private:
  string name;
  string unit;

public:
  CWindSensor(T lowLimit, T highLimit, string name_ = "Wind",
              string unit_ = "km/h")
      : CSensor<T>(lowLimit, highLimit) {
    this->checkLimit(0.0f, 500.0f);
    name = name_;
    unit = unit_;
  };

  string getName() const { return name; }

  string getUnit() const { return unit; }
};

template <class T> class CPressureSensor : public CSensor<T> {
private:
  string name;
  string unit;

public:
  CPressureSensor(T lowLimit, T highLimit, string name_ = "Pressure",
                  string unit_ = "hPa")
      : CSensor<T>(lowLimit, highLimit) {
    this->checkLimit(0.0f, 500.0f);
    name = name_;
    unit = unit_;
  };

  string getName() const { return name; }

  string getUnit() const { return unit; }
};

template <class T> class CInsolationSensor : public CSensor<T> {
private:
  string name;
  string unit;

public:
  CInsolationSensor(float lowLimit, float highLimit,
                    string name_ = "Insolation", string unit_ = "W/m^2")
      : CSensor<T>(lowLimit, highLimit) {
    this->checkLimit(0.0f, 10000.0f);
    name = name_;
    unit = unit_;
  }

  string getName() const { return name; }
  string getUnit() const { return unit; }
};