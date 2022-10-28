// Current_Reg.c

#include <Velocity_Reg.h>

void	Set_param_vel_reg(Velocity_Reg_Def *v)
{
  	v->W_ref 		= 0.0;	
	v->W_fb 		= 0;
	v->KtrW			= 1.0;								// коэффициент передачи по скорости
	v->Err			= 0.0;
	//v->Kp			= 1.0;								// Parameter: Proportional gain
	v->Up			= 0.0;								// Variable: Proportional output
	v->Ui			= 0.0;								// Variable: Integral output
	v->Ud			= 0.0;								// Variable: Derivative output
	v->OutPreSat 	= 0.0; 								// Variable: Pre-saturated output
	v->OutMax		= VELOCITY_MAX;						// Parameter: Maximum output
	v->OutMin		= VELOCITY_MIN;	  					// Parameter: Minimum output
	v->Out			= 0.0;   							// Output: PID output
	v->SatErr		= 0.0;								// Variable: Saturated difference
	//v->Ki			= 0.005;  		  					// Parameter: Integral gain
	v->Kc			= 1.0;		     					// Parameter: Integral correction gain
	//v->Kd			= 0.0; 			    				// Parameter: Derivative gain
	v->Up1			= 0.0;	   							// History: Previous proportional output
	v->KtrPWM		= F_CLK / (F_SWITCH * VELOCITY_MAX);// коэффициент преобразования в ток
	v->UnsOut		= 0;	
}

void	Calc_vel_reg(Velocity_Reg_Def *v)
{
 	// Compute the error
    v->Err = v->W_ref - ((float)v->W_fb * v->KtrW);

    // Compute the proportional output
    v->Up = v->Kp * v->Err;

    // Compute the integral output
    v->Ui = v->Ui + v->Ki * v->Up + v->Kc * v->SatErr;

    // Compute the derivative output
    v->Ud = v->Kd * (v->Up - v->Up1);

    // Compute the pre-saturated output
    v->OutPreSat = v->Up + v->Ui + v->Ud;

    // Saturate the output
    if (v->OutPreSat > v->OutMax)
      v->Out =  v->OutMax;
    else if (v->OutPreSat < v->OutMin)
      v->Out =  v->OutMin;
    else
      v->Out = v->OutPreSat;

    // Compute the saturate difference
    v->SatErr = v->Out - v->OutPreSat;

    // Update the previous proportional output
    v->Up1 = v->Up;
	
	// Calc out
	v->UnsOut = (uint16_t)(v->Out * v->KtrPWM);
}
