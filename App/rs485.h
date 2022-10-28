// rs485.h

#include <stdint.h>
#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <assert.h>

typedef enum {eCOM1 = 0, eCOM2, eCOM3, eCOM4, eCOM5} Port;
typedef enum {e8N1, e8O1, e8E1, e8N2, e8O2, e8E2} Format;

void InitRS(USART_TypeDef* USARTx, uint32_t baudRate, Format format, uint16_t timeOut);
void AssignTxEnablePin(GPIO_TypeDef *txEnablePort, uint16_t txEnablePin);
uint16_t Read(uint8_t *buf, uint16_t size);
void Write(USART_TypeDef* USARTx, uint8_t *buf, uint16_t size);
void Purge(void);
void InterruptHandler(USART_TypeDef* USARTx);

void TxBegin(void);
void TxEnd(USART_TypeDef* USARTx);
void EnableIRQChannel(USART_TypeDef* USARTx);
//USART_TypeDef *GetDev(void);
uint16_t GetWordLength(Format format);
uint16_t GetStopBits(Format format);
uint16_t GetParity(Format format);

#define SLAVE_ADDR_RS485	0x70