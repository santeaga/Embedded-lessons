# Embedded Lessons - Codex Instructions

This repository contains educational materials for an Embedded Systems course.

## Teaching principles

- Code and explanations must prioritize clarity and teaching value.
- Do not oversimplify important embedded concepts.
- Explain what happens at the MCU/peripheral level when it is relevant to the lesson.
- Prefer small, focused examples that demonstrate one concept at a time.
- Do not hide important hardware behavior behind unnecessary abstractions.
- Comments should explain WHY something is done, not repeat WHAT the code says.
- Preserve useful existing course material unless explicitly asked to remove it.
- When correcting an inaccurate explanation, preserve the useful teaching idea where possible and replace only the incorrect part.

## Language rules

- Student-facing educational materials must be written in Ukrainian unless explicitly requested otherwise.
- Teacher notes must also be written in Ukrainian unless explicitly requested otherwise.
- Keep source code, identifiers, API names, enum names, filenames, commands, and official technical names in their original English form.
- When introducing an important technical term, prefer the Ukrainian term followed by the English term in parentheses when useful, for example:
  - роздільна здатність (resolution)
  - дискретизація (sampling)
  - квантування (quantization)
  - ослаблення (attenuation)
  - похибка (error)
- Do not translate API names or hardware identifiers such as `analogRead()`, `analogReadMilliVolts()`, `ADC_11db`, `GPIO`, `SAR`, `UART`, `I2C`, or `SPI`.
- Technical English terms may be reused after they have been introduced clearly in Ukrainian.

## Lesson complexity and structure

- Teach the core concept first, then add deeper implementation details.
- Prefer progressive disclosure:
  1. basic concept;
  2. practical example;
  3. deeper hardware explanation;
  4. optional / advanced details.
- Do not overload beginner-facing material with framework internals unless they are required to understand the concept.
- Put framework-specific nuances, edge cases, board-specific caveats, and advanced implementation details into `teacher_notes.md` or clearly marked optional sections.
- Do not delete technically useful advanced material merely because it is too detailed for the main student flow; move it to the appropriate level.
- If a lesson contains more material than is realistic for one class, distinguish core material from extended / optional material rather than removing it.

## C/C++ rules

- Use fixed-width integer types (`uint8_t`, `uint16_t`, `uint32_t`, etc.) when the required size matters.
- Do not use `int` automatically for values whose range or hardware representation is important.
- Prefer `constexpr` for compile-time constants.
- Avoid magic numbers.
- Use bitwise operations explicitly when they are part of the concept being taught.
- Avoid unnecessary dynamic memory allocation.
- Keep code readable for students who are learning C/C++ and embedded development.
- Select accumulator and intermediate-expression types from the worst-case numeric range, not by habit.
- Do not introduce abstractions, classes, templates, or libraries unless they materially improve the lesson.

## Embedded-specific rules

- Clearly distinguish hardware behavior from software/framework behavior.
- Do not assume that Arduino abstractions explain how the hardware works.
- When appropriate, explain the relationship between the high-level API and the MCU peripheral.
- Hardware-specific assumptions must be documented.
- Do not silently assume a particular ESP32 variant when the target MCU has not been specified.
- Clearly distinguish idealized equations used for teaching from real MCU behavior.
- Do not present nominal supply voltage, reference voltage, measurable ADC range, or absolute-maximum input voltage as if they were the same thing.
- When board-specific details are important, identify the exact MCU / development-board family to which they apply.

## Student-facing vs teacher-facing material

- `README.md`, `theory.md`, `quiz.md`, and `labs/*.md` are primarily student-facing.
- `teacher_notes.md` is instructor-facing and may contain deeper implementation details, framework caveats, worked solutions, expected results, and teaching prompts.
- Keep student-facing README files concise and navigational.
- Do not put board bring-up archaeology, framework-version history, internal HAL behavior, or long compatibility notes into the student README unless they are required to run the lesson.
- Do not include quiz answers inline in the student quiz when a teacher answer key exists.

## Repository rules

- Follow the existing repository structure and naming conventions.
- Do not modify existing lessons unless explicitly requested.
- Do not delete existing examples or source material unless explicitly requested.
- Do not modify approved files under `specs/` unless explicitly requested.
- Do not commit or push changes unless explicitly requested.
- Do not commit PlatformIO `.pio` directories, compiled binaries, build logs, caches, or other generated build artifacts.
- Keep generated build artifacts excluded through `.gitignore`.
- Do not create duplicate documentation files when an existing file can be updated cleanly.

## Verification rules

- When code is changed, build every affected PlatformIO example.
- Fix compilation errors before considering the task complete.
- Do not claim hardware behavior was physically verified unless it was actually tested on hardware.
- Distinguish successful compilation from successful hardware validation.
- When a technical statement depends on a specific framework or MCU version, prefer official vendor/framework documentation.
- At the end of a substantial change, show:
  - the changed file tree;
  - build results where applicable;
  - a concise summary of what changed;
  - any unresolved assumptions or hardware-dependent points.
