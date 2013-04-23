////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  .NET Micro Framework
//  STM32F4 Port
//
//  *** Serial Driver ***
//
//  Copyright ©2011 Oberon microsystems inc.
//  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>

#ifdef STM32F4XX
#include "..\stm32f4xx.h"
#else
#include "..\stm32f2xx.h"
#endif

typedef  USART_TypeDef* ptr_USART_TypeDef;

// IO addresses
static const ptr_USART_TypeDef g_STM32F4_Uart_Ports[] = {USART1, USART2, USART3, UART4, UART5, USART6}; 

// Pins
static const BYTE g_STM32F4_Uart_RxD_Pins[] = STM32F4_UART_RXD_PINS;
static const BYTE g_STM32F4_Uart_TxD_Pins[] = STM32F4_UART_TXD_PINS;
static const BYTE g_STM32F4_Uart_CTS_Pins[] = STM32F4_UART_CTS_PINS;
static const BYTE g_STM32F4_Uart_RTS_Pins[] = STM32F4_UART_RTS_PINS;

UINT16 sr;

void STM32F4_USART_Handle_ERROR_IRQ(int ComPortNum, USART_TypeDef* uart)
{
    INTERRUPT_START;

	if(sr & USART_SR_PE)
	{
		USART_SetEvent(ComPortNum, USART_EVENT_ERROR_RXPARITY );
	}
	else if(sr & USART_SR_FE)
	{
		USART_SetEvent(ComPortNum, USART_EVENT_ERROR_FRAME);
	}
	else if(sr & USART_SR_ORE)
	{
		USART_SetEvent(ComPortNum, USART_EVENT_ERROR_OVERRUN);
	}

    INTERRUPT_END;
}

void STM32F4_USART_Handle_RX_IRQ (int ComPortNum, USART_TypeDef* uart)
{
    INTERRUPT_START;

    char c = (char)(uart->DR); // read RX data
    USART_AddCharToRxBuffer(ComPortNum, c);
    Events_Set(SYSTEM_EVENT_FLAG_COM_IN);

    INTERRUPT_END;
}

void STM32F4_USART_Handle_TX_IRQ (int ComPortNum, USART_TypeDef* uart)
{
    INTERRUPT_START;

    char c;
    if (USART_RemoveCharFromTxBuffer(ComPortNum, c)) {
        uart->DR = c;  // write TX data
    } else {
        uart->CR1 &= ~USART_CR1_TXEIE; // TX int disable
    }
    Events_Set(SYSTEM_EVENT_FLAG_COM_OUT);

    INTERRUPT_END;
}

void STM32F4_USART_Interrupt0 (void* param)
{
	sr = USART1->SR;
	//Checks for Parity error, Framing Error, and Overrun Error
	if ((sr & USART_SR_PE) || (sr & USART_SR_FE) || (sr & USART_SR_ORE))  STM32F4_USART_Handle_ERROR_IRQ(0, USART1);

    if (sr & USART_SR_RXNE) STM32F4_USART_Handle_RX_IRQ(0, USART1);
    if (sr & USART_SR_TXE)  STM32F4_USART_Handle_TX_IRQ(0, USART1);
}

void STM32F4_USART_Interrupt1 (void* param)
{
    sr = USART2->SR;
	//Checks for Parity error, Framing Error, and Overrun Error
	if ((sr & USART_SR_PE) || (sr & USART_SR_FE) || (sr & USART_SR_ORE))
		STM32F4_USART_Handle_ERROR_IRQ(1, USART2);

	if (sr & USART_SR_RXNE)
		STM32F4_USART_Handle_RX_IRQ(1, USART2);
    if (sr & USART_SR_TXE)
		STM32F4_USART_Handle_TX_IRQ(1, USART2);
}

void STM32F4_USART_Interrupt2 (void* param)
{
    sr = USART3->SR;
	//Checks for Parity error, Framing Error, and Overrun Error
	if ((sr & USART_SR_PE) || (sr & USART_SR_FE) || (sr & USART_SR_ORE))  STM32F4_USART_Handle_ERROR_IRQ(2, USART3);

	if (sr & USART_SR_RXNE) STM32F4_USART_Handle_RX_IRQ(2, USART3);
    if (sr & USART_SR_TXE)  STM32F4_USART_Handle_TX_IRQ(2, USART3);
}

void STM32F4_USART_Interrupt3 (void* param)
{
    sr = UART4->SR;
	//Checks for Parity error, Framing Error, and Overrun Error
	if ((sr & USART_SR_PE) || (sr & USART_SR_FE) || (sr & USART_SR_ORE))  STM32F4_USART_Handle_ERROR_IRQ(3, UART4);

	if (sr & USART_SR_RXNE) STM32F4_USART_Handle_RX_IRQ(3, UART4);
    if (sr & USART_SR_TXE)  STM32F4_USART_Handle_TX_IRQ(3, UART4);
}

void STM32F4_USART_Interrupt4 (void* param)
{
    sr = UART5->SR;
	//Checks for Parity error, Framing Error, and Overrun Error
	if ((sr & USART_SR_PE) || (sr & USART_SR_FE) || (sr & USART_SR_ORE))  STM32F4_USART_Handle_ERROR_IRQ(4, UART5);

	if (sr & USART_SR_RXNE) STM32F4_USART_Handle_RX_IRQ(4, UART5);
    if (sr & USART_SR_TXE)  STM32F4_USART_Handle_TX_IRQ(4, UART5);
}

void STM32F4_USART_Interrupt5 (void* param)
{
    sr = USART6->SR;
	//Checks for Parity error, Framing Error, and Overrun Error
	if ((sr & USART_SR_PE) || (sr & USART_SR_FE) || (sr & USART_SR_ORE))  STM32F4_USART_Handle_ERROR_IRQ(5, USART6);

	if (sr & USART_SR_RXNE) STM32F4_USART_Handle_RX_IRQ(5, USART6);
    if (sr & USART_SR_TXE)  STM32F4_USART_Handle_TX_IRQ(5, USART6);
}


BOOL CPU_USART_Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
    if (ComPortNum >= TOTAL_USART_PORT) return FALSE;
    if (Parity >= USART_PARITY_MARK) return FALSE;
    
    GLOBAL_LOCK(irq);
    
    ptr_USART_TypeDef uart = g_STM32F4_Uart_Ports[ComPortNum];
    UINT32 clk;
    
    // enable UART clock
    if (ComPortNum == 5) { // COM6 on APB2
        RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
        clk = SYSTEM_APB2_CLOCK_HZ;
    } else if (ComPortNum) { // COM2-5 on APB1
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN >> 1 << ComPortNum;
        clk = SYSTEM_APB1_CLOCK_HZ;
    } else { // COM1 on APB2
        // USART1 is not used on Cerberus but USART6 is used on Cerbuino.
        // This is just for Cerbuino. 
        //RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
        //clk = SYSTEM_APB2_CLOCK_HZ;
        RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
        clk = SYSTEM_APB2_CLOCK_HZ;
    }
    
    //  baudrate
    UINT16 div = (UINT16)((clk + (BaudRate >> 1)) / BaudRate); // rounded
    uart->BRR = div;
    
    // control
    UINT16 ctrl = USART_CR1_TE | USART_CR1_RE;
    if (DataBits == 9) ctrl |= USART_CR1_M;
    if (Parity) ctrl |= USART_CR1_PCE;
    if (Parity == USART_PARITY_ODD) ctrl |= USART_CR1_PS;
    uart->CR1 = ctrl;
    
    if (StopBits == USART_STOP_BITS_ONE)
		StopBits = 0;
    uart->CR2 = (UINT16)(StopBits << 12);
    //if (DataBits == USART_STOP_BITS_ONE) DataBits = 0;
    //uart->CR2 = (UINT16)(DataBits << 12);

    ctrl = 0;
    if (FlowValue & USART_FLOW_HW_OUT_EN) ctrl |= USART_CR3_CTSE;
    if (FlowValue & USART_FLOW_HW_IN_EN)  ctrl |= USART_CR3_RTSE;
    uart->CR3 = ctrl;

    GPIO_PIN rxPin, txPin, ctsPin, rtsPin;
    CPU_USART_GetPins(ComPortNum, rxPin, txPin, ctsPin, rtsPin);
    UINT32 alternate = 0x72; // AF7 = USART1-3
    //if (ComPortNum >= 3) alternate = 0x82; // AF8 = UART4-6
    if (ComPortNum == 0) alternate = 0x82; // AF8 = UART4-6 -->> Only because USART1 is really USART6
    CPU_GPIO_DisablePin(rxPin, RESISTOR_PULLUP, 0, (GPIO_ALT_MODE)alternate);
    CPU_GPIO_DisablePin(txPin, RESISTOR_DISABLED, 1, (GPIO_ALT_MODE)alternate);
    if (FlowValue & USART_FLOW_HW_OUT_EN)
        if (ctsPin == GPIO_PIN_NONE) return FALSE;
        CPU_GPIO_DisablePin(ctsPin, RESISTOR_DISABLED, 0, (GPIO_ALT_MODE)alternate);
    if (FlowValue & USART_FLOW_HW_IN_EN)
        if (rtsPin == GPIO_PIN_NONE) return FALSE;
        CPU_GPIO_DisablePin(rtsPin, RESISTOR_DISABLED, 1, (GPIO_ALT_MODE)alternate);

    CPU_USART_ProtectPins(ComPortNum, FALSE);
    
    switch (ComPortNum) {
    //case 0: CPU_INTC_ActivateInterrupt(USART1_IRQn, STM32F4_USART_Interrupt0, 0); break;
    case 0: CPU_INTC_ActivateInterrupt(USART6_IRQn, STM32F4_USART_Interrupt0, 0); break;
    case 1: CPU_INTC_ActivateInterrupt(USART2_IRQn, STM32F4_USART_Interrupt1, 0); break;
    case 2: CPU_INTC_ActivateInterrupt(USART3_IRQn, STM32F4_USART_Interrupt2, 0); break;
    case 3: CPU_INTC_ActivateInterrupt(UART4_IRQn, STM32F4_USART_Interrupt3, 0); break;
    case 4: CPU_INTC_ActivateInterrupt(UART5_IRQn, STM32F4_USART_Interrupt4, 0); break;
    case 5: CPU_INTC_ActivateInterrupt(USART6_IRQn, STM32F4_USART_Interrupt5, 0);
    }

    uart->CR1 |= USART_CR1_UE; // start uart


    return TRUE;
}

BOOL CPU_USART_Uninitialize( int ComPortNum )
{
    GLOBAL_LOCK(irq);
    
    g_STM32F4_Uart_Ports[ComPortNum]->CR1 = 0; // stop uart

    switch (ComPortNum) {
    case 0: CPU_INTC_DeactivateInterrupt(USART1_IRQn); break;
    case 1: CPU_INTC_DeactivateInterrupt(USART2_IRQn); break;
    case 2: CPU_INTC_DeactivateInterrupt(USART3_IRQn); break;
    case 3: CPU_INTC_DeactivateInterrupt(UART4_IRQn); break;
    case 4: CPU_INTC_DeactivateInterrupt(UART5_IRQn); break;
    case 5: CPU_INTC_DeactivateInterrupt(USART6_IRQn);
    }

    CPU_USART_ProtectPins(ComPortNum, TRUE);
    
    // disable UART clock
    if (ComPortNum == 5) { // COM6 on APB2
        RCC->APB2ENR &= ~RCC_APB2ENR_USART6EN;
    } else if (ComPortNum) { // COM2-5 on APB1
        RCC->APB1ENR &= ~(RCC_APB1ENR_USART2EN >> 1 << ComPortNum);
    } else { // COM1 on APB2
        RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
    }

    return TRUE;
}

BOOL CPU_USART_TxBufferEmpty( int ComPortNum )
{
    if (g_STM32F4_Uart_Ports[ComPortNum]->SR & USART_SR_TXE) return TRUE;
    return FALSE;
}

BOOL CPU_USART_TxShiftRegisterEmpty( int ComPortNum )
{
    if (g_STM32F4_Uart_Ports[ComPortNum]->SR & USART_SR_TC) return TRUE;
    return FALSE;
}

void CPU_USART_WriteCharToTxBuffer( int ComPortNum, UINT8 c )
{
#ifdef DEBUG
    ASSERT(CPU_USART_TxBufferEmpty(ComPortNum));
#endif
    g_STM32F4_Uart_Ports[ComPortNum]->DR = c;
}

void CPU_USART_TxBufferEmptyInterruptEnable( int ComPortNum, BOOL Enable )
{
    ptr_USART_TypeDef uart = g_STM32F4_Uart_Ports[ComPortNum];
    if (Enable) {
        uart->CR1 |= USART_CR1_TXEIE;  // tx int enable
    } else {
        uart->CR1 &= ~USART_CR1_TXEIE; // tx int disable
    }
}

BOOL CPU_USART_TxBufferEmptyInterruptState( int ComPortNum )
{
    if (g_STM32F4_Uart_Ports[ComPortNum]->CR1 & USART_CR1_TXEIE) return TRUE;
    return FALSE;
}

void CPU_USART_RxBufferFullInterruptEnable( int ComPortNum, BOOL Enable )
{
    ptr_USART_TypeDef uart = g_STM32F4_Uart_Ports[ComPortNum];
    if (Enable) {
        uart->CR1 |= USART_CR1_RXNEIE;  // rx int enable
    } else {
        uart->CR1 &= ~USART_CR1_RXNEIE; // rx int disable
    }
}

BOOL CPU_USART_RxBufferFullInterruptState( int ComPortNum )
{
    if (g_STM32F4_Uart_Ports[ComPortNum]->CR1 & USART_CR1_RXNEIE) return TRUE;
    return FALSE;
}

BOOL CPU_USART_TxHandshakeEnabledState( int ComPortNum )
{
    // The state of the CTS input only matters if Flow Control is enabled
    if (g_STM32F4_Uart_Ports[ComPortNum]->CR3 & USART_CR3_CTSE)
    {
        GPIO_PIN pin = 0x0B;
        if (ComPortNum == 1) pin = 0x00;
        else if (ComPortNum == 2) pin = 0x1D;
        return !CPU_GPIO_GetPinState(pin); // CTS active
    }
    return TRUE; // If this handshake input is not being used, it is assumed to be good
}

void CPU_USART_ProtectPins( int ComPortNum, BOOL On )  // idempotent
{
    if (On) {
        CPU_USART_RxBufferFullInterruptEnable(ComPortNum, FALSE);
        CPU_USART_TxBufferEmptyInterruptEnable(ComPortNum, FALSE);
    } else {
        CPU_USART_TxBufferEmptyInterruptEnable(ComPortNum, TRUE);
        CPU_USART_RxBufferFullInterruptEnable(ComPortNum, TRUE);
    }
}

UINT32 CPU_USART_PortsCount()
{
    return TOTAL_USART_PORT;
}

void CPU_USART_GetPins( int ComPortNum, GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin )
{
    rxPin = txPin = ctsPin = rtsPin = GPIO_PIN_NONE;
    if ((UINT32)ComPortNum >= ARRAYSIZE_CONST_EXPR(g_STM32F4_Uart_RxD_Pins)) return;
    rxPin = g_STM32F4_Uart_RxD_Pins[ComPortNum];
    txPin = g_STM32F4_Uart_TxD_Pins[ComPortNum];
    if ((UINT32)ComPortNum >= ARRAYSIZE_CONST_EXPR(g_STM32F4_Uart_CTS_Pins)) return; // no CTS/RTS
    ctsPin = g_STM32F4_Uart_CTS_Pins[ComPortNum];
    rtsPin = g_STM32F4_Uart_RTS_Pins[ComPortNum];
}

void CPU_USART_GetBaudrateBoundary( int ComPortNum, UINT32 & maxBaudrateHz, UINT32 & minBaudrateHz )
{
    UINT32 clk = SYSTEM_APB2_CLOCK_HZ;
    if (ComPortNum && ComPortNum < 5) clk = SYSTEM_APB1_CLOCK_HZ;
    maxBaudrateHz = clk >> 4;
    minBaudrateHz = clk >> 16;
}

BOOL CPU_USART_SupportNonStandardBaudRate( int ComPortNum )
{
    return TRUE;
}

BOOL CPU_USART_IsBaudrateSupported( int ComPortNum, UINT32& BaudrateHz )
{
    UINT32 max = SYSTEM_APB2_CLOCK_HZ >> 4;
    if (ComPortNum && ComPortNum < 5) max = SYSTEM_APB1_CLOCK_HZ >> 4;
    if (BaudrateHz <= max) return TRUE;
    BaudrateHz = max;
    return FALSE;
}


