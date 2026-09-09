# CAN Communication Between STM32L476RG and TM4C123GH6PM

A demonstration of inter-microcontroller communication using the Controller Area Network (CAN) protocol. This project implements a button-triggered LED control system where button presses on the **STM32L476RG** generate CAN messages received by the **TM4C123GH6PM**, which responds by cycling through LED states.

---

## Overview

This project showcases CAN protocol implementation across two different microcontroller platforms using CMSIS (Cortex Microcontroller Software Interface Standard) abstractions. The system demonstrates practical CAN communication patterns including message transmission, reception, and filter configuration.

**Key Features:**
- Button interrupt-driven CAN message transmission
- Cyclic LED pattern control on receiver node
- CMSIS-based hardware abstraction
- Multi-platform CAN driver implementation
- Clean separation between application and driver layers

---

## Hardware Specifications

### STM32L476RG Node (Transmitter)
- **Microcontroller:** STM32L476RG (ARM Cortex-M4F)
- **CAN Interface:** CAN1
- **GPIO Pins:** PB8 (RX), PB9 (TX)
- **Input:** Button on PC13 (falling edge detection)
- **Configuration:**
  - Message ID: 0x123 (extended)
  - DLC: 1 byte per message
  - Bit timing: SJW=1tq, BS1=13tq, BS2=2tq, Prescaler=1

### TM4C123GH6PM Node (Receiver)
- **Microcontroller:** TM4C123GH6PM (ARM Cortex-M4)
- **CAN Interface:** CAN0
- **GPIO Pins:** PB4 (RX), PB5 (TX)
- **Output:** 4 LEDs controlled sequentially
- **Configuration:**
  - Expected Message ID: 0x11 (standard)
  - Bit timing: BRP=10, SJW=1, TS1=15, TS2=4 (250 kbps @ 50 MHz)
  - Mask: 0x7FF

---

## Application Behavior

### STM32L476RG (Transmitter Node)

**Operation:**
1. Button press on PC13 triggers an external interrupt (EXTI15_10)
2. Interrupt handler transmits a CAN message with extended ID 0x123
3. LED pattern value is incremented after each transmission
4. Cycles through 5 different LED states: 0x1, 0x2, 0x4, 0x8, 0x10

**Message Format:**
- CAN ID: 0x123 (extended)
- RTR: Data frame
- DLC: 1 byte
- Data: LED pattern value (0x1, 0x2, 0x4, 0x8, 0x10)

### TM4C123GH6PM (Receiver Node)

**Operation:**
1. Receives CAN messages with standard ID 0x11
2. Extracts LED pattern value from received message
3. Updates LED states based on received pattern
4. **LED Behavior:** One LED is active (on) while three are inactive (off)
5. Active LED changes cyclically with each received message

**Message Reception:**
- Listens for standard ID 0x11
- Message object filter mask: 0x7FF
- FIFO-based reception with automatic message object handling

---

## CAN Communication Details

### Initialization Parameters (STM32L476RG)

```c
Bit Timing:
  - SJW (Synchronization Jump Width): 1 time quantum
  - BS1 (Bit Segment 1): 13 time quanta
  - BS2 (Bit Segment 2): 2 time quanta
  - Prescaler: 1

CAN Mode:
  - RFLM: Receive FIFO Locked Mode enabled
  - Loopback: Disabled
  - Silent Mode: Disabled
```

### Initialization Parameters (TM4C123GH6PM)

```c
Bit Timing (250 kbps):
  - BRP (Bit Rate Prescaler): 10
  - SJW (Sync Jump Width): 1
  - TS1 (Time Segment 1): 15
  - TS2 (Time Segment 2): 4

CAN Mode:
  - Test Mode: Enabled (with loopback during testing)
```

### Filter Configuration

**STM32L476RG Filter:**
- Bank: 0
- Mode: Mask mode
- Scale: 32-bit
- Filter Register 1: 0x00000120 (ID)
- Filter Register 2: 0x1FFFFFF0 (Mask)
- FIFO Assignment: FIFO1

**TM4C123GH6PM Filter:**
- Message Object: 1
- Expected ID: 0x11 (standard)
- Mask: 0x7FF
- Mode: Mask filtering (when enabled)

---

## Project Structure

```
stm32l476-tm4c123-can/
├── stm32L4_node/
│   ├── application/
│   │   └── main_node1.c          # STM32L476RG application (button handler, LED cycling)
│   └── can_driver/
│       ├── Inc/
│       │   └── can_driver.h       # CAN driver API definitions
│       └── Src/
│           └── can_driver.c       # CAN driver implementation (GPIO, init, TX/RX, filters)
├── tm4c123g6pm_node/
│   ├── application/
│   │   └── main.c                 # TM4C123GH6PM application (LED control)
│   └── can_driver/
│       ├── Inc/
│       │   └── can_driver.h       # CAN driver API definitions
│       └── Src/
│           └── can_driver.c       # CAN driver implementation (GPIO, init, TX/RX, filters)
└── README.md                       # This file
```

---

## CAN Driver Architecture

### Common Interface (Both Nodes)

**Core Functions:**

- `can_gpio_init()` – Configures GPIO pins for CAN communication
- `can_init()` – Initializes CAN controller with timing and mode parameters
- `can_transmit()` – Transmits a CAN message
- `can_receive()` – Receives a CAN message from FIFO
- `can_filter_init()` (STM32L476RG) or `can_filter_config()` (TM4C123GH6PM) – Configures message filtering

### STM32L476RG CAN Driver Features

- Multi-port GPIO support (PA11/PA12, PB8/PB9, PD0/PD1)
- Flexible CAN functional options (TTCM, ABOM, AWUM, NART, RFLM, TXFP)
- 32-bit or 16-bit filter scale selection
- Mask mode and identifier list mode filtering
- Interrupt-driven reception (CAN1_RX0_IRQHandler, CAN1_RX1_IRQHandler)
- Mail box-based transmission

### TM4C123GH6PM CAN Driver Features

- Multiple CAN module support (CAN0, CAN1)
- Flexible GPIO pin mapping per CAN module
- Bit timing configuration for various baud rates
- Test mode support (loopback, silent, basic modes)
- Message object-based transmission and reception
- Automatic message object allocation for TX

---

## Development Environment

### Required Tools
- **STM32L476RG:** STM32 HAL/CMSIS libraries, STM32CubeMX or equivalent
- **TM4C123GH6PM:** TivaWare Peripheral Driver Library, ARM CMSIS Core
- **Compiler:** ARM GCC (arm-none-eabi-gcc)
- **Build System:** Make or CMake
- **Debugger:** ST-Link v2 (STM32) and ICDI (TM4C123)

### Required Libraries
- CMSIS Core (Cortex-M generic definitions)
- Device-specific CMSIS headers (stm32l4xx.h, tm4c123gh6pm.h)
- CAN driver code (included in this repository)

---

## Build and Usage Instructions

### Building the STM32L476RG Application

```bash
# Navigate to STM32L4 node directory
cd stm32L4_node

# Build the project (adjust for your build system)
make build          # or: arm-none-eabi-gcc -o app.elf *.c ...

# Flash to device (using ST-Link)
make flash          # or: st-flash write app.bin 0x08000000
```

### Building the TM4C123GH6PM Application

```bash
# Navigate to TM4C node directory
cd tm4c123g6pm_node

# Build the project (adjust for your build system)
make build          # or: arm-none-eabi-gcc -o app.elf *.c ...

# Flash to device (using ICDI/J-Link)
make flash          # or: lm4flash app.bin
```

### Hardware Setup

1. **CAN Bus Wiring:**
   - Connect STM32L476RG CAN_TX (PB9) to TM4C123GH6PM CAN_RX (PB4)
   - Connect STM32L476RG CAN_RX (PB8) to TM4C123GH6PM CAN_TX (PB5)
   - Connect GND between both boards (common ground)

2. **Power Supply:**
   - Ensure both microcontrollers are properly powered
   - Optional: Add 120Ω termination resistors on CAN bus if using long cables

3. **Running the Application:**
   - Program both microcontrollers with their respective firmware
   - Press the button on STM32L476RG node
   - Observe LED state changes on TM4C123GH6PM node

---

## Implementation Notes

### STM32L476RG

- Uses **EXTI (External Interrupt)** on PC13 for button detection
- Transmits CAN messages via **CAN1_RX0_IRQHandler** and **CAN1_RX1_IRQHandler** on reception
- Implements **mail box-based transmission** with automatic mail box selection
- **Filter configuration** uses mask mode to accept specific CAN IDs
- Supports **multiple GPIO options** (PA, PB, PD) for CAN interface flexibility

### TM4C123GH6PM

- Receives CAN messages via **message objects**
- Uses **test mode** with loopback for initial development and testing
- Implements **free message object search** (`can_get_free_object()`) for TX buffering
- Supports **basic CAN mode** for simplified operation
- Message filtering uses **mask-based identification** with configurable thresholds
- **Note:** The current implementation may require minor updates to fully integrate LED control output handling

---

## Features

- ✅ **CMSIS-Based Design:** Platform-independent hardware abstraction using CMSIS standards
- ✅ **Dual-Platform Implementation:** Independent CAN drivers for STM32L4 and TM4C123
- ✅ **Button-Triggered Communication:** Interrupt-driven message transmission
- ✅ **Cyclic LED Control:** Sequential LED pattern cycling on receiver
- ✅ **Robust Message Filtering:** Hardware-level CAN ID filtering
- ✅ **Modular Architecture:** Clear separation between application and driver layers
- ✅ **Configurable Timing:** Flexible CAN bit timing parameters
- ✅ **Test Mode Support:** Loopback and silent mode capabilities for development

---

## Future Enhancements

- Add error handling and status reporting
- Implement bidirectional CAN communication
- Extend to support additional CAN messages
- Add USB debugging interface
- Implement CAN error detection and recovery
- Support for CAN FD (Flexible Data-rate)

---

## License

This project is provided as-is for educational and portfolio purposes.

---

## Contact

For questions or issues, refer to the repository documentation or contact the project maintainer.
