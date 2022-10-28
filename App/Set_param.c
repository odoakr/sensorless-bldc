// Set_param.c

#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include <Velocity_Reg.h>
#include <util.h>
#include <stdbool.h>

uint8_t		Set_param(uint8_t *buf);

// params
extern Velocity_Reg_Def	Vel_reg;
extern uint16_t	pre_commutation;
extern uint16_t limit_dec_f_force_comm;
extern float	current_protection;
extern uint16_t	level_pwm_start;
extern uint16_t	cnt_on_reg_speed;
extern uint16_t	level_correct_speed;
extern float	step_add_velocity;
extern float	step_dec_velocity;
extern uint16_t	level_detect_stop;
extern uint16_t	pre_commutation;
extern uint32_t	add_force_time;
extern int16_t	add_Uin;
extern uint16_t limit_dec_f_force_comm;
extern uint16_t step_dec_f_force_comm;
extern uint16_t level_on_feedback;
extern uint16_t level_pause_detect_ZC;
extern uint16_t n_pair_polus;
extern uint16_t	drive_mode;
extern float 	Unom;
extern float	current_protect_avg;
extern float	slave_addr;

extern bool 	flash_update;

uint8_t	Set_param(uint8_t *buf)
{
  	int16_t i;
	
	switch(*(buf + 1))
	{
	  	case	1:
		  	pre_commutation = GetUint16(buf + 2);
			flash_update = true;
		break;
		case	2:	
		  	Vel_reg.Kp = GetFloat32(buf + 2);
			flash_update = true;
		break;
		case	3:
		  	Vel_reg.Ki = GetFloat32(buf + 2);
			flash_update = true;
		break;
		case	4:
		  	Vel_reg.Kd = GetFloat32(buf + 2);
			flash_update = true;
		break;
		case	5:
		  	add_Uin = GetInt16(buf + 2);
			flash_update = true;
		break;
		case	6:
		  	limit_dec_f_force_comm = GetUint16(buf + 2);
			flash_update = true;
		break;
		case	7:	
		  	current_protection = GetFloat32(buf + 2);
			flash_update = true;
		break;
		case	8:
		  	level_pwm_start = GetUint16(buf + 2);
			flash_update = true;
		break;
		case	9:
		  	cnt_on_reg_speed = GetUint16(buf + 2);
			flash_update = true;
		break;
		case	10:
		  	level_correct_speed = GetUint16(buf + 2);
			flash_update = true;
		break;
		case	11:
		  	step_add_velocity = GetFloat32(buf + 2);
			flash_update = true;
		break;
		case	12:	
		  	step_dec_velocity = GetFloat32(buf + 2);
			flash_update = true;
		break;
		case	13:
		  	level_detect_stop = GetUint16(buf + 2);
			flash_update = true;
		break;
		case	14:
		  	add_force_time = GetUint32(buf + 2);
			flash_update = true;
		break;
		case	15:	
		  	step_dec_f_force_comm = GetUint16(buf + 2);
			flash_update = true;
		break;
		case	16:
		  	level_on_feedback = GetUint16(buf + 2);
			flash_update = true;
		break;
		case	17:
		  	level_pause_detect_ZC = GetUint16(buf + 2);
			flash_update = true;
		break;
		case	18:
		  	n_pair_polus = GetUint16(buf + 2);
			flash_update = true;
		break;
		case	19:
		  	drive_mode = GetUint16(buf + 2);
			flash_update = true;
		break;
		case	20:
		  	Unom = GetFloat32(buf + 2);
			flash_update = true;
		break;
		case	21:
		  	current_protect_avg = GetFloat32(buf + 2);
			flash_update = true;
		break;
		case	22:
		  	slave_addr = GetFloat32(buf + 2);
			flash_update = true;
		break;
		default:
	 	break;
	}
	
	i = 2;
	
	return i;
}