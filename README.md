# CAN Communication between STM32L476RG and TM4C123GH6PM

A comprehensive embedded systems project demonstrating Controller Area Network (CAN) communication between two microcontroller platforms using CMSIS standards. The project implements an **interactive event-driven system** where a user input triggers a CAN communication event that cycles through LED states on the receiver node.

## Project Overview

This project implements a **dual-node CAN communication system** with:
- **STM32L476RG** (Cortex-M4, 80 MHz) – Sender node with user input detection
- **TM4C123GH6PM** (Cortex-M4, 80 MHz) – Receiver node with LED output

Both nodes communicate over a CAN bus with **platform-independent driver implementations** using CMSIS standards.

---

## Project Behavior

### Communication Flow

```
User Click → STM32L476 SW Interrupt → CAN Message Transmission → TM4C123 → LED Cycle Update
```

### Detailed Operation

1. **User Input Detection (STM32L476)**
   - User presses/clicks an external button connected to **GPIO PC13** (EXTI15_10 interrupt)
   - A **software-triggered interrupt (SW interrupt)** is generated via the External Interrupt (EXTI) controller

2. **CAN Message Transmission (STM32L476)**
   - Upon each interrupt, the STM32L476 transmits a CAN message with:
     - **CAN ID**: `0x123` (extended ID)
     - **Data Payload**: LED state byte (1 byte only, DLC=1)
     - **LED States**: Cycles through `{0x1, 0x2, 0x4, 0x8, 0x10}` (5 different states)

3. **CAN Reception & LED Update (TM4C123)**
   - The TM4C123 receives the CAN message via its receiver FIFO
   - Decodes the received LED state byte
   - **Updates connected LEDs** to match the received state
   - Each click advances the LED cycle to the next state

4. **LED Cycle Behavior**
   - **State 0**: `0x01` (binary: 00001) – LED1 on
   - **State 1**: `0x02` (binary: 00010) – LED2 on
   - **State 2**: `0x04` (binary: 00100) – LED3 on
   - **State 3**: `0x08` (binary: 01000) – LED4 on
   - **State 4**: `0x10` (binary: 10000) – LED5 on
   - Cycles back to **State 0** after State 4 (continuous loop)

### Key Characteristics

- **Event-Driven**: Communication occurs only when user clicks (interrupt-triggered)
- **Single-Byte Payload**: Each message contains only 1 byte of LED state data
- **Cyclic Pattern**: LEDs light up one at a time in a 5-state sequence
- **Synchronous Operation**: LED update is immediate upon message reception
- **Interrupt-Based Reception**: TM4C123 uses FIFO-based message reception

---

## Main Features

- **Interactive Event-Driven Communication**: Button-triggered CAN messaging with LED feedback
- **Software Interrupt Handling**: External interrupt-based user input detection (EXTI)
- **CMSIS-Based CAN Drivers**: Platform-specific implementations with consistent API
- **Dual CAN Node Architecture**: Sender and receiver with different roles
- **Configurable CAN Parameters**: Bit timing, filtering, and operational modes
- **Multiple GPIO Pin Options**: Flexible CAN pin mapping for different hardware designs
- **Interrupt-Driven Message Reception**: Efficient FIFO-based reception using interrupts
- **Test & Debug Modes**: Loopback and silent modes for development and testing

---

## Hardware Requirements

### STM32L476RG Node (Sender)
- **Microcontroller**: STM32L476RG (ARM Cortex-M4, 80 MHz)
- **Button Input**: External button on **GPIO PC13** (with pull-up configuration)
- **CAN Interface**: CAN1 peripheral with GPIO pins:
  - Option 1: PA11 (RX), PA12 (TX)
  - Option 2: **PB8 (RX), PB9 (TX)** ← Currently used in main_node1.c
  - Option 3: PD0 (RX), PD1 (TX)
- **CAN Transceiver**: MCP2551 or equivalent
- **Debugger**: ST-LINK v2 or compatible
- **Optional**: External 8 MHz crystal oscillator for clock accuracy

### TM4C123GH6PM Node (Receiver)
- **Microcontroller**: TM4C123GH6PM (ARM Cortex-M4, 80 MHz)
- **LED Outputs**: 5 LEDs connected to GPIO pins (driven by received data bits)
- **CAN Interface**: CAN0 peripheral with GPIO pins:
  - Option 1: **PB4 (RX), PB5 (TX)** ← Currently used in main.c
  - Option 2: PE4 (RX), PE5 (TX)
  - Option 3: PF0 (RX), PF3 (TX)
  - Option 4: PA0 (RX), PA1 (TX)
- **CAN Transceiver**: MCP2551 or equivalent
- **Debugger**: TI ICDI or compatible

### CAN Bus
- **Media**: Twisted-pair CAN bus wiring (CAN_H and CAN_L)
- **Termination**: 120Ω resistors at both ends of the bus (critical for signal integrity)
- **Isolation**: Optional CAN bus isolation modules for noise immunity

---

## Software Requirements

- **Compiler**: ARM GCC, Keil MDK, or IAR Embedded Workbench
- **STM32L4 Libraries**:
  - CMSIS-Core v5.x or later
  - STM32L4xx HAL/CMSIS Device Support
  - STM32CubeL4 (or equivalent peripheral libraries)
- **TM4C Drivers**:
  - TivaWare Peripheral Driver Library
  - CMSIS-Core for TM4C
- **Build Tools**: Make, CMake, or IDE-native build system
- **CAN Knowledge**: Basic understanding of CAN protocol, bit timing, and message filtering

---

## Project Structure

```
stm32l476-tm4c123-can/
│
├── stm32L4_node/
│   ├── application/
│   │   └── main_node1.c              # STM32L476RG main application
│   │                                  # - Initializes button on PC13 (EXTI)
│   │                                  # - Configures CAN1 on PB8/PB9
│   │                                  # - Sends 5-byte LED pattern on button press
│   │
│   └── can_driver/
│       ├── Inc/
│       │   └── can_driver.h           # CAN driver header (STM32L4 platform)
│       │                              # - can_gpio_init(): Configure CAN pins
│       │                              # - can_init(): Initialize CAN controller
│       │                              # - can_transmit(): Send CAN messages
│       │                              # - can_filter_init(): Configure message filters
│       │                              # - can_receive(): Receive CAN messages
│       │
│       └── Src/
│           └── can_driver.c           # CAN driver implementation (STM32L4)
│                                      # - Supports PA11/PA12, PB8/PB9, PD0/PD1
│                                      # - Configurable bit timing & filtering
│                                      # - Mailbox-based transmission
│                                      # - FIFO-based reception
│
├── tm4c123g6pm_node/
│   ├── application/
│   │   └── main.c                    # TM4C123GH6PM main application
│   │                                  # - Initializes CAN0 on PB4/PB5
│   │                                  # - Receives LED state messages
│   │                                  # - Updates LEDs based on data
│   │
│   └── can_driver/
│       ├── Inc/
│       │   └── can_driver.h           # CAN driver header (TM4C platform)
│       │                              # - can_gpio_init(): Configure CAN pins
│       │                              # - can_init(): Initialize CAN controller
│       │                              # - can_filter_config(): Message object setup
│       │                              # - can_transmit(): Send CAN messages
│       │                              # - can_receive(): Receive CAN messages
│       │
│       └── Src/
│           └── can_driver.c           # CAN driver implementation (TM4C)
│                                      # - Supports PB4/PB5, PE4/PE5, PF0/PF3, PA0/PA1
│                                      # - Message object-based filtering
│                                      # - Configurable timing & modes
│
├── README.md                          # This file (project documentation)
├── LICENSE                            # MIT License
└── .gitignore                         # Git ignore file (if present)
```

---

## How to Build and Run

### STM32L476RG Node Setup

#### 1. Environment Configuration
- Import project into **STM32CubeIDE**, Keil µVision, or IAR Embedded Workbench
- Select STM32L476RG device
- Configure system clock to **80 MHz**

#### 2. CAN Configuration (main_node1.c)
```c
// GPIO Selection: PB8 (RX), PB9 (TX) – already configured
can_gpio_init(CAN1, CAN_GPIO_PB8_PB9);

// CAN Initialization: 250 kbps bit rate
can_init(CAN1, 
         TTCM_DISABLE,
         ABOM_DISABLE,
         AWUM_DISABLE,
         NART_DISABLE,
         RFLM_ENABLE,      // Receive FIFO Locked Mode
         TXFP_DISABLE,
         CAN_BTR_SJW_1tq,   // SJW = 1
         CAN_BTR_BS1_13tq,  // BS1 = 13
         CAN_BTR_BS2_2tq,   // BS2 = 2
         CAN_BTR_PRESCALER  // Prescaler = 1
         );

// Filter Configuration: Accept messages with ID 0x12x
can_filter_init(CAN1, 0, true, false, 0x00000120, 0x1FFFFFF0, FIFO1, true);
```

#### 3. Build & Flash
```bash
# Using STM32CubeIDE
- Project → Build Project
- Run → Debug or Release

# Using command line (if Makefile present)
make -C stm32L4_node clean build
# Flash using ST-LINK
STM32_Programmer_CLI -c port=SWD -d stm32L4_node/build/firmware.elf
```

#### 4. Verification
- Connect debugger and apply power
- Press button on PC13 to trigger SW interrupt
- Observe CAN message transmission in debugger
- (Verify with logic analyzer on PB8/PB9 if available)

---

### TM4C123GH6PM Node Setup

#### 1. Environment Configuration
- Import project into **TM4C Code Composer Studio** or Keil
- Select TM4C123GH6PM device
- Configure system clock to **80 MHz**

#### 2. CAN Configuration (main.c)
```c
// GPIO Selection: PB4 (RX), PB5 (TX) – already configured
can_gpio_init(CAN0, CAN0_GPIO_PB4_PB5);

// CAN Initialization: 250 kbps bit rate
can_init(CAN0,
         true,   // Test mode
         10,     // BRP (Bit Rate Prescaler) = 10
         1,      // SJW = 1
         15,     // TS1 = 15
         4,      // TS2 = 4
         true,   // Loopback mode (for testing)
         false,  // Silent mode disabled
         false   // Basic CAN disabled
         );

// Filter Configuration: Receive messages with expected ID
can_filter_config(CAN0, 1, 0x123, 0x7FF, false, false, false, true);
```

#### 3. Build & Flash
```bash
# Using TM4C Code Composer Studio
- Project → Build Project
- Run → Debug or Download

# Using command line (if Makefile present)
make -C tm4c123g6pm_node clean build
# Flash using LM Flash Programmer or OpenOCD
lmflash build/firmware.bin
```

#### 4. Verification
- Connect debugger and apply power
- Connect LED outputs to GPIO pins
- Press button on STM32L476 node
- Observe LEDs cycling through states on TM4C123

---

## Important Configuration Notes

### CAN Baud Rate (Critical for Communication)

Both nodes **must use the same bit timing** for successful communication. Based on system clock = **80 MHz**:

**STM32L476RG (APB1 clock = 80 MHz)**
```c
#define CAN_BTR_SJW_1tq      1       // Synchronization Jump Width
#define CAN_BTR_BS1_13tq     13      // Bit Segment 1
#define CAN_BTR_BS2_2tq      2       // Bit Segment 2
#define CAN_BTR_PRESCALER    1       // Prescaler
// Result: 250 kbps = 80 MHz / ((13+2+1)*1) / 4 = 250 kbps
```

**TM4C123GH6PM (System clock = 80 MHz)**
```c
#define CAN_BRP      10      // Baudrate Prescaler
#define CAN_SJW      1       // Synchronization Jump Width
#define CAN_TS1      15      // Time Segment 1
#define CAN_TS2      4       // Time Segment 2
// Result: 250 kbps = 80 MHz / ((15+4+1)*10) = 250 kbps
```

### Message Format

**Transmitted by STM32L476 (via button press)**
- **CAN ID**: `0x123` (Extended 29-bit ID)
- **DLC**: 1 byte
- **Data**: LED state byte (0x01, 0x02, 0x04, 0x08, 0x10)

**Received by TM4C123**
- Expected **ID**: `0x123`
- **Mask**: `0x7FF` (11-bit standard ID mask)
- **Data Length**: Variable (1-8 bytes)
- **Processing**: Byte 0 used as LED output value

### Message Filtering

**STM32L476RG** (Receive Filter)
```c
// Mask mode, 32-bit filter
// Accepts IDs matching pattern: 0x123 (±mask)
#define FILTER_REGISTER_1   (0x00000120 << 3) | (1 << 2)
#define FILTER_REGISTER_2   (0x1FFFFFF0 << 3) | (1 << 2)
```

**TM4C123GH6PM** (Message Object Filter)
```c
// Message Object 1 for ID reception
#define RX_EXPECTED_ID  0x11      // Can be customized
#define RX_MASK         0x7FF     // Mask for ID filtering
```

### GPIO Pin Selection

If your hardware uses different CAN pins, update the pin selection:

**For STM32L476** – Edit `can_gpio_init()` call:
```c
can_gpio_init(CAN1, CAN_GPIO_PA11_PA12);  // Alternate option 1
can_gpio_init(CAN1, CAN_GPIO_PB8_PB9);    // Current (main_node1.c)
can_gpio_init(CAN1, CAN_GPIO_PD0_PD1);    // Alternate option 2
```

**For TM4C123** – Edit `can_gpio_init()` call:
```c
can_gpio_init(CAN0, CAN0_GPIO_PB4_PB5);   // Current (main.c)
can_gpio_init(CAN0, CAN0_GPIO_PE4_PE5);   // Alternate option 1
can_gpio_init(CAN0, CAN0_GPIO_PF0_PF3);   // Alternate option 2
can_gpio_init(CAN0, CAN1_GPIO_PA0_PA1);   // Alternate option 3 (CAN1)
```

### Button Input & Interrupt Configuration

**STM32L476** – Button on **GPIO PC13** with external interrupt:
```c
// Interrupt configuration in sw_init()
RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;      // Enable GPIOC clock
GPIOC->MODER &= ~GPIO_MODER_MODE13;       // Input mode
RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;     // Enable SYSCFG
SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;  // Map to PC13
EXTI->FTSR1 |= EXTI_FTSR1_FT13;            // Falling edge trigger
EXTI->IMR1 |= EXTI_IMR1_IM13;              // Enable interrupt
NVIC_EnableIRQ(EXTI15_10_IRQn);            // Enable NVIC
```

**LED Cycle Logic** – In interrupt handler:
```c
void EXTI15_10_IRQHandler(void) {
    if ((EXTI->PR1 & EXTI_PR1_PIF13) != 0) {
        can_transmit(CAN1, 0x123, true, false, 1, &leds_data[led_index]);
        led_index++;
        if (led_index >= 5U) led_index = 0U;
        EXTI->PR1 |= EXTI_PR1_PIF13;   // Clear interrupt flag
    }
}
```

### Test Modes

**Loopback Mode** – Node receives its own transmitted messages
- **Use Case**: Test CAN driver without external bus
- **Enable**: `can_init(..., LOOPBACK_ENABLE, ...)`

**Silent Mode** – Node receives but does not transmit
- **Use Case**: Monitor-only node (passive listening)
- **Enable**: `can_init(..., SILENT_ENABLE, ...)`

**Normal Mode** – Full bidirectional communication
- **Enable**: `can_init(..., LOOPBACK_DISABLE, SILENT_DISABLE, ...)`

---

## CAN Driver API Reference

### STM32L476RG Driver Functions

```c
// GPIO Initialization
int can_gpio_init(CAN_TypeDef* CANx, CAN_GPIO_Option option);
// - CANx: CAN1
// - option: CAN_GPIO_PA11_PA12, CAN_GPIO_PB8_PB9, or CAN_GPIO_PD0_PD1

// CAN Controller Initialization
int can_init(CAN_TypeDef* CANx, bool TTCM, bool ABOM, bool AWUM,
             bool NART, bool RFLM, bool TXFP, uint32_t SJW,
             uint32_t BS1, uint32_t BS2, uint32_t PRESCALER,
             bool LOOPBACK, bool SILENT);
// - Returns: CAN_InitStatus_SUCCESS (0) or CAN_InitStatus_FAILED (1)

// Message Transmission
int can_transmit(CAN_TypeDef *CANx, uint32_t ID, bool EXT, bool RTR,
                 uint8_t DLC, uint8_t *DATA);
// - ID: 11-bit (standard) or 29-bit (extended) identifier
// - EXT: true for extended ID, false for standard ID
// - RTR: Remote transmission request flag
// - DLC: Data length (0-8 bytes)
// - Returns: Mailbox number (0-2) or TRANSMISSION_FAILED (-1)

// Message Filter Configuration
int can_filter_init(CAN_TypeDef* CANx, uint32_t filter_number,
                    bool scale_32bit, bool id_list_mode,
                    uint32_t filter_register1, uint32_t filter_register2,
                    uint32_t fifo, bool enable);
// - filter_number: 0-27
// - scale_32bit: 32-bit or 16-bit filter scale
// - id_list_mode: true=list mode, false=mask mode
// - Returns: FILTER_CONFIG_SUCCESS (0) or FILTER_CONFIG_ERROR (-1)

// Message Reception
int can_receive(CAN_TypeDef* CANx, uint8_t fifo, bool release,
                uint32_t *id, bool *ext, bool *rtr, uint8_t *fmi,
                uint8_t *length, uint8_t *data);
// - fifo: 0 or 1
// - release: Automatically release FIFO after reading
// - Returns: RECEPTION_SUCCESS (0) or RECEPTION_FAILED (-1)
```

### TM4C123GH6PM Driver Functions

```c
// GPIO Initialization
int can_gpio_init(CAN_Option CANx, CAN_GPIO_Option option);
// - CANx: CAN0 or CAN1
// - option: CAN0_GPIO_PB4_PB5, CAN0_GPIO_PE4_PE5, etc.

// CAN Controller Initialization
int can_init(CAN_Option CANx, bool test, uint8_t BRP, uint8_t SJW,
             uint8_t TS1, uint8_t TS2, bool loopback, bool silent,
             bool basic);
// - test: Test mode enable flag
// - BRP: Bit Rate Prescaler (1-1024)
// - Returns: 0 for success, non-zero for failure

// Message Object & Filter Configuration
int can_filter_config(CAN_Option CANx, uint8_t msg_obj, uint32_t id,
                      uint32_t mask, bool extended, bool rtr,
                      bool use_mask, bool enable);
// - msg_obj: Message object number
// - id: CAN identifier to accept
// - mask: Identifier mask for filtering
// - Returns: 0 for success, non-zero for failure

// Message Transmission
int can_transmit(CAN_Option CANx, uint32_t id, bool extended, bool rtr,
                 uint8_t *data, uint8_t len);
// - len: Data length (0-8 bytes)
// - Returns: 0 for success, non-zero for failure

// Message Reception
int can_receive(CAN_Option CANx, uint32_t *id, bool *extended, bool *rtr,
                uint8_t *data, uint8_t *len);
// - Extracts received message from FIFO
// - Returns: 0 for success, non-zero for failure
```

---

## Troubleshooting

| Issue | Possible Cause | Solution |
|-------|---|---|
| **No CAN communication** | Baud rate mismatch | Verify both nodes use same bit timing parameters |
| | Incorrect GPIO pins | Check pin configuration matches physical CAN bus connection |
| | Missing CAN transceiver | Ensure CAN transceiver module is properly connected |
| **Messages not received** | Filter configuration incorrect | Verify receiver filter accepts transmitted CAN ID |
| | FIFO full or overflow | Enable RFLM or handle FIFO release properly |
| **LED not updating** | Data not reaching TM4C123 | Check CAN message reception in TM4C123 code |
| | GPIO LED pins not configured | Verify LED pins are in output mode |
| **Initialization fails** | Invalid bit timing parameters | Check SJW, BS1, BS2 are within valid ranges |
| | CAN module already in use | Ensure CAN peripheral is not initialized twice |
| **Button press not detected** | EXTI configuration incomplete | Verify PC13 EXTI setup and NVIC enable |
| | Pull-up resistor missing | Enable internal pull-up on PC13 |

---

## References

- **STM32L476 Datasheet**: ARM Cortex-M4 CAN communication details
- **TM4C123 Datasheet**: TI Cortex-M4 CAN module specifications
- **CAN 2.0 Specification**: Controller Area Network protocol details
- **CMSIS Documentation**: Cortex Microcontroller Software Interface Standards

---

## License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.

---

## Author

**EJ-JATIMohammed**

---

**Last Updated**: 2026

**Note**: This implementation demonstrates practical CAN communication between different microcontroller platforms using CMSIS standards. For production use, consider additional error handling, validation, and safety measures as required by your application domain.
