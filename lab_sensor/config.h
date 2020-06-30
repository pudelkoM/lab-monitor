#ifndef CONFIG_H_

namespace lab_sensor {

// Device identity.
constexpr char host_name[] = "esp32s2_1";

// Influx configuration.
constexpr char url[] = "http://10.128.13.253:8086/write?db=temps&precision=s";
constexpr char influx_line_format[] =
    "temperature,host=%s,rack=%s,sensor=%s value=%0.0f";
constexpr int reporting_interval_ms = 15000;

// WIFI configuration.
#define WIFI_SSID "<your-ssid>"
#define WIFI_PASS "<your-password>"
}  // namespace lab_sensor

#endif  // CONFIG_H_
