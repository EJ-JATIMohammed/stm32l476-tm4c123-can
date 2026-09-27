# CAN Communication between STM32L476RG and TM4C123GH6PM

A comprehensive embedded systems project demonstrating Controller Area Network (CAN) communication between two microcontroller platforms using CMSIS (Cortex Microcontroller Software Interface Standard).

## Project Overview

This project implements a dual-node CAN communication system with:
- **STM32L476RG** (Cortex-M4 based ARM microcontroller)
- **TM4C123GH6PM** (Cortex-M4 based TI microcontroller)

Both nodes communicate over a CAN bus, demonstrating platform-independent CAN driver implementation using CMSIS standards.

## Main Features

- **Dual-Node CAN Communication**: Full-duplex communication between STM32L4 and TM4C123 microcontrollers
- **CMSIS-Based Drivers**: Platform-specific CAN drivers implementing a common interface
- **Configurable CAN Parameters**: Support for various CAN speeds, bit timing, and operational modes
- **Flexible Message Filtering**: Multiple filter configurations for receiving specific CAN messages
- **GPIO Abstraction**: Simplified GPIO initialization for different pin mapping options
- **Interrupt-Driven Reception**: Efficient message reception using interrupt handlers
- **Test Modes**: Support for loopback and silent modes for debugging and testing

## Hardware Requirements

### STM32L476RG Node
- STM32L476RG microcontroller (ARM Cortex-M4, 80 MHz)
- ST-LINK debugger/programmer
- CAN transceiver module (e.g., MCP2551 or equivalent)
- External 8 MHz crystal oscillator (optional, for clock accuracy)

### TM4C123GH6PM Node
- TM4C123GH6PM microcontroller (ARM Cortex-M4, 80 MHz)
- TI ICDI debugger/programmer
- CAN transceiver module (e.g., MCP2551 or equivalent)
- External oscillator (as per TM4C datasheet)

### CAN Bus
- CAN transceiver modules for both nodes
- Twisted-pair CAN bus wiring with 120Ω termination resistors at both ends

## Software Requirements

- ARM Cortex-M compiler (e.g., ARM GCC, Keil MDK, or IAR Embedded Workbench)
- STM32Cube HAL or CMSIS-Core libraries for STM32L4 series
- TivaWare peripheral driver library for TM4C123
- CAN communication protocol understanding

## Project Structure

```
stm32l476-tm4c123-can/
│
├── stm32L4_node/
│   ├── application/
│   │   └── main_node1.c          # STM32L476RG main application
│   └── can_driver/
│       ├── Inc/
│       │   └── can_driver.h      # CAN driver header with function declarations
│       └── Src/
│           └── can_driver.c      # CAN driver implementation for STM32L4
│
├── tm4c123g6pm_node/
│   ├── application/
│   │   └── main.c               # TM4C123GH6PM main application
│   └── can_driver/
│       ├── Inc/
│       │   └── can_driver.h      # CAN driver header with function declarations
│       └── Src/
│           └── can_driver.c      # CAN driver implementation for TM4C123
│
├── README.md                      # This file
└── LICENSE                        # MIT License
```

## How to Build and Use

### STM32L476RG Node

1. **Setup Project Environment**
   - Import the project into your IDE (STM32CubeIDE, Keil, or IAR)
   - Configure STM32Cube for the STM32L476RG
   - Set up clock configuration (80 MHz system clock)

2. **CAN Configuration**
   - Define CAN GPIO pins (options: PA11/PA12, PB8/PB9, or PD0/PD1)
   - Configure CAN baud rate using bit timing parameters:
     - SJW (Synchronization Jump Width)
     - BS1 (Bit Segment 1)
     - BS2 (Bit Segment 2)
     - Prescaler (for baud rate calculation)

3. **Build and Flash**
   ```bash
   # Compile the STM32L4 firmware
   make -C stm32L4_node
   
   # Flash to the device using ST-LINK
   STM32_Programmer_CLI -c port=SWD -d build/firmware.bin
   ```

### TM4C123GH6PM Node

1. **Setup Project Environment**
   - Import the project into your IDE
   - Configure TivaWare drivers for the TM4C123GH6PM
   - Set up system clock (80 MHz)

2. **CAN Configuration**
   - Define CAN GPIO pins (options: PB4/PB5, PE4/PE5, PF0/PF3, or PA0/PA1)
   - Configure CAN timing parameters:
     - BRP (Bit Rate Prescaler)
     - SJW (Synchronization Jump Width)
     - TS1, TS2 (Time Segments 1 and 2)

3. **Build and Flash**
   ```bash
   # Compile the TM4C123 firmware
   make -C tm4c123g6pm_node
   
   # Flash to the device using LM Flash Programmer
   lmflash build/firmware.bin
   ```

## Important Configuration Notes

### CAN Baud Rate

Both nodes must be configured with the same CAN baud rate. Common configurations:

**STM32L476RG (APB1 Clock = 80 MHz):**
- 250 kbps: Prescaler=1, BS1=13, BS2=2, SJW=1

**TM4C123GH6PM (System Clock = 80 MHz):**
- 250 kbps: BRP=10, TS1=15, TS2=4, SJW=1

### Message Filtering

- **STM32L476RG**: Uses filter banks with 32-bit or 16-bit scale, mask or list mode
- **TM4C123GH6PM**: Uses message objects with identifier and mask filtering

Configure filters to accept messages with specific CAN IDs according to your protocol.

### GPIO Pin Selection

Both microcontrollers support multiple GPIO options for CAN pins. Select the pins appropriate for your hardware design and update the `can_gpio_init()` function call accordingly.

### Test and Debug Modes

- **Loopback Mode**: Node receives its own transmitted messages (useful for testing)
- **Silent Mode**: Node receives but does not transmit (useful for monitoring)
- **Normal Mode**: Standard bidirectional CAN operation

## CAN Driver API

### STM32L476RG Interface

```c
int can_gpio_init(CAN_TypeDef* CANx, CAN_GPIO_Option option);
int can_init(CAN_TypeDef* CANx, bool TTCM, bool ABOM, bool AWUM, 
             bool NART, bool RFLM, bool TXFP, uint32_t SJW, 
             uint32_t BS1, uint32_t BS2, uint32_t PRESCALER, 
             bool LOOPBACK, bool SILENT);
int can_transmit(CAN_TypeDef *CANx, uint32_t ID, bool EXT, 
                 bool RTR, uint8_t DLC, uint8_t *DATA);
int can_filter_init(CAN_TypeDef* CANx, uint32_t filter_number, 
                    bool scale_32bit, bool id_list_mode, 
                    uint32_t filter_register1, uint32_t filter_register2, 
                    uint32_t fifo, bool enable);
int can_receive(CAN_TypeDef* CANx, uint8_t fifo, bool release, 
                uint32_t *id, bool *ext, bool *rtr, uint8_t *fmi, 
                uint8_t *length, uint8_t *data);
```

### TM4C123GH6PM Interface

```c
int can_gpio_init(CAN_Option CANx, CAN_GPIO_Option option);
int can_init(CAN_Option CANx, bool test, uint8_t BRP, uint8_t SJW, 
             uint8_t TS1, uint8_t TS2, bool loopback, bool silent, bool basic);
int can_filter_config(CAN_Option CANx, uint8_t msg_obj, uint32_t id, 
                      uint32_t mask, bool extended, bool rtr, 
                      bool use_mask, bool enable);
int can_transmit(CAN_Option CANx, uint32_t id, bool extended, bool rtr, 
                 uint8_t *data, uint8_t len);
int can_receive(CAN_Option CANx, uint32_t *id, bool *extended, bool *rtr, 
                uint8_t *data, uint8_t *len);
```

## Troubleshooting

- **No Communication**: Verify CAN baud rates match on both nodes and physical CAN bus connections
- **Missing Messages**: Check filter configuration and ensure receiving node filters accept transmitted IDs
- **Initialization Errors**: Review GPIO pin selection and ensure alternate function configuration is correct

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Author

EJ-JATIMohammed

---

**Note**: This implementation uses CMSIS standards for embedded systems programming. Refer to the individual microcontroller datasheets and reference manuals for detailed hardware specifications and register configurations.
