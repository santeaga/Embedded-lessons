#include <Arduino.h>
#include <cstdint>

constexpr uint8_t ADC_PIN = 4;
constexpr uint8_t ADC_BITS = 12;
constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint32_t SAMPLE_DELAY_MS = 100;

void setup() {
    Serial.begin(SERIAL_BAUD);
    analogReadResolution(ADC_BITS);
    // This core initializes the channel lazily on its first read.
    (void)analogRead(ADC_PIN);
    // The divider can approach the 3.3 V rail; even this range can saturate.
    analogSetPinAttenuation(ADC_PIN, ADC_11db);
}

void loop() {
    const uint16_t raw = analogRead(ADC_PIN);
    Serial.println(raw);
    delay(SAMPLE_DELAY_MS);
}
