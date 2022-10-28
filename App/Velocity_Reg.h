// Velocity_Reg.h
#include <stdint.h>
#include <Drive_param.h>

typedef struct
{
	float		W_ref;	
	uint16_t	W_fb;
	float		KtrW;			// коэффициент передачи по току
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
	float		KtrPWM;			// коэффициент преобразования в pwm
	uint16_t	UnsOut;			// PID output
}Velocity_Reg_Def;

void	Set_param_vel_reg(Velocity_Reg_Def *v);
void	Calc_vel_reg(Velocity_Reg_Def *Curr_reg);