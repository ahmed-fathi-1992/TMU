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
/*			         static Structures Definitions	                       */
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
//static uint8_t gu8_TMU_Init_flag = 0 ;

/************************************************************************/
/*		            TMU static FUNCTIONS' PROTOTYPES		        */
/************************************************************************/

static void T0_OV_Callback(void);
static EnmTMUError_t Start_Timer(void);


/************************************************************************/
/*		                       TMU FUNCTIONS	                        */
/************************************************************************/



EnmTMUError_t TMU_Init (const TMU_ConfigType * ConfigPtr ) 
{
	EnmTMUError_t Error_Num = NOT_INIT;
	
		Timer_cfg_s Timer0_cfg= {TIMER_0,TIMER_MODE,TIMER_PRESCALER_64,TIMER_INTERRUPT_MODE,T0_OV_Callback};
		Timer_cfg_s Timer1_cfg= {TIMER_1,TIMER_MODE,TIMER_PRESCALER_64,TIMER_INTERRUPT_MODE,T0_OV_Callback};	
		Timer_cfg_s Timer2_cfg= {TIMER_2,TIMER_MODE,TIMER_PRESCALER_64,TIMER_INTERRUPT_MODE,T0_OV_Callback};
			
if (gu8_TMU_Timer_Ch != TIMER_NO_CH) // if TMU already initialized 
{
	Error_Num = RE_INIT;
} 
else
{

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
					gu8_TMU_Timer_Ch = TIMER_0 ; //save the timer channel to start its timer later in statr_timer function
					Error_Num = E_OK;
				}
			break;
			
			case TIMER_1:
			    {
					Timer_Init(&Timer1_cfg);
					gu8_TMU_Timer_Ch = TIMER_1 ; //save the timer channel to start its timer later in statr_timer function
					Error_Num = E_OK;
				}
			break;
			
			case TIMER_2:
			    {
				   Timer_Init(&Timer2_cfg);
				   gu8_TMU_Timer_Ch = TIMER_2 ;//save the timer channel to start its timer later in statr_timer function
				   Error_Num = E_OK;
			    }
			break;
			
			default:
			Error_Num = INVALID_ARGUMENT;
			break;
		}
	}
}
	return Error_Num ;	
}


EnmTMUError_t TMU_DeInit ( void )
{
	EnmTMUError_t Error_Num = NOT_INIT;
		
		uint8_t u8_index = 0;
		
		Timer_cfg_s Timer0_cfg= {TIMER_0,TIMER_MODE,TIMER_NO_CLOCK,TIMER_POLLING_MODE,NULL};// initialize disable configuration struct for timer 0 
		Timer_cfg_s Timer1_cfg= {TIMER_1,TIMER_MODE,TIMER_NO_CLOCK,TIMER_POLLING_MODE,NULL};// initialize disable configuration struct for timer 1
		Timer_cfg_s Timer2_cfg= {TIMER_2,TIMER_MODE,TIMER_NO_CLOCK,TIMER_POLLING_MODE,NULL};// initialize disable configuration struct for timer 2

	if (gu8_TMU_Timer_Ch == TIMER_NO_CH) // if TMU not initialized
	{
		Error_Num = NOT_INIT;
	} 
	else
	{
	
			switch (gu8_TMU_Timer_Ch)
			{
				case TIMER_0:
				{					
					Timer_Stop(gu8_TMU_Timer_Ch);
					Timer_Init(&Timer0_cfg);
					gu8_TMU_Timer_Ch = TIMER_NO_CH ;         // reset initialization channel 
					gu8_Timer_start_flag = 0;               // reset start flag
					  for (u8_index = 0; u8_index < MAX_NUM_OF_TASKS; u8_index++)// clear the buffer
					    { gapstr_Tasks_Buffer[u8_index].ID = 0;}		    
					Error_Num = E_OK;
				}
				break;
				
				case TIMER_1:
				{
					Timer_Stop(gu8_TMU_Timer_Ch);
					Timer_Init(&Timer1_cfg);
					gu8_TMU_Timer_Ch = TIMER_NO_CH ;        // reset initialization channel 
					gu8_Timer_start_flag = 0;              // reset start flag
					 for (u8_index = 0; u8_index < MAX_NUM_OF_TASKS; u8_index++)      // clear the buffer
						 { gapstr_Tasks_Buffer[u8_index].ID = 0;}
					Error_Num = E_OK;
				}
				break;
				
				case TIMER_2:
				{
					Timer_Stop(gu8_TMU_Timer_Ch);
					Timer_Init(&Timer2_cfg);
					gu8_TMU_Timer_Ch = TIMER_NO_CH ;    // reset initialization channel 
					gu8_Timer_start_flag = 0;          // reset start flag
					 for (u8_index = 0; u8_index < MAX_NUM_OF_TASKS; u8_index++)// clear the buffer
					     { gapstr_Tasks_Buffer[u8_index].ID = 0;}					
					Error_Num = E_OK;
				}
				break;
				
				default:
				Error_Num = NOT_INIT;
				break;
			}
	}
return Error_Num;	
} 


EnmTMUError_t TMU_Start_Timer(TMU_funptr funcPtr ,uint32_t delay , uint8_t periodicity,uint8_t ID)
{
	EnmTMUError_t Error_Num = INVALID_ARGUMENT; 
	
	uint8_t u8_index = 0;
	
if ( funcPtr == NULL )
{
		Error_Num = NULL_PTR;
}else
 {
	 if (gu8_TMU_Timer_Ch == TIMER_NO_CH)  // if TMU not initialized
	 {
		 Error_Num = NOT_INIT;
	 } 
	 else
	 {
		     for (u8_index = 0; u8_index < MAX_NUM_OF_TASKS; u8_index++)
		     {
			     if (gapstr_Tasks_Buffer[u8_index].ID == ID) // if ID duplicated  means re-start task error  
			     {
				     Error_Num = RE_START;
			     }
			     
		     }
	 }

 
 }

if (Error_Num == INVALID_ARGUMENT)
{
		if(gu8_Timer_start_flag == 0) // if it is the first task added in the schedule 
		{
			gu8_Timer_start_flag = 1;
		     Start_Timer(); 
		}
		
		if (ID == 0)
		{
			Error_Num = INVALID_ARGUMENT;
		} 
		else
		{
      		for (u8_index = 0; u8_index < MAX_NUM_OF_TASKS; u8_index++)
      		{
	      		if (gapstr_Tasks_Buffer[u8_index].ID == 0) // add the task in the first free array slot
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
			
		


		
} 
else
{
}
	
if (Error_Num != E_OK)
{
	gu8_Timer_start_flag = 0;
} 

return Error_Num ;	
}


EnmTMUError_t TMU_Stop_Timer(uint8_t ID)
{
   	uint8_t u8_index = 0;
	EnmTMUError_t Error_Num = STOP_NO_START;
	
if (gu8_TMU_Timer_Ch == TIMER_NO_CH) // if TMU not initialized  
{
	Error_Num = NOT_INIT;
} 
else
{
		for ( u8_index = 0;  u8_index < MAX_NUM_OF_TASKS; u8_index++)
		{
			if (gapstr_Tasks_Buffer[u8_index].ID == ID)
			{
				gapstr_Tasks_Buffer[u8_index].ID = 0; // delete the task from the schedule 
				Error_Num = E_OK;
				break;
			}
		}
		
}

	
 return Error_Num;	
}



EnmTMUError_t TMU_Dispatch(void)
{
	EnmTMUError_t Error_Num = NOT_INIT ;
	
	uint8_t index = 0;
	
	if (gu8_TOV_Flag == 1 ) // if timer event occurred 
	{
		gu8_TOV_Flag = 0; // reset timer event flag
		
			for (index=0; index < MAX_NUM_OF_TASKS; index++)
			{
				if (gapstr_Tasks_Buffer[index].ID > 0) // if the task is active NOTE: if task ID = 0 it means it is inactive
				{					
					gapstr_Tasks_Buffer[index].Counts-- ;
					
					if (gapstr_Tasks_Buffer[index].Counts <= 0 ) // delay ran out and time to execute 
					{
						
							   switch (gapstr_Tasks_Buffer[index].periodicity)
							   {
								   case PERIODIC:
								    {
		    						   gapstr_Tasks_Buffer[index].Counts = gapstr_Tasks_Buffer[index].Time_delay ; // reload to make another delay period 
			                           gapstr_Tasks_Buffer[index].pfun_TMU();									   
									}

								   break;
								   case ONE_SHOT:
								    {
									    gapstr_Tasks_Buffer[index].ID = 0 ; // delete the task from the schedule 
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


static void T0_OV_Callback(void)
{
 Timer_Start(gu8_TMU_Timer_Ch, 6); // preloaded timer with 6 tick to count 250 tick and tick = 4 us so final timer resolution = 1000 us
 gu8_TOV_Flag =1 ;
}




