#include <stdint.h>
#include <stdbool.h>
#include "can_driver.h"

/* =================== TX CONFIG =================== */
#define TX_ID           0x33
#define TX_EXTENDED     false
#define TX_RTR          false
#define TX_DATA         {'1','2','3'}
#define TX_LEN          3

/* =================== RX CONFIG =================== */
uint32_t rx_id;
bool     rx_extended;
bool     rx_rtr;
uint8_t  rx_data[8];
uint8_t  rx_len;

/* =================== CAN INIT CONFIG =================== */
#define CAN_TEST_MODE       true
#define CAN_LOOPBACK        true
#define CAN_SILENT          false
#define CAN_BASIC           false

//can speed 250 Kbps for sysclock = 50Mhz  :
#define CAN_BRP             10U
#define CAN_SJW             1U
#define CAN_TS1             15U
#define CAN_TS2             4U

/* =================== RX FILTER CONFIG =================== */
#define RX_MSG_OBJ          1
#define RX_EXPECTED_ID      0x11
#define RX_MASK             0x7FF
#define RX_STANDARD_ID      false
#define RX_DATA_FRAME       false
#define RX_USE_MASK         false
#define RX_ENABLE           true


int main(void)
{
    uint8_t tx_data[8] = TX_DATA;
    can_gpio_init(CAN0, CAN0_GPIO_PB4_PB5);
    can_init(
        CAN0,
        CAN_TEST_MODE,  // Test mode
        CAN_BRP,        // Baudrate prescaler
        CAN_SJW,        // Synchronization jump width
        CAN_TS1,        // Time segment 1
        CAN_TS2,        // Time segment 2
        CAN_LOOPBACK,   // Loopback
        CAN_SILENT,     // Silent
        CAN_BASIC       // Basic
    );
    can_filter_config(
        CAN0,
        RX_MSG_OBJ,        // Message object
        RX_EXPECTED_ID,    // ID attendu
        RX_MASK,           // Masque
        RX_STANDARD_ID,    // Standard ID
        RX_DATA_FRAME,     // Data frame
        RX_USE_MASK,       // Use mask
        RX_ENABLE          // Enable filter
    );

    while (1)
    {
        can_transmit(CAN0, TX_ID, TX_EXTENDED, TX_RTR, tx_data, TX_LEN);
        can_receive(
            CAN0,
            &rx_id,
            &rx_extended,
            &rx_rtr,
            rx_data,
            &rx_len
        );
    }
}
