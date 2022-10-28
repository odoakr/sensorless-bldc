// Calc_velocity.c
#include	<Calc_velocity.h>

#define		LEN_MASS_VEL	8
uint32_t	P_ZC_mass[LEN_MASS_VEL] = {0};
uint32_t	cnt_P_ZC_mass = 0;
uint32_t	summ_P_ZC_mass = 0;

uint16_t	Calc_velocity(uint32_t Period)
{
  	uint32_t W, T, i;
	
	T = (uint32_t)Period;
	
	P_ZC_mass[cnt_P_ZC_mass++] = T;
	if (cnt_P_ZC_mass == LEN_MASS_VEL)
	  	cnt_P_ZC_mass = 0;
				
	summ_P_ZC_mass = 0;
	for (i = 0; i < LEN_MASS_VEL; i++)
	  	summ_P_ZC_mass += P_ZC_mass[i];
	T = summ_P_ZC_mass >> 3; // check LEN_MASS_VEL!!!
	
	W = (uint32_t)24000000 / T;
				
	return	(uint16_t)W;
}

void Init_calc_velocity(void)
{
  	int8_t	i;
	
	summ_P_ZC_mass = 0;
	for(i = 0; i < LEN_MASS_VEL; i++)
		P_ZC_mass[i] = 0;
}