// Current_Reg.h
#include <stdint.h>
#include <Drive_param.h>

typedef struct
{
	float		I_ref;	
	uint16_t	I_fb;
	float		KtrI;			// коэффициент передачи по току
	float		Err;
	float		Kp;				// Parameter: Proportional gain
	float		Up;				// Variable: Proportional output
	float		Ui;				// Variable: Integral output
	float		Ud;				// Variable: Derivative output
	float		OutPreSat; 		// Variable: Pre-saturated output
	float		OutMax;		    // Parameter: Maximum output
	float		OutMin;	    	// Parameter: Minimum output
	float		Out;   			// Output: PID output
	float		SatErr;			// Variable: Saturated difference
	float		Ki;			    // Parameter: Integral gain
	float		Kc;		     	// Parameter: Integral correction gain
	float		Kd; 		    // Parameter: Derivative gain
	float		Up1;		   	// History: Previous proportional output
	float		KtrPWM;			// коэффициент преобразования в длительность ШИМ
	uint16_t	UnsOut;			// PID output 
}Current_Reg_Def;

void	Set_param_curr_reg(Current_Reg_Def *v);
void	Calc_current_reg(Current_Reg_Def *Curr_reg);