// Current_protection.c
#include <Current_protection.h>

uint16_t	ADC_measIin[N_MEAS_I];
uint16_t	cnt_meas_I = 0;
// params
float	current_protection = CURRENT_PROT;
float	current_protect_avg = CURRENT_PROT_AVG_HI;

bool	Current_protection(uint16_t Current)
{
  	float fCurrent;
	bool alarm;
	
	fCurrent= (float)Current * KTR_CURRENT;
	
	if (fCurrent >= current_protection)
	  	alarm = true;
	else
	  	alarm = false;
	
	return alarm;
}

bool	Current_protect_avg(float Current)
{
	bool alarm;
	
	if (Current >= current_protect_avg)
	  	alarm = true;
	else
	  	alarm = false;
	
	return alarm;
}

uint16_t	Filtr_Iin(void)
{
  	uint16_t i;
	uint32_t Summ = 0;
	
	for(i = 0; i < N_MEAS_I; i++)
	  	Summ += ADC_measIin[i];
	
	return (uint16_t)(Summ / N_MEAS_I);
	
}

void	Save_Iin(uint16_t	I)
{
  	ADC_measIin[cnt_meas_I++] = I;
	if (cnt_meas_I == N_MEAS_I)
	  	cnt_meas_I = 0;
}