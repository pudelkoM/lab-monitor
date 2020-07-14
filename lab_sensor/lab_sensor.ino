#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "bme280.h"
#include "config.h"
#include "driver/temp_sensor.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "http_send.h"
#include "macros.h"
#include "temp_sensor.h"
#include "wifi.h"

#define SEALEVELPRESSURE_HPA (1013.2)

namespace lab_sensor {

static const char* TAG = "Lab Sensor";

constexpr char sensor_id[] = "ambient";

std::string format_data_line(std::string hostname, std::string rack,
                             std::string sensor, const float temperature,
                             const float humidity, const float pressure) {
  auto size =
      ::snprintf(nullptr, 0, influx_line_format, hostname.c_str(), rack.c_str(),
                 sensor.c_str(), temperature, humidity, pressure) +
      1;
  std::string line;
  line.resize(size);
  ::snprintf(&line[0], line.size(), influx_line_format, hostname.c_str(),
             rack.c_str(), sensor.c_str(), temperature);
  line.pop_back();  // remove trailing null.
  return line;
}

void main_task(void* arg) {
  int err_count = 0;
  while (true) {
    auto bme = bme280::get();
    float temperature = bme->readTemperature();
    float humidity = bme->readHumidity();
    float pressure = bme->readPressure() / 100.0F;
    float altitude = bme->readAltitude(SEALEVELPRESSURE_HPA);

    ESP_LOGI(TAG, "Temperature: %.2f°C", temperature);
    ESP_LOGI(TAG, "Humidity: %.0f%%", humidity);
    ESP_LOGI(TAG, "Pressure: %.2fhPa", pressure);
    ESP_LOGI(TAG, "Altitude: %.2fm", altitude);

    std::string line = format_data_line(host_name, "stratum", sensor_id,
                                        temperature, humidity, pressure);
    ESP_LOGI(TAG, "Sending %s", line.c_str());
    esp_err_t err = http_send::post(url, line);
    if (err != ESP_OK) {
      LOG_IF_ESP_ERROR(err);
      if (err_count++ > 4) {
        esp_restart();
      }
    }
    vTaskDelay(reporting_interval_ms / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  esp_log_level_set("*", ESP_LOG_INFO);
  wifi::setup_wifi();
  ESP_ERROR_CHECK(sensor::setup());
  ESP_ERROR_CHECK(bme280::setup());
  BaseType_t ret = xTaskCreate(main_task, "main", 4096, NULL, 5, NULL);
}

}  // namespace lab_sensor

void setup() { lab_sensor::setup(); }

void loop() {
  // Do nothing, as we use FreeRTOS tasks.
  delay(10000000);
}
