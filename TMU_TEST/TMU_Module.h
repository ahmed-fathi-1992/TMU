/*
 * TMU_Module.h
 *
 * Created: 24/02/2020 17:40:45
 *  Author: FATHI
 */


#ifndef TMU_MODULE_H_
#define TMU_MODULE_H_



/************************************************************************/
/*				 INCLUDES			        */
/************************************************************************/

#include "TMU_Config.h"
#include "std_types.h"
#include "Timer.h"
#include "Error_Codes.h"

/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/



#define PERIODIC    1
#define ONE_SHOT    0

/************************************************************************/
/*                   typedefs for Timer                        */
/************************************************************************/

typedef void (*TMU_funptr) (void);


/************************************************************************/
/*			  Structures Definitions		                            */
/************************************************************************/

typedef struct TMU_ConfigType
{
	uint8_t Timer_CH;
	//uint8_t Resolution;
}TMU_ConfigType;


/************************************************************************/
/*		         TMU FUNCTIONS' PROTOTYPES		        */
/************************************************************************/


/**
 * Input: Pointer to a structure contains the information needed to initialize the TMU.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module.
 *
 */

EnmTMUError_t TMU_Init (const TMU_ConfigType * ConfigPtr ) ;

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function DeInit TMU .
 *
 */
EnmTMUError_t TMU_DeInit ( void );

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

EnmTMUError_t TMU_Start_Timer(TMU_funptr funcPtr ,uint32_t delay , uint8_t periodicity,uint8_t ID);

/**
 * Input:
 * 	 ID: The ID of the task needed to be stop .
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stop the task .
 *
 */
EnmTMUError_t TMU_Stop_Timer(uint8_t ID);
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function manage the tasks and execute every task in its time slot .
 *
 */
EnmTMUError_t TMU_Dispatch(void);




#endif /* TMU_MODULE_H_ */
