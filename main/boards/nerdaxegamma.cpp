#include <math.h>
#include "serial.h"
#include "board.h"
#include "nvs_config.h"
#include "nerdaxegamma.h"

#include "drivers/nerdaxe/DS4432U.h"
#include "drivers/nerdaxe/EMC2101.h"
#include "drivers/nerdaxe/INA260.h"
#include "drivers/nerdaxe/adc.h"
#include "drivers/nerdaxe/TPS546.h"

#define BM1370_RST_PIN GPIO_NUM_1

bool tempinit = false;

static const char* TAG="nerdaxeGamma";

NerdaxeGamma::NerdaxeGamma() : NerdAxe() {
    m_deviceModel = "NerdaxeGamma";
    m_asicModel = "BM1370";
    m_version = 200;
    m_asicCount = 1;

    m_asicJobIntervalMs = 1500;
    m_asicFrequency = 525.0;
    m_asicVoltage = 1.15;
    m_initVoltage = 1.15;
    m_fanInvertPolarity = false;
    m_fanPerc = 100;

    m_maxPin = 19.0;
    m_minPin = 5.0;
    m_maxVin = 5.5;
    m_minVin = 4.5;

    m_asicMaxDifficulty = 512;
    m_asicMinDifficulty = 128;

#ifdef NERDAXEGAMMA
    m_theme = new ThemeNerdaxe();
#endif
    m_asics = new BM1370();
}


bool NerdaxeGamma::initBoard()
{
    ADC_init();
    SERIAL_init();

    // Init I2C
    if (i2c_master_init() != ESP_OK) {
        ESP_LOGE(TAG, "I2C initializing failed");
        return false;
    }

    EMC2101_init(m_fanInvertPolarity);
    EMC2101_set_ideality_factor(EMC2101_IDEALITY_1_0319);
    EMC2101_set_beta_compensation(EMC2101_BETA_11);
    setFanSpeed(m_fanPerc);
    

    //Init voltage controller
    if (TPS546_init() != ESP_OK) {
        ESP_LOGE(TAG, "TPS546 init failed!");
        return ESP_FAIL;
    }
    setVoltage(0.0);

    gpio_pad_select_gpio(BM1370_RST_PIN);
    gpio_set_direction(BM1370_RST_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(BM1370_RST_PIN, 0);

    return true;
}

bool NerdaxeGamma::initAsics() {

    // set output voltage
    setVoltage(0.0);

    // wait 500ms
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // set reset low
    gpio_set_level(BM1370_RST_PIN, 0);

    // wait 250ms
    vTaskDelay(250 / portTICK_PERIOD_MS);

    // set the init voltage
    // use the higher voltage for initialization
    setVoltage(fmaxf(m_initVoltage, m_asicVoltage));

    // wait 500ms
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // release reset pin
    gpio_set_level(BM1370_RST_PIN, 1);

    // delay for 250ms
    vTaskDelay(250 / portTICK_PERIOD_MS);

    SERIAL_clear_buffer();
    if (!m_asics->init(m_asicFrequency, m_asicCount, m_asicMaxDifficulty)) {
        ESP_LOGE(TAG, "error initializing asics!");
        return false;
    }
    SERIAL_set_baud(m_asics->setMaxBaud());
    SERIAL_clear_buffer();

    vTaskDelay(500 / portTICK_PERIOD_MS);

    m_isInitialized = true;
    return true;
}

bool NerdaxeGamma::setVoltage(float core_voltage)
{
    ESP_LOGI(TAG, "Set ASIC voltage = %.3fV", core_voltage);
    TPS546_set_vout(core_voltage);

    return true;
}

float NerdaxeGamma::readTemperature(int index) {
    
    if (!m_isInitialized) return EMC2101_get_internal_temp() + 5;

    if (!index) {
        //return (float)TPS546_get_temperature(); - vr_temp (voltage regulator temp)
        return EMC2101_get_external_temp(); //External board Temp
        //return EMC2101_get_internal_temp() + 5;
    } else {
        return 0.0;
    }

}

float NerdaxeGamma::getVin() {
    return TPS546_get_vin();
}

float NerdaxeGamma::getIin() {
    return TPS546_get_iout();
}

float NerdaxeGamma::getPin() {
    return TPS546_get_vout() * TPS546_get_iout();
}

float NerdaxeGamma::getVout() {
    return ADC_get_vcore() / 1000.0;
}

float NerdaxeGamma::getIout() {
    return TPS546_get_iout();
}

float NerdaxeGamma::getPout() {
    return getPin();
}

