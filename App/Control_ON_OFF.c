// Control_ON_OFF.c

#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include <Commutate_EC.h>

uint8_t	Control_ON_OFF(uint8_t *buf);

extern Commutate_EC_Def	EC;

uint8_t	Control_ON_OFF(uint8_t *buf)
{
	if (EC.Flag_ON_OFF == 0)
	  EC.Flag_ON_OFF = 1;
	else
	  EC.Flag_ON_OFF = 0;
	
	return 0;
}