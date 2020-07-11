#include "bme280.h"

#include <Wire.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace lab_sensor {
namespace bme280 {

static const char *TAG = "bme280";

Adafruit_BME280 bme;

esp_err_t setup(void) {
  if (!Wire.begin(BME280_PIN_SDA, BME280_PIN_SCL)) {
    ESP_LOGE(TAG, "Could not init i2c");
    return ESP_FAIL;
  }
  if (!bme.begin(BME280_I2C_ADDRESS, &Wire)) {
    ESP_LOGE(TAG, "bme init failed!");
    return ESP_FAIL;
  }
  bme.getTemperatureSensor()->printSensorDetails();
  bme.getPressureSensor()->printSensorDetails();
  bme.getHumiditySensor()->printSensorDetails();

  return ESP_OK;
}

Adafruit_BME280 *get() { return &bme; }

}  // namespace bme280
}  // namespace lab_sensor
