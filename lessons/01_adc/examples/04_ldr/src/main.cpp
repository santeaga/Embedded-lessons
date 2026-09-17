#include <Arduino.h>
#include <cstdint>

constexpr uint8_t ADC_PIN = 4;
constexpr uint8_t ADC_BITS = 12;
constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint32_t SAMPLE_DELAY_MS = 100;
constexpr uint16_t ADC_MAX_CODE = static_cast<uint16_t>((1u << ADC_BITS) - 1u);

// First record your dark/bright readings, then replace these illustrative values.
constexpr uint16_t MEASURED_DARK_RAW = 500;
constexpr uint16_t MEASURED_BRIGHT_RAW = 3000;
constexpr bool THRESHOLD_CALIBRATED = false;
constexpr uint16_t LIGHT_THRESHOLD =
    (static_cast<uint32_t>(MEASURED_DARK_RAW) + MEASURED_BRIGHT_RAW) / 2u;
static_assert(MEASURED_DARK_RAW < MEASURED_BRIGHT_RAW,
              "Upper LDR should produce a higher code in brighter light");
static_assert(MEASURED_BRIGHT_RAW <= ADC_MAX_CODE, "Use a 12-bit measurement");

void setup() {
    Serial.begin(SERIAL_BAUD);
    analogReadResolution(ADC_BITS);
    // This core initializes the channel lazily on its first read.
    (void)analogRead(ADC_PIN);
    analogSetPinAttenuation(ADC_PIN, ADC_11db);
}

void loop() {
    const uint16_t raw = analogRead(ADC_PIN);
    const uint32_t calibratedMv = analogReadMilliVolts(ADC_PIN);
    Serial.print("raw="); Serial.print(raw);
    Serial.print(" | calibrated_mV="); Serial.print(calibratedMv);
    Serial.print(" | light state=");
    if (!THRESHOLD_CALIBRATED) {
        Serial.println("observe first; set measured endpoints and calibration flag");
    } else {
        Serial.println(raw >= LIGHT_THRESHOLD ? "bright" : "dark");
    }
    delay(SAMPLE_DELAY_MS);
}
