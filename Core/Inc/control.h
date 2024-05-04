
#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"
#include "system.h"
#include "USART.h"

// ���������
#define VARR_STATE      0      // R: ���������
//--------------------------------------------------
//#define CMD_PAR         20  // 0x14: �������� ��������� (����� �� ������)
#define CMD_SET_PAR     21  // 0x15: ��������� ���������
#define CMD_GET_PAR     22  // 0x16: ������ �������� ���������

#define CMD_RESET		0		// ����� �������� CMD
//--------------------------------------------------
#define STAT_NULL  	        0x0000	// Null
#define STAT_START  	    0x0001	// �������� �������
#define STAT_LVL    	    0x0002	// ������� ������� �����
#define STAT_KN_1    	    0x0004	// �������� ��������� �����������
#define STAT_ALRM       	0x0008	// ��������������
#define STAT_ERR       	    0x0010	// ������
//--------------------------------------------------
#define PAR_STATE  		0x0001	//0 ������ ��������� 0


// ���������� ��������
enum CONTROL_VAR
{
	CONTROL_LEVEL,			// ���������
	CONTROL_STLVL,			// ���������
	CONTROL_BAT,			// Battery level (V)
	CONTROL_CURR,			// Battery current (A)
	CONTROL_PWM,			// ���������
	CONTROL_ADC_1,			// ADC Level 1
	CONTROL_ADC_2,			// ADC Level 2
	CONTROL_STAT,			// ���������

	CONTROL_VAR_NUM    		// ���������� ����������
};
// ���������� ��������
enum PARAM_VAR
{
	PARAM_BCOEF=0,			// Bright LED
	PARAM_CCOEF_1,			// Bright LED
	PARAM_CCOEF_2,			// Bright LED
	PARAM_CSHIFT,			// Bright LED
	PARAM_CLIMIT,			// Bright LED
	PARAM_DELTA, 			// Bright LED

	PARAM_VAR_NUM    		// ���������� ����������
};

#pragma pack (1)
typedef struct
{
	float vdev;		// ���������� ������� ������� (V)
	float curr;		// 
	float quant;		// 
    u16 state;
    u16 stt_kn;
	u16 start;		// ������� ������
    u16 tick;
    s16 level;
    u16 pwm;
    u16 delta_pwm;
    s16 adc_det[2];
	u32 realtime;   
	u32 count_pkt_in;   
	u32 count_pkt_out;   
	u32 count_pkt_err;
}
VAR_PAR;
#pragma pack(4)

extern VAR_PAR dev_var;

//----------------------------------------------------------------
uint8_t Control_prop(uint8_t par, uint8_t * name, uint8_t * prop, uint8_t * nbyte);
void Control_get(uint8_t par, void * value);
void Control_set(uint8_t par, void * value);
//----------------------------------------------------------------
uint8_t Params_prop(uint8_t par, uint8_t * name, uint8_t * prop, uint8_t * nbyte);
void Params_get(uint8_t par, void * value);
void Params_set(uint8_t par, void * value);
//----------------------------------------------------------------

u8 calc_CS(u8 * addr, u16 size);
u16 conv_baud_uint(u16 baud);

// ������� ���������
void CONTROL_Proc(void);

#endif // __CONTROL_H
