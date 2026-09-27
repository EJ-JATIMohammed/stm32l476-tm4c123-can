# CAN Communication between STM32L476RG and TM4C123GH6PM

*A CMSIS-based embedded systems project demonstrating CAN communication between two ARM Cortex-M4 microcontrollers using low-level, platform-specific drivers.*

## Project Overview

This project explores a two-node Controller Area Network (CAN) system built with an STM32L476RG transmitter and a TM4C123GH6PM receiver. The STM32 node monitors a user button through an external interrupt; each button press advances a five-state LED pattern and transmits the next state over CAN.

The TM4C123 node is intended to receive the state and update its LEDs accordingly. The central data flow is:

**User Event → Interrupt → CAN Transmission → CAN Reception → LED State Update**

This repository focuses on the separation between application logic and reusable CAN driver code for two different ARM Cortex-M4 platforms.

## System Architecture

```text
                CAN BUS
┌──────────────────────┐
│     STM32L476RG      │
│    Transmitter Node  │
│                      │
│  Button → EXTI       │
│       ↓              │
│  CAN Transmission    │
└──────────┬───────────┘
           │
           │ CAN
           │
┌──────────▼───────────┐
│    TM4C123GH6PM      │
│     Receiver Node    │
│                      │
│  CAN Reception       │
│       ↓              │
│  LED State Update    │
└──────────────────────┘
```

Operation:

1. A user presses the button connected to the STM32L476RG node.
2. The button event is handled through an external interrupt.
3. The STM32 node transmits the next LED-state value over the CAN bus.
4. The TM4C123GH6PM node receives the message and applies the corresponding LED state.

## LED Cycle

```text
Button Press
     ↓
LED1
     ↓
LED2
     ↓
LED3
     ↓
LED4
     ↓
LED5
     ↓
LED1 ...
```

Each button press advances the receiver LED state by one position, then wraps back to LED1 after LED5.

## Project Architecture

```text
stm32l476-tm4c123-can/
├── stm32L4_node/
│   ├── application/
│   │   └── main_node1.c
│   └── can_driver/
│       ├── Inc/
│       │   └── can_driver.h
│       └── Src/
│           └── can_driver.c
│
├── tm4c123g6pm_node/
│   ├── application/
│   │   └── main.c
│   └── can_driver/
│       ├── Inc/
│       │   └── can_driver.h
│       └── Src/
│           └── can_driver.c
│
├── README.md
└── LICENSE
```

- `stm32L4_node/` — STM32L476RG application and CAN driver.
- `tm4c123g6pm_node/` — TM4C123GH6PM application and CAN driver.
- `application/` — Application-level logic for each node.
- `can_driver/Inc/` — CAN driver headers.
- `can_driver/Src/` — CAN driver implementations.
- `README.md` — Project documentation.
- `LICENSE` — Project license.

## Main Technologies

| Technology   | Purpose                          |
| ------------ | -------------------------------- |
| STM32L476RG  | CAN transmitter node             |
| TM4C123GH6PM | CAN receiver node                |
| CAN          | Communication protocol           |
| CMSIS        | Low-level MCU software interface |
| C            | Embedded software implementation |

## Hardware

- STM32L476RG development board
- TM4C123GH6PM development board
- CAN transceivers
- Push button
- LEDs
- CAN bus wiring

## Communication

The STM32 application prepares a one-byte payload representing the next LED state and sends it when the button interrupt occurs. The state values defined in the STM32 application are `0x01`, `0x02`, `0x04`, `0x08`, and `0x10`.

The source currently contains separate test configurations for the two node applications, including CAN identifiers and loopback/test-mode settings. These settings should be aligned for the intended physical STM32-to-TM4C communication before deployment; the receiver must accept the identifier and frame format transmitted by the STM32 node. The payload convention is the LED-state byte described above.

## Build and Run

The repository contains the C applications and platform-specific CAN drivers, but does not include project files or a repository-defined command-line build system. Build each node with the toolchain and IDE configured for its target board:

1. Open or create an embedded C project for the STM32L476RG node and add the files under `stm32L4_node/`.
2. Build the STM32 application and program it onto the STM32L476RG board using the board’s supported debug/programming workflow.
3. Open or create a corresponding embedded C project for the TM4C123GH6PM node and add the files under `tm4c123g6pm_node/`.
4. Build the TM4C123 application and program it onto the TM4C123GH6PM board using the board’s supported debug/programming workflow.
5. Connect the CAN transceivers and CAN bus wiring, then verify that both nodes use compatible CAN settings and matching message configuration.

## Results

With both applications configured for the intended physical CAN link, each button press on the STM32L476RG advances the LED state observed on the TM4C123GH6PM from LED1 through LED5 and back to LED1.

## License

This project is licensed under the [MIT License](LICENSE).

## Author

EJ-JATIMohammed
