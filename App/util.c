// util.c
#include <util.h>

float GetFloat32(uint8_t *buf)
{
  	U_float		f_val;
	
	f_val.U8_val[0] = *buf;
	f_val.U8_val[1] = *(buf + 1);
	f_val.U8_val[2] = *(buf + 2);
	f_val.U8_val[3] = *(buf + 3);
	
	return f_val.Fl_val;
}

int16_t GetInt16(uint8_t *buf)
{
  	Int_RS i_val;
	
	i_val.U8_val[0] = *buf;
	i_val.U8_val[1] = *(buf + 1);
	
	return i_val.Int_val;
}

uint16_t GetUint16(uint8_t *buf)
{
  	Uint16 u_val;
	
	u_val.U8_val[0] = *buf;
	u_val.U8_val[1] = *(buf + 1);
	
  	return u_val.Uint_val;
}

uint32_t 	GetUint32(uint8_t *buf)
{
  	Uint32 u_val;
	
	u_val.U8_val[0] = *buf;
	u_val.U8_val[1] = *(buf + 1);
	u_val.U8_val[2] = *(buf + 2);
	u_val.U8_val[3] = *(buf + 3);
	
  	return u_val.Uint_val;
}

void PutFloat32(uint8_t *buf, float val)
{
  	U_float		f_val;
	
	f_val.Fl_val = val;
	*buf  		= f_val.U8_val[0];
	*(buf + 1) 	= f_val.U8_val[1];
	*(buf + 2) 	= f_val.U8_val[2];
	*(buf + 3) 	= f_val.U8_val[3];
}

void PutInt16(uint8_t *buf, int16_t val)
{
  	Int_RS i_val;
	
	i_val.Int_val = val;
	*buf 		= i_val.U8_val[0];
	*(buf + 1)	= i_val.U8_val[1];
}

void PutUint16(uint8_t *buf, uint16_t val)
{
  	Uint16 u_val;
	
	u_val.Uint_val = val;
	*buf 		= u_val.U8_val[0];
	*(buf + 1)	= u_val.U8_val[1];
}

void PutUint32(uint8_t *buf, uint32_t val)
{
  	Uint32 u_val;
	
	u_val.Uint_val = val;
	*buf 		= u_val.U8_val[0];
	*(buf + 1)	= u_val.U8_val[1];
	*(buf + 2)	= u_val.U8_val[2];
	*(buf + 3)	= u_val.U8_val[3];
}