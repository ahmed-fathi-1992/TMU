/*
 * DIO.c
 *
 * Created: 17/02/2020 01:03:13
 *  Author: FATHI
 */ 

/************************************************************************/
/*				 INCLUDES			        */
/************************************************************************/

#include "DIO.h"


/************************************************************************/
/*		         DIO FUNCTIONS		        */
/************************************************************************/


/*
*Input: DIO_Cfg_s -> to get PORT name, pins to be initiated and the required direction
*Output: No output
*In/Out:
*Description: This function can set the direction of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_init (DIO_Cfg_s *DIO_info)
{
	if (DIO_info == NULL)
	{
		return E_NOK;
	}else
	{
		switch (DIO_info->GPIO)
		{
			case GPIOA:
			PORTA_DIR = PORTA_DIR & (~DIO_info->pins);
			PORTA_DIR = PORTA_DIR |(DIO_info->pins & DIO_info->dir);
			break;
			case GPIOB:
			PORTB_DIR = PORTB_DIR & (~DIO_info->pins );
			PORTB_DIR = PORTB_DIR |(DIO_info->pins  & DIO_info->dir);
			break;
			case GPIOC:
			PORTC_DIR = PORTC_DIR & (~DIO_info->pins );
			PORTC_DIR = PORTC_DIR |(DIO_info->pins  & DIO_info->dir);
			break;
			case GPIOD:
			PORTD_DIR = PORTD_DIR & (~DIO_info->pins );
			PORTD_DIR = PORTD_DIR |(DIO_info->pins  & DIO_info->dir);
			break;
			default:
			return E_NOK;
			break;
		}	
	}
	return E_OK;
}


/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*		value 	-> The desired value
*					- HIGH
*					- LOW
*Output: No output
*In/Out: No In/Out
*Description: This function can set the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Write (uint8_t GPIO, uint8_t pins, uint8_t value)
{
		switch (GPIO)
		{
			case GPIOA:
			PORTA_DATA = PORTA_DATA & (~pins);
			PORTA_DATA = PORTA_DATA |(pins & value);
			break;
			case GPIOB:
			PORTB_DATA = PORTB_DATA & (~pins );
			PORTB_DATA = PORTB_DATA |(pins  & value);
			break;
			case GPIOC:
			PORTC_DATA = PORTC_DATA & (~pins );
			PORTC_DATA = PORTC_DATA |(pins  & value);
			break;
			case GPIOD:
			PORTD_DATA = PORTD_DATA & (~pins );
			PORTD_DATA = PORTD_DATA |(pins  & value);
			break;
			default:
			return E_NOK;
			break;
		}
		
		return E_OK;	
	
}

/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*		value 	-> The desired value
*					- HIGH
*					- LOW
*Output: data -> the acquired data wether it was PORT data or pins data
*In/Out:
*Description: This function gets the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Read (uint8_t GPIO,uint8_t pins, uint8_t *data)
{
	if (data == NULL)
	{
		return E_NOK;
	}else
	{		switch (GPIO)
		{
			case GPIOA:
			if(PORTA_PIN & pins )
			 *data = TRUE ; 
			 else
			 *data = FALSE ; 
			break;
			
			case GPIOB:
			if(PORTB_PIN & pins )
			*data = TRUE ; 
			else
			*data = FALSE ; 
			break;
			
			case GPIOC:
			if( PORTC_PIN & pins)
			*data = TRUE ;
			else
			*data = FALSE ;  
			break;
			
			case GPIOD:
			if( PORTD_PIN & pins)
			*data = TRUE ; 
			else
			*data = FALSE ; 
			break;
			
			default:
			return E_NOK;
			break;
		}
		
	}
		return E_OK;
	
	
}

/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*Output: data -> No output
*In/Out:
*Description: This function toggles the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Toggle (uint8_t GPIO, uint8_t pins)
{
		switch (GPIO)
		{
			case GPIOA:
			PORTA_DATA = PORTA_DATA ^ (pins);
			break;
			case GPIOB:
			PORTB_DATA = PORTB_DATA ^ (pins);
			break;
			case GPIOC:
			PORTC_DATA = PORTC_DATA ^ (pins);
			break;
			case GPIOD:
			PORTD_DATA = PORTD_DATA ^ (pins);
			break;
			default:
			return E_NOK;
			break;
		}
		
		return E_OK;
}
