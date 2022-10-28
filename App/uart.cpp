// uart.cpp

#include <uart.h>
#include <cassert>
#include <thread.h>


static Uart *uartTable[] = {NULL, NULL, NULL, NULL, NULL};


//////////////////////////////////////////////////////////////////////////////////////


Uart::Uart(Port port)
	: initialized_(false)
	, port_(port)
	, timeOut_(0)
	, txEnablePort_(NULL)
	, txEnablePin_(0)
	, rxQueue_(xQueueCreate(RxQueueSize, sizeof(uint8_t)))
{
	assert(rxQueue_);
	uartTable[port_] = this;
}


Uart::~Uart()
{
}


void Uart::Init(uint32_t baudRate, Format format, uint16_t timeOut)
{
	timeOut_ = timeOut;
	
	USART_DeInit(GetDev());

	USART_InitTypeDef init = {0};
	init.USART_BaudRate = baudRate;
	init.USART_WordLength = GetWordLength(format);
	init.USART_StopBits = GetStopBits(format);
	init.USART_Parity = GetParity(format);
	init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_Init(GetDev(), &init);


	EnableIRQChannel();


	USART_ITConfig(GetDev(), USART_IT_RXNE, ENABLE);

	USART_Cmd(GetDev(), ENABLE);
	
	initialized_ = true;
}


void Uart::AssignTxEnablePin(GPIO_TypeDef *txEnablePort, uint16_t txEnablePin)
{
	txEnablePort_ = txEnablePort;
	txEnablePin_ = txEnablePin;
	
	GPIO_WriteBit(txEnablePort_, txEnablePin_, Bit_RESET);
}


uint16_t Uart::Read(void *buf, uint16_t size)
{
	assert(initialized_);

	uint16_t received = 0;
	uint8_t *ptr = static_cast<uint8_t *>(buf);
	
	while (received < size)
	{
		if (xQueueReceive(rxQueue_, ptr++, timeOut_ / portTICK_RATE_MS) != pdTRUE)
			break;
		
		++received;
	}
	
	return received;
}


void Uart::Write(void *buf, uint16_t size)
{
	assert(initialized_);

	TxBegin();

	uint8_t *ptr = static_cast<uint8_t *>(buf);
	
	while (size--)
	{
		// ждать опустошения регистра передатчика
		while (USART_GetFlagStatus(GetDev(), USART_FLAG_TXE) == RESET)
		{
			Thread::Yield();
		}
		
		USART_SendData(GetDev(), *(ptr++));
	}

	TxEnd();
}


// очистка очереди приёма
void Uart::Purge(void)
{
	assert(initialized_);
	
	uint8_t junk;
	while (xQueueReceive(rxQueue_, &junk, 0) == pdTRUE)
		;
}


void Uart::InterruptHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	
	if (USART_GetITStatus(GetDev(), USART_IT_RXNE) == SET)
	{
		uint8_t data = USART_ReceiveData(GetDev());
		xQueueSendToBackFromISR(rxQueue_, &data, &xHigherPriorityTaskWoken);
	}
	
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}


// разрешить драйвер передатчика
void Uart::TxBegin(void)
{
	if (txEnablePort_ != NULL)
	{
		GPIO_WriteBit(txEnablePort_, txEnablePin_, Bit_SET);
	}
}


// запретить драйвер передатчика
void Uart::TxEnd(void)
{
	if (txEnablePort_ != NULL)
	{
		// ждать завершения передачи
		while (USART_GetFlagStatus(GetDev(), USART_FLAG_TC) == RESET)
		{
			Thread::Yield();
		}
	
		GPIO_WriteBit(txEnablePort_, txEnablePin_, Bit_RESET);
	}
}



void Uart::EnableIRQChannel(void)
{
	uint8_t chn[] = {USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn, UART5_IRQn};

	assert(port_ < sizeof(chn) / sizeof(chn[0]));
	
	NVIC_InitTypeDef nvic = {0};
	nvic.NVIC_IRQChannel = chn[port_];
	nvic.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&nvic);	
}


USART_TypeDef *Uart::GetDev(void)
{
	USART_TypeDef *usart[] = {USART1, USART2, USART3, UART4, UART5};
	assert(port_ < sizeof(usart) / sizeof(usart[0]));
	return usart[port_];
}


uint16_t Uart::GetWordLength(Format format)
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

uint16_t Uart::GetStopBits(Format format)
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
	
uint16_t Uart::GetParity(Format format)
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
extern "C" void USART1_IRQHandler(void)
{
	if (uartTable[0] != NULL)
		uartTable[0]->InterruptHandler();
}

extern "C" void USART2_IRQHandler(void)
{
	if (uartTable[1] != NULL)
		uartTable[1]->InterruptHandler();
}

extern "C" void USART3_IRQHandler(void)
{
	if (uartTable[2] != NULL)
		uartTable[2]->InterruptHandler();
}

extern "C" void UART4_IRQHandler(void)
{
	if (uartTable[3] != NULL)
		uartTable[3]->InterruptHandler();
}

extern "C" void UART5_IRQHandler(void)
{
	if (uartTable[4] != NULL)
		uartTable[4]->InterruptHandler();
}
