// Control_EC.c

#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include <Commutate_EC.h>
#include <Adc.h>
#include <Current_Reg.h>
#include <Current_protection.h>
#include <Velocity_Reg.h>
#include <Detect_zero_cross.h>
#include <Gen_event_comm.h>
#include <stdbool.h>
#include <Drive_param.h>
#include <stdlib.h>
#include <Calc_velocity.h>
#include <Mes_time.h>
#include <math.h>
#include <Voltage_protection.h>
#include <sound.h>

Commutate_EC_Def	EC;
Current_Reg_Def		Curr_reg;
Velocity_Reg_Def	Vel_reg;
Mes_Time_Def		MessTime;

uint16_t	Emf, Uin;

extern uint16_t ADCConvertedValue[];

uint16_t	State	= STATE_STOP;
uint16_t	cnt_ON_reg_vel = 0;
uint16_t 	Cnt_wait = 0;
portTickType xLastWakeTime;
portTickType xFrequency = 1;
//uint16_t	cnt_delay = 0;

uint16_t	duty_r12, duty_r13, duty_r23;
float	R1, R2, R3;

uint16_t	tempPWM = 0;
uint16_t	sound_duty = 0;

// params
uint16_t	level_pwm_start = LEVEL_PWM_START;
uint16_t	cnt_on_reg_speed = CNT_ON_REG_SPEED;
uint16_t	level_correct_speed = LEVEL_CORRECT_SPEED;
float		step_add_velocity = STEP_ADD_VELOCITY;
float		step_dec_velocity = STEP_DEC_VELOCITY;
uint16_t	level_detect_stop = LEVEL_DETECT_STOP;
uint16_t	drive_mode = DIRECT_CONTROL;

extern uint16_t ADC_InjectedConvertedValueTab[];
extern uint16_t ADC_RegularConvertedValueTab[];
uint16_t	ADC_mass[500];

extern float W_rs485;
float	Wref_RS = 0.0;

uint16_t	ADC_Iin = 0;
float		Iin_avg = 0.0;
uint16_t	cnt_I_limit = 0;

extern	bool 	flash_update;
extern	bool 	write_flash_params(void);
extern	bool	Read_flash_params(void);
extern	void 	InterruptHandlerTIMCC(TIM_TypeDef* TIMx, Commutate_EC_Def* EC);
extern	void	Set_def_params(void);

void		Save_temp_data(void);
void		Calc_R1R2R3(uint16_t duty12, uint16_t duty13, uint16_t duty23);
uint16_t	Meas_I_proc(void);
uint16_t	Detect_Alarm_EC(float R1, float R2, float R3, uint16_t duty12, uint16_t duty13, uint16_t duty23);
void		Measure_Iavg_Uin(void);
void		Stop_drive(void);
void		DetectStop(void);

void	State_Stop(void);
void	State_prepos(void);
void	State_accerate(void);
void	State_rotate_fb(void);
void	State_reg_velocity(void);
void	State_direct_control(void);
void	State_meas_r12(void);
void	State_meas_r13(void);
void	State_meas_r23(void);
void	State_sound(void);
void	State_stop_alarm(void);

void	Calc_R1R2R3(uint16_t duty12, uint16_t duty13, uint16_t duty23)
{
  	float	duty_f12, duty_f13, duty_f23;
	float	R12, R13, R23;
	
	duty_f12 = (float)duty12 / (float)(F_CLK / F_SWITCH - 1);
	duty_f13 = (float)duty13 / (float)(F_CLK / F_SWITCH - 1);
	duty_f23 = (float)duty23 / (float)(F_CLK / F_SWITCH - 1);
	
	R12 = ((float)Uin * KTR_UIN * duty_f12) / I_MEAS_R - R_CURRENT; 
	R13 = ((float)Uin * KTR_UIN * duty_f13) / I_MEAS_R - R_CURRENT;
	R23 = ((float)Uin * KTR_UIN * duty_f23) / I_MEAS_R - R_CURRENT;
	
	R1 = (R12 + R13 - R23) / 2.0;
	R2 = (R12 + R23 - R13) / 2.0;
	R3 = (R13 + R23 - R12) / 2.0;
}

uint16_t	Detect_Alarm_EC(float R1, float R2, float R3, uint16_t duty12, uint16_t duty13, uint16_t duty23)
{
  	float	dR1, dR2, dR3, Ravg;
	
  	if ((duty12 < 20) || (duty13 < 20) || (duty23 < 20))
	{
	  	EC.Status |= BIT_SHORT;
	  	return ALARM_SHORT;
	}
	
	if ((duty12 >= /*(F_CLK / F_SWITCH - 1)*/350) || (duty13 >= /*(F_CLK / F_SWITCH - 1)*/350) || (duty23 >= /*(F_CLK / F_SWITCH - 1)*/350))
	{
	  	EC.Status |= BIT_OPEN;
	  	return ALARM_OPEN;
	}
	
	Ravg = (R1 + R2 + R3) / 3.0;
	dR1 = (R1 - Ravg) / Ravg * 100.0;
	dR2 = (R2 - Ravg) / Ravg * 100.0;
	dR3 = (R3 - Ravg) / Ravg * 100.0;
	if ((dR1 > 10.0) || (dR2 > 10.0) || (dR3 > 10.0) || (dR1 < -10.0) || (dR2 < -10.0) || (dR3 < -10.0))
	{
	  	EC.Status |= BIT_DISBALANCE;
		return ALARM_DISBALANCE;
	}
	
	return	ALARM_NO;
}

void	Save_temp_data(void)
{
  	static uint16_t cnt_buff = 0;
	
	ADC_InjectedConvertedValueTab[cnt_buff] = ADC_GetInjectedConversionValue(ADC2, ADC_InjectedChannel_1);//ADCConvertedValue[U_BACKEMF];//EC.Event; 
	ADC_RegularConvertedValueTab[cnt_buff] = ADCConvertedValue[U_IN] >> 1;//tempPWM >> 1;
	ADC_mass[cnt_buff] = ADC_Iin; 
	cnt_buff++;
	if (cnt_buff == LEN_BUFF_INJECT)
	  cnt_buff = 0;
}

void ControlEC_Task(void *param)
{
  	uint16_t CCR_Val = 0;
	uint16_t PeriodPWM = F_CLK / F_SWITCH - 1; 
	
	// переменные для управления двигателем
	EC.Dir = STOP_ROTATE;
	EC.N_phase = STOP;
	EC.Event = NOTHING;
	EC.Flag_ON_OFF = 0;
	
	EC.Flag_meas_R = 1;
		
	Set_param_vel_reg(&Vel_reg);
	
	Set_def_params();
	flash_update = Read_flash_params();
	
	Adc_init();
	
	Init_gpioEC();
	Init_Control_EC(TIM1, PeriodPWM, CCR_Val, 0);
        
	// init GenEvent
	InitGenEventComm(TIM3);
	
	// init Time ZC mess
	InitMesTime(TIM2);
	Start_Mes_Time(TIM2, &MessTime);
	
	Start_Event(TIM3, 0xFFFF);
	
	while (1)
	{
		// Wait for the next cycle.
		xLastWakeTime = xTaskGetTickCount();
        vTaskDelayUntil( &xLastWakeTime, xFrequency );
	  	
	  //if (cnt_delay == 0)
	  //{
		Measure_Iavg_Uin();
		
		switch(State)
		{	
		  	case	STATE_PREPOS:
			  	State_prepos();
			break;
			case	STATE_ACCERATE:
			  	State_accerate();
			break;
			case	STATE_ROTATE_FB:
			   	State_rotate_fb();
			break;
			case	STATE_REG_VELOCITY:
			   	State_reg_velocity();
			break;
			case	STATE_DIRECT_CONTROL:
			  	State_direct_control();
			break;
			case	STATE_STOP:
			   	State_Stop();
			break;
			case	STATE_MEAS_R12:
			  	State_meas_r12();
			break;
			case	STATE_MEAS_R13:
			  	State_meas_r13();
			break;
			case	STATE_MEAS_R23:
			  	State_meas_r23();
			break;
			case	STATE_SOUND:
			  	State_sound();
			break;
			case	STATE_STOP_ALARM:
			  	State_stop_alarm();
			break;
		}
	  // cnt_delay = 7;
	  //}
	  //taskYIELD();
		
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		
	}
}

// обработчики прерываний
void TIM1_UP_IRQHandler(void)
{
    //uint16_t Uemf;
    
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        //ADC_ExternalTrigConvCmd(ADC1, ENABLE);
	Uin = ADCConvertedValue[U_IN];
	ADC_Iin = ADC_GetInjectedConversionValue(ADC2, ADC_InjectedChannel_1);
	//Uemf = ADC_GetInjectedConversionValue(ADC2, ADC_InjectedChannel_1);
        
        #if DEBUG_DATA
	Save_Iin(ADC_Iin);
	
	if (State == STATE_ACCERATE/*STATE_ACCERATE*/)
		Save_temp_data();
	#endif

	if (Current_protection(ADC_Iin))
	{
		EC.Flag_ON_OFF = 0;
		EC.Status |= BIT_CURRENT;
	}
	
	// control commutation
	InterruptHandlerTIMUP(TIM1, &EC);
	
	switch(State)
	{
	  	case	STATE_STOP:
		  	Set_current(TIM1, &EC, 0);
			TIM_SetCompare4(TIM1, F_CLK / F_SWITCH - 1 - 1);
		break;
		
		case	STATE_PREPOS:
		  	Set_current(TIM1, &EC, ((F_CLK / F_SWITCH - 1) - tempPWM));
			//TIM_SetCompare4(TIM1, tempPWM >> 1);                          // for ubemf
                        TIM_SetCompare4(TIM1, (F_CLK / F_SWITCH - 1) - (tempPWM >> 1)); // for current
		break;
		
		case	STATE_ACCERATE:
		  	Emf = DetectZeroCross(ADCConvertedValue[U_BACKEMF], Uin, &EC, TIM3);
                        //Emf = DetectZeroCross(Uemf, 0, &EC, TIM3);
			Set_current(TIM1, &EC, ((F_CLK / F_SWITCH - 1) - tempPWM));
			//TIM_SetCompare4(TIM1, tempPWM >> 1);                          // for ubemf
                        TIM_SetCompare4(TIM1, (F_CLK / F_SWITCH - 1) - (tempPWM >> 1)); // for current
		break;
			
		case	STATE_ROTATE_FB:
		  	Emf = DetectZeroCross(ADCConvertedValue[U_BACKEMF], Uin, &EC, TIM3);
                        //Emf = DetectZeroCross(Uemf, 0, &EC, TIM3);
			Set_current(TIM1, &EC, ((F_CLK / F_SWITCH - 1) - tempPWM));
			//TIM_SetCompare4(TIM1, tempPWM >> 1);                          // for ubemf
                        TIM_SetCompare4(TIM1, (F_CLK / F_SWITCH - 1) - (tempPWM >> 1)); // for current
		break;
			
		case	STATE_REG_VELOCITY:
		  	Emf = DetectZeroCross(ADCConvertedValue[U_BACKEMF], Uin, &EC, TIM3);
                        //Emf = DetectZeroCross(Uemf, 0, &EC, TIM3);
			Set_current(TIM1, &EC, ((F_CLK / F_SWITCH - 1) - tempPWM));
			//TIM_SetCompare4(TIM1, tempPWM >> 1);                          // for ubemf
                        TIM_SetCompare4(TIM1, (F_CLK / F_SWITCH - 1) - (tempPWM >> 1)); // for current
		break;
		
		case	STATE_DIRECT_CONTROL:
		  	Emf = DetectZeroCross(ADCConvertedValue[U_BACKEMF], Uin, &EC, TIM3);
                        //Emf = DetectZeroCross(Uemf, 0, &EC, TIM3);
			Set_current(TIM1, &EC, ((F_CLK / F_SWITCH - 1) - tempPWM));
			//TIM_SetCompare4(TIM1, tempPWM >> 1);                          // for ubemf
                        TIM_SetCompare4(TIM1, (F_CLK / F_SWITCH - 1) - (tempPWM >> 1)); // for current
		break;
		
		case	STATE_MEAS_R12:
			duty_r12 = Meas_I_proc();
		break;
		
		case	STATE_MEAS_R13:
			duty_r13 = Meas_I_proc();
		break;
		
		case	STATE_MEAS_R23:
			duty_r23 = Meas_I_proc();
		break;
		case	STATE_SOUND:
		  	Set_current(TIM1, &EC, sound_duty);
			TIM_SetCompare4(TIM1, (sound_duty >> 1) + 2);
		break;
		case	STATE_STOP_ALARM:
		  	Set_current(TIM1, &EC, 0);
			TIM_SetCompare4(TIM1, 2);
		break;
		default:
			Set_current(TIM1, &EC, 0);
			TIM_SetCompare4(TIM1, 2);
		break;		
	}
}

void TIM3_IRQHandler(void)
{
  	InterruptHandlerTIMCC(TIM3, &EC);
}

void TIM2_IRQHandler(void)
{
  	InterruptHandlerTIMUp(TIM2, &MessTime);
}

void	State_Stop(void)
{
  	if (flash_update == true)
	  	flash_update = write_flash_params();
	
	if	(EC.Flag_ON_OFF == 1)
	{
	  	if (W_rs485 >= 0.0)
	  		Set_Pos_Dir(&EC);
		else
		  	Set_Neg_Dir(&EC);
	  	StartPrePos(&EC);
		Start_Event(TIM3, 0xFFFF);
		State = STATE_PREPOS;  
		Cnt_wait = 0;
		EC.Flag_enable_feedback = 0;
		EC.cnt_valid_velocity = 0;
		Init_calc_velocity();
		Set_param_vel_reg(&Vel_reg);
		tempPWM = 0;
		Set_mass_commutation(0);
		EC.Status &= ~BIT_CURRENT;
		EC.Status &= ~BIT_CURRENT_AVG;
		EC.Status &= ~BIT_STALL;
  		ADC_ITConfig(ADC2, ADC_IT_JEOC, DISABLE);
	}
	
	/*if ((EC.Flag_meas_R == 1) && ((EC.Status & BIT_VOLTAGE) == 0))
	{
	  	// инициализация токового регулятора
		Set_param_curr_reg(&Curr_reg);
		Curr_reg.I_ref = I_MEAS_R;
		Set_Pos_Dir(&EC);
	  	Measure_Event(&EC);
	  	EC.Flag_meas_R = 0;
		Curr_reg.UnsOut = 0;
		Vel_reg.UnsOut = 0;
		State = STATE_MEAS_R12;
		Cnt_wait = 0;
	}*/
	
}//STATE_STOP

void	State_prepos(void)
{
  	if (Vel_reg.UnsOut < level_pwm_start)
	  	Vel_reg.UnsOut++;
	else
	{
		if (Cnt_wait == 1)
		{
		  	State = STATE_ACCERATE;
	  		Start_EC(&EC, TIM1);
		}
		Cnt_wait++;
	}
	
	tempPWM = Correct_Uin(Uin, Vel_reg.UnsOut);
	Stop_drive();
}//STATE_PREPOS

void	State_accerate(void)
{
	if	(EC.Flag_enable_feedback == 1)
	{
	  	Cnt_wait = 0;
		Init_meas_PforW();
		Vel_reg.UnsOut = level_pwm_start; 
		Vel_reg.Up1 = Vel_reg.UnsOut;
		Vel_reg.OutPreSat = Vel_reg.UnsOut;
		State = STATE_ROTATE_FB;
		EC.cnt_miss_zc = 0;
		cnt_ON_reg_vel = 0;
	}
	
	tempPWM = Correct_Uin(Uin, Vel_reg.UnsOut);
	DetectStop();
	Stop_drive();
}//STATE_ACCERATE

void	State_rotate_fb(void)
{
  	static uint16_t	W_old = 0;
	
	if (abs(W_old - EC.Wdrive) < level_correct_speed)
		cnt_ON_reg_vel++;
	else
		cnt_ON_reg_vel = 0;
	W_old = EC.Wdrive;
	
	if (cnt_ON_reg_vel >= cnt_on_reg_speed)
	{
	  	if (drive_mode == STABILIZATION_VELOCITY)
		{
			Vel_reg.W_fb = EC.Wdrive;
			Vel_reg.W_ref = EC.Wdrive;
			Vel_reg.Out = (float)Vel_reg.UnsOut / Vel_reg.KtrPWM;
			Vel_reg.Up1 = Vel_reg.Out;
			Vel_reg.Up = Vel_reg.Out;
			Vel_reg.OutPreSat = Vel_reg.Out;
			Vel_reg.Ui = Vel_reg.Out;
			State = STATE_REG_VELOCITY;
		}
		else if (drive_mode == DIRECT_CONTROL)
		  	State = STATE_DIRECT_CONTROL;
		else
		{
		  	Vel_reg.W_fb = EC.Wdrive;
			Vel_reg.W_ref = EC.Wdrive;
			Vel_reg.Out = (float)Vel_reg.UnsOut / Vel_reg.KtrPWM;
			Vel_reg.Up1 = Vel_reg.Out;
			Vel_reg.Up = Vel_reg.Out;
			Vel_reg.OutPreSat = Vel_reg.Out;
			Vel_reg.Ui = Vel_reg.Out;
			State = STATE_REG_VELOCITY;
		}  	
	}
	
	tempPWM = Correct_Uin(Uin, Vel_reg.UnsOut);
	DetectStop();
	Stop_drive();
}//STATE_ROTATE_FB

void	State_reg_velocity(void)
{	
	//==================регулятор скорости=============
	Vel_reg.W_fb = EC.Wdrive;
	Calc_vel_reg(&Vel_reg);
	//=================================================
	
	if (W_rs485 < 0.0)
	  	Wref_RS = W_rs485 * (-1.0);
	else
	  	Wref_RS = W_rs485;
	
	if (!Current_protect_avg(Iin_avg))
	{
	  	if (cnt_I_limit != 0)
		{
			cnt_I_limit--;
			if (cnt_I_limit == 0)
	  			EC.Status &= ~BIT_CURRENT_AVG;
		}
		
		if (Vel_reg.W_ref < Wref_RS)
		{
			if (Wref_RS - Vel_reg.W_ref >= step_add_velocity)
				Vel_reg.W_ref += step_add_velocity;
			else
				Vel_reg.W_ref = Wref_RS;
		}
		if (Vel_reg.W_ref > Wref_RS)
		{
			if (Vel_reg.W_ref - Wref_RS >= step_dec_velocity)
				Vel_reg.W_ref -= step_dec_velocity;
			else
				Vel_reg.W_ref = Wref_RS;
		}
	}
	else
	{
	  	cnt_I_limit = TIME_I_LIMIT;
	  	EC.Status |= BIT_CURRENT_AVG;
	  	Vel_reg.W_ref -= step_dec_velocity;
		if (Vel_reg.W_ref < 0)
		{
			Vel_reg.W_ref = 0;
			EC.Flag_ON_OFF = 0;
		}
	}
	
	tempPWM = Correct_Uin(Uin, Vel_reg.UnsOut);
	DetectStop();
	Stop_drive();
}//STATE_REG_VELOCITY

void	State_direct_control(void)
{
  	if (W_rs485 < 0.0)
	  	Wref_RS = W_rs485 * (-1.0);
	else
	  	Wref_RS = W_rs485;
	
	Wref_RS = Wref_RS * K_W_PWM + C_W_PWM;
	
	if ((Wref_RS > 25.0) && (Wref_RS <= F_CLK / F_SWITCH))
	{
	  	if (!Current_protect_avg(Iin_avg))
		{
		  	if (cnt_I_limit != 0)
			{
			  	cnt_I_limit--;
				if (cnt_I_limit == 0)
	  				EC.Status &= ~BIT_CURRENT_AVG;
			}
			
			if (Vel_reg.UnsOut < (uint16_t)Wref_RS)
			{
				if ((uint16_t)Wref_RS - Vel_reg.UnsOut >= step_add_velocity)
					Vel_reg.UnsOut += step_add_velocity;
				else
					Vel_reg.UnsOut = (uint16_t)Wref_RS;
			}
			if (Vel_reg.UnsOut > (uint16_t)Wref_RS)
			{
				if (Vel_reg.UnsOut - (uint16_t)Wref_RS >= step_dec_velocity)
					Vel_reg.UnsOut -= step_dec_velocity;
				else
					Vel_reg.UnsOut = (uint16_t)Wref_RS;
			}
		}
		else
		{
		  	cnt_I_limit = TIME_I_LIMIT;
		  	EC.Status |= BIT_CURRENT_AVG;
		  	Vel_reg.UnsOut -= (uint16_t)step_dec_velocity;
			if (Vel_reg.UnsOut >= F_CLK / F_SWITCH - 1)
			{
			  	Vel_reg.UnsOut = 0;
				EC.Flag_ON_OFF = 0;
			}
		}
	}
	else
	  	Vel_reg.UnsOut = (uint16_t)25.0;
	
	tempPWM = Correct_Uin(Uin, Vel_reg.UnsOut);
	DetectStop();
	Stop_drive();
}//STATE_DIRECT_CONTROL

void	State_meas_r12(void)
{
  	if (Cnt_wait > N_WAIT_MEAS)
	{
	  	Measure_Event(&EC);
	  	EC.Flag_meas_R = 0;
		State = STATE_MEAS_R13;
		Cnt_wait = 0;
	}
	else
	  	Cnt_wait++;
}//STATE_MES_R12

void	State_meas_r13(void)
{
  	if (Cnt_wait > N_WAIT_MEAS)
	{
		Measure_Event(&EC);
	  	EC.Flag_meas_R = 0;
		State = STATE_MEAS_R23;
		Cnt_wait = 0;
	}
	else
	  	Cnt_wait++;
 	
}//STATE_MES_R13

void	State_meas_r23(void)
{
  	if (Cnt_wait > N_WAIT_MEAS)
	{
	  	Calc_R1R2R3(duty_r12, duty_r13, duty_r23);
		Measure_Event(&EC);
	  	EC.Flag_meas_R = 0;

		EC.Status = Detect_Alarm_EC(R1, R2, R3, duty_r12, duty_r13, duty_r23);
		State = STATE_SOUND;
		EC.Flag_sound = 1;
		sound_duty = 0;
		
		Init_Control_EC(TIM1, 65535, 0, 100);
		
		Set_Pos_Dir(&EC);
		StartPrePos(&EC);
		Start_Event(TIM3, 0xFFFF);
		Cnt_wait = 0;
	}
	else
	  	Cnt_wait++;
}//STATE_MES_R23

void	State_sound(void)
{
  	uint16_t tempStatus;
	
  	tempStatus = EC.Status;
	tempStatus &= ~BIT_VOLTAGE;
	tempStatus &= ~BIT_CURRENT;
	
  	if (Cnt_wait > TIME_SOUND)
	{
	  	sound_duty = 0;
	  	if ((tempStatus == ALARM_OPEN) || (tempStatus == ALARM_SHORT) || (tempStatus == ALARM_DISBALANCE))
		  	State = STATE_STOP_ALARM;
		else
		  	State = STATE_STOP;
		
		Cnt_wait = 0;
		EC.Flag_sound = 0;
		
		Stop_EC(&EC);
		Stop_Event(TIM3);
		Init_Control_EC(TIM1, F_CLK / F_SWITCH - 1, 0, 100);
	}
	else
	  	sound_duty = Sound_proc(tempStatus, Cnt_wait++);
}//STATE_SOUND

void	State_stop_alarm(void)
{
  	
}//STATE_STOP_ALARM

uint16_t	Meas_I_proc(void)
{
  	//GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
	
	if (Curr_reg.UnsOut >= TIME_MEAS * 2)
		TIM_SetCompare4(TIM1, Curr_reg.UnsOut >> 1);
	else if (Curr_reg.UnsOut >= TIME_MEAS + 2)
	  	TIM_SetCompare4(TIM1, Curr_reg.UnsOut - TIME_MEAS);
	else
	  	TIM_SetCompare4(TIM1, 2);
	
   	Curr_reg.I_fb = ADC_Iin; 
	Calc_current_reg(&Curr_reg);
	Set_current(TIM1, &EC, Curr_reg.UnsOut);
	
	return Curr_reg.UnsOut;
}

void	Measure_Iavg_Uin(void)
{
  	Iin_avg = ((float)Filtr_Iin()) * KTR_CURRENT;
	if (Check_Voltage(Uin))
	  	EC.Status &= ~BIT_VOLTAGE;
	else
	  	EC.Status|= BIT_VOLTAGE;
}

void	Stop_drive(void)
{
  	if	((EC.Flag_ON_OFF == 0) || (EC.Flag_fault_start != 0))
	{
		Stop_EC(&EC);
		Stop_Event(TIM3);
		State	= STATE_STOP;
		Cnt_wait = 0;
	}
}

void	DetectStop(void)
{
  	if (EC.cnt_miss_zc > level_detect_stop)
	{
	  	EC.Status |= BIT_STALL;
	  	EC.Flag_fault_start = 1;
	}
}