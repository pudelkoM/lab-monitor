#ifndef MACROS_H_
#define MACROS_H_

#include <esp_err.h>
#include <esp_log.h>

namespace lab_sensor {

// Helper class to convert ESP error type to bool.
class BooleanEspStatus {
 public:
  BooleanEspStatus(esp_err_t err) : err_(err) {}
  operator bool() const { return err_ == ESP_OK; }

 private:
  esp_err_t err_;
};

// Run expr and log the error, if it occurs.
#define ESP_ERROR_LOG(expr)                                    \
  if (const BooleanEspStatus __ret = expr) {                   \
  } else {                                                     \
    ESP_LOGE(TAG, "'%s' failed with error message: %s", #expr, \
             esp_err_to_name(__ret));                          \
  }
}  // namespace lab_sensor

#endif  // MACROS_H_
