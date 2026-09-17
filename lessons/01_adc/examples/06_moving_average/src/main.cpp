#include <Arduino.h>
#include <cstdint>

constexpr uint8_t ADC_PIN = 4;
constexpr uint8_t ADC_BITS = 12;
constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint32_t SAMPLE_DELAY_MS = 100;

constexpr uint16_t ADC_MAX_CODE = static_cast<uint16_t>((1u << ADC_BITS) - 1u);
constexpr uint8_t WINDOW_SIZE = 16;
static_assert(static_cast<uint32_t>(ADC_MAX_CODE) * WINDOW_SIZE <= UINT32_MAX,
              "Accumulator must hold the full window");
uint16_t samples[WINDOW_SIZE] = {};
uint8_t nextIndex = 0;
uint8_t filled = 0;
uint32_t sum = 0;

void setup() {
    Serial.begin(SERIAL_BAUD);
    analogReadResolution(ADC_BITS);
    // This core initializes the channel lazily on its first read.
    (void)analogRead(ADC_PIN);
    analogSetPinAttenuation(ADC_PIN, ADC_11db);
}

void loop() {
    const uint16_t raw = analogRead(ADC_PIN);
    sum -= samples[nextIndex];
    samples[nextIndex] = raw;
    sum += raw;
    ++nextIndex;
    if (nextIndex == WINDOW_SIZE) {
        nextIndex = 0;
    }
    if (filled < WINDOW_SIZE) {
        ++filled;
    }
    // Divide only by real samples during startup to avoid a false low ramp.
    const uint16_t average = static_cast<uint16_t>(sum / filled);
    Serial.print("raw="); Serial.print(raw);
    Serial.print(" | moving_average="); Serial.println(average);
    delay(SAMPLE_DELAY_MS);
}
