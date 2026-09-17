#include <Arduino.h>
#include <cstdint>

constexpr uint8_t ADC_PIN = 4;
constexpr uint8_t ADC_BITS = 12;
constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint32_t SAMPLE_DELAY_MS = 100;

constexpr adc_attenuation_t SETTINGS[] = {ADC_0db, ADC_2_5db, ADC_6db, ADC_11db};
constexpr const char* LABELS[] = {"0 dB (~950 mV)", "2.5 dB (~1250 mV)",
                                "6 dB (~1750 mV)", "11 dB (~3100 mV)"};
constexpr uint8_t SETTING_COUNT = 4;
constexpr uint32_t SETTING_DELAY_MS = 1000;

void setup() {
    Serial.begin(SERIAL_BAUD);
    analogReadResolution(ADC_BITS);
    // This core initializes the channel lazily on its first read.
    (void)analogRead(ADC_PIN);
}

void loop() {
    // Hold the input steady during each sweep to compare the ranges.
    for (uint8_t index = 0; index < SETTING_COUNT; ++index) {
        analogSetPinAttenuation(ADC_PIN, SETTINGS[index]);
        const uint16_t raw = analogRead(ADC_PIN);
        Serial.print(LABELS[index]);
        // Per-pin range changes do not refresh this core's calibration handle.
        // Raw codes alone make this a valid range/saturation comparison.
        Serial.print(" | raw="); Serial.println(raw);
        delay(SETTING_DELAY_MS);
    }
}
