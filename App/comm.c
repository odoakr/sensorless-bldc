// com.c

#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include <rs485.h>
#include <stdbool.h>

#define BUFSIZE_REC 0xFE
#define BUFSIZE 0xFE

uint8_t buf_tr[BUFSIZE_REC];
// params
float	slave_addr = SLAVE_ADDR_RS485;

extern uint8_t Comm_Command(uint8_t *buf);
extern uint8_t Read_BackEMF(uint8_t *buf);
extern uint8_t Control_velocityRS(uint8_t *buf);
extern uint8_t Control_ON_OFF(uint8_t *buf);
extern uint8_t Set_param(uint8_t *buf);
extern uint8_t Read_param(uint8_t *buf);
extern uint8_t Read_R1R2R3(uint8_t *buf);
extern uint8_t ReadAccTab(uint8_t *buf);
extern uint8_t WriteAccTab(uint8_t *buf);
extern uint8_t Req_Read_DefaultParam(uint8_t *buf);
extern uint8_t Set_Req_Write_Param(uint8_t *buf);

typedef struct tag_PACKET_HANDLER {
	uint8_t id;
	uint8_t (*func)(uint8_t *buf);
} PACKET_HANDLER;


// таблица обработчиков запросов
static const PACKET_HANDLER handlerTable[] = {
	 {0x02, Comm_Command}
	,{0x04, Read_BackEMF}
	,{0x06, Control_ON_OFF}
	,{0x08, Read_R1R2R3}
	,{0x10, ReadAccTab}
	,{0x12, Control_velocityRS}
	,{0x14, WriteAccTab}
	,{0x16, Req_Read_DefaultParam}
	,{0x18, Set_Req_Write_Param}
	,{0x30, Read_param}
	,{0x32, Set_param}
	,{0xFF, NULL}
};

static uint8_t Crc8(uint8_t *pcBlock, uint8_t len);
static bool ParsePacket(uint8_t *buf);

void Comm_Task(void *param)
{
	uint8_t buf_rec[BUFSIZE], i;
	uint16_t cnt = 0;
	
	static uint16_t datasize = 0;
	
	// init rs485
  	InitRS(USART1, 115200, e8N1, 100);
	
	while (1)
	{
		if (Read(buf_rec + datasize, 1) == 1)
		{
		  	++datasize;
			
			if (datasize == 1)	// проверка адреса
			{
				  if (buf_rec[0] == (uint8_t)slave_addr)
				  {
					buf_rec[1] = BUFSIZE;	// временно максимальная длина пакета 
					memset(buf_rec + 2, 0, BUFSIZE - 2);
				  }
				  else
					datasize = 0;
			}
			else if (datasize == 2)	// прием id
			{
			  	datasize = datasize;
			}
			else if (datasize == 3) // прием длины пакета
			{
			  if ((buf_rec[2] > BUFSIZE) || (buf_rec[2] < 4))
			  {
			  	// неверная длина пакета
				datasize = 0;
			  }
			}
			else if (datasize >= buf_rec[2])
			{	
			  	uint16_t n = buf_rec[2] - 1;

				// контрольная сумма в порядке?
				if (Crc8(buf_rec, n) == buf_rec[n])
				{
					// разбор пакета
				 	memcpy(buf_tr, buf_rec, buf_rec[2]);
					if (ParsePacket(buf_tr))
					  Write(USART1, buf_tr, buf_tr[2]);
				
					// удалить обработанный пакет из буфера
					datasize = 0;
				}
				else
				{
				  	// ответ при несовпадении КС
				  	for (i = 0; i < 1; i++)
		  				buf_tr[i] = cnt++;
		  			Write(USART1, buf_tr, 1);
					// контрольная сумма неверна
					datasize = 0;
				}
			}
		}
		else
		{	
		  	// таймаут приёма
			Purge();
			datasize = 0;
		}

		taskYIELD();
	}
}

/*
  Name  : CRC-8
  Poly  : 0x31     x^8 + x^5 + x^4 + 1
  Init  : 0xFF
  Revert: false
  XorOut: 0x00
  Check : 0xF7 ("123456789")
  MaxLen: 15 байт(127 бит) - обнаружение
    одинарных, двойных, тройных и всех нечетных ошибок
*/
static uint8_t Crc8(uint8_t *pcBlock, uint8_t len)
{
	unsigned char crc = 0xFF;
	unsigned char i;

	while (len--)
	{
		crc ^= *pcBlock++;

		for (i = 0; i < 8; ++i)
			crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : (crc << 1);
	}

	return crc & 0xFF;
}

// вызов соответствующего обработчика в соответствии с идентификатором принятого блока
static bool ParsePacket(uint8_t *buf)
{
	uint8_t len = 0;
	uint8_t i = 0;

	while ((handlerTable[i].id != buf[1]) && (handlerTable[i].func != NULL))
		++i;

	if (handlerTable[i].func == NULL)
		return FALSE;

	len = handlerTable[i].func(buf + 2);	// формирование тела ответа

	buf[0] = (uint8_t)slave_addr;	// идентификатор ответа равен SLAVE_ADDR_RS485
	buf[1]++;	// id
	buf[2] = len + 4;
	buf[len + 3] = Crc8(buf, len + 3);

	return TRUE;
}



