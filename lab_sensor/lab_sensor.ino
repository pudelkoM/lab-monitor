#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "config.h"
#include "driver/temp_sensor.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "http_send.h"
#include "macros.h"
#include "temp_sensor.h"
#include "wifi.h"

namespace lab_sensor {

static const char* TAG = "Lab Sensor";

constexpr char sensor_id[] = "internal";

std::string format_data_line(std::string hostname, std::string rack,
                             std::string sensor, const float temperature) {
  auto size = ::snprintf(nullptr, 0, influx_line_format, hostname.c_str(),
                         rack.c_str(), sensor.c_str(), temperature) +
              1;
  std::string line;
  line.resize(size);
  ::snprintf(&line[0], line.size(), influx_line_format, hostname.c_str(),
             rack.c_str(), sensor.c_str(), temperature);
  line.pop_back();  // remove trailing null.
  return line;
}

void main_task(void* arg) {
  while (true) {
    auto t = sensor::get_temperature();
    ESP_LOGI(TAG, "Temperature out celsius %.0f°C", t);
    std::string line = format_data_line(host_name, "stratum", sensor_id, t);
    ESP_LOGI(TAG, "Sending %s", line.c_str());
    ESP_ERROR_LOG(http_send::post(url, line));
    vTaskDelay(reporting_interval_ms / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  esp_log_level_set("*", ESP_LOG_VERBOSE);
  wifi::setup_wifi();
  ESP_ERROR_CHECK(sensor::setup());
  BaseType_t ret = xTaskCreate(main_task, "main", 4096, NULL, 5, NULL);
}

}  // namespace lab_sensor

void setup() { lab_sensor::setup(); }

void loop() {
  // Do nothing, as we use FreeRTOS tasks.
  delay(10000000);
}
