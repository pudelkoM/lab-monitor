#include "http_send.h"

#include "esp_http_client.h"
#include "esp_log.h"

namespace lab_sensor {
namespace http_send {

static const char *TAG = "http client";

esp_err_t _http_event_handle(esp_http_client_event_t *evt) {
  switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
      ESP_LOGV(TAG, "HTTP_EVENT_ERROR");
      break;
    case HTTP_EVENT_ON_CONNECTED:
      ESP_LOGV(TAG, "HTTP_EVENT_ON_CONNECTED");
      break;
    case HTTP_EVENT_HEADER_SENT:
      ESP_LOGV(TAG, "HTTP_EVENT_HEADER_SENT");
      break;
    case HTTP_EVENT_ON_HEADER:
      ESP_LOGV(TAG, "HTTP_EVENT_ON_HEADER");
      printf("%.*s", evt->data_len, (char *)evt->data);
      break;
    case HTTP_EVENT_ON_DATA:
      ESP_LOGV(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
      if (!esp_http_client_is_chunked_response(evt->client)) {
        printf("%.*s", evt->data_len, (char *)evt->data);
      }

      break;
    case HTTP_EVENT_ON_FINISH:
      ESP_LOGV(TAG, "HTTP_EVENT_ON_FINISH");
      break;
    case HTTP_EVENT_DISCONNECTED:
      ESP_LOGV(TAG, "HTTP_EVENT_DISCONNECTED");
      break;
  }
  return ESP_OK;
}

esp_err_t post(std::string url, std::string payload) {
  esp_http_client_config_t config = {
      .url = url.c_str(),
      .method = HTTP_METHOD_POST,
      .event_handler = _http_event_handle,
  };
  esp_http_client_handle_t client = esp_http_client_init(&config);
  if (client == NULL) {
    ESP_LOGE(TAG, "esp_http_client_init failed.");
    return ESP_ERR_INVALID_ARG;
  }
  ESP_ERROR_CHECK(
      esp_http_client_set_header(client, "Content-Type", "application/text"));
  ESP_ERROR_CHECK(
      esp_http_client_set_post_field(client, payload.data(), payload.size()));

  // esp_err_t err = ESP_OK;
  esp_err_t err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    ESP_LOGV(TAG, "Status = %d, content_length %d",
             esp_http_client_get_status_code(client),
             esp_http_client_get_content_length(client));
  }
  esp_http_client_cleanup(client);
  return err;
}

}  // namespace http_send
}  // namespace lab_sensor
