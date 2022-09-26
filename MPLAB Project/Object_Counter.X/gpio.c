


#include <pic16f874.h>
#include "stdutils.h"
#include "gpio.h"

#pragma warning push
#pragma warning disable 752
#pragma warning disable 1496




/***************************************************************************************************
                  void GPIO_PinDirection(gpioPins_et enm_pinNumber, uint8_t var_pinDirn_u8)
 ***************************************************************************************************
 * I/P Arguments: 
                gpioPins_et: pin number which needs to be updated.
                             Refer the gpio.h for pin numbering.
                gpioDirection_et: Direction to be configured ie. INPUT/OUTPUT.
                              

 * Return value    : none

 * description :This function sets the specified direction as INPUT/OUTPUT.  
 ***************************************************************************************************/
void GPIO_PinDirection(gpioPins_et enm_pinNumber, uint8_t var_pinDirn_u8)
{

    uint8_t var_portNumber_u8;


    var_portNumber_u8 =  (enm_pinNumber>>3);  // Divide the pin number by 8 go get the PORT number
    enm_pinNumber  =   enm_pinNumber & 0x07;  // lower 3-bits contains the bit number of a byte 

    /* Go to particular port after decoding from the pin number and 
        set the direction as specified*/
    switch(var_portNumber_u8)
    {
       
    case 0:
        util_UpdateBit(TRISA,enm_pinNumber,var_pinDirn_u8);
        break;
       


    case 1:
        util_UpdateBit(TRISB,enm_pinNumber,var_pinDirn_u8);
        break;

       

    case 2:
        util_UpdateBit(TRISC,enm_pinNumber,var_pinDirn_u8);
        break;

        
        
    case 3:
        util_UpdateBit(TRISD,enm_pinNumber,var_pinDirn_u8);
        break;
     

    case 4:
        util_UpdateBit(TRISE,enm_pinNumber,var_pinDirn_u8);
        break;
  
    }                
}









/***************************************************************************************************
            void GPIO_PinWrite(gpioPins_et enm_pinNumber, uint8_t var_pinValue_u8)
 ***************************************************************************************************
 * I/P Arguments: 
                gpioPins_et: pin number which needs to be updated.
                             Refer the gpio.h for pin numbering.
                gpioValue_et: Value to be updated(LOW/HIGH) on the selected pin.
                              

 * Return value    : none

 * description :This function updates the specified value on the selected pin.  
                Before updating the pins status, the pin function should be selected and then
                the pin should be configured as OUTPUT 
***************************************************************************************************/
void GPIO_PinWrite(gpioPins_et enm_pinNumber, uint8_t var_pinValue_u8)
{

    uint8_t var_portNumber_u8;


    var_portNumber_u8 =  (enm_pinNumber>>3);  // Divide the pin number by 8 go get the PORT number
    enm_pinNumber  =   enm_pinNumber & 0x07;  // lower 3-bits contains the bit number of a byte 

    /* Go to particular port after decoding from the pin number and 
        update the value of the specified pin*/
    switch(var_portNumber_u8)
    {
       
    case 0:
        util_UpdateBit(PORTA,enm_pinNumber,var_pinValue_u8);
        break;
       
        

    case 1:
        util_UpdateBit(PORTB,enm_pinNumber,var_pinValue_u8);
        break;
       
        
       
    case 2:
        util_UpdateBit(PORTC,enm_pinNumber,var_pinValue_u8);
        break;
        
       
        
    case 3:
        util_UpdateBit(PORTD,enm_pinNumber,var_pinValue_u8);
        break;
   


    case 4:
        util_UpdateBit(PORTE,enm_pinNumber,var_pinValue_u8);
        break;
             
    }                
}








/***************************************************************************************************
                    uint8_t GPIO_PinRead(gpioPins_et enm_pinNumber)
 ***************************************************************************************************
 * I/P Arguments: 
                gpioPins_et: pin number which needs to be read.
                             Refer the gpio.h for pin numbering.

 * Return value    : 
                 uint8_t:    Status of the specified pin.

 * description :This function returns the status of the selected pin.
                Before reading the pins status, the pin should be configured as INPUT 
 ***************************************************************************************************/
uint8_t GPIO_PinRead(gpioPins_et enm_pinNumber)
{
    uint8_t var_portNumber_u8;
    uint8_t returnStatus = 0;

    var_portNumber_u8 =  (enm_pinNumber>>3); // Divide the pin number by 8 go get the PORT number
    enm_pinNumber  =     enm_pinNumber & 0x07; // lower 3-bits contains the bit number of a byte 

    /* Go to particular port after decoding from the pin number and read the pins status */
    switch(var_portNumber_u8)     
    { 
   
    case 0: 
        returnStatus = util_GetBitStatus(PORTA,enm_pinNumber);
        break;
        

    case 1:
        returnStatus = util_GetBitStatus(PORTB,enm_pinNumber);
        break;
         

    case 2:
        returnStatus = util_GetBitStatus(PORTC,enm_pinNumber);
        break;
       
                

    case 3:
        returnStatus = util_GetBitStatus(PORTD,enm_pinNumber);
        break;
  


    case 4: 
        returnStatus = util_GetBitStatus(PORTE,enm_pinNumber);
        break;                   
    }                         
    return returnStatus;
}

#pragma warning pop