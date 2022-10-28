// util.h
#include <stm32f10x.h>

float 		GetFloat32(uint8_t *buf);
int16_t 	GetInt16(uint8_t *buf);
uint16_t 	GetUint16(uint8_t *buf);
uint32_t 	GetUint32(uint8_t *buf);
void 		PutFloat32(uint8_t *buf, float val);
void 		PutInt16(uint8_t *buf, int16_t val);
void 		PutUint16(uint8_t *buf, uint16_t val);
void 		PutUint32(uint8_t *buf, uint32_t val);

typedef union U_float{
	float	Fl_val;
	uint8_t U8_val[4];
}U_float;

typedef union Int_RS{
  	int16_t	Int_val;
	uint8_t U8_val[2];
}Int_RS;

typedef	union Uint16{
  	uint16_t 	Uint_val;
	uint8_t 	U8_val[2];
}Uint16;

typedef union Uint32{
  	uint32_t	Uint_val;
	uint8_t 	U8_val[4];
}Uint32;