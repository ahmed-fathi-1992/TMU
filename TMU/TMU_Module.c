/*
 * TMU_Module.c
 *
 * Created: 24/02/2020 17:40:28
 *  Author: FATHI
 */ 


/************************************************************************/
/*			          	 INCLUDES		                     	        */
/************************************************************************/

#include "TMU_Module.h"
#include "softwareDelay.h "

/************************************************************************/
/*			           Structures Definitions	                       */
/************************************************************************/

typedef struct TMU_Task_s
{
	TMU_funptr pfun_TMU;
	uint32_t Time_delay;
	uint8_t periodicity;
	uint32_t Counts;
	uint16_t ID;
	
}TMU_Task_s;


/************************************************************************/
/*                GLOBAL STATIC VARIABLES                           */
/************************************************************************/

static TMU_Task_s  gapstr_Tasks_Buffer[ MAX_NUM_OF_TASKS ] = {{0}};
static volatile	uint8_t gu8_TOV_Flag = 0;
static uint8_t gu8_TMU_Timer_Ch = TIMER_NO_CH ; 
static uint8_t gu8_Timer_start_flag = 0 ; 


/************************************************************************/
/*		         TIMER static FUNCTIONS' PROTOTYPES		        */
/************************************************************************/

static void T0_OV_Callback(void);
static EnmTMUError_t Start_Timer(void);

/************************************************************************/
/*		                       TMU FUNCTIONS	                        */
/************************************************************************/


/**
 * Input: Pointer to a structure contains the information needed to initialize the TMU. 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the module.
 * 							
 */
EnmTMUError_t TMU_Init (const TMU_ConfigType * ConfigPtr ) 
{
	EnmTMUError_t Error_Num = NOT_INIT;
	
		Timer_cfg_s Timer0_cfg= {TIMER_0,TIMER_MODE,TIMER_PRESCALER_64,TIMER_INTERRUPT_MODE,T0_OV_Callback};
		Timer_cfg_s Timer1_cfg= {TIMER_1,TIMER_MODE,TIMER_PRESCALER_64,TIMER_INTERRUPT_MODE,T0_OV_Callback};	
		Timer_cfg_s Timer2_cfg= {TIMER_2,TIMER_MODE,TIMER_PRESCALER_64,TIMER_INTERRUPT_MODE,T0_OV_Callback};

	if (ConfigPtr == NULL)
	{
		Error_Num = NULL_PTR;
	}else
	{
		switch (ConfigPtr->Timer_CH)
		{
			case TIMER_0:
			    {
					Timer_Init(&Timer0_cfg);
					gu8_TMU_Timer_Ch = TIMER_0 ;
					Error_Num = E_OK;
				}
			break;
			
			case TIMER_1:
			    {
					Timer_Init(&Timer1_cfg);
					gu8_TMU_Timer_Ch = TIMER_1 ; 
					Error_Num = E_OK;
				}
			break;
			
			case TIMER_2:
			    {
				   Timer_Init(&Timer2_cfg);
				   gu8_TMU_Timer_Ch = TIMER_2 ;
				   Error_Num = E_OK;
			    }
			break;
			
			default:
			Error_Num = INVALID_ARGUMENT;
			break;
		}
	}
	return Error_Num ;	
}

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function DeInit TMU .
 * 							
 */
EnmTMUError_t TMU_DeInit ( void )
{
	
return E_OK;	
} 

/**
 * Input: 
 *  	TMU_funptr: pointer to function need to be added to buffer
 * 	    delay: time delay  before execute the function 
 * 	    periodicity:  periodic or one shot time function
 * 	    ID:  define the task and must be unique 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function add new task to delay schedule .
 * 							
 */
EnmTMUError_t TMU_Start_Timer(TMU_funptr funcPtr ,uint32_t delay , uint8_t periodicity,uint8_t ID)
{
	EnmTMUError_t Error_Num = NOT_INIT; 
	
	uint8_t u8_index = 0;
	
if ( funcPtr == NULL )
{
		Error_Num = NULL_PTR;
}else
 {
    for (u8_index = 0; u8_index < MAX_NUM_OF_TASKS; u8_index++)
   {
    	if (gapstr_Tasks_Buffer[u8_index].ID == ID)
    	{
    		Error_Num = RE_START;
	    }
	
   }
 
 }

if (Error_Num == NOT_INIT)
{
		if(gu8_Timer_start_flag == 0)
		{
			gu8_Timer_start_flag = 1;
		     Start_Timer();
		}

		for (u8_index = 0; u8_index < MAX_NUM_OF_TASKS; u8_index++)
		{
			if (gapstr_Tasks_Buffer[u8_index].ID == 0)
			{
				gapstr_Tasks_Buffer[u8_index].pfun_TMU = funcPtr ;
				gapstr_Tasks_Buffer[u8_index].Time_delay = delay ;
				gapstr_Tasks_Buffer[u8_index].Counts = delay ;
				gapstr_Tasks_Buffer[u8_index].periodicity = periodicity ;
				gapstr_Tasks_Buffer[u8_index].ID = ID ;
				Error_Num = E_OK;
				break;
			}
		}
		
		if (u8_index == MAX_NUM_OF_TASKS)
		{
			Error_Num = BUFFER_FULL;
		}
		
} 
else
{
}
	

return Error_Num ;	
}

/**
 * Input: 
 * 	 ID: The ID of the task needed to be stop .
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function stop the task .
 * 							
 */
EnmTMUError_t TMU_Stop_Timer(uint8_t ID)
{
   	uint8_t u8_index = 0;
	EnmTMUError_t Error_Num = STOP_NO_START;
	
if (gu8_TMU_Timer_Ch == TIMER_NO_CH)
{
	Error_Num = NOT_INIT;
} 
else
{
		for ( u8_index = 0;  u8_index < MAX_NUM_OF_TASKS; u8_index++)
		{
			if (gapstr_Tasks_Buffer[u8_index].ID == ID)
			{
				gapstr_Tasks_Buffer[u8_index].ID = 0;
				Error_Num = E_OK;
				break;
			}
		}
		
}

	
 return Error_Num;	
}
/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function manage the tasks and execute every task in its time slot .
 * 							
 */
EnmTMUError_t TMU_Dispatch(void)
{
	EnmTMUError_t Error_Num = NOT_INIT ;
	
	uint8_t index = 0;
	
	if (gu8_TOV_Flag == 1 )
	{
		gu8_TOV_Flag = 0;
		
			for (index=0; index < MAX_NUM_OF_TASKS; index++)
			{
				if (gapstr_Tasks_Buffer[index].ID > 0)
				{					
					gapstr_Tasks_Buffer[index].Counts-- ;
					
					if (gapstr_Tasks_Buffer[index].Counts <= 0 )
					{
						
							   switch (gapstr_Tasks_Buffer[index].periodicity)
							   {
								   case PERIODIC:
								    {
		    						   gapstr_Tasks_Buffer[index].Counts = gapstr_Tasks_Buffer[index].Time_delay ;
			                           gapstr_Tasks_Buffer[index].pfun_TMU();									   
									}

								   break;
								   case ONE_SHOT:
								    {
									    gapstr_Tasks_Buffer[index].ID = 0 ;
									    gapstr_Tasks_Buffer[index].pfun_TMU();
								    }

								   break;

							       default:
								   Error_Num = INVALID_ARGUMENT;
								   break;
							   }
						
					}

				}
			}
		
	} 
	else
	{
	}
	
return Error_Num;	
}

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function used to start the timer one time with the start of the first task add to schedule  .
 * 							
 */

EnmTMUError_t Start_Timer(void)
{
	EnmTMUError_t Error_Num = NOT_INIT;
			
			switch (gu8_TMU_Timer_Ch)
			{
				case TIMER_0:
				{
					Timer_Start(TIMER_0, 6);// preloaded timer with 6 tick to count 250 tick and tick = 4 us so final timer resolution = 1000 us
					Error_Num = E_OK;
				}
				break;
				
				case TIMER_1:
				{
					Timer_Start(TIMER_1, (65536 - 250) );// preloaded timer with 65286 tick to count 250 tick and tick = 4 us so final timer resolution = 1000 us
					Error_Num = E_OK;
				}
				break;
				
				case TIMER_2:
				{
					Timer_Start(TIMER_2, 6);// preloaded timer with 6 tick to count 250 tick and tick = 4 us so final timer resolution = 1000 us
					Error_Num = E_OK;
				}
				break;
				
				default:
				Error_Num = INVALID_ARGUMENT;
				break;
			}
		return Error_Num ;		
}


/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: 			
 * Description: This is the callBack function used to set a flag every timer overflow to make periodic interrupt every 1 ms .
 * 							
 */
static void T0_OV_Callback(void)
{
 Timer_Start(TIMER_0, 6); // preloaded timer with 6 tick to count 250 tick and tick = 4 us so final timer resolution = 1000 us
 gu8_TOV_Flag =1 ;
 PORTB_DATA = ~PORTB_DATA;	//for debug	
}




