#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_

#include <esp_err.h>

namespace lab_sensor {
namespace sensor {
esp_err_t setup(void);
float get_temperature(void);
}  // namespace sensor
}  // namespace lab_sensor

#endif  // TEMP_SENSOR_H_
