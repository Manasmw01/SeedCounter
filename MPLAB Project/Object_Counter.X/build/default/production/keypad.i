# 1 "keypad.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.40\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "keypad.c" 2
# 42 "keypad.c"
# 1 "./keypad.h" 1
# 46 "./keypad.h"
# 1 "./gpio.h" 1




# 1 "./stdutils.h" 1
# 24 "./stdutils.h"
typedef signed char sint8_t;
typedef unsigned char uint8_t;

typedef signed int sint16_t;
typedef unsigned int uint16_t;

typedef signed long int sint32_t;
typedef unsigned long int uint32_t;
# 136 "./stdutils.h"
typedef enum
{
   E_FALSE,
   E_TRUE
}Boolean_et;


typedef enum
{
    E_FAILED,
    E_SUCCESS,
    E_BUSY,
    E_TIMEOUT
}Status_et;


typedef enum
{
 E_BINARY=2,
 E_DECIMAL = 10,
 E_HEX = 16
}NumericSystem_et;
# 6 "./gpio.h" 2


typedef enum
{
    PA_0,PA_1,PA_2,PA_3,PA_4,PA_5,PA_6,PA_7,
    PB_0,PB_1,PB_2,PB_3,PB_4,PB_5,PB_6,PB_7,
    PC_0,PC_1,PC_2,PC_3,PC_4,PC_5,PC_6,PC_7,
    PD_0,PD_1,PD_2,PD_3,PD_4,PD_5,PD_6,PD_7,
    PE_0,PE_1,PE_2,PE_3,PE_4,PE_5,PE_6,PE_7,
    P_NC = 0xff
}gpioPins_et;
# 26 "./gpio.h"
void GPIO_PinDirection(gpioPins_et enm_pinNumber, uint8_t var_pinDirn_u8);
void GPIO_PinWrite(gpioPins_et enm_pinNumber, uint8_t var_pinValue_u8);
uint8_t GPIO_PinRead(gpioPins_et enm_pinNumber);
# 47 "./keypad.h" 2
# 65 "./keypad.h"
uint8_t KEYPAD_GetKey();
void KEYPAD_Init(
        gpioPins_et row_0,
        gpioPins_et row_1,
        gpioPins_et row_2,
        gpioPins_et row_3,
        gpioPins_et col_0,
        gpioPins_et col_1,
        gpioPins_et col_2,
        gpioPins_et col_3 );
# 43 "keypad.c" 2
# 1 "./delay.h" 1
# 29 "./delay.h"
void DELAY_us(uint16_t us_count);
void DELAY_ms(uint16_t ms_count);
void DELAY_sec(uint16_t var_delaySecCount_u16);
# 44 "keypad.c" 2



gpioPins_et A_RowsPins_U8[(4)];
gpioPins_et A_ColsPins_U8[(4)];

const uint8_t A_KeyLookUptable_U8[(4)][(4)]=
{
     '0','1','2','3',
      '4','5','6','7',
      '8','9','A','B',
      'C','D','E','F'
};





static void keypad_WaitForKeyRelease(void);
static void keypad_WaitForKeyPress(void);
# 80 "keypad.c"
void KEYPAD_Init(
        gpioPins_et row_0,
        gpioPins_et row_1,
        gpioPins_et row_2,
        gpioPins_et row_3,
        gpioPins_et col_0,
        gpioPins_et col_1,
        gpioPins_et col_2,
        gpioPins_et col_3 )
{
    uint8_t i;

    A_RowsPins_U8[0] = row_0;
    A_RowsPins_U8[1] = row_1;
    A_RowsPins_U8[2] = row_2;
    A_RowsPins_U8[3] = row_3;

    A_ColsPins_U8[0] = col_0;
    A_ColsPins_U8[1] = col_1;
    A_ColsPins_U8[2] = col_2;
    A_ColsPins_U8[3] = col_3;

    for(i=0;i<(4);i++)
    {
        GPIO_PinDirection(A_RowsPins_U8[i],0x00u);
    }


    for(i=0;i<(4);i++)
    {
        GPIO_PinDirection(A_ColsPins_U8[i],0x01u);
    }
}
# 126 "keypad.c"
static void keypad_WaitForKeyRelease(void)
{

    uint8_t i,v_keyStatus_u8;

    for (i=0;i<(4);i++)
    {
        GPIO_PinWrite(A_RowsPins_U8[i],0x00u);
    }

    do
    {
        do
        {
            v_keyStatus_u8 = 1;
            for(i=0; i<(4); i++)
            {
                v_keyStatus_u8 &= GPIO_PinRead(A_ColsPins_U8[i]);
            }
        }while(v_keyStatus_u8 == 0);

        DELAY_us((10));

        v_keyStatus_u8 = 1;
        for(i=0; i<(4); i++)
        {
            v_keyStatus_u8 &= GPIO_PinRead(A_ColsPins_U8[i]);
        }
    }while(v_keyStatus_u8 == 0);
}
# 172 "keypad.c"
static void keypad_WaitForKeyPress(void)
{
    uint8_t i,v_keyStatus_u8;

    for (i=0;i<(4);i++)
    {
        GPIO_PinWrite(A_RowsPins_U8[i],0x00u);
    }

    do
    {
        do
        {
            v_keyStatus_u8 = 1;
            for(i=0; i<(4); i++)
            {
                v_keyStatus_u8 &= GPIO_PinRead(A_ColsPins_U8[i]);
            }
        }while(v_keyStatus_u8 != 0);


        DELAY_us((10));

        v_keyStatus_u8 = 1;
        for(i=0; i<(4); i++)
        {
            v_keyStatus_u8 &= GPIO_PinRead(A_ColsPins_U8[i]);
        }
    }while(v_keyStatus_u8 != 0);
}
# 227 "keypad.c"
uint8_t KEYPAD_GetKey(void)
{
    uint8_t i,j,v_KeyPressed_u8 = 0;





    for (i=0;i<(4);i++)
    {
        GPIO_PinWrite(A_RowsPins_U8[i],0x01u);
    }

    for (i=0;(i<(4));i++)
    {
        GPIO_PinWrite(A_RowsPins_U8[i],0x00u);



        for(j=0; (j<(4)); j++)
        {

           if(GPIO_PinRead(A_ColsPins_U8[j]) == 0)
            {
                v_KeyPressed_u8 = 1;
                break;
            }
        }

        if(v_KeyPressed_u8 ==1)
        {
            break;
        }

        GPIO_PinWrite(A_RowsPins_U8[i],0x01u);
    }

    if(i<(4))
        v_KeyPressed_u8 = A_KeyLookUptable_U8[i][j];
    else
        v_KeyPressed_u8 = 0;


    return v_KeyPressed_u8;
}
