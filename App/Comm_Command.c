// Comm_Command.c
#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include <Adc.h>
#include <Commutate_EC.h>
#include <Velocity_Reg.h>
#include <util.h>
#include <Drive_param.h>

uint8_t Comm_Command(uint8_t *buf);
extern uint16_t ADCConvertedValue[];
extern float Iin_avg;
extern uint16_t	ADC_Iin;
extern Commutate_EC_Def	EC;
extern Velocity_Reg_Def	Vel_reg;
extern uint16_t	State, Period_test, cnt_step;
extern uint32_t	T32;
extern uint16_t n_pair_polus;
extern uint16_t	tempPWM;

uint8_t Comm_Command(uint8_t *buf)
{	
	uint8_t i = 0;
	float fCurrent;
	float fUin;
	float n;
	
	fUin = (float)ADCConvertedValue[U_IN] * KTR_UIN;
	PutFloat32(buf + i, fUin);
	i = 4;
	
	fCurrent = Iin_avg; 
	PutFloat32(buf + i, fCurrent);
	i += 4;
	
	PutUint16(buf + i, tempPWM);
	i += 2;
	
	PutUint32(buf + i, EC.Status);
	i += 4;
	
	PutUint16(buf + i, ADC_Iin);
	i += 2;
	
	n = ((float)EC.Wdrive * 60.0) / (6.0 * (float)n_pair_polus);
	PutUint16(buf + i, (uint16_t)n);
	i += 2;
	
	PutUint16(buf + i, State);
	i += 2;
	
	PutUint16(buf + i, EC.cnt_miss_zc);
	i += 2;
	
	return i;
}
