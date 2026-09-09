#include "can_driver.h"
#include "tm4c123gh6pm.h"

#define GPIO_PCTL_CAN    0x8U  /* CAN function */




int can_gpio_init(CAN_Option CANx, CAN_GPIO_Option option)
{
    switch (CANx)
    {
        case CAN0:
            switch (option)
            {
 
                case CAN0_GPIO_PB4_PB5:
                    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_GPIOB_Msk;
                    while (!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_GPIOB_Msk));

                    GPIO_PORTB_AFSEL_R |= GPIO_AFSEL_4 | GPIO_AFSEL_5;
                    GPIO_PORTB_PCTL_R =
                        (GPIO_PORTB_PCTL_R & ~(GPIO_PCTL_PMC4_Msk | GPIO_PCTL_PMC5_Msk)) |
                        (GPIO_PCTL_CAN << GPIO_PCTL_PMC4_Pos) |
                        (GPIO_PCTL_CAN << GPIO_PCTL_PMC5_Pos);

                    GPIO_PORTB_DIR_R |= GPIO_DIR_5;     /* TX pin */
                    GPIO_PORTB_DIR_R &= ~GPIO_DIR_4;    /* RX pin */

                    GPIO_PORTB_DEN_R |= GPIO_DEN_4 | GPIO_DEN_5;
                    break;

                case CAN0_GPIO_PE4_PE5:
                    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_GPIOE_Msk;
                    while (!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_GPIOE_Msk));

                    GPIO_PORTE_AFSEL_R |= GPIO_AFSEL_4 | GPIO_AFSEL_5;
                    GPIO_PORTE_PCTL_R =
                        (GPIO_PORTE_PCTL_R & ~(GPIO_PCTL_PMC4_Msk | GPIO_PCTL_PMC5_Msk)) |
                        (GPIO_PCTL_CAN << GPIO_PCTL_PMC4_Pos) |
                        (GPIO_PCTL_CAN << GPIO_PCTL_PMC5_Pos);

                    GPIO_PORTE_DIR_R |= GPIO_DIR_5;
                    GPIO_PORTE_DIR_R &= ~GPIO_DIR_4;

                    GPIO_PORTE_DEN_R |= GPIO_DEN_4 | GPIO_DEN_5;
                    break;
                case CAN0_GPIO_PF0_PF3:
                    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_GPIOF_Msk;
                    while (!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_GPIOF_Msk));

                    GPIO_PORTF_AFSEL_R |= GPIO_AFSEL_0 | GPIO_AFSEL_3;
                    GPIO_PORTF_PCTL_R =
                        (GPIO_PORTF_PCTL_R & ~(GPIO_PCTL_PMC0_Msk | GPIO_PCTL_PMC3_Msk)) |
                        (GPIO_PCTL_CAN << GPIO_PCTL_PMC0_Pos) |
                        (GPIO_PCTL_CAN << GPIO_PCTL_PMC3_Pos);

                    GPIO_PORTF_DIR_R |= GPIO_DIR_3;
                    GPIO_PORTF_DIR_R &= ~GPIO_DIR_0;

                    GPIO_PORTF_DEN_R |= GPIO_DEN_0 | GPIO_DEN_3;
                    break;

                default:
                    return CAN_GPIO_INIT_STATUS_FAILED;
            }
            break;

        case CAN1:
            switch (option)
            {
                case CAN1_GPIO_PA0_PA1:
                    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_GPIOA_Msk;
                    while (!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_GPIOA_Msk));

                    GPIO_PORTA_AFSEL_R |= GPIO_AFSEL_0 | GPIO_AFSEL_1;
                    GPIO_PORTA_PCTL_R =
                        (GPIO_PORTA_PCTL_R & ~(GPIO_PCTL_PMC0_Msk | GPIO_PCTL_PMC1_Msk)) |
                        (GPIO_PCTL_CAN << GPIO_PCTL_PMC0_Pos) |
                        (GPIO_PCTL_CAN << GPIO_PCTL_PMC1_Pos);

                    GPIO_PORTA_DIR_R |= GPIO_DIR_1;
                    GPIO_PORTA_DIR_R &= ~GPIO_DIR_0;

                    GPIO_PORTA_DEN_R |= GPIO_DEN_0 | GPIO_DEN_1;
                    break;

                default:
                    return CAN_GPIO_INIT_STATUS_FAILED;
            }
            break;

        default:
            return CAN_GPIO_INIT_STATUS_FAILED;
    }

    return CAN_GPIO_INIT_STATUS_SUCCESS;
}


/* CAN Bit Timing Register (CANx_BIT_R) positions */
#define CAN_BIT_SJW_Pos  6
#define CAN_BIT_TS1_Pos  8
#define CAN_BIT_TS2_Pos  12
#define CAN_BIT_BRP_Pos  0  

#define CAN_INIT_STATUS_SUCCESS   0U   /* Initialization successful */
#define CAN_INIT_STATUS_FAILED    1U   /* Initialization failed */


/* CAN Bit Timing Register (CANx_BIT_R) positions */
#define CAN_BIT_SJW_Pos  6
#define CAN_BIT_TS1_Pos  8
#define CAN_BIT_TS2_Pos  12
#define CAN_BIT_BRP_Pos  0

int can_init(
    CAN_Option CANx,
    bool       test,
    uint8_t    BRP,      /* Baud rate prescaler */
    uint8_t    SJW,      /* Synchronization jump width */
    uint8_t    TS1,      /* Time segment 1 */
    uint8_t    TS2,      /* Time segment 2 */
    bool       loopback, /* Enable loopback mode */
    bool       silent,   /* Enable silent mode */
    bool       basic     /* Enable basic mode */
)
{
    /* Input parameter validation */
    if ((BRP == 0U) || (SJW == 0U) || (TS1 == 0U) || (TS2 == 0U))
    {
        return CAN_INIT_STATUS_FAILED;
    }

    if (CANx == CAN0)
    {
        /* Enable CAN0 clock */
        SYSCTL_RCGCCAN_R |= SYSCTL_RCGCCAN_R0;

        /* Wait until CAN0 is ready */
        while ((SYSCTL_PRCAN_R & SYSCTL_PRCAN_R0) == 0U)
        {
            ;
        }

        /* Enter initialization mode */
        CAN0_CTL_R |= CAN_CTL_INIT;
        while ((CAN0_CTL_R & CAN_CTL_INIT) == 0U)
        {
            ;
        }

        /* Enable configuration change */
        CAN0_CTL_R |= CAN_CTL_CCE;

        /* Clear bit timing register */
        CAN0_BIT_R = 0U;

        /* Configure bit timing */
        CAN0_BIT_R =
            ((((uint32_t)SJW - 1U) << CAN_BIT_SJW_Pos) |
             (((uint32_t)TS1 - 1U) << CAN_BIT_TS1_Pos) |
             (((uint32_t)TS2 - 1U) << CAN_BIT_TS2_Pos) |
             (((uint32_t)BRP - 1U) << CAN_BIT_BRP_Pos));

        /* Configure test modes */
        if (test == true)
        {
            CAN0_CTL_R |= CAN_CTL_TEST;

            /* Clear previous test configuration */
            CAN0_TST_R &= ~(CAN_TST_LBACK | CAN_TST_SILENT | CAN_TST_BASIC);

            if (loopback == true)
            {
                CAN0_TST_R |= CAN_TST_LBACK;
            }

            if (silent == true)
            {
                CAN0_TST_R |= CAN_TST_SILENT;
            }

            if (basic == true)
            {
                CAN0_TST_R |= CAN_TST_BASIC;
            }
        }
        else
        {
            /* Disable test mode */
            CAN0_CTL_R &= ~CAN_CTL_TEST;
        }

        /* Exit initialization mode */
        CAN0_CTL_R &= ~CAN_CTL_INIT;
        while ((CAN0_CTL_R & CAN_CTL_INIT) != 0U)
        {
            ;
        }
    }
    else if (CANx == CAN1)
    {
        /* Enable CAN1 clock */
        SYSCTL_RCGCCAN_R |= SYSCTL_RCGCCAN_R1;

        /* Wait until CAN1 is ready */
        while ((SYSCTL_PRCAN_R & SYSCTL_PRCAN_R1) == 0U)
        {
            ;
        }

        /* Enter initialization mode */
        CAN1_CTL_R |= CAN_CTL_INIT;
        while ((CAN1_CTL_R & CAN_CTL_INIT) == 0U)
        {
            ;
        }

        /* Enable configuration change */
        CAN1_CTL_R |= CAN_CTL_CCE;

        /* Clear bit timing register */
        CAN1_BIT_R = 0U;

        /* Configure bit timing */
        CAN1_BIT_R =
            ((((uint32_t)SJW - 1U) << CAN_BIT_SJW_Pos) |
             (((uint32_t)TS1 - 1U) << CAN_BIT_TS1_Pos) |
             (((uint32_t)TS2 - 1U) << CAN_BIT_TS2_Pos) |
             (((uint32_t)BRP - 1U) << CAN_BIT_BRP_Pos));

        /* Configure test modes */
        if (test == true)
        {
            CAN1_CTL_R |= CAN_CTL_TEST;
            CAN1_TST_R &= ~(CAN_TST_LBACK | CAN_TST_SILENT | CAN_TST_BASIC);

            if (loopback == true)
            {
                CAN1_TST_R |= CAN_TST_LBACK;
            }

            if (silent == true)
            {
                CAN1_TST_R |= CAN_TST_SILENT;
            }

            if (basic == true)
            {
                CAN1_TST_R |= CAN_TST_BASIC;
            }
        }
        else
        {
            CAN1_CTL_R &= ~CAN_CTL_TEST;
        }

        /* Exit initialization mode */
        CAN1_CTL_R &= ~CAN_CTL_INIT;
        while ((CAN1_CTL_R & CAN_CTL_INIT) != 0U)
        {
            ;
        }
    }
    else
    {
        return CAN_INIT_STATUS_FAILED; 
    }

    return CAN_INIT_STATUS_SUCCESS;
}



uint8_t can_get_free_object(CAN_Option CANx)
{
    uint8_t i;

    if (CANx == CAN0)
    {
        for (i = 0U; i < 16U; i++)
        {
            if ((CAN0_TXRQ1_R & (1U << i)) == 0U)
            {
                return (i + 1U);
            }
        }
        for (i = 0U; i < 16U; i++)
        {
            if ((CAN0_TXRQ2_R & (1U << i)) == 0U)
            {
                return (i + 17U);
            }
        }
    }
    else if (CANx == CAN1)
    {
        for (i = 0U; i < 16U; i++)
        {
            if ((CAN1_TXRQ1_R & (1U << i)) == 0U)
            {
                return (i + 1U);
            }
        }
        for (i = 0U; i < 16U; i++)
        {
            if ((CAN1_TXRQ2_R & (1U << i)) == 0U)
            {
                return (i + 17U);
            }
        }
    }

    return 0U;
}

int can_transmit(
    CAN_Option CANx,      /* Select CAN module */
    uint32_t   id,        /* CAN identifier */
    bool       extended,  /* Extended ID */
    bool       rtr,       /* Remote frame request */
    uint8_t   *data,      /* Pointer to data buffer */
    uint8_t    len        /* Data length (0–8 bytes) */
)
{
    uint8_t msgObj = 0;
    int32_t status = 0; 

    if (len > 8U)
    {
        status = -1;
    }
    else
    {
        msgObj = can_get_free_object(CANx);

        if (msgObj == 0U)
        {
            status = -2;
        }
        else
        {
            if (CANx == CAN0)
            {
                while ((CAN0_IF1CRQ_R & CAN_IF1CRQ_BUSY) != 0U) 
					{
						;
					}

                CAN0_IF1CMSK_R = 0U;
                CAN0_IF1CMSK_R = CAN_IF1CMSK_WRNRD   |
                                 CAN_IF1CMSK_ARB     |
                                 CAN_IF1CMSK_CONTROL |
                                 CAN_IF1CMSK_DATAA   |
                                 CAN_IF1CMSK_DATAB;
					
                /*set up id */
                CAN0_IF1ARB1_R = 0U;
                CAN0_IF1ARB2_R = 0U;
                if (extended == true)
                {
                    CAN0_IF1ARB1_R = (uint16_t)(id & 0xFFFFU);
                    CAN0_IF1ARB2_R = (uint16_t)((id >> 16U) & 0x1FFFU) | CAN_IF1ARB2_XTD;
                }
                else
                {
                    CAN0_IF1ARB1_R = 0U;
                    CAN0_IF1ARB2_R = (uint16_t)((id & 0x7FFU) << 2U);
                }

				/*set up rtr */
                CAN0_IF1ARB2_R |= CAN_IF1ARB2_MSGVAL;
      
                if (rtr == true)
                {
                    CAN0_IF1ARB2_R &= (uint16_t)~(CAN_IF1ARB2_DIR);
                }
                else
                {
                    CAN0_IF1ARB2_R |= CAN_IF1ARB2_DIR;
                }

                /* STEP 6: Control bits (Without TXRQST) */
                CAN0_IF1MCTL_R = 0U;
                CAN0_IF1MCTL_R = (uint16_t)((len & 0x0FU) | CAN_IF1MCTL_EOB | CAN_IF1MCTL_NEWDAT);

                /* STEP 7: Data */
                if ((rtr == false) )
                {
                    CAN0_IF1DA1_R = ((uint16_t)data[1] << 8U) | (uint16_t)data[0];
                    CAN0_IF1DA2_R = ((uint16_t)data[3] << 8U) | (uint16_t)data[2];
                    CAN0_IF1DB1_R = ((uint16_t)data[5] << 8U) | (uint16_t)data[4];
                    CAN0_IF1DB2_R = ((uint16_t)data[7] << 8U) | (uint16_t)data[6];
                }

                /* STEP 9: Set TXRQST request */
                CAN0_IF1MCTL_R |= CAN_IF1MCTL_TXRQST;

                /* STEP 8: Start Transfer */
                CAN0_IF1CRQ_R = (msgObj & 0x3FU);
                
                while ((CAN0_IF1CRQ_R & CAN_IF1CRQ_BUSY) != 0U) { /* Wait */ }
            }
            else if (CANx == CAN1)
            {
                while ((CAN1_IF1CRQ_R & CAN_IF1CRQ_BUSY) != 0U) { /* Wait */ }

                CAN1_IF1CMSK_R = 0U;
                CAN1_IF1CMSK_R = CAN_IF1CMSK_WRNRD   |
                                 CAN_IF1CMSK_ARB     |
                                 CAN_IF1CMSK_CONTROL |
                                 CAN_IF1CMSK_DATAA   |
                                 CAN_IF1CMSK_DATAB;

                CAN1_IF1ARB1_R = 0U;
                CAN1_IF1ARB2_R = 0U;

                if (extended == true)
                {
                    CAN1_IF1ARB1_R = (uint16_t)(id & 0xFFFFU);
                    CAN1_IF1ARB1_R = (uint16_t)((id >> 16U) & 0x1FFFU) | CAN_IF1ARB2_XTD;
                }
                else
                {
                    CAN1_IF1ARB1_R = 0U;
                    CAN1_IF1ARB1_R = (uint16_t)((id & 0x7FFU) << 2U);
                }

                CAN1_IF1ARB1_R |= CAN_IF1ARB2_MSGVAL;

                if (rtr == true)
                {
                    CAN1_IF1ARB1_R &= (uint16_t)~(CAN_IF1ARB2_DIR);
                }
                else
                {
                    CAN1_IF1ARB1_R |= CAN_IF1ARB2_DIR;
                }


                CAN1_IF1MCTL_R = 0U;
                CAN1_IF1MCTL_R = (uint16_t)((len & 0x0FU) | CAN_IF1MCTL_EOB | CAN_IF1MCTL_NEWDAT);

                if (rtr == false)
                {
                    CAN1_IF1DA1_R = ((uint16_t)data[1] << 8U) | (uint16_t)data[0];
                    CAN1_IF1DA2_R = ((uint16_t)data[3] << 8U) | (uint16_t)data[2];
                    CAN1_IF1DB1_R = ((uint16_t)data[5] << 8U) | (uint16_t)data[4];
                    CAN1_IF1DB2_R = ((uint16_t)data[7] << 8U) | (uint16_t)data[6];
                }

                CAN1_IF1MCTL_R |= CAN_IF1MCTL_TXRQST;

                CAN1_IF1CRQ_R = (msgObj & 0x3FU);

                while ((CAN1_IF1CRQ_R & CAN_IF1CRQ_BUSY) != 0U) 
                { 
					;
				}
            }
            else
            {
                status = -2; 
            }
        }
    }

    return status;
}

int can_filter_config(
    CAN_Option CANx,      /* Select CAN module */
    uint8_t    msg_obj,   /* Message object number */
    uint32_t   id,        /* CAN identifier */
    uint32_t   mask,      /* Identifier mask */
    bool       extended,  /* Extended ID enable */
    bool       rtr,       /* Remote transmission request */
    bool       use_mask,  /* Enable mask filtering */
    bool       enable     /* Enable message object */
)
{
    if(CANx == CAN0)
    {
        /* Wait IF2 free */
        while(CAN0_IF2CRQ_R & CAN_IF2CRQ_BUSY);

        /* Enable writing to relevant fields */
        CAN0_IF2CMSK_R = CAN_IF2CMSK_ARB | CAN_IF2CMSK_CONTROL | CAN_IF2CMSK_MASK | CAN_IF2CMSK_WRNRD;

        /* Configure arbitration (ID) */
        if(extended)
        {
            CAN0_IF2ARB1_R = id & 0xFFFF;
            CAN0_IF2ARB2_R = ((id >> 16) & 0x1FFF) | CAN_IF2ARB2_XTD | CAN_IF2ARB2_MSGVAL;
        }
        else
        {
            CAN0_IF2ARB2_R = (id & 0x7FF)<<2 | CAN_IF2ARB2_MSGVAL;
        }

        /* Configure mask */
        if(extended)
        {
            CAN0_IF2MSK1_R = mask & 0xFFFF;
            CAN0_IF2MSK2_R = ((mask >> 16) & 0x1FFF) | (use_mask ? CAN_IF2MSK2_MXTD : 0);
        }
        else
        {
            CAN0_IF2MSK2_R = ((mask & 0x7FF)<<2) | (use_mask ? 0x8000 : 0);
        }

        /* Remote / Data frame */
        if(rtr) CAN0_IF2MCTL_R = CAN_IF2MCTL_RMTEN;
        else CAN0_IF2MCTL_R = 0;

        /* Enable the filter */
        if(enable) CAN0_IF2MCTL_R |= CAN_IF2MCTL_NEWDAT;

        /* Transfer to message object */
        CAN0_IF2CRQ_R = msg_obj;
        while(CAN0_IF2CRQ_R & CAN_IF2CRQ_BUSY);
    }
    else if(CANx == CAN1)
    {
        /* Wait IF2 free */
        while(CAN1_IF2CRQ_R & CAN_IF2CRQ_BUSY);

        /* Enable writing to relevant fields */
        CAN1_IF2CMSK_R = CAN_IF2CMSK_ARB | CAN_IF2CMSK_CONTROL | CAN_IF2CMSK_MASK | CAN_IF2CMSK_WRNRD;

        /* Configure arbitration (ID) */
        if(extended)
        {
            CAN1_IF2ARB1_R = id & 0xFFFF;
            CAN1_IF2ARB2_R = ((id >> 16) & 0x1FFF) | CAN_IF2ARB2_XTD | CAN_IF2ARB2_MSGVAL;
        }
        else
        {
            CAN1_IF2ARB2_R = (id & 0x7FF)<<2 | CAN_IF2ARB2_MSGVAL;
        }

        /* Configure mask */
        if(extended)
        {
            CAN1_IF2MSK1_R = mask & 0xFFFF;
            CAN1_IF2MSK2_R = ((mask >> 16) & 0x1FFF) | (use_mask ? CAN_IF2MSK2_MXTD : 0);
        }
        else
        {
            CAN1_IF2MSK2_R = ((mask & 0x7FF)<<2) | (use_mask ? 0x8000 : 0);
        }

        /* Remote / Data frame */
        if(rtr)
		{
			CAN1_IF2MCTL_R = CAN_IF2MCTL_RMTEN;
		}
        else 
		{
			CAN1_IF2MCTL_R = 0;
		}

        /* Enable the filter */
        
        if(enable) 
		{
			CAN1_IF2MCTL_R |= CAN_IF2MCTL_NEWDAT;
		}
        
        /* Transfer to message object */
        CAN1_IF2CRQ_R = msg_obj;
        while(CAN1_IF2CRQ_R & CAN_IF2CRQ_BUSY);
    }
    else
    {
        return -1; // Invalid CAN module
    }

    return 0; 
}





static uint8_t get_new_data_index(CAN_Option CANx)
{
    uint32_t nwda1 = 0;
    uint32_t nwda2 = 0;
    uint8_t i;

    if (CANx == CAN0)
    {
        nwda1 = CAN0_NWDA1_R;
        nwda2 = CAN0_NWDA2_R;
    }
    else
    {
        nwda1 = CAN1_NWDA1_R;
        nwda2 = CAN1_NWDA2_R;
    }

    for (i = 0; i < 16; i++)
    {
        if ((nwda1 & (1u << i)) != 0)
        {
            return (i + 1); 
        }
    }    
	
    for (i = 0; i < 16; i++)
    {
        if ((nwda2 & (1u << i)) != 0)
        {
            return (i + 17); 
        }
    }

    return 0; 
}



int can_receive(
    CAN_Option CANx,      /* Select CAN module */
    uint32_t  *id,        /* Pointer to received identifier */
    bool      *extended,  /* Pointer to extended ID flag */
    bool      *rtr,       /* Pointer to RTR flag */
    uint8_t   *data,      /* Pointer to received data buffer */
    uint8_t   *len        /* Pointer to received data length */
)
{
    /* Wait IF2 ready */
    if (CANx == CAN0)
    {
        while ((CAN0_IF2CRQ_R & CAN_IF2CRQ_BUSY) != 0)
        {
        }
    }
    else
    {
        while ((CAN1_IF2CRQ_R & CAN_IF2CRQ_BUSY) != 0)
        {
        }
    }

    /* Configure IF2 to read message object */
    if (CANx == CAN0)
    {
        /* Remplacement des masques par les versions IF2 */
        CAN0_IF2CMSK_R = CAN_IF2CMSK_WRNRD | 
						CAN_IF2CMSK_ARB | 
						CAN_IF2CMSK_CONTROL | 
						CAN_IF2CMSK_DATAA | 
						CAN_IF2CMSK_DATAB;
		
        CAN0_IF2CRQ_R = get_new_data_index(CAN0);

        while ((CAN0_IF2CRQ_R & CAN_IF2CRQ_BUSY) != 0)
        {
        }

        uint32_t arb2 = CAN0_IF2ARB2_R;

        *extended = ((arb2 & CAN_IF2ARB2_XTD) != 0) ? true : false;
        *rtr = ((CAN0_IF2MCTL_R & CAN_IF2MCTL_RMTEN) != 0) ? true : false;

        if (*extended)
        {
            *id = ((arb2 & 0x1FFF) << 16) | CAN0_IF2ARB1_R;
        }
        else
        {
            *id = (arb2 >> 2) & 0x7FF;
        }

        *len = CAN0_IF2MCTL_R & 0x0F;

        data[0] = CAN0_IF2DA1_R & 0xFF;
        data[1] = (CAN0_IF2DA1_R >> 8) & 0xFF;
        data[2] = CAN0_IF2DA2_R & 0xFF;
        data[3] = (CAN0_IF2DA2_R >> 8) & 0xFF;
        data[4] = CAN0_IF2DB1_R & 0xFF;
        data[5] = (CAN0_IF2DB1_R >> 8) & 0xFF;
        data[6] = CAN0_IF2DB2_R & 0xFF;
        data[7] = (CAN0_IF2DB2_R >> 8) & 0xFF;
    }
    else
    {
        CAN1_IF2CMSK_R = CAN_IF2CMSK_WRNRD |
                 		CAN_IF2CMSK_ARB | 
						CAN_IF2CMSK_CONTROL | 
						CAN_IF2CMSK_DATAA | 
						CAN_IF2CMSK_DATAB;
		
        CAN1_IF2CRQ_R = get_new_data_index(CAN1);

        while ((CAN1_IF2CRQ_R & CAN_IF2CRQ_BUSY) != 0)
        {
        }

        uint32_t arb2 = CAN1_IF2ARB2_R;

        *extended = ((arb2 & CAN_IF2ARB2_XTD) != 0) ? true : false;
        *rtr = ((CAN1_IF2MCTL_R & CAN_IF2MCTL_RMTEN) != 0) ? true : false;
        /*load id */
        if (*extended)
        {
            *id = ((arb2 & 0x1FFF) << 16) | CAN1_IF2ARB1_R;
        }
        else
        {
            *id = (arb2 >> 2) & 0x7FF;
        }
        /*load  data */
        *len = CAN1_IF2MCTL_R & 0x0F;

        data[0] = CAN1_IF2DA1_R & 0xFF;
        data[1] = (CAN1_IF2DA1_R >> 8) & 0xFF;
		
        data[2] = CAN1_IF2DA2_R & 0xFF;
        data[3] = (CAN1_IF2DA2_R >> 8) & 0xFF;
		
        data[4] = CAN1_IF2DB1_R & 0xFF;
        data[5] = (CAN1_IF2DB1_R >> 8) & 0xFF;
		
        data[6] = CAN1_IF2DB2_R & 0xFF;
        data[7] = (CAN1_IF2DB2_R >> 8) & 0xFF;
    }

    return 0;
}