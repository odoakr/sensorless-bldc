// Read_BackEMF.c

#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include <Adc.h>
#include <util.h>

extern uint16_t ADC_InjectedConvertedValueTab[];
extern uint16_t ADC_RegularConvertedValueTab[];
extern uint16_t	ADC_mass[];
uint16_t	index = 0;

uint8_t	Read_BackEMF(uint8_t *buf);

uint8_t	Read_BackEMF(uint8_t *buf)
{	
  uint16_t i, cnt_read;
  
  cnt_read = *(buf + 1);
  
  if (cnt_read <= 125)
  {
		for (i = 0; i < cnt_read; i++)
		{
		  	if (index < LEN_BUFF_INJECT)
				PutUint16((buf + i*2), ADC_InjectedConvertedValueTab[index++]);
			else if ((index >= LEN_BUFF_INJECT) && (index < LEN_BUFF_INJECT * 2))
			  	PutUint16((buf + i*2), ADC_RegularConvertedValueTab[(index++) - LEN_BUFF_INJECT]);
			else
			  	PutUint16((buf + i*2), ADC_mass[(index++) - (LEN_BUFF_INJECT * 2)]);
		}
		
		if (index >= LEN_BUFF_INJECT * 3)
		  	index = 0;
		
		i = i * 2;
  }
  else
  {	
		for (i = 0; i < 125; i++)
			PutUint16(buf, 0);
		i = i * 2;
  }
  
  return i;
}

