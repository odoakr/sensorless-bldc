// rs485.c

#include <rs485.h>

static const uint16_t RxQueueSize = 16;
xQueueHandle rxQueue_;
uint16_t timeOut_ = 100;

void InitRS(USART_TypeDef* USARTx, uint32_t baudRate, Format format, uint16_t timeOut)
{
  	timeOut_ = timeOut;
	
	USART_DeInit(USARTx);

	USART_InitTypeDef init = {0};
	init.USART_BaudRate = baudRate;
	init.USART_WordLength = GetWordLength(format);
	init.USART_StopBits = GetStopBits(format);
	init.USART_Parity = GetParity(format);
	init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_Init(USARTx, &init);
	
	rxQueue_ = xQueueCreate(RxQueueSize, sizeof(uint8_t));

	EnableIRQChannel(USARTx);

	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

	USART_Cmd(USARTx, ENABLE);
}

void AssignTxEnablePin(GPIO_TypeDef *txEnablePort, uint16_t txEnablePin)
{
  	uint16_t txEnablePin_ = txEnablePin;
	GPIO_TypeDef *txEnablePort_ = txEnablePort;

	GPIO_WriteBit(txEnablePort_, txEnablePin_, Bit_RESET);
}

uint16_t Read(uint8_t *buf, uint16_t size)
{
//	assert(initialized_);

	uint16_t received = 0;
	uint8_t *ptr = buf;
	
	while (received < size)
	{
		if (xQueueReceive(rxQueue_, ptr++, timeOut_ / portTICK_RATE_MS) != pdTRUE)
			break;
		
		++received;
	}
	
	return received;
}

void Write(USART_TypeDef* USARTx, uint8_t *buf, uint16_t size)
{
//	assert(initialized_);

	TxBegin();

	uint8_t *ptr = buf;
	
	while (size--)
	{
		// ждать опустошения регистра передатчика
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
		{
			  taskYIELD();
		}
		
		USART_SendData(USARTx, *(ptr++));
	}

	TxEnd(USARTx);
}

// очистка очереди приёма
void Purge(void)
{
//	assert(initialized_);
	
	uint8_t junk;
	while (xQueueReceive(rxQueue_, &junk, 0) == pdTRUE)
		;
}

void InterruptHandler(USART_TypeDef* USARTx)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
	if (USART_GetITStatus(USARTx, USART_IT_RXNE) == SET)
	{
		uint8_t data = USART_ReceiveData(USARTx);
		//if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_4) == 0)  // without driver rs485
		  xQueueSendToBackFromISR(rxQueue_, &data, &xHigherPriorityTaskWoken);
	}
	
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

// разрешить драйвер передатчика
void TxBegin(void)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET);
}

// запретить драйвер передатчика
void TxEnd(USART_TypeDef* USARTx)
{
	// ждать завершения передачи
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
	{
	  	taskYIELD();
	}
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET);

}

void EnableIRQChannel(USART_TypeDef* USARTx)
{
  	uint8_t chn;

	assert((USARTx == USART1) ||(USARTx == USART2) || (USARTx == USART3) || (USARTx == UART4) || (USARTx == UART5));
	
	if (USARTx == USART1)
	  chn = USART1_IRQn;
	else if (USARTx == USART2)
	  chn = USART2_IRQn;
	/*
	else if (USARTx == USART3)
	  chn = USART3_IRQn;
	else if (USARTx == UART4)
	  chn = UART4_IRQn;
	else
	{
	  	if (USARTx == UART5)
		  chn = UART5_IRQn;
	}
	*/ 
	NVIC_InitTypeDef nvic = {0};
	nvic.NVIC_IRQChannel = chn;
	nvic.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&nvic);	
}

/*
USART_TypeDef *GetDev(void)
{
	USART_TypeDef *usart[] = {USART1, USART2, USART3, UART4, UART5};
	assert(port_ < sizeof(usart) / sizeof(usart[0]));
	return usart[port_];
}
*/

uint16_t GetWordLength(Format format)
{
	switch (format)
	{
	case e8N1: case e8O1: case e8E1: case e8N2: case e8O2: case e8E2:
		return USART_WordLength_8b;
	default:
		assert(!"Unknown format");
	}

	return USART_WordLength_8b;
}

uint16_t GetStopBits(Format format)
{
	switch (format)
	{
	case e8N1: case e8O1: case e8E1:
		return USART_StopBits_1;
	case e8N2: case e8O2: case e8E2:
		return USART_StopBits_2;
	default:
		assert(!"Unknown format");
	}

	return USART_StopBits_1;
}
	
uint16_t GetParity(Format format)
{
	switch (format)
	{
	case e8N1: case e8N2:
		return USART_Parity_No;
	case e8O1: case e8O2:
		return USART_Parity_Odd;
	case e8E1: case e8E2:
		return USART_Parity_Even;
	default:
		assert(!"Unknown format");
	}

	return USART_Parity_No;
}

/////////////////////////////////////////////////////////////////////////////////

// обработчики прерываний
extern void USART1_IRQHandler(void)
{
	InterruptHandler(USART1);
}

