# Embedded Lessons - Codex Instructions

This repository contains educational materials for an Embedded Systems course.

## Teaching principles

- Code must prioritize clarity and teaching value.
- Do not oversimplify important embedded concepts.
- Explain what happens at the MCU/peripheral level when it is relevant to the lesson.
- Prefer small, focused examples that demonstrate one concept at a time.
- Do not hide important behavior behind unnecessary abstractions.
- Comments should explain WHY something is done, not repeat WHAT the code says.

## C/C++ rules

- Use fixed-width integer types (`uint8_t`, `uint16_t`, `uint32_t`, etc.) when the required size matters.
- Do not use `int` automatically for values whose range or hardware representation is important.
- Prefer `constexpr` for compile-time constants.
- Avoid magic numbers.
- Use bitwise operations explicitly when they are part of the concept being taught.
- Avoid unnecessary dynamic memory allocation.
- Keep code readable for students who are learning C/C++ and embedded development.

## Embedded-specific rules

- Clearly distinguish hardware behavior from software/framework behavior.
- Do not assume that Arduino abstractions explain how the hardware works.
- When appropriate, explain the relationship between the high-level API and the MCU peripheral.
- Hardware-specific assumptions must be documented.
- Do not silently assume a particular ESP32 variant when the target MCU has not been specified.

## Repository rules

- Do not modify existing lessons unless explicitly requested.
- Do not delete existing examples unless explicitly requested.
- Do not commit or push changes unless explicitly requested.
- Follow the existing repository structure and naming conventions.
