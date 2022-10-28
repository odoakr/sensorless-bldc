// Read_param.c

#include <stm32f10x.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include <string.h>
#include <Velocity_Reg.h>
#include <util.h>

uint8_t	Read_param(uint8_t *buf);

// params
extern uint16_t	pre_commutation;
extern Velocity_Reg_Def	Vel_reg;
extern uint16_t limit_dec_f_force_comm;
extern float	current_protection;
extern uint16_t	level_pwm_start;
extern uint16_t	cnt_on_reg_speed;
extern uint16_t	level_correct_speed;
extern float	step_add_velocity;
extern float	step_dec_velocity;
extern uint16_t	level_detect_stop;
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

uint8_t	Read_param(uint8_t *buf)
{
  	int16_t i;
	
	switch(*(buf + 1))
	{
	  	case	1:
		  	PutUint16(buf + 2, pre_commutation);
			i = 4;
		break;
		case	2:	
		  	PutFloat32(buf + 2, Vel_reg.Kp);
			i = 6;
		break;
		case	3:
		  	PutFloat32(buf + 2, Vel_reg.Ki);
			i = 6;
		break;
		case	4:
			PutFloat32(buf + 2, Vel_reg.Kd);
			i = 6;
		break;
		case	5:
		  	PutUint16(buf + 2, add_Uin);
			i = 4;
		break;
		case	6:
		  	PutUint16(buf + 2, limit_dec_f_force_comm);
			i = 4;
		break;
		case	7:	
		  	PutFloat32(buf + 2, current_protection);
			i = 6;
		break;
		case	8:
		  	PutUint16(buf + 2, level_pwm_start);
			i = 4;
		break;
		case	9:
		  	PutUint16(buf + 2, cnt_on_reg_speed);
			i = 4;
		break;
		case	10:
		  	PutUint16(buf + 2, level_correct_speed);
			i = 4;
		break;
		case	11:
		  	PutFloat32(buf + 2, step_add_velocity);
			i = 6;
		break;
		case	12:	
		  	PutFloat32(buf + 2, step_dec_velocity);
			i = 6;
		break;
		case	13:
		  	PutUint16(buf + 2, level_detect_stop);
			i = 4;
		break;
		case	14:
		  	PutUint32(buf + 2, add_force_time);
			i = 6;
		break;
		case	15:	
		  	PutUint16(buf + 2, step_dec_f_force_comm);
			i = 4;
		break;
		case	16:
		  	PutUint16(buf + 2, level_on_feedback);
			i = 4;
		break;
		case	17:
		  	PutUint16(buf + 2, level_pause_detect_ZC);
			i = 4;
		break;
		case	18:
		  	PutUint16(buf + 2, n_pair_polus);
			i = 4;
		break;
		case	19:
		  	PutUint16(buf + 2, drive_mode);
			i = 4;
		break;
		case	20:
		  	PutFloat32(buf + 2, Unom);
			i = 6;
		break;
		case	21:
		  	PutFloat32(buf + 2, current_protect_avg);
			i = 6;
		break;
		case	22:
		  	PutFloat32(buf + 2, slave_addr);
			i = 6;
		break;
		default:
	 	break;
	}
	
	return i;
}





