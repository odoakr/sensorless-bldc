// ReadAccTab.c

#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include <Adc.h>
#include <util.h>
#include <Gen_event_comm.h>

extern uint16_t N_full[SIZE_ACC_TAB];
extern uint16_t DeltaT[SIZE_ACC_TAB];

uint8_t	ReadAccTab(uint8_t *buf);

uint8_t	ReadAccTab(uint8_t *buf)
{
  	static uint16_t index_tab_read = 0;
  	uint16_t i, cnt_read;
	
  	cnt_read = *(buf + 1);
	
	if (cnt_read <= 125)
	{
		for (i = 1; i < cnt_read + 1; i++)
		{
			if (index_tab_read < SIZE_ACC_TAB)
				PutUint16((buf - 1 + i*2), N_full[index_tab_read++]);
			else if ((index_tab_read >= SIZE_ACC_TAB) && (index_tab_read < SIZE_ACC_TAB * 2))
				PutUint16((buf - 1 + i*2), DeltaT[(index_tab_read++) - SIZE_ACC_TAB]);
			else
			  	PutUint16((buf - 1 + i*2), 0);
			
			if (index_tab_read >= SIZE_ACC_TAB * 2)
				index_tab_read = 0;
		}
		
		i = cnt_read * 2;
	}
	else
	{	
		for (i = 0; i < SIZE_ACC_TAB; i++)
			PutUint16(buf - 1, 0);
		i = SIZE_ACC_TAB * 2;
	}
	
	return i;
}

