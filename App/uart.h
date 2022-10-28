// uart.h

#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <queue.h>

class Uart
{
public:
	enum Port {eCOM1 = 0, eCOM2, eCOM3, eCOM4, eCOM5};
	enum Format {e8N1, e8O1, e8E1, e8N2, e8O2, e8E2};

	Uart(Port port);
	~Uart();
	
	void Init(uint32_t baudRate, Format format, uint16_t timeOut);
	void AssignTxEnablePin(GPIO_TypeDef *txEnablePort, uint16_t txEnablePin);
	uint16_t Read(void *buf, uint16_t size);
	void Write(void *buf, uint16_t size);
	void Purge(void);
	void InterruptHandler(void);
	
protected:
	Uart(void);				// default ctor disabled
	Uart(const Uart &);		// copy ctor disabled

	void TxBegin(void);
	void TxEnd(void);

	void EnableIRQChannel(void);
	
	USART_TypeDef *GetDev(void);
	uint16_t GetWordLength(Format format);
	uint16_t GetStopBits(Format format);
	uint16_t GetParity(Format format);
	
private:	
	bool initialized_;
	Port port_;
	uint16_t timeOut_;
	
	GPIO_TypeDef *txEnablePort_;
	uint16_t txEnablePin_;

	static const uint16_t RxQueueSize = 16;
	xQueueHandle rxQueue_;
};


#endif


