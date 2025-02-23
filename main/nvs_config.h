#pragma once

#include <stdint.h>

// Max length 15

#define NVS_CONFIG_WIFI_SSID "wifissid"
#define NVS_CONFIG_WIFI_PASS "wifipass"
#define NVS_CONFIG_HOSTNAME "hostname"
#define NVS_CONFIG_STRATUM_URL "stratumurl"
#define NVS_CONFIG_STRATUM_PORT "stratumport"
#define NVS_CONFIG_STRATUM_USER "stratumuser"
#define NVS_CONFIG_STRATUM_PASS "stratumpass"
#define NVS_CONFIG_ASIC_FREQ "asicfrequency"
#define NVS_CONFIG_ASIC_VOLTAGE "asicvoltage"
#define NVS_CONFIG_ASIC_JOB_INTERVAL "asicjobinterval"
#define NVS_CONFIG_FLIP_SCREEN "flipscreen"
#define NVS_CONFIG_INVERT_SCREEN "invertscreen"
#define NVS_CONFIG_INVERT_FAN_POLARITY "invertfanpol"
#define NVS_CONFIG_AUTO_FAN_SPEED "autofanspeed"
#define NVS_CONFIG_FAN_SPEED "fanspeed"
#define NVS_CONFIG_BEST_DIFF "bestdiff"
#define NVS_CONFIG_SELF_TEST "selftest"
#define NVS_CONFIG_AUTO_SCREEN_OFF "autoscreenoff"
#define NVS_CONFIG_OVERHEAT_TEMP "overheat_temp"

#define NVS_CONFIG_INFLUX_ENABLE "influx_enable"
#define NVS_CONFIG_INFLUX_URL "influx_url"
#define NVS_CONFIG_INFLUX_TOKEN "influx_token"
#define NVS_CONFIG_INFLUX_PORT "influx_port"
#define NVS_CONFIG_INFLUX_BUCKET "influx_bucket"
#define NVS_CONFIG_INFLUX_ORG "influx_org"
#define NVS_CONFIG_INFLUX_PREFIX "influx_prefix"

#define NVS_CONFIG_SWARM "swarmconfig"

#ifdef __cplusplus
extern "C"
{
#endif

    char *nvs_config_get_string(const char *key, const char *default_value);
    void nvs_config_set_string(const char *key, const char *default_value);
    uint16_t nvs_config_get_u16(const char *key, const uint16_t default_value);
    void nvs_config_set_u16(const char *key, const uint16_t value);
    uint64_t nvs_config_get_u64(const char *key, const uint64_t default_value);
    void nvs_config_set_u64(const char *key, const uint64_t value);

#ifdef __cplusplus
}
#endif
