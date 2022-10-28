// sound.c
#include <sound.h>

uint16_t	Sound_proc(uint16_t status, uint16_t	cnt)
{
  	uint16_t	duty;
	//uint16_t	time_sound_on = TIME_SOUND;
	
	switch	(status)
	{
	  	case	ALARM_NO:	
		  	if (cnt < TIME_SOUND / 2)
				duty = SOUND_DUTY;
			else 
			  	duty = 0;
		break;
		case	ALARM_OPEN:
		  	if (cnt < TIME_SOUND / 4)
				duty = SOUND_DUTY;
			else if ((cnt >= TIME_SOUND / 4) && (cnt <= TIME_SOUND / 2))	
				duty = 0;
			else if ((cnt >= TIME_SOUND / 2) && (cnt <= TIME_SOUND / 2 + TIME_SOUND / 4))
				duty = SOUND_DUTY;
			else 
				duty = 0;
		break;
		case	ALARM_SHORT:
		  	if (cnt < TIME_SOUND / 6)
				duty = SOUND_DUTY;
			else if ((cnt >= TIME_SOUND / 6) && (cnt <= TIME_SOUND / 3))	
				duty = 0;
			else if ((cnt >= TIME_SOUND / 3) && (cnt <= TIME_SOUND / 3 + TIME_SOUND / 6))
				duty = SOUND_DUTY;
			else if ((cnt >= TIME_SOUND / 3 + TIME_SOUND / 6) && (cnt <= 2 * TIME_SOUND / 3))
				duty = 0;
			else if ((cnt >= 2 * TIME_SOUND / 3) && (cnt <= 2 * TIME_SOUND / 3 + TIME_SOUND / 6))
				duty = SOUND_DUTY;
			else 
				duty = 0;
		break;
		case	ALARM_DISBALANCE:
		  	if (cnt < TIME_SOUND / 8)
				duty = SOUND_DUTY;
			else if ((cnt >= TIME_SOUND / 8) && (cnt <= TIME_SOUND / 4))	
				duty = 0;
			else if ((cnt >= TIME_SOUND / 4) && (cnt <= TIME_SOUND / 4 + TIME_SOUND / 8))
				duty = SOUND_DUTY;
			else if ((cnt >= TIME_SOUND / 4 + TIME_SOUND / 8) && (cnt <= TIME_SOUND / 2))
				duty = 0;
			else if ((cnt >= TIME_SOUND / 2) && (cnt <= TIME_SOUND / 2 + TIME_SOUND / 8))
				duty = SOUND_DUTY;
			else if ((cnt >= TIME_SOUND / 2 + TIME_SOUND / 8) && (cnt <= TIME_SOUND / 2 + TIME_SOUND / 4))
				duty = 0;
			else if ((cnt >= TIME_SOUND / 2 + TIME_SOUND / 4) && (cnt <= TIME_SOUND / 2 + TIME_SOUND / 4 + TIME_SOUND / 8))
			  	duty = SOUND_DUTY;
			else
			  	duty = 0;
		break;
	}
	
	return duty;
}