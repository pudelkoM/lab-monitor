#ifndef HTTP_SEND_H_
#define HTTP_SEND_H_

#include <esp_err.h>

#include <string>

namespace lab_sensor {
namespace http_send {
esp_err_t post(std::string url, std::string payload);
}  // namespace http_send
}  // namespace lab_sensor

#endif  // HTTP_SEND_H_
