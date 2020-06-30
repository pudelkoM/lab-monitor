#include "temp_sensor.h"

#include "driver/temp_sensor.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace lab_sensor {
namespace sensor {

static const char *TAG = "temp sensor";

static TaskHandle_t sensor_poll_task_handle = NULL;

static float last_temp_ = 0;

constexpr int sensor_poll_interval_ms = 2000;

void tempsensor_poll_task(void *arg) {
  ESP_LOGI(TAG, "Initializing Temperature sensor");
  float tsens_out;
  temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(temp_sensor_get_config(&temp_sensor));
  ESP_LOGI(TAG, "default dac %d, clk_div %d", temp_sensor.dac_offset,
           temp_sensor.clk_div);
  temp_sensor.dac_offset = TSENS_DAC_L2;
  temp_sensor.clk_div = 18;
  ESP_ERROR_CHECK(temp_sensor_set_config(temp_sensor));
  ESP_ERROR_CHECK(temp_sensor_start());
  ESP_LOGI(TAG, "Temperature sensor started");
  while (1) {
    ESP_ERROR_CHECK(temp_sensor_read_celsius(&tsens_out));
    ESP_LOGV(TAG, "Internal temperature: %0.2f°C", tsens_out);
    last_temp_ = tsens_out;
    vTaskDelay(sensor_poll_interval_ms / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
  sensor_poll_task_handle = nullptr;
}

esp_err_t setup(void) {
  BaseType_t ret = xTaskCreate(tempsensor_poll_task, "temp", 2048, NULL, 5,
                               &sensor_poll_task_handle);
  if (ret != pdPASS) {
    ESP_LOGE(TAG, "Failed to create sensor poll task");
    return ESP_ERR_NO_MEM;
  }
  return ESP_OK;
}

float get_temperature(void) { return last_temp_; }

esp_err_t stop() {
  vTaskDelete(sensor_poll_task_handle);
  return ESP_OK;
}

}  // namespace sensor
}  // namespace lab_sensor
