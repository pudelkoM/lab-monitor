#ifndef WIFI_H_
#define WIFI_H_

#include "esp_err.h"

namespace lab_sensor {
namespace wifi {
#define EXAMPLE_ESP_MAXIMUM_RETRY 10

esp_err_t setup_wifi(void);
}  // namespace wifi
}  // namespace lab_sensor

#endif  // WIFI_H_
