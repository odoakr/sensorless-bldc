// params.c
#include <stm32f10x.h>
#include <stdbool.h>
#include <Velocity_Reg.h>
#include <util.h>
#include <Gen_event_comm.h>
#include <rs485.h>

// Create a section in the compiler. 
#pragma section = "ParamData"
// Place a variable in that section. 
const float MyVariable[0x80] @ "ParamData";
/*
// Create a section in the compiler.
#pragma section = "ParamDataDefault"
// Place a variable in that section. 
const float ParamDefault[0x100] @ "ParamDataDefault";
*/
// Create a section in the compiler. 
#pragma section = "AccelerateData"
const uint16_t AccelerateTab[0x100] @ "AccelerateData";

extern uint16_t N_full[SIZE_ACC_TAB];
extern uint16_t DeltaT[SIZE_ACC_TAB];

uint16_t N_full_default[SIZE_ACC_TAB];
uint16_t DeltaT_default[SIZE_ACC_TAB];

extern	uint16_t	pre_commutation;
		uint16_t	pre_commutation_def;
extern Velocity_Reg_Def	Vel_reg;
extern	uint16_t limit_dec_f_force_comm;
		uint16_t limit_dec_f_force_comm_def;
extern	float	current_protection;
		float	current_protection_def;
extern	uint16_t	level_pwm_start;
		uint16_t	level_pwm_start_def;
extern	uint16_t	cnt_on_reg_speed;
		uint16_t	cnt_on_reg_speed_def;
extern	uint16_t	level_correct_speed;
		uint16_t	level_correct_speed_def;
extern	float	step_add_velocity;
		float	step_add_velocity_def;
extern	float	step_dec_velocity;
		float	step_dec_velocity_def;
extern	uint16_t	level_detect_stop;
		uint16_t	level_detect_stop_def;
extern	uint16_t	pre_commutation;
		uint16_t	pre_commutation_def;
extern	uint32_t	add_force_time;
		uint32_t	add_force_time_def;
extern	int16_t	add_Uin;
		int16_t	add_Uin_def;
extern	uint16_t limit_dec_f_force_comm;
		uint16_t limit_dec_f_force_comm_def;
extern	uint16_t step_dec_f_force_comm;
		uint16_t step_dec_f_force_comm_def;
extern	uint16_t level_on_feedback;
		uint16_t level_on_feedback_def;
extern	uint16_t level_pause_detect_ZC;
		uint16_t level_pause_detect_ZC_def;
extern	uint16_t n_pair_polus;
		uint16_t n_pair_polus_def;
extern	uint16_t	drive_mode;	
		uint16_t	drive_mode_def;	
extern	float 	Unom;
		float 	Unom_def;
extern 	float	current_protect_avg;
		float	current_protect_avg_def;
extern 	float	slave_addr;
		float	slave_addr_def;

char *AddrParam(void);
int check = 0;
char *Addr_flash;

// Private typedef -----------------------------------------------------------
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

// Private define ------------------------------------------------------------
// Define the STM32F10x FLASH Page Size depending on the used STM32 device 
#ifdef STM32F10X_LD
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#elif defined STM32F10X_MD
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#elif defined STM32F10X_HD
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#elif defined STM32F10X_CL
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)  
#endif // STM32F10X_LD 

#define START_ADDR  		((uint32_t)0x08007C00)
#define END_ADDR    		((uint32_t)0x08008000)

#define START_ADDR_PAGE1	((uint32_t)0x08007C00)
#define END_ADDR_PAGE1		((uint32_t)0x08008000)
/*
#define START_ADDR_PAGE2	((uint32_t)0x08007400)
#define END_ADDR_PAGE2		((uint32_t)0x08007800)
*/
// Private variables ---------------------------------------------------------  
uint32_t EraseCounter = 0x00, Address = 0x00;
uint32_t Data;
__IO uint32_t NbrOfPage = 0x00;
volatile FLASH_Status FLASHStatus;
volatile TestStatus MemoryProgramStatus;

bool 	flash_update = false;

char 	*AddrParam(void);
char 	*AddrAccerateData(void);
bool 	write_flash_params(void);
bool	Read_flash_params(void);
void	Read_flash_default_params(void);
void	Set_def_params(void);
void	write_flash_float(float Data);
void	write_flash_uint16(uint16_t data);

uint8_t		Req_Read_DefaultParam(uint8_t *buf);
uint8_t		Set_Req_Write_Param(uint8_t *buf);

char *AddrParam(void)
{
	// Return start address of section ParamData. 
	return __section_begin("ParamData");
}

char *AddrAccerateData(void)
{
  	// Return start address of section AccerateData. 
	return __section_begin("AccelerateData");
}

void	write_flash_float(float Data)
{
  	U_float temp_f;
	Uint32	temp_data;
	
	temp_f.Fl_val = Data;
	temp_data.U8_val[0] = temp_f.U8_val[0];
	temp_data.U8_val[1] = temp_f.U8_val[1];
	temp_data.U8_val[2] = temp_f.U8_val[2];
	temp_data.U8_val[3] = temp_f.U8_val[3];
	FLASHStatus = FLASH_ProgramWord(Address, temp_data.Uint_val);
	Address += 4;
	
}

void	write_flash_uint16(uint16_t data)
{
	FLASHStatus = FLASH_ProgramHalfWord(Address, data);
	Address += 2;
}

bool write_flash_params(void)
{	
  	uint16_t i;
  	FLASHStatus = FLASH_COMPLETE;
 	MemoryProgramStatus = PASSED;
	
	// Unlock the Flash Program Erase controller 
  	FLASH_Unlock();

  	// Define the number of page to be erased 
  	NbrOfPage = (END_ADDR_PAGE1 - START_ADDR_PAGE1) / FLASH_PAGE_SIZE;

  	// Clear All pending flags 
  	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

  	// Erase the FLASH pages 
  	for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
  	{
    	FLASHStatus = FLASH_ErasePage(START_ADDR_PAGE1 + (FLASH_PAGE_SIZE * EraseCounter));
  	}
  	
	//  FLASH Word program of data 0x15041979 at addresses defined by StartAddr and EndAddr
  	Address = (uint32_t)AddrParam(); 
	
	write_flash_float((float)pre_commutation);
	write_flash_float(Vel_reg.Kp);
	write_flash_float(Vel_reg.Ki);
	write_flash_float(Vel_reg.Kd);
	write_flash_float(current_protection);
	write_flash_float((float)level_pwm_start);	
	write_flash_float((float)cnt_on_reg_speed);
	write_flash_float((float)level_correct_speed);
	write_flash_float(step_add_velocity);
	write_flash_float(step_dec_velocity);
	write_flash_float((float)level_detect_stop);
	write_flash_float((float)add_force_time);
	write_flash_float((float)add_Uin);
	write_flash_float((float)limit_dec_f_force_comm);
	write_flash_float((float)step_dec_f_force_comm);
	write_flash_float((float)level_on_feedback);
	write_flash_float((float)level_pause_detect_ZC);
	write_flash_float((float)n_pair_polus);
	write_flash_float((float)drive_mode);
	write_flash_float(Unom);
	write_flash_float(current_protect_avg);
	write_flash_float(slave_addr);
	// запись признака, что параметры были сохранены
	write_flash_float((float)1);
	
	Address = (uint32_t)AddrAccerateData();
	
	for (i = 0; i < sizeof(N_full)/sizeof(N_full[0]); i++)
		write_flash_uint16(N_full[i]);
		
	for (i = 0; i < sizeof(DeltaT)/sizeof(DeltaT[0]); i++)
		write_flash_uint16(DeltaT[i]);
		
  	return false;
}

bool	Read_flash_params(void)
{	
  	uint16_t i;
	
	if (MyVariable[22] == 0)	// нет параметров во флэш
	{
	  	Read_flash_default_params();
	  	return true;
	}
	else if (MyVariable[22] == 1) // есть параметры во флэш
	{
	  	pre_commutation 		= (uint16_t)MyVariable[0];
		Vel_reg.Kp 				= MyVariable[1];
		Vel_reg.Ki 				= MyVariable[2];
		Vel_reg.Kd				= MyVariable[3];
		current_protection 		= MyVariable[4];
		level_pwm_start			= (uint16_t)MyVariable[5];
		cnt_on_reg_speed		= (uint16_t)MyVariable[6];
		level_correct_speed 	= (uint16_t)MyVariable[7];
		step_add_velocity		= MyVariable[8];
		step_dec_velocity		= MyVariable[9];
		level_detect_stop		= (uint16_t)MyVariable[10];
		add_force_time			= (uint32_t)MyVariable[11];
		add_Uin					= (int16_t)MyVariable[12];
		limit_dec_f_force_comm	= (uint16_t)MyVariable[13];
		step_dec_f_force_comm	= (uint16_t)MyVariable[14];
		level_on_feedback		= (uint16_t)MyVariable[15];
		level_pause_detect_ZC	= (uint16_t)MyVariable[16];
		n_pair_polus			= (uint16_t)MyVariable[17];
		drive_mode				= (uint16_t)MyVariable[18];
		Unom					= MyVariable[19];
		current_protect_avg		= MyVariable[20];
		slave_addr				= MyVariable[21];
		
		for (i = 0; i < sizeof(N_full)/sizeof(N_full[0]); i++)
		  	N_full[i] = AccelerateTab[i];
		
		for (i = 0; i < sizeof(DeltaT)/sizeof(DeltaT[0]); i++)
		  	DeltaT[i] = AccelerateTab[i + sizeof(N_full)/sizeof(N_full[0])];
		
	  	return false;
	}
	else
	{
	  	return false;
	}
}

void	Read_flash_default_params(void)
{
  	uint16_t i;
	
  	pre_commutation 		= pre_commutation_def;
	Vel_reg.Kp 				= 1.0;
	Vel_reg.Ki 				= 0.005;
	Vel_reg.Kd				= 0.0;
	current_protection 		= current_protection_def;
	level_pwm_start			= level_pwm_start_def;
	cnt_on_reg_speed		= cnt_on_reg_speed_def;
	level_correct_speed 	= level_correct_speed_def;
	step_add_velocity		= step_add_velocity_def;
	step_dec_velocity		= step_dec_velocity_def;
	level_detect_stop		= level_detect_stop_def;
	add_force_time			= add_force_time_def;
	add_Uin					= add_Uin_def;
	limit_dec_f_force_comm	= limit_dec_f_force_comm_def;
	step_dec_f_force_comm	= step_dec_f_force_comm_def;
	level_on_feedback		= level_on_feedback_def;
	level_pause_detect_ZC	= level_pause_detect_ZC_def;
	n_pair_polus			= n_pair_polus_def;
	drive_mode				= drive_mode_def;
	Unom					= Unom_def;
	current_protect_avg		= current_protect_avg_def;
	slave_addr				= slave_addr_def;
	
	for (i = 0; i < sizeof(N_full)/sizeof(N_full[0]); i++)
	  	N_full[i] = N_full_default[i];
		
	for (i = 0; i < sizeof(DeltaT)/sizeof(DeltaT[0]); i++)
	  	DeltaT[i] = DeltaT_default[i];
}

void	Set_def_params(void)
{
  	uint16_t i;
	
	pre_commutation_def			= pre_commutation;
	current_protection_def		= current_protection;
	level_pwm_start_def			= level_pwm_start;
	cnt_on_reg_speed_def		= cnt_on_reg_speed;
	level_correct_speed_def		= level_correct_speed;
	step_add_velocity_def		= step_add_velocity;
	step_dec_velocity_def		= step_dec_velocity;
	level_detect_stop_def		= level_detect_stop;
	add_force_time_def			= add_force_time;
	add_Uin_def					= add_Uin;
	limit_dec_f_force_comm_def	= limit_dec_f_force_comm;
	step_dec_f_force_comm_def	= step_dec_f_force_comm;
	level_on_feedback_def		= level_on_feedback;
	level_pause_detect_ZC_def	= level_pause_detect_ZC;
	n_pair_polus_def			= n_pair_polus;
	drive_mode_def				= drive_mode;
	Unom_def					= Unom;
	current_protect_avg_def		= current_protect_avg;
	slave_addr_def				= slave_addr;
	
  	for (i = 0; i < sizeof(N_full)/sizeof(N_full[0]); i++)
	  	N_full_default[i] = N_full[i];
		
	for (i = 0; i < sizeof(DeltaT)/sizeof(DeltaT[0]); i++)
	  	DeltaT_default[i] = DeltaT[i];
}

uint8_t		Req_Read_DefaultParam(uint8_t *buf)
{
  	Read_flash_default_params();
	return 0;
}

uint8_t		Set_Req_Write_Param(uint8_t *buf)
{
  	flash_update = true;
	return 0;
}
