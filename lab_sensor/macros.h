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
  inline esp_err_t ErrorCode() const { return err_; }

 private:
  esp_err_t err_;
};

// A macro for simplify checking the return value of a ESP function call.
#define RETURN_IF_ESP_ERROR(expr)            \
  if (const BooleanEspStatus __ret = expr) { \
  } else /* NOLINT */                        \
    return __ret.ErrorCode()

// Run expr and log the error, if it occurs.
#define LOG_IF_ESP_ERROR(expr)                                 \
  if (const BooleanEspStatus __ret = expr) {                   \
  } else                                                       \
    ESP_LOGE(TAG, "'%s' failed with error message: %s", #expr, \
             esp_err_to_name(__ret));

}  // namespace lab_sensor

#endif  // MACROS_H_
