#ifndef BME280_H_
#define BME280_H_

#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <esp_err.h>

#include "config.h"

namespace lab_sensor {
namespace bme280 {
constexpr uint8_t BME280_I2C_ADDRESS = 0x77;
constexpr uint8_t BME280_I2C_ADDRESS_ALTERNATE = 0x76;

esp_err_t setup(void);
Adafruit_BME280 *get();
}  // namespace bme280
}  // namespace lab_sensor

#endif  // BME280_H_
