// Control_velocity.c

#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include <Adc.h>
#include <Velocity_Reg.h>
#include <Commutate_EC.h>
#include <math.h>
#include <Drive_param.h>

uint8_t	Control_velocityRS(uint8_t *buf);
float W_rs485 = 0;

uint16_t n_pair_polus = N_PAIR_POLUS;

extern Velocity_Reg_Def	Vel_reg;
extern Commutate_EC_Def	EC;

uint8_t	Control_velocityRS(uint8_t *buf)
{
  	int16_t i, velocity_RS;
	
	velocity_RS = *(buf + 1);
	velocity_RS += *(buf + 2) << 8;
	
	if ((velocity_RS > VELOCITY_MIN_RPM / 2) && (velocity_RS < VELOCITY_MIN_RPM))
	{
	  	EC.Flag_ON_OFF = 1;
		velocity_RS = VELOCITY_MIN_RPM;
	}
	if ((velocity_RS < (-1) * VELOCITY_MIN_RPM / 2) && (velocity_RS > (-1) * VELOCITY_MIN_RPM))
	{
	  	EC.Flag_ON_OFF = 1;
	  	velocity_RS = (-1) * VELOCITY_MIN_RPM;
	}
	
	if ((velocity_RS >= VELOCITY_MIN_RPM) || (velocity_RS <= (-1) * VELOCITY_MIN_RPM))
	  	EC.Flag_ON_OFF = 1;
	
	if ((velocity_RS >= 0) && (velocity_RS <= VELOCITY_MIN_RPM / 2))
	{
	  	EC.Flag_ON_OFF = 0;
		velocity_RS = 0;
	}
	
	if ((velocity_RS >= (-1) * VELOCITY_MIN_RPM / 2) && (velocity_RS <= 0))
	{
	  	EC.Flag_ON_OFF = 0;
	  	velocity_RS = 0;
	}
		
	W_rs485 = (float)n_pair_polus * 6.0 * (float)velocity_RS / 60.0;
		  
	i = 0;
	
	return i;
}
