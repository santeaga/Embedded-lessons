#include <Arduino.h>
#include <cstdint>

constexpr uint8_t ADC_PIN = 4;
constexpr uint8_t ADC_BITS = 12;
constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint32_t SAMPLE_DELAY_MS = 100;

constexpr uint16_t ADC_MAX_CODE = static_cast<uint16_t>((1u << ADC_BITS) - 1u);
// A hypothetical 0..3.3 V endpoint model, NOT the S3 calibration curve.
constexpr uint32_t IDEAL_FULL_SCALE_MV = 3300;

void setup() {
    Serial.begin(SERIAL_BAUD);
    analogReadResolution(ADC_BITS);
    // This core initializes the channel lazily on its first read.
    (void)analogRead(ADC_PIN);
    analogSetPinAttenuation(ADC_PIN, ADC_11db);
}

void loop() {
    const uint16_t raw = analogRead(ADC_PIN);
    const uint32_t idealMv =
        (static_cast<uint32_t>(raw) * IDEAL_FULL_SCALE_MV) / ADC_MAX_CODE;
    // This call makes a NEW conversion, so the two results are not one sample.
    const uint32_t calibratedMv = analogReadMilliVolts(ADC_PIN);
    Serial.print("raw="); Serial.print(raw);
    Serial.print(" | ideal_mV(model)="); Serial.print(idealMv);
    Serial.print(" | calibrated_mV="); Serial.println(calibratedMv);
    delay(SAMPLE_DELAY_MS);
}
