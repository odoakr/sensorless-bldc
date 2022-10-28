// Read_R1R2R3.c

#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include <util.h>

extern float	R1;
extern float	R2;
extern float	R3;

uint8_t	Read_R1R2R3(uint8_t *buf);

uint8_t	Read_R1R2R3(uint8_t *buf)
{
  	uint8_t i = 0;
	
	PutFloat32(buf, R1);
	i = 4;
	PutFloat32(buf + 4, R2);
	i += 4;
	PutFloat32(buf + 8, R3);
	i += 4;
	
	return i;
}