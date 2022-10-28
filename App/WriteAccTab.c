// WriteAccTab.c

#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include <Adc.h>
#include <util.h>
#include <Gen_event_comm.h>
#include <stdbool.h>

extern uint16_t N_full[SIZE_ACC_TAB];
extern uint16_t DeltaT[SIZE_ACC_TAB];

extern bool 	flash_update;

uint16_t index_tab = 0;
uint8_t	WriteAccTab(uint8_t *buf);

uint8_t	WriteAccTab(uint8_t *buf)
{
  	static uint16_t index_tab_write = 0;
  	uint16_t i, cnt_write;
	
	cnt_write = *(buf + 1);
	if (cnt_write < SIZE_ACC_TAB * 2)
	{
		for (i = 1; i < cnt_write + 1; i++)
		{
			if (index_tab_write < SIZE_ACC_TAB)
				N_full[index_tab_write++] = GetUint16(buf + 2*i);
			else if ((index_tab_write >= SIZE_ACC_TAB) && (index_tab_write < SIZE_ACC_TAB * 2))
				DeltaT[(index_tab_write++) - SIZE_ACC_TAB] = GetUint16(buf + 2*i);
			else
			  	index_tab_write = index_tab_write;
			
			if (index_tab_write >= SIZE_ACC_TAB * 2)
			{
			  	flash_update = true;
				index_tab_write = 0;
			}
		}
	}
	
	i = 2;
	
	return i;
}