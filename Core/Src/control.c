
#include "string.h"
#include "mk_conf_tree.h"
#include "setup.h"
#include "system.h"
#include "USART.h"
#include "control.h"

VAR_PAR dev_var;
u32 mTimeout_cntrl = 0;
u32 mTimeout_shift = 0;
//============================================================================
uint8_t Control_prop( uint8_t par, uint8_t * name, uint8_t * prop, uint8_t * nbyte )
{
	char * str;
    uint8_t size = 0;
	if( name ) { switch( par )
        {
		case CONTROL_BAT        :	str ="DC (V)"; break;
		case CONTROL_CURR       :	str ="DC current (A)"; break;
		case CONTROL_LEVEL      :	str ="Level (0..100)"; break;
		case CONTROL_STLVL      :	str ="Start level (0..100)"; break;
		case CONTROL_PWM        :	str ="PWM code (0..65535)"; break;
		case CONTROL_STAT       :	str ="KN state"; break;
		case CONTROL_ADC_1      :	str ="ADC Level 1"; break;
		case CONTROL_ADC_2      :	str ="ADC Level 2"; break;
        default: return 0;
        }
        while( *str ) { *name++ = *str++; size++; } *name = 0; 
    }
	if( prop ) switch( par )
        {
        case CONTROL_CURR      :   
        case CONTROL_BAT       :    *prop = REAL; break;
		case CONTROL_STAT      :    
        case CONTROL_PWM       :   
        case CONTROL_ADC_1     :   
		case CONTROL_ADC_2     :	*prop = UINT|RO; break;
        case CONTROL_STLVL     :   
		case CONTROL_LEVEL     :	*prop = UINT; break;
        default: return 0;
        }
	if( nbyte ) switch( par )
        {
        case CONTROL_CURR      :   
        case CONTROL_BAT       :    *nbyte = 4; break;
		case CONTROL_STAT      :  
        case CONTROL_PWM       :   
        case CONTROL_ADC_1     :   
		case CONTROL_ADC_2     :	*nbyte = 2; break;
        case CONTROL_STLVL     :   
		case CONTROL_LEVEL     :	*nbyte = 1; break;
        default: return 0;
        }
    return size;
}
//============================================================================
void Control_get(uint8_t par, void * value)
{
    switch( par )
    {
        case CONTROL_ADC_1   :	*(Uint32*)value = dev_var.adc_det[0]; break;
        case CONTROL_ADC_2   :	*(Uint32*)value = dev_var.adc_det[1]; break;
        case CONTROL_LEVEL   :	*(Uint32*)value = dev_var.level; break;
        case CONTROL_STLVL   :	*(Uint32*)value = Setup.start_level; break;
        case CONTROL_PWM     :	*(Uint32*)value = dev_var.pwm; break;
        case CONTROL_BAT     :	*(float*)value = dev_var.vdev; break;
        case CONTROL_CURR    :	*(float*)value = dev_var.curr; break;
        case CONTROL_STAT    :	*(Uint32*)value = dev_var.stt_kn; break;
        default:;
    }
}
//============================================================================
void Control_set(uint8_t par, void * value)
{
	u8 i = *(u8*)value;
	float f = *(float*)value;
    switch( par )
    {
		case CONTROL_BAT	  :	f = f/(float)dev_var.adc_det[0]; Setup.bt_coef = f;
                                //SETUP_Save();	
                                break;
		case CONTROL_CURR	  :	Setup.shift_curr += f;
                                //SETUP_Save();	
                                break;
		case CONTROL_LEVEL	  :	if(i<=100)dev_var.level = i;
                                else dev_var.level = 100;
                                dev_var.state = STAT_LVL;
                                break;
		case CONTROL_STLVL	  :	if(i<=100)Setup.start_level = i;
                                else Setup.start_level = 100;
                                SETUP_Save();
                                break;
        default:;
    }
}
//============================================================================
uint8_t Params_prop( uint8_t par, uint8_t * name, uint8_t * prop, uint8_t * nbyte )
{
	char * str;
    uint8_t size = 0;
	if( name ) {switch( par )
        {
        case PARAM_DELTA       :	str ="Delta level (mS)"; break;
        case PARAM_BCOEF       :	str ="PWM coef"; break;
        case PARAM_CCOEF_1     :	str ="Current coef 1"; break;
        case PARAM_CCOEF_2     :	str ="Current coef 2"; break;
        case PARAM_CSHIFT      :	str ="Current shift"; break;
        case PARAM_CLIMIT      :	str ="Current limit (4,95A max)"; break;
        default: return 0;
        }
        while( *str ) { *name++ = *str++; size++; } *name = 0; 
    }
	if( prop ) switch( par )
        {
        case PARAM_CLIMIT     :	
        case PARAM_CSHIFT     :	
        case PARAM_CCOEF_1    :	
        case PARAM_CCOEF_2    :	
        case PARAM_BCOEF      :	*prop = REAL; break;
        case PARAM_DELTA      :	*prop = UINT; break;
        default: return 0;
        }
	if( nbyte ) switch( par )
        {
        case PARAM_CLIMIT     :	
        case PARAM_CSHIFT     :	
        case PARAM_CCOEF_1    :	
        case PARAM_CCOEF_2    :	
        case PARAM_BCOEF      :	*nbyte = 4; break;
        case PARAM_DELTA      :	*nbyte = 2; break;
        default: return 0;
        }
    return size;
}
//============================================================================
void Params_get(uint8_t par, void * value)
{
    switch( par )
    {
		case PARAM_DELTA     	:	*(Uint32*)value = Setup.delta; break;
		case PARAM_BCOEF     	:	*(float*)value = Setup.pwm_coef; break;
		case PARAM_CCOEF_1     	:	*(float*)value = Setup.coef_curr1; break;
		case PARAM_CCOEF_2     	:	*(float*)value = Setup.coef_curr2; break;
		case PARAM_CSHIFT     	:	*(float*)value = Setup.shift_curr; break;
		case PARAM_CLIMIT     	:	*(float*)value = Setup.limit_curr; break;
        default:;
    }
}
//============================================================================
void Params_set(uint8_t par, void * value)
{
	u16 i = *(u16*)value;
	float f = *(float*)value;
    switch( par )
    {
		case PARAM_DELTA	  :	Setup.delta = i; break;
		case PARAM_BCOEF	  :	Setup.pwm_coef = f; break;
		case PARAM_CCOEF_1	  :	Setup.coef_curr1 = f; break;
		case PARAM_CCOEF_2	  :	Setup.coef_curr2 = f; break;
		case PARAM_CSHIFT	  :	Setup.shift_curr = f; break;
		case PARAM_CLIMIT	  :	Setup.limit_curr = f; 
                                dev_var.quant = f/100.0f;
                                break;
        default:;
    }
}
//=========================================================================
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
}
//=========================================================================
// Функция обработки
void CONTROL_Proc(void)
{
	static Uint32 timeout_type = 0;
	static Uint32 timeout_kn_1 = 0;
	static Uint16 level_old = 0;
	static Uint16 kn_old = 0;

	if(dev_var.start == 0)
	{
        dev_var.start = 1;
        dev_var.state = STAT_START;
	}
//----------------------------------------------------    
	if(timeout_kn_1+100 <= HAL_GetTick())
	{
        timeout_kn_1 = HAL_GetTick();
        Uint16 kn_on_off = HAL_GPIO_ReadPin(KN_1_GPIO_Port, KN_1_Pin);
        if(kn_old != kn_on_off){
            if(kn_on_off == 0){
                dev_var.stt_kn = 1;
                dev_var.level = Setup.start_level;
                dev_var.state = STAT_KN_1;
            }else{
                dev_var.stt_kn = 0;
                dev_var.level = 0;
                dev_var.state = STAT_KN_1;
            }
            kn_old = kn_on_off;
        } 
    }
//----------------------------------------------------    
	if(timeout_type+Setup.delta <= HAL_GetTick())
	{
        timeout_type = HAL_GetTick();
        
        if(level_old != dev_var.level)
        {
            if(level_old < dev_var.level) 
            {
                if(dev_var.curr < Setup.limit_curr)
                {
                    level_old++;
                }
                else 
                {
                    if(dev_var.curr > Setup.limit_curr) level_old--;
                    else dev_var.level = level_old;
                }
            }
            else if(level_old > dev_var.level) 
            {
                level_old--;
            }
            u32 tmp = 65535;
            tmp = level_old*tmp/100;
            dev_var.pwm = tmp;
            TIM4->CCR2 = dev_var.pwm;
        } 
	}
}
//=========================================================================

