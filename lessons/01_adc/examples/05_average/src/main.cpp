#include <Arduino.h>
#include <cstdint>

constexpr uint8_t ADC_PIN = 4;
constexpr uint8_t ADC_BITS = 12;
constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint32_t SAMPLE_DELAY_MS = 100;

constexpr uint16_t ADC_MAX_CODE = static_cast<uint16_t>((1u << ADC_BITS) - 1u);
constexpr uint16_t SAMPLE_COUNT = 32;
constexpr uint32_t BETWEEN_SAMPLES_MS = 2;
static_assert(static_cast<uint32_t>(ADC_MAX_CODE) * SAMPLE_COUNT <= UINT32_MAX,
              "Accumulator must hold the worst-case sum");

void setup() {
    Serial.begin(SERIAL_BAUD);
    analogReadResolution(ADC_BITS);
    // This core initializes the channel lazily on its first read.
    (void)analogRead(ADC_PIN);
    analogSetPinAttenuation(ADC_PIN, ADC_11db);
}

void loop() {
    // 32 * 4095 = 131040, which exceeds uint16_t's 65535 maximum.
    uint32_t sum = 0;
    uint16_t raw = 0;
    for (uint16_t sample = 0; sample < SAMPLE_COUNT; ++sample) {
        raw = analogRead(ADC_PIN);
        sum += raw;
        delay(BETWEEN_SAMPLES_MS);
    }
    const uint16_t average = static_cast<uint16_t>(sum / SAMPLE_COUNT);
    // Show the last member of this batch alongside its batch average.
    Serial.print("raw="); Serial.print(raw);
    Serial.print(" | average="); Serial.println(average);
    delay(SAMPLE_DELAY_MS);
}
