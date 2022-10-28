// Drive_param.h

#define F_CLK			72000000
#define	F_SWITCH		30000
#define FLOAT_F_CLK		72000000.0
#define FLOAT_F_SWITCH	30000.0
#define N_CLK_INJECT	112

#define	N_MEAS_I		64
#define STEP_INJECT		F_CLK / F_SWITCH / N_MEAS_I

#define	N_PAIR_POLUS	6

#define CURRENT_MAX				30.0
#define KTR_CURRENT				0.00403
#define CURRENT_PROT			15.0
#define CURRENT_PROT_AVG_LOW	3.0
#define CURRENT_PROT_AVG_HI		4.0
#define	TIME_I_LIMIT			50

#define KTR_UIN			0.0154 //0.00533
#define U_NOM			32.0
#define U_MAX			34.0
#define U_MIN			5.0

#define R_CURRENT		0.004
#define I_MEAS_R		0.75
#define TIME_MEAS		74

#define K_W_PWM			(FLOAT_F_CLK / FLOAT_F_SWITCH - 25.0) / (3600.0 - 600.0) //0.792	
#define C_W_PWM			(25.0 - K_W_PWM * 600.0) //-450.0	

#define	N_WAIT_MEAS		250

#define VELOCITY_MAX	3600.0
#define VELOCITY_MIN	37.5
#define VELOCITY_MIN_RPM	100


#define	PRE_COMMUTATION		11500

#define STATE_STOP				0
#define STATE_PREPOS			1
#define	STATE_ACCERATE			2
#define STATE_ROTATE			3
#define	STATE_ROTATE_FB			4
#define STATE_REG_VELOCITY		5
#define STATE_DIRECT_CONTROL	6
#define STATE_MEAS_R12			7
#define STATE_MEAS_R13			8
#define STATE_MEAS_R23			9
#define STATE_SOUND				10
#define STATE_STOP_ALARM		11

#define	ALARM_NO				0
#define ALARM_OPEN				1
#define ALARM_SHORT				2
#define ALARM_DISBALANCE		3
#define ALARM_VOLTAGE			4
#define ALARM_CURRENT			5

#define BIT_OPEN				0x0001
#define BIT_SHORT				0x0002
#define BIT_DISBALANCE			0x0004
#define BIT_VOLTAGE				0x0008
#define BIT_CURRENT				0x0010
#define BIT_STALL				0x0020
#define BIT_CURRENT_AVG			0x0040

#define		N_LOCK_MES_EMF		3
#define		N_LOCK_MES_CURRENT	1

#define		STEP_DEC_F_FORCE_COMM	200
#define		LIMIT_DEC_F_FORCE_COMM	32500
#define		LEVEL_DETECT_STOP		10
#define		LEVEL_PAUSE_DETECT_ZC	75
#define		CNT_ON_REG_SPEED		50
#define		LEVEL_CORRECT_SPEED		500
#define		LEVEL_PWM_START			200
#define		LEVEL_ON_FEEDBACK		4
#define		ADD_FORCE_TIME			10000
#define		ADD_UIN				1

#define		STEP_ADD_VELOCITY		40.0
#define		STEP_DEC_VELOCITY		40.0

// drive mode
#define	DIRECT_CONTROL			1
#define	STABILIZATION_VELOCITY	2
#define	STABILIZATION_CURRENT	3

#define SOUND_DUTY				250
#define TIME_SOUND				1000

#define DEBUG_DATA				1