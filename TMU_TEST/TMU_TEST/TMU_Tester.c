#include "TMU_Module.h"

ERROR_STATUS Test1()
{
    TMU_ConfigType TMU1;
    TMU1.Timer_CH = TIMER_0;
    return TMU_Init(NULL);
}
ERROR_STATUS Test2()
{
    TMU_ConfigType TMU1;
    TMU1.Timer_CH = TIMER_0;
    return TMU_Init(&TMU1);
}
ERROR_STATUS Test3()
{
    TMU_ConfigType TMU1;
    TMU1.Timer_CH = 6;
    return TMU_Init(&TMU1);
}

ERROR_STATUS Test4()
{
    TMU_ConfigType TMU1;
    TMU1.Timer_CH = TIMER_0;
    TMU_Init(&TMU1);
    return TMU_Start_Timer(NULL, 100, PERIODIC, 50);
}

void Toggle(void){}
ERROR_STATUS Test5()
{
    TMU_ConfigType TMU1;
    TMU1.Timer_CH = TIMER_0;
    TMU_Init(&TMU1);
    return TMU_Start_Timer(Toggle, 100, PERIODIC, 50);
}

ERROR_STATUS Test6()
{
    TMU_ConfigType TMU1;
    TMU1.Timer_CH = TIMER_0;
    TMU_Init(&TMU1);
    return TMU_Start_Timer(Toggle, 100, ONE_SHOT, 50);
}

// failed
ERROR_STATUS Test7()
{
    TMU_ConfigType TMU1;
    TMU1.Timer_CH = TIMER_0;
    TMU_Init(&TMU1);
    return TMU_Start_Timer(Toggle, 100, 5, 50);
}



ERROR_STATUS Test8()
{
    TMU_ConfigType TMU1;
    TMU1.Timer_CH = TIMER_0;
    TMU_Init(&TMU1);
    TMU_Start_Timer(Toggle, 100, PERIODIC, 50);
    return TMU_Start_Timer(Toggle, 100, PERIODIC, 50);
}

ERROR_STATUS Test9()
{
    TMU_ConfigType TMU1;
    TMU1.Timer_CH = TIMER_0;
    TMU_Init(&TMU1);
    TMU_Start_Timer(Toggle, 100, PERIODIC, 51);
    TMU_Start_Timer(Toggle, 100, PERIODIC, 52);
    TMU_Start_Timer(Toggle, 100, PERIODIC, 53);
    return TMU_Start_Timer(Toggle, 100, PERIODIC, 55);
}

ERROR_STATUS Test10()
{
    TMU_ConfigType TMU1;
    TMU1.Timer_CH = TIMER_0;
    TMU_Init(&TMU1);
    TMU_Start_Timer(Toggle, 100, PERIODIC, 50);
    return TMU_Stop_Timer(50);
}

ERROR_STATUS Test11()
{
    TMU_ConfigType TMU1;
    TMU1.Timer_CH = TIMER_0;
    TMU_Init(&TMU1);
    return TMU_Stop_Timer(50);
}


ERROR_STATUS Test12()
{
    TMU_ConfigType TMU1;
    TMU1.Timer_CH = TIMER_0;
    TMU_Init(&TMU1);
    return TMU_DeInit();
}

ERROR_STATUS Test13()
{
    TMU_ConfigType TMU1;
    TMU1.Timer_CH = TIMER_0;
    //TMU_Init(&TMU1);
    return TMU_DeInit();
}



