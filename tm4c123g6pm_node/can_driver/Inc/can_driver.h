#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

/* Status codes for CAN GPIO initialization */
#define CAN_GPIO_INIT_STATUS_SUCCESS   0
#define CAN_GPIO_INIT_STATUS_FAILED    1

/* SYSCTL_RCGCGPIO_R – GPIO Run Mode Clock Gating Control */
#define SYSCTL_RCGCGPIO_GPIOA_Pos      0
#define SYSCTL_RCGCGPIO_GPIOA_Msk      (1U << SYSCTL_RCGCGPIO_GPIOA_Pos)  /*!< Enable GPIOA clock */

#define SYSCTL_RCGCGPIO_GPIOB_Pos      1
#define SYSCTL_RCGCGPIO_GPIOB_Msk      (1U << SYSCTL_RCGCGPIO_GPIOB_Pos)  /*!< Enable GPIOB clock */

#define SYSCTL_RCGCGPIO_GPIOC_Pos      2
#define SYSCTL_RCGCGPIO_GPIOC_Msk      (1U << SYSCTL_RCGCGPIO_GPIOC_Pos)  /*!< Enable GPIOC clock */

#define SYSCTL_RCGCGPIO_GPIOD_Pos      3
#define SYSCTL_RCGCGPIO_GPIOD_Msk      (1U << SYSCTL_RCGCGPIO_GPIOD_Pos)  /*!< Enable GPIOD clock */

#define SYSCTL_RCGCGPIO_GPIOE_Pos      4
#define SYSCTL_RCGCGPIO_GPIOE_Msk      (1U << SYSCTL_RCGCGPIO_GPIOE_Pos)  /*!< Enable GPIOE clock */

#define SYSCTL_RCGCGPIO_GPIOF_Pos      5
#define SYSCTL_RCGCGPIO_GPIOF_Msk      (1U << SYSCTL_RCGCGPIO_GPIOF_Pos)  /*!< Enable GPIOF clock */

/* SYSCTL_PRGPIO_R – GPIO Peripheral Ready */
#define SYSCTL_PRGPIO_GPIOA_Pos        0
#define SYSCTL_PRGPIO_GPIOA_Msk        (1U << SYSCTL_PRGPIO_GPIOA_Pos)    /*!< GPIOA ready */

#define SYSCTL_PRGPIO_GPIOB_Pos        1
#define SYSCTL_PRGPIO_GPIOB_Msk        (1U << SYSCTL_PRGPIO_GPIOB_Pos)    /*!< GPIOB ready */

#define SYSCTL_PRGPIO_GPIOC_Pos        2
#define SYSCTL_PRGPIO_GPIOC_Msk        (1U << SYSCTL_PRGPIO_GPIOC_Pos)    /*!< GPIOC ready */

#define SYSCTL_PRGPIO_GPIOD_Pos        3
#define SYSCTL_PRGPIO_GPIOD_Msk        (1U << SYSCTL_PRGPIO_GPIOD_Pos)    /*!< GPIOD ready */

#define SYSCTL_PRGPIO_GPIOE_Pos        4
#define SYSCTL_PRGPIO_GPIOE_Msk        (1U << SYSCTL_PRGPIO_GPIOE_Pos)    /*!< GPIOE ready */

#define SYSCTL_PRGPIO_GPIOF_Pos        5
#define SYSCTL_PRGPIO_GPIOF_Msk        (1U << SYSCTL_PRGPIO_GPIOF_Pos)    /*!< GPIOF ready */

/* GPIO Alternate Function Select (AFSEL) */
#define GPIO_AFSEL_0                   (1U << 0)  /*!< Pin 0 alternate function */
#define GPIO_AFSEL_1                   (1U << 1)  /*!< Pin 1 alternate function */
#define GPIO_AFSEL_2                   (1U << 2)
#define GPIO_AFSEL_3                   (1U << 3)
#define GPIO_AFSEL_4                   (1U << 4)
#define GPIO_AFSEL_5                   (1U << 5)
#define GPIO_AFSEL_6                   (1U << 6)
#define GPIO_AFSEL_7                   (1U << 7)

/* GPIO Direction (DIR) */
#define GPIO_DIR_0                     (1U << 0)
#define GPIO_DIR_1                     (1U << 1)
#define GPIO_DIR_2                     (1U << 2)
#define GPIO_DIR_3                     (1U << 3)
#define GPIO_DIR_4                     (1U << 4)
#define GPIO_DIR_5                     (1U << 5)
#define GPIO_DIR_6                     (1U << 6)
#define GPIO_DIR_7                     (1U << 7)

/* GPIO Digital Enable (DEN) */
#define GPIO_DEN_0                     (1U << 0)
#define GPIO_DEN_1                     (1U << 1)
#define GPIO_DEN_2                     (1U << 2)
#define GPIO_DEN_3                     (1U << 3)
#define GPIO_DEN_4                     (1U << 4)
#define GPIO_DEN_5                     (1U << 5)
#define GPIO_DEN_6                     (1U << 6)
#define GPIO_DEN_7                     (1U << 7)

/* GPIO Port Control (PCTL) – 4 bits per pin */
#define GPIO_PCTL_PMC0_Pos              0U
#define GPIO_PCTL_PMC0_Msk              (0xFUL << GPIO_PCTL_PMC0_Pos)

#define GPIO_PCTL_PMC1_Pos              4U
#define GPIO_PCTL_PMC1_Msk              (0xFUL << GPIO_PCTL_PMC1_Pos)

#define GPIO_PCTL_PMC2_Pos              8U
#define GPIO_PCTL_PMC2_Msk              (0xFUL << GPIO_PCTL_PMC2_Pos)

#define GPIO_PCTL_PMC3_Pos              12U
#define GPIO_PCTL_PMC3_Msk              (0xFUL << GPIO_PCTL_PMC3_Pos)

#define GPIO_PCTL_PMC4_Pos              16U
#define GPIO_PCTL_PMC4_Msk              (0xFUL << GPIO_PCTL_PMC4_Pos)

#define GPIO_PCTL_PMC5_Pos              20U
#define GPIO_PCTL_PMC5_Msk              (0xFUL << GPIO_PCTL_PMC5_Pos)

#define GPIO_PCTL_PMC6_Pos              24U
#define GPIO_PCTL_PMC6_Msk              (0xFUL << GPIO_PCTL_PMC6_Pos)

#define GPIO_PCTL_PMC7_Pos              28U
#define GPIO_PCTL_PMC7_Msk              (0xFUL << GPIO_PCTL_PMC7_Pos)

/* ---------------- CAN Module ---------------- */
typedef enum
{
    CAN0,   /* CAN module 0 */
    CAN1    /* CAN module 1 */
} CAN_Option;

/* ---------------- CAN GPIO Pin Options ---------------- */
typedef enum
{
    CAN0_GPIO_PB4_PB5,   /* PB4 = CAN0RX, PB5 = CAN0TX */
    CAN0_GPIO_PE4_PE5,   /* PE4 = CAN0RX, PE5 = CAN0TX */
    CAN0_GPIO_PF0_PF3,   /* PF0 = CAN0RX, PF3 = CAN0TX */

    CAN1_GPIO_PA0_PA1    /* PA0 = CAN1RX, PA1 = CAN1TX */
} CAN_GPIO_Option;

/* ---------------- Function Prototypes ---------------- */

/* GPIO configuration for CAN pins */
int can_gpio_init(
    CAN_Option      CANx,     /* Select CAN module (CAN0 or CAN1) */
    CAN_GPIO_Option option    /* Select GPIO pin mapping */
);

/* CAN peripheral initialization */
int can_init(
    CAN_Option CANx,    /* Select CAN module */
    bool       test,    /* Enable test mode */
    uint8_t    BRP,     /* Baudrate prescaler */
    uint8_t    SJW,     /* Synchronization jump width */
    uint8_t    TS1,     /* Time segment 1 */
    uint8_t    TS2,     /* Time segment 2 */
    bool       loopback,/* Enable loopback mode */
    bool       silent,  /* Enable silent mode */
    bool       basic    /* Enable basic CAN mode */
);

/* CAN filter / message object configuration */
int can_filter_config(
    CAN_Option CANx,      /* Select CAN module */
    uint8_t    msg_obj,   /* Message object number */
    uint32_t   id,        /* CAN identifier */
    uint32_t   mask,      /* Identifier mask */
    bool       extended,  /* Extended ID enable */
    bool       rtr,       /* Remote transmission request */
    bool       use_mask,  /* Enable mask filtering */
    bool       enable     /* Enable message object */
);

/* CAN message transmission */
int can_transmit(
    CAN_Option CANx,      /* Select CAN module */
    uint32_t   id,        /* CAN identifier */
    bool       extended,  /* Extended ID */
    bool       rtr,       /* Remote frame request */
    uint8_t   *data,      /* Pointer to data buffer */
    uint8_t    len        /* Data length (0–8 bytes) */
);

/* CAN message reception */
int can_receive(
    CAN_Option CANx,      /* Select CAN module */
    uint32_t  *id,        /* Pointer to received identifier */
    bool      *extended,  /* Pointer to extended ID flag */
    bool      *rtr,       /* Pointer to RTR flag */
    uint8_t   *data,      /* Pointer to received data buffer */
    uint8_t   *len        /* Pointer to received data length */
);

#endif 
