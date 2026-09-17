# Specification: Lesson 01 — ADC / Analog Input

**Status:** Approved for implementation  
**Primary target:** ESP32-S3  
**Framework:** Arduino-ESP32  
**Build system:** PlatformIO  
**Primary teaching board:** YD-ESP32-S3 / ESP32-S3-DevKitC-1-compatible board  
**Repository:** `Embedded-lessons`

---

## 1. Purpose

Create a complete educational lesson about Analog-to-Digital Conversion (ADC) for an Embedded Systems course.

The lesson must preserve the useful material from the existing Module 1.6 student and teacher materials, but correct technical inaccuracies and expand the topic from a simple `analogRead()` + LDR exercise into a proper embedded-systems introduction to ADCs.

The central teaching flow is:

**real analog world → sampling → quantization → digital code → ADC architectures → DAC basics → SAR ADC → ESP32-S3 ADC → attenuation / dB → calibration → LDR / voltage divider → code → measurement → noise filtering**

The lesson must explain not only *how to call an API*, but also *what happens inside the hardware*.

---

## 2. Source material to preserve and improve

The new lesson is based on two existing course documents:

- `Модуль 1.6. Введення ADC.docx` — student-facing material.
- `1.6. (ARCHIVED)Введення ADC_ Як _читати_ світло (Аналоговий вхід)..docx` — extended / teacher material.

Important existing ideas that must remain:

- analog vs digital signal introduction;
- transition from digital input to analog measurement;
- ADC as a converter from voltage to a numeric code;
- 12-bit resolution and 0…4095 codes;
- voltage divider;
- LDR / photoresistor;
- ESP32-S3 ADC pin discussion;
- attenuation;
- ADC non-linearity / imperfect measurements;
- moving average;
- practical “smart light” idea;
- glossary and quiz concept.

Do not copy incorrect or oversimplified statements verbatim. Use the corrections in this specification.

---

## 3. Learning objectives

After the lesson, a student should be able to:

1. Explain the difference between an analog signal and a digital signal.
2. Explain why a digital MCU requires an ADC to measure analog voltage.
3. Distinguish **sampling** from **quantization**.
4. Explain sampling period and sampling frequency:
   - `Ts`
   - `Fs = 1 / Ts`
5. Explain the basic Nyquist condition:
   - `Fs > 2 × Fmax`
   - and understand that violating it causes aliasing.
6. Explain ADC resolution, number of codes, maximum code, and LSB.
7. Explain why **resolution is not the same thing as accuracy**.
8. Describe the basic operation and trade-offs of:
   - Flash ADC
   - SAR ADC
   - Sigma-Delta ADC
   - Dual-Slope ADC
9. Explain what a DAC does.
10. Explain why a DAC is used internally inside a SAR ADC.
11. Step through a small SAR conversion bit-by-bit.
12. Explain the ESP32-S3 ADC architecture at an introductory level.
13. Identify ADC1 and ADC2 channels on ESP32-S3.
14. Explain why ADC1 is preferred for general sensor examples.
15. Explain ADC attenuation and why attenuation extends the measurable input range.
16. Explain dB for voltage ratios.
17. Distinguish:
   - raw ADC code;
   - ideal calculated voltage;
   - calibrated voltage.
18. Use:
   - `analogRead()`
   - `analogReadMilliVolts()`
   - `analogReadResolution()`
   - `analogSetPinAttenuation()`
19. Build and calculate a voltage divider.
20. Use an LDR in a voltage divider and predict whether the ADC value rises or falls as illumination changes.
21. Observe ADC noise and reduce it with averaging / moving average.
22. Compare MCU ADC readings with a multimeter measurement.

---

## 4. Important technical corrections

### 4.1 Digital input is not literally “0 V or 3.3 V”

The existing introductory analogy may remain, but it must be clearly identified as a simplification.

Do not teach:

> Digital input understands exactly 0 V and exactly 3.3 V only.

Instead explain:

- a digital input interprets voltage using logic thresholds;
- voltages below a low threshold are interpreted as LOW;
- voltages above a high threshold are interpreted as HIGH;
- values in between may be undefined;
- 3.3 V is the MCU I/O supply domain, not a universal definition of logic HIGH.

Keep this short because logic thresholds are not the main subject of this lesson.

---

### 4.2 Resolution is not accuracy

Do not describe a 12-bit ADC as “12-bit accuracy”.

Use:

- **resolution** — number of representable ADC codes;
- **accuracy** — how close the measured result is to the true input value.

A 12-bit ADC can still have:

- offset error;
- gain error;
- INL;
- DNL;
- reference-voltage error;
- noise;
- input-source / acquisition error.

The lesson must explicitly contain:

> **12-bit resolution does not automatically mean 12-bit measurement accuracy.**

---

### 4.3 12-bit ADC math

For an `N`-bit ADC:

```text
Number of codes = 2^N
Maximum code    = 2^N - 1
```

For 12 bits:

```text
2^12 = 4096 codes
code range = 0 ... 4095
```

For an ideal ADC with full-scale input range `VFS`, define the ideal quantization step approximately as:

```text
LSB ≈ VFS / 2^N
```

For an ideal rounding quantizer, introduce the common approximation:

```text
quantization error ≈ ±0.5 LSB
```

Also explain the common educational endpoint-mapping formula:

```text
V ≈ Code / (2^N - 1) × VFS
```

Do not present these two expressions as a contradiction.

Explain that:

- `VFS / 2^N` describes the width of an ideal quantization bin;
- `Code / (2^N - 1) × VFS` is often used as a convenient endpoint mapping;
- a real ESP32-S3 ADC should not be treated as an ideal `0…3.3 V` ADC.

---

### 4.4 Do not use `3.3 V / 4095` as the real ESP32-S3 calibration model

The old material uses:

```text
V = ADC / 4095 × 3.3
```

Keep this only as a clearly labeled **idealized educational example**.

The real ESP32-S3 ADC:

- has an internal reference around 1.1 V nominally;
- reference value varies between chips;
- uses input attenuation to extend the measurable input range;
- is not perfectly linear;
- benefits from calibration.

Do not say that `3.3 V` is automatically the ADC reference voltage.

---

## 5. Lesson theory structure

### 5.1 Analog world vs digital world

Preserve the original transition:

- light;
- temperature;
- sound;
- pressure;
- other physical quantities are continuous in nature;
- MCU logic and stored data are digital.

Keep the existing water-tap analogy as an introductory analogy if useful.

Then move quickly from analogy to electrical engineering terminology.

Required distinction:

```text
analog quantity → continuously variable voltage/current
digital representation → finite set of codes
```

---

### 5.2 Sampling

Teach sampling as a time-domain process.

Required concepts:

```text
Ts = sampling period
Fs = sampling frequency
Fs = 1 / Ts
```

Explain that the ADC does not continuously “know the waveform”; it performs conversions at particular moments in time.

Use a diagram similar to:

```text
Analog signal
     ~~~~~~~~
   /          \
--/------------\-------> time
  ↑  ↑  ↑  ↑  ↑
 samples
```

Introduce Nyquist only at the level needed for this course:

```text
Fs > 2 × Fmax
```

Explain:

- this is the theoretical minimum for a band-limited signal;
- practical systems normally use margin and an anti-aliasing filter;
- insufficient sampling causes aliasing.

Do not turn this lesson into a DSP course.

Optional teacher demonstration:

- use a function generator if available;
- show a sine wave and too-low sampling rate;
- otherwise use a conceptual diagram.

---

### 5.3 Quantization

Teach quantization separately from sampling.

Required idea:

- sampling chooses **when** the signal is measured;
- quantization chooses **which digital level** represents the measured amplitude.

Use a staircase transfer diagram.

Show a small 3-bit example before jumping to 12 bits:

```text
3-bit ADC
2^3 = 8 codes
000 ... 111
```

Explain quantization error visually.

---

### 5.4 Resolution, code range, LSB

Required formulas:

```text
Levels = 2^N
MaxCode = 2^N - 1
LSB ≈ VFS / 2^N
```

Examples:

#### 3-bit example
Use a small example students can calculate manually.

#### 12-bit example
Show:

```text
2^12 = 4096
0 ... 4095
```

Do not equate resolution with accuracy.

---

### 5.5 ADC error sources

Keep this concise but technically correct.

Introduce:

- quantization error;
- offset error;
- gain error;
- INL — Integral Non-Linearity;
- DNL — Differential Non-Linearity;
- electrical noise;
- reference-voltage variation;
- source impedance / settling effects.

The goal is recognition, not mathematical characterization.

Use a simple conceptual graphic comparing:

```text
ideal transfer characteristic
vs
real transfer characteristic
```

---

## 6. ADC architectures

Create a comparison section for at least these four architectures.

| Architecture | Basic principle | Strength | Weakness | Typical use |
|---|---|---|---|---|
| Flash | Many comparators operate in parallel | Extremely fast | Large area, power, comparator count | Very high-speed conversion |
| SAR | Binary search using comparator + internal DAC | Good balance of speed, resolution and power | Sequential conversion | MCUs, control systems |
| Sigma-Delta | Oversampling, modulation, digital filtering | High resolution / precision | More latency, lower bandwidth | Audio, precision sensing |
| Dual-Slope | Integrate unknown signal, then integrate reference | Accurate, strong noise rejection | Slow | Digital multimeters |

### 6.1 Flash ADC

Explain:

- resistor ladder;
- comparator bank;
- approximately `2^N - 1` comparators for an N-bit basic flash converter;
- encoder.

Do not spend excessive lesson time here.

### 6.2 Sigma-Delta ADC

Explain:

- oversampling;
- modulator;
- 1-bit or low-bit internal stream concept;
- digital filtering / decimation;
- trade bandwidth for precision.

Do not derive noise-shaping equations.

### 6.3 Dual-Slope ADC

Explain:

1. integrate unknown voltage for a known time;
2. integrate a known reference of opposite polarity;
3. measure time required to return to zero.

Connect this architecture to multimeters.

### 6.4 SAR ADC

This is the architecture to teach in detail because ESP32-S3 uses SAR ADCs.

---

## 7. DAC basics

Introduce DAC immediately before the detailed SAR section.

### 7.1 Core concept

```text
ADC:
Analog voltage → Digital code

DAC:
Digital code → Analog voltage
```

Explain:

- N-bit DAC;
- digital input code;
- analog output level;
- `2^N` possible input codes.

A simple idealized DAC relation may be used:

```text
VDAC ≈ Code / 2^N × Vref
```

Explain that the exact transfer characteristic depends on DAC architecture.

### 7.2 Brief DAC architectures

Mention only enough to support understanding:

- resistor string;
- R-2R ladder.

Include an R-2R ladder diagram or conceptual illustration, but do not turn this into a full DAC-design lesson.

### 7.3 Critical ESP32-S3 note

Do not imply that the ESP32-S3 exposes a general-purpose analog-output DAC peripheral.

The Arduino-ESP32 DAC API is available for classic ESP32 and ESP32-S2, not ESP32-S3.

The **internal DAC used conceptually in a SAR converter is part of the ADC conversion mechanism** and is not the same as a user-accessible DAC output peripheral.

This distinction must be explicit.

---

## 8. SAR ADC — detailed bit-by-bit explanation

Required simplified block diagram:

```text
                 +----------------+
Vin ------------>| +              |
                 |   Comparator   |----> SAR logic/register ----> Result
VDAC ------------| -              |             |
                 +----------------+             |
                                                v
                                              [ DAC ]
                                                |
                                                +------> VDAC
```

Where useful, add a sample-and-hold block before the comparator.

Explain the algorithm as a binary search.

### 8.1 Required 4-bit worked example

Use:

```text
Vref = 3.3 V
Vin  ≈ 2.2 V
N    = 4
```

Use the simplified DAC relation:

```text
VDAC = Code / 16 × 3.3 V
```

Step through:

| Trial | VDAC | Comparator | Decision |
|---|---:|---|---|
| `1000` | 1.65 V | `Vin > VDAC` | keep MSB |
| `1100` | 2.475 V | `Vin < VDAC` | clear bit |
| `1010` | 2.0625 V | `Vin > VDAC` | keep bit |
| `1011` | 2.26875 V | `Vin < VDAC` | clear bit |

Final result:

```text
1010₂ = 10
```

Explain what happened:

- first test half-scale;
- each next comparison halves the remaining search interval;
- an N-bit SAR conversion requires one decision per bit, plus practical acquisition/control overhead.

The student should see that SAR behaves like a **binary search in voltage**.

---

## 9. ESP32-S3 ADC section

### 9.1 Architecture

Teach:

- ESP32-S3 contains **two 12-bit SAR ADC units**;
- total: 20 analog-capable channels.

### 9.2 Channels

Use the ESP32-S3 channel mapping:

```text
ADC1:
GPIO1  -> ADC1_CH0
GPIO2  -> ADC1_CH1
GPIO3  -> ADC1_CH2
GPIO4  -> ADC1_CH3
GPIO5  -> ADC1_CH4
GPIO6  -> ADC1_CH5
GPIO7  -> ADC1_CH6
GPIO8  -> ADC1_CH7
GPIO9  -> ADC1_CH8
GPIO10 -> ADC1_CH9

ADC2:
GPIO11 -> ADC2_CH0
...
GPIO20 -> ADC2_CH9
```

The examples for this lesson must use **ADC1**.

Preferred example pin:

```cpp
constexpr uint8_t ADC_PIN = 4;
```

Before implementation, Codex must ensure the selected pin is actually exposed and safe on the target YD-ESP32-S3 board variant.

### 9.3 ADC2 and Wi-Fi

Do not use the oversimplified statement:

> ADC2 simply does not work when Wi-Fi is enabled.

Teach instead:

- ADC2 is shared with Wi-Fi hardware/resources;
- official ESP32-S3 documentation warns about simultaneous use;
- current ESP-IDF oneshot drivers provide resource protection, but ADC2 may still be unavailable / fail while the resource is in use;
- examples in this lesson should therefore use ADC1.

This is both simpler and safer for students.

### 9.4 Oneshot vs continuous conversion

Introduce briefly:

**OneShot**
- one conversion when requested;
- `analogRead()` / `analogReadMilliVolts()` are appropriate introductory examples.

**Continuous**
- repeated sampling at configured sampling rate;
- useful for waveform/data acquisition;
- can be mentioned as an advanced continuation, not implemented deeply in the first lesson.

---

## 10. ESP32-S3 attenuation

Explain physically:

```text
Vin → input attenuator → internal ADC input → SAR ADC
```

The attenuator reduces the voltage reaching the ADC core, allowing a larger external voltage range to be measured.

### 10.1 Arduino-ESP32 names

For Arduino-ESP32, use:

```cpp
ADC_0db
ADC_2_5db
ADC_6db
ADC_11db
```

ESP32-S3 approximate measurable ranges from current Arduino-ESP32 documentation:

| Arduino setting | Approx. ESP32-S3 measurable input range |
|---|---:|
| `ADC_0db` | 0 … 0.95 V |
| `ADC_2_5db` | 0 … 1.25 V |
| `ADC_6db` | 0 … 1.75 V |
| `ADC_11db` | 0 … 3.10 V |

These are approximate usable measurement ranges, not permission to exceed electrical absolute-maximum ratings.

Do not claim that `ADC_11db` makes the ADC an ideal `0…3.3 V` converter.

### 10.2 ESP-IDF naming difference

Mention in teacher notes / advanced note:

- Arduino API uses `ADC_11db`;
- current ESP-IDF documentation uses `ADC_ATTEN_DB_12`;
- older ESP-IDF documentation may show `ADC_ATTEN_DB_11`.

Do not mix Arduino and ESP-IDF enum names inside one code example.

### 10.3 Attenuation selection rule

Do not say “11 dB is always recommended”.

Teach:

> Use the lowest attenuation that still covers the required input-voltage range.

Reason:

- unnecessary attenuation changes the usable transfer range;
- measurement characteristics depend on attenuation;
- maximize useful input range without saturation.

---

## 11. dB explanation

This lesson needs a compact but real explanation of decibels.

### 11.1 Voltage gain ratio

For voltage ratios under the same impedance conditions:

```text
Gain_dB = 20 × log10(Vout / Vin)
```

If the signal is attenuated, this value is negative.

Example:

```text
Vin  = 2.0 V
Vout = 1.0 V

Gain_dB = 20 × log10(1 / 2)
        ≈ -6.02 dB
```

### 11.2 Positive attenuation notation

Attenuation/loss is also commonly expressed as a positive number:

```text
Attenuation_dB = 20 × log10(Vin / Vout)
```

Same example:

```text
Attenuation ≈ +6.02 dB
```

Therefore:

```text
6 dB voltage attenuation ≈ ×0.5 voltage ratio
12 dB voltage attenuation ≈ ×0.25 voltage ratio
```

More generally:

```text
Vout / Vin = 10^(-A_dB / 20)
```

Make it clear why an ADC attenuation setting can extend the external input-voltage range.

Do not confuse voltage dB (`20 log`) with power dB (`10 log`).

---

## 12. Raw ADC vs calibrated voltage

This is a mandatory practical concept.

### 12.1 `analogRead()`

Use:

```cpp
uint16_t raw = analogRead(ADC_PIN);
```

Explain:

- returns raw ADC code;
- default Arduino-ESP32 read resolution is 12 bits;
- for ESP32-S3 examples use 0…4095;
- raw value is not a calibrated voltage measurement.

### 12.2 `analogReadMilliVolts()`

Use:

```cpp
uint32_t millivolts = analogReadMilliVolts(ADC_PIN);
```

Explain:

- performs an ADC conversion;
- returns a calibrated result in millivolts using the Arduino-ESP32 calibration path;
- it is preferable when the goal is an actual voltage estimate rather than only a relative sensor level.

### 12.3 `analogReadResolution()`

Include:

```cpp
analogReadResolution(12);
```

Important nuance:

On ESP32-S3, Arduino’s `analogReadResolution()` may shift the returned result to the requested width rather than changing the underlying SAR hardware resolution in the same way classic ESP32 can.

Do not teach “calling `analogReadResolution(10)` physically turns the ESP32-S3 ADC into a native 10-bit ADC” without qualification.

---

## 13. Voltage divider

Preserve and expand the original voltage-divider lesson.

For:

```text
Vin
 |
R1
 |
 +---- Vout
 |
R2
 |
GND
```

Use:

```text
Vout = Vin × R2 / (R1 + R2)
```

Students must calculate several examples manually.

Also derive the unknown upper resistor:

```text
R1 = R2 × (Vin / Vout - 1)
```

For the LDR arrangement used in this lesson:

```text
3.3 V
 |
LDR (R1)
 |
 +---- ADC input
 |
10 kΩ (R2)
 |
GND
```

Then:

```text
Vout = 3.3 × 10k / (RLDR + 10k)
```

Therefore:

- bright light → `RLDR` decreases → `Vout` increases;
- darkness → `RLDR` increases → `Vout` decreases.

Use at least these approximate examples:

```text
RLDR = 500 Ω
RLDR = 10 kΩ
RLDR = 50 kΩ
```

Calculate:

- `Vout`;
- approximate ideal ADC code for the selected model;
- expected qualitative behavior.

Important:
If powered from 3.3 V and using ESP32-S3 `ADC_11db`, the divider can approach a voltage above the documented ~3.10 V measurable range. Explain possible saturation and do not use “4095 means exactly 3.3 V”.

---

## 14. Noise, averaging, and filtering

Do not hide raw ADC instability.

First show a raw stream such as:

```text
2047
2052
2044
2051
2048
...
```

Ask students why the physical input has not visibly changed while the numeric result moves.

Discuss:

- electrical noise;
- ADC noise;
- reference variation;
- quantization;
- wiring / breadboard pickup.

### 14.1 Simple arithmetic average

Start with averaging N samples:

```text
average = sum(samples) / N
```

Use fixed-width types and ensure the accumulator is wide enough.

Example requirement:

- sample count: power-of-two or another clear compile-time constant;
- accumulator type must not overflow for maximum ADC value × sample count.

### 14.2 Moving average

Then implement a moving average as the next step.

Explain:

- smoother output;
- less visible noise;
- introduces response delay / latency;
- filtering cannot recover information that was never sampled correctly.

### 14.3 Optional hardware filter

Mention and optionally demonstrate a small capacitor such as approximately 100 nF from ADC input to GND where appropriate.

Explain that official Espressif guidance suggests a bypass/filter capacitor and multisampling as ways to reduce ADC noise.

Do not present the capacitor as a universal fix without discussing source impedance and response speed.

---

## 15. Required code examples

Create clear, independent PlatformIO examples.

All code must follow the repository `AGENTS.md` rules.

Do not use `int` automatically where the range matters.

Use, where appropriate:

```cpp
constexpr uint8_t ADC_PIN = 4;
constexpr uint8_t ADC_BITS = 12;
constexpr uint16_t ADC_MAX_CODE = (1u << ADC_BITS) - 1u;
```

If multiplication / accumulation can exceed 16 bits, use an appropriate wider type.

### Example 1 — Basic raw ADC

Goal:

- initialize Serial;
- configure 12-bit read resolution;
- read ADC1 pin;
- print raw value.

No voltage calculation yet.

### Example 2 — Raw + ideal voltage + calibrated millivolts

Print side-by-side:

```text
raw | ideal_mV | calibrated_mV
```

The “ideal” calculation must be explicitly labeled as a model.

Avoid floating point if it makes the example unnecessarily complicated; an integer-millivolt calculation is acceptable if the math remains clear.

### Example 3 — Attenuation

Use:

```cpp
analogSetPinAttenuation(ADC_PIN, ...);
```

Demonstrate different settings.

Print enough information for students to observe saturation / range changes.

### Example 4 — LDR voltage divider

Read LDR divider and print:

```text
raw
calibrated_mV
light state
```

Use threshold logic only after students have observed the raw behavior.

Do not hard-code a meaningless universal light threshold without explaining that it must be determined from actual measurements.

### Example 5 — Averaging

Read multiple samples and show:

```text
raw
average
```

The code must clearly show why the accumulator uses a wider integer type.

### Example 6 — Moving average

Implement a simple fixed-size moving-average buffer.

Requirements:

- no dynamic allocation;
- fixed-size array;
- clear index wrap-around;
- explain initial buffer-fill behavior;
- comments explain **why**, not restate code.

---

## 16. Practical laboratories

### Lab 1 — See the ADC raw code

Hardware:

- ESP32-S3 board;
- breadboard;
- LDR + 10 kΩ divider or potentiometer if available;
- jumper wires.

Tasks:

1. Build input circuit.
2. Read `analogRead()`.
3. Observe minimum / maximum practical values.
4. Change input slowly.
5. Record several raw readings.

Questions:

- Why is the code not perfectly stable?
- What does 4096 mean?
- Why is maximum code 4095?

---

### Lab 2 — Measure voltage three ways

Recommended additional tool:

- digital multimeter.

For several input voltages, create a table:

| Multimeter voltage | ADC raw | Ideal calculated voltage | `analogReadMilliVolts()` |
|---:|---:|---:|---:|
| | | | |

Students must compare:

1. multimeter;
2. raw ADC;
3. ideal mathematical conversion;
4. calibrated Arduino result.

Discussion:

- which values differ?
- why?
- is resolution equal to accuracy?
- does the error remain constant across the range?

---

### Lab 3 — Attenuation experiment

Use a controllable analog voltage source, preferably a potentiometer powered from 3.3 V.

Test several attenuation settings.

For each setting:

- slowly change input;
- observe usable range;
- observe saturation;
- compare with documented ranges.

Safety:
Do not feed external voltages outside the safe MCU input limits.

---

### Lab 4 — LDR / light sensor

Circuit:

```text
3.3 V
 |
LDR
 |
 +---- ADC1 GPIO
 |
10 kΩ
 |
GND
```

Tasks:

1. Measure raw value in darkness.
2. Measure room-light value.
3. Illuminate with a phone flashlight.
4. Measure voltage using multimeter if available.
5. Define a threshold based on measured data.
6. Control an LED using the threshold.

Students must explain why brighter light increases the ADC value in this circuit orientation.

Then ask:

> What changes if LDR and 10 kΩ resistor are swapped?

Expected conceptual answer:

- output behavior inverts;
- bright light produces a lower voltage/code instead of higher.

---

### Lab 5 — Noise and averaging

Tasks:

1. Hold the input approximately constant.
2. Print raw ADC value rapidly enough to observe variation.
3. Record min / max.
4. Add arithmetic averaging.
5. Compare stability.
6. Add moving average.
7. Compare stability and response delay.

Optional:

- add ~100 nF capacitor from ADC input to GND;
- compare behavior again.

---

### Optional advanced demo — Sampling / aliasing

Only if suitable signal-generation equipment is available.

Show:

- known periodic input;
- high enough sample rate;
- deliberately insufficient sample rate;
- apparent lower-frequency alias.

This demo is optional and must not block the lesson.

---

## 17. Quiz requirements

Keep the original quiz idea, but rewrite questions that imply technically false absolutes.

Create **10 questions**, each with 4 answer choices.

Required topics:

1. analog vs digital;
2. sampling;
3. quantization;
4. 12-bit codes / 4095;
5. resolution vs accuracy;
6. SAR principle;
7. purpose of internal DAC in SAR;
8. attenuation;
9. voltage divider / LDR direction;
10. raw vs calibrated measurement.

Do not use the old absolute question:

> If `analogRead()` returns 0, the pin must be exactly 0 V.

Instead use wording such as:

> In an ideal ADC model, what part of the input range does code 0 represent?

Likewise, for the LDR do not claim that full illumination must always produce exactly 4095.

Provide a separate answer key in teacher notes.

---

## 18. Teacher notes requirements

`teacher_notes.md` must include:

- recommended teaching order;
- key points students commonly confuse;
- questions to ask students during the lesson;
- worked calculations;
- SAR bit-by-bit walkthrough;
- warnings about misleading simplifications;
- demo ideas;
- expected lab observations;
- quiz answer key.

Specific misconceptions to address:

- “analog pin directly measures resistance” — false;
- “12-bit means 12-bit accurate” — false;
- “4095 always means 3.3 V” — false;
- “Vref is automatically VCC” — false for this ADC model;
- “sampling and quantization are the same process” — false;
- “more attenuation gives more ADC bits” — false;
- “ADC2 is just another unrestricted ADC1” — false;
- “ESP32-S3 has an external-output DAC because SAR uses a DAC internally” — false.

---

## 19. Repository structure to create

Codex should create:

```text
lessons/
└── 01_adc/
    ├── README.md
    ├── theory.md
    ├── teacher_notes.md
    ├── quiz.md
    ├── examples/
    │   ├── 01_raw_adc/
    │   │   ├── platformio.ini
    │   │   └── src/
    │   │       └── main.cpp
    │   ├── 02_voltage_calibration/
    │   │   ├── platformio.ini
    │   │   └── src/
    │   │       └── main.cpp
    │   ├── 03_attenuation/
    │   │   ├── platformio.ini
    │   │   └── src/
    │   │       └── main.cpp
    │   ├── 04_ldr/
    │   │   ├── platformio.ini
    │   │   └── src/
    │   │       └── main.cpp
    │   ├── 05_average/
    │   │   ├── platformio.ini
    │   │   └── src/
    │   │       └── main.cpp
    │   └── 06_moving_average/
    │       ├── platformio.ini
    │       └── src/
    │           └── main.cpp
    └── labs/
        ├── 01_raw_adc.md
        ├── 02_measure_voltage.md
        ├── 03_attenuation.md
        ├── 04_ldr.md
        └── 05_noise_filtering.md
```

If the repository already has an established structure by implementation time, follow that structure rather than creating unnecessary competing conventions.

---

## 20. Purpose of each lesson file

### `README.md`

Student-facing overview:

- goals;
- required hardware;
- concise lesson flow;
- links to theory, examples and labs.

### `theory.md`

Full student theory:

- clear explanations;
- equations;
- tables;
- diagrams;
- no teacher-only answers.

### `teacher_notes.md`

Instructor-facing expansion:

- deeper explanations;
- corrections;
- worked solutions;
- teaching prompts;
- quiz answers;
- demo notes.

### `quiz.md`

Student quiz only.

Do not mark correct answers inline in the student quiz.

### `examples/*`

Buildable PlatformIO projects.

### `labs/*.md`

Student instructions.

Do not include finished solution code directly in the lab instructions if the solution already exists under `examples/`.

---

## 21. PlatformIO requirements

Use the board environment that matches the current course board configuration.

Do not guess silently if the repository already defines a board.

For an ESP32-S3 DevKit-compatible target, a typical configuration may resemble:

```ini
[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200
```

But Codex must inspect the repository before choosing the final board ID.

Do not introduce unnecessary libraries for ADC examples.

---

## 22. C/C++ style requirements

Follow `AGENTS.md`.

Additionally:

- use `constexpr` for compile-time configuration;
- use `<cstdint>` / Arduino fixed-width integer types where size matters;
- do not use `int` by habit for ADC values;
- avoid macros when `constexpr` is sufficient;
- avoid dynamic memory allocation;
- avoid magic numbers;
- separate configuration constants from logic;
- comments should explain reasoning and hardware behavior;
- examples should be small and focused;
- do not hide ADC behavior behind a large class or abstraction.

Preferred style:

```cpp
constexpr uint8_t ADC_PIN = 4;
constexpr uint8_t ADC_BITS = 12;
constexpr uint16_t ADC_MAX_CODE =
    static_cast<uint16_t>((1u << ADC_BITS) - 1u);
```

For sample accumulation, select a type based on worst-case range rather than convenience.

---

## 23. Diagrams required

Use Mermaid or clear ASCII diagrams where appropriate.

Required visuals:

1. analog vs sampled waveform;
2. sampling vs quantization;
3. ADC staircase transfer curve;
4. ADC vs DAC direction;
5. simplified R-2R or resistor-string DAC concept;
6. SAR block diagram;
7. SAR binary-search progression;
8. ESP32-S3 input attenuation path;
9. voltage divider;
10. LDR divider.

Do not reuse the old diagram that shows:

```text
Analog -> ADC -> DAC -> Filter -> Analog
```

as if it were only an illustration of ADC quantization.

If that concept is used, explain that it represents an entire ADC→DAC reconstruction chain, not just ADC operation.

---

## 24. Hardware list

Required:

- ESP32-S3 development board;
- breadboard;
- LDR;
- 10 kΩ resistor;
- jumper wires;
- USB cable;
- VS Code + PlatformIO.

Strongly recommended:

- digital multimeter;
- potentiometer around 10 kΩ.

Optional:

- oscilloscope;
- function generator;
- 100 nF ceramic capacitor;
- logic analyzer where useful for timing experiments.

---

## 25. Out of scope for Codex

Do not create:

- PPTX presentation;
- polished slide graphics;
- a full DSP course;
- a full DAC design lesson;
- ESP-IDF versions of every Arduino example;
- advanced INL/DNL derivations;
- production sensor calibration algorithms.

A presentation will be created separately from the same lesson structure.

---

## 26. Acceptance criteria

The implementation is complete when:

- all requested lesson files exist;
- all PlatformIO examples compile for the configured ESP32-S3 target;
- examples use ADC1;
- `analogRead()` and `analogReadMilliVolts()` are both demonstrated;
- attenuation is demonstrated correctly;
- 12-bit resolution is never described as accuracy;
- `3.3 V / 4095` is never presented as the universal real ESP32-S3 voltage conversion;
- sampling and quantization are taught separately;
- at least four ADC architectures are explained;
- DAC basics appear before the SAR deep dive;
- SAR conversion is demonstrated bit-by-bit;
- the ESP32-S3 external-DAC limitation is stated;
- dB formulas and at least one voltage-ratio example are present;
- voltage-divider calculations are present;
- LDR behavior is calculated and demonstrated;
- noise is shown before filtering;
- averaging and moving average examples exist;
- the student quiz contains 10 questions;
- teacher notes contain the quiz key and worked examples;
- no code uses dynamic allocation unnecessarily;
- no commit or push is performed unless explicitly requested.

---

## 27. Verified ESP32-S3 reference notes

These board-specific points were checked against official Espressif documentation before writing this specification.

### Arduino-ESP32 ADC API

- `analogRead()` returns a raw, non-calibrated value.
- `analogReadMilliVolts()` returns a calibrated value in millivolts.
- default `analogRead()` resolution is 12 bits.
- ESP32-S3 attenuation ranges are approximately:
  - 0 dB: 0–0.95 V
  - 2.5 dB: 0–1.25 V
  - 6 dB: 0–1.75 V
  - 11 dB Arduino setting: 0–3.10 V

Reference:  
https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html

### ESP32-S3 ADC architecture

- two SAR ADC units;
- 20 analog channels;
- ADC1: GPIO1–GPIO10;
- ADC2: GPIO11–GPIO20;
- ADC2 shares resources with Wi-Fi;
- nominal internal Vref is around 1.1 V and varies between chips.

References:  
https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/adc.html  
https://documentation.espressif.com/esp32_s3_datasheet_en.pdf

### ESP32-S3 ADC noise

Espressif recommends considering a bypass capacitor around 100 nF on an ADC input and/or multisampling to reduce noise, depending on the application.

Reference:  
https://docs.espressif.com/projects/esp-hardware-design-guidelines/en/latest/esp32s3/schematic-checklist.html

### DAC availability

Arduino-ESP32 documents user-accessible DAC channels for:

- ESP32
- ESP32-S2

ESP32-S3 does not expose that general-purpose DAC peripheral.

Reference:  
https://docs.espressif.com/projects/arduino-esp32/en/latest/api/dac.html

---

## 28. Instruction to Codex

Implement this lesson according to this specification and the repository `AGENTS.md`.

Before editing:

1. inspect the current repository structure;
2. inspect existing PlatformIO configuration if present;
3. reuse existing naming/style conventions;
4. report any conflict between this specification and the actual repository.

During implementation:

- create the lesson incrementally;
- keep theory and code technically consistent;
- build every PlatformIO example;
- fix compilation errors before considering the task complete;
- show the resulting file tree and a concise summary of changes.

Do **not** commit or push.
