/*
 * TMU.c
 *
 * Created: 24/02/2020 17:34:05
 *  Author: FATHI
 */ 


#include "TMU_Module.h"


uint8_t test = 0 ;

void T_LED(void);
void T_LED_2(void);

int main(void)
{
	
	PORTC_DIR = 0xFF;
	PORTD_DIR = 0xFF;
	TMU_ConfigType Tmu_cfg= {TIMER_0};
	TMU_Init (&Tmu_cfg );
	
	TMU_Start_Timer(T_LED,1000 , PERIODIC, 20); //task 1
	TMU_Start_Timer(T_LED_2,500 , PERIODIC, 30); // task 2

    while(1)
    {
		TMU_Dispatch();
		if (test>10)
		{
			//TMU_Stop_Timer(20);
			TMU_DeInit ( );
		}
    }
}


void T_LED(void)
{
	PORTC_DATA = ~PORTC_DATA;
	test++;
}

void T_LED_2(void)
{
	PORTD_DATA = ~PORTD_DATA;
	
}