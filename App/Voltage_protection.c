// Voltage_protection.c

#include	<Voltage_protection.h>

//params
float Unom = U_NOM;

bool		Check_Voltage(uint16_t U)
{
  	bool	alarm = true;
	float	fU;
	
	fU = (float)U * KTR_UIN;
	if ((fU > U_MAX) || (fU < U_MIN))
	  	alarm = false;

	return	alarm;
}

uint16_t	Correct_Uin(uint16_t Uin, uint16_t Out)
{
  	float	k_Uin;
        float   fOut;
        uint16_t pwm;
	
  	k_Uin = Unom / ((float)Uin * KTR_UIN);
	
        fOut = (float)Out * k_Uin;
        
        if (fOut > (float)(F_CLK / F_SWITCH - 1))
          pwm = 2399;
        else if (fOut < 0.0)
          pwm = 0;
        else
          pwm = (uint16_t)((float)Out * k_Uin);
        
	return pwm;
}