/*
 * File:   main.c
 * Author: Manu
 *
 * Created on 18 May, 2022, 2:24 PM
 */
// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config CP = OFF         // FLASH Program Memory Code Protection bits (Code protection off)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF         // Low Voltage In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection (Code Protection off)
#pragma config WRT = ON         // FLASH Program Memory Write Enable (Unprotected program memory may be written to by EECON control)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
//
//#include <xc.h>
////#include <htc.h>
//#define _XTAL_FREQ 8000000
//void main()
//{
//  TRISE=0X00;
//  PORTE=0X00;
//  while(1)
//  { 
//    PORTE=0XFF;
//    __delay_ms(1000);
//    PORTE=0X00;
//    __delay_ms(1000);
//  }
//}

#include <xc.h>
#include <pic16f874.h>
#include "keypad.h"
#include "gpio.h"
#include "stdutils.h"
#include "uart.h"

//#define SegOne   0x02
//#define SegTwo   0x04
//#define SegThree 0x08
//#define SegFour  0x10	
//#define SegFive  0x20

#define SegOne   0x02
#define SegTwo   0x04
#define SegThree 0x08
#define SegFour  0x10	
#define SegFive  0x20
#define Sensor_Input RB0
#define Relay RE0
#define LED   RE1
#define OFF 0
#define ON  1

#define SETVAL  '3' //7
#define START   '3' //8
#define RESET_1   '3' //9
#define READVAL '3' ///
#define UP      '3' //4
#define DOWN    '3' //5
#define RIGHT   '3' //6
#define LEFT    '3' //*

//#define SETVAL  '8' //7
//#define START   '2' //8
//#define RESET_1   'F' //9
//#define READVAL '9' ///
//#define UP      '0' //4
//#define DOWN    '1' //5
//#define RIGHT   '5' //6
//#define LEFT    '4' //*


#define _XTAL_FREQ 8000000
#define F_CPU 8000000UL
#define M_GetBaudRateGeneratorValue(baudrate)  (uint32_t)((F_CPU / (64*(uint32_t)baudrate)) - 1)

unsigned long cnt=0,set_cnt=0;
unsigned int timer0_cnt,seg_blink_cnt,seg_set;
uint8_t RegValue,get_key_f,counter_mode1,counter_mode2,setval_mode,blink_seg_f;

void DELAY_ms1(unsigned int ms_Count) 
{
    unsigned int i, j;
    for (i = 0; i < ms_Count; i++) 
    {
        for (j = 0; j < 100; j++);
    }
}
//void UART_TxChar(char v_uartData_u8)
//{
//    while(TXIF==0);       // Wait till the transmitter register becomes empty
//    TXIF=0;               // Clear transmitter flag
//    TXREG=v_uartData_u8;  // load the char to be transmitted into transmit reg
//}
//void UART_SetBaudRate()
//{
//    uint8_t RegValue;
//    RegValue = (uint8_t)M_GetBaudRateGeneratorValue(9600);
//    SPBRG = RegValue; 
//}
//void UART_Init()
//{    
//    TRISC=0x80;      // Configure Rx pin as input and Tx as output  
//    TXSTA=0x20;      // Asynchronous mode, 8-bit data & enable transmitter
//    RCSTA=0x90;      // 8-bit continous receive enable
//    UART_SetBaudRate();
//}


int main() {
   // char seg_code[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
     char seg_code[] = {0x02,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09};
    int   temp,i,j;
    unsigned long num;
    unsigned char key,status=0;
    
    
    
    /* Configure the ports as output */
    TRISB = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    ADCON1 = 0x06;
    
    Relay=OFF;
    LED=OFF; //off
   // GPIO_PinWrite(PE_0,HIGH);
   // GPIO_PinWrite(PE_1,HIGH);
    
    KEYPAD_Init(PC_0,PC_1,PC_2,PC_3,PA_0,PA_1,PA_2,PA_3);
   // UART_Init();  // Initialize UARt at 9600 baud rate
   // UART_SetBaudRate();
    UART_Init(9600);
    DELAY_ms1(10);
    
    TRISBbits.TRISB0 =0;
    OPTION_REGbits.INTEDG=0;//falling edge
    INTCONbits.INTE=0;
    
    
    //setting for Timer0 interrupt 20ms
    OPTION_REGbits.PS0=1;
    OPTION_REGbits.PS1=1;
    OPTION_REGbits.PS2=1;
    OPTION_REGbits.PSA=0;
    OPTION_REGbits.T0CS=0;
    INTCONbits.PEIE=1;
    INTCONbits.T0IE=1;
    TMR0=100;  
    
    
    
    
   // INTCONbits.INTE=0;
    INTCONbits.GIE=1;
    while (1) 
    {
      
            //UART_TxNumber(C_DECIMAL_U8,cnt,5);
           // UART_TxChar('\r');
            //UART_TxChar('\n');
        if(counter_mode1==0&&counter_mode2==0)
        {
            while(1)
            {
                if(get_key_f==1)
                {
                    key = KEYPAD_GetKey();
                    get_key_f=0;
                    if((key>=0x30&&key<=0x39)||(key>=0x41&&key<=0x46))
                    {
                        UART_TxChar(key);
                        UART_TxChar('\r');
                        UART_TxChar('\n');
                    }
                }
               /* PORTB = SegOne; 
                PORTD = 0x00;               
                DELAY_ms1(1);
                PORTB = SegTwo; 
                PORTD =0x00;               
                DELAY_ms1(1);
                PORTB = SegThree; 
                PORTD = 0x00;               
                DELAY_ms1(1);
                PORTB = SegFour; 
                PORTD = 0x00;               
                DELAY_ms1(1);
                PORTB = SegFive; 
                PORTD = 0x00;               
                DELAY_ms1(1);*/
               // cnt=0;
                num = cnt;
                temp = num / 10000;
                num = num % 10000;
                PORTB = SegOne; 
                PORTD = seg_code[temp];               
                DELAY_ms1(1);

                temp = num / 1000;
                num = num % 1000;
                PORTB = SegTwo; 
                PORTD = seg_code[temp];
                DELAY_ms1(1);

                temp = num / 100;
                num = num % 100;
                PORTB = SegThree;
                PORTD = seg_code[temp];
                DELAY_ms1(1);

                temp = num / 10;
                PORTB = SegFour;
                PORTD = seg_code[temp];
                DELAY_ms1(1);    

                temp = num % 10;
                PORTB = SegFive;
                PORTD = seg_code[temp];
                DELAY_ms1(1);
                if(key==START)
                {
                    TRISBbits.TRISB0 =1;
                    INTCONbits.INTE=1;
                    cnt=0;
                    LED=ON;
                    Relay=ON;
                    counter_mode1=1;
                    break;
                }
                if(key==SETVAL)
                {
                    TRISBbits.TRISB0 =0;
                    INTCONbits.INTE=0;
                    cnt=0;
                    GPIO_PinWrite(Relay,HIGH);
                    GPIO_PinWrite(LED,HIGH);
                    setval_mode=1;
                    seg_set=1;
                    break;
                }
            }
        }
            
        if(counter_mode1==1)
        {   
            while(1)
            {
                if(get_key_f==1)
                {
                    key = KEYPAD_GetKey();
                    get_key_f=0;
                    if((key>=0x30&&key<=0x39)||(key>=0x41&&key<=0x46))
                    {
                        UART_TxChar(key);
                        UART_TxChar('\r');
                        UART_TxChar('\n');
                    }
                }

                num = cnt;
                temp = num / 10000;
                num = num % 10000;
                PORTB = SegOne; 
                PORTD = seg_code[temp];               
                DELAY_ms1(1);

                temp = num / 1000;
                num = num % 1000;
                PORTB = SegTwo; 
                PORTD = seg_code[temp];
                DELAY_ms1(1);

                temp = num / 100;
                num = num % 100;
                PORTB = SegThree;
                PORTD = seg_code[temp];
                DELAY_ms1(1);

                temp = num / 10;
                PORTB = SegFour;
                PORTD = seg_code[temp];
                DELAY_ms1(1);    

                temp = num % 10;
                PORTB = SegFive;
                PORTD = seg_code[temp];
                DELAY_ms1(1);
                if(key==RESET_1)
                {
                    INTCONbits.INTE=0;
                    TRISBbits.TRISB0 =0;
                    cnt=0;
                    LED=OFF;
                    Relay=OFF;
                    counter_mode1=0;
                    counter_mode2=0;
                    break;
                }
                
            }    
        }
        if(counter_mode2==1)
        {   
            while(1)
            {
                if(get_key_f==1)
                {
                    key = KEYPAD_GetKey();
                    get_key_f=0;
                    if((key>=0x30&&key<=0x39)||(key>=0x41&&key<=0x46))
                    {
                        UART_TxChar(key);
                        UART_TxChar('\r');
                        UART_TxChar('\n');
                    }
                }
                
                if(cnt>=set_cnt)
                {
                    
                    TRISBbits.TRISB0 =0;
                    INTCONbits.INTE=0;
                    
                    //cnt=0;
                    set_cnt=0;
                    LED=OFF;
                    Relay=OFF;
                    counter_mode1=0;
                    counter_mode2=0;
                    INTCONbits.INTE=0;
                  //  break;
                }

                num = cnt;
                temp = num / 10000;
                num = num % 10000;
                PORTB = SegOne; 
                PORTD = seg_code[temp];               
                DELAY_ms1(1);

                temp = num / 1000;
                num = num % 1000;
                PORTB = SegTwo; 
                PORTD = seg_code[temp];
                DELAY_ms1(1);

                temp = num / 100;
                num = num % 100;
                PORTB = SegThree;
                PORTD = seg_code[temp];
                DELAY_ms1(1);

                temp = num / 10;
                PORTB = SegFour;
                PORTD = seg_code[temp];
                DELAY_ms1(1);    

                temp = num % 10;
                PORTB = SegFive;
                PORTD = seg_code[temp];
                DELAY_ms1(1);
                if(key==RESET_1)
                {
                    INTCONbits.INTE=0;
                    INTCONbits.INTF=0;
                    TRISBbits.TRISB0 =0;
                    cnt=0;
                    set_cnt=0;
                    LED=OFF;
                    Relay=OFF;
                    counter_mode1=0;
                    counter_mode2=0;
                    break;
                }
                
                
            }    
        }
        if(setval_mode==1)
        {   
            while(1)
            {
                if(get_key_f==1)
                {
                    key = KEYPAD_GetKey();
                    get_key_f=0;
                    if((key>=0x30&&key<=0x39)||(key>=0x41&&key<=0x46))
                    {
                        UART_TxChar(key);
                        UART_TxChar('\r');
                        UART_TxChar('\n');
                    }
                    if(key==RIGHT)    
                    {
                        if(seg_set==1)
                        {}
                        else
                        seg_set--; 
                    }
                    if(key==LEFT)    
                    {
                        if(seg_set==5)
                        {}
                        else
                        seg_set++; 
                    }
                    if(key==UP)
                    {
                        if(seg_set==1)
                        {
                            if(set_cnt<99999)
                            set_cnt++;
                            
                        }
                        if(seg_set==2)
                        {
                            if(set_cnt<99990)
                            set_cnt=set_cnt+10;
                        }
                        if(seg_set==3)
                        {
                            if(set_cnt<99900)
                            set_cnt=set_cnt+100;
                        }
                        if(seg_set==4)
                        {
                             if(set_cnt<99000)
                            set_cnt=set_cnt+1000;                        
                        }
                        if(seg_set==5)
                        {
                            if(set_cnt<90000)
                            set_cnt=set_cnt+10000;                          
                        }                                                 
                    }
                    if(key==DOWN)
                    {
                        if(seg_set==1)
                        {   
                            if(set_cnt>0)
                            set_cnt--;
                            else
                            {}
                        }
                        if(seg_set==2)
                        {
                            if(set_cnt>10)
                            set_cnt=set_cnt-10;
                            else
                            {}
                        }
                        if(seg_set==3)
                        {
                            if(set_cnt>100)
                            set_cnt=set_cnt-100;
                        }
                        if(seg_set==4)
                        {
                            if(set_cnt>1000)
                            set_cnt=set_cnt-1000;                        
                        }
                        if(seg_set==5)
                        {
                            if(set_cnt>10000)
                            set_cnt=set_cnt-10000;                          
                        }                                                 
                    }
                    if(key==START)
                    {
                        INTCONbits.INTE=1;
                        TRISBbits.TRISB0 =1;
                        cnt=0;
                        LED=ON;
                        Relay=ON;
                        counter_mode2=1;
                        counter_mode1=0;
                        setval_mode=0;
                        break;
                    }
                    if(key==RESET_1)
                    {
                        INTCONbits.INTE=0;
                         TRISBbits.TRISB0 =0;
                        cnt=0;
                        LED=OFF;
                        Relay=OFF;
                        counter_mode1=0;
                        counter_mode2=0;
                        setval_mode=0;
                        break;
                    }
                }

                num = set_cnt;
               
                temp = num / 10000;
                num = num % 10000;
                if(blink_seg_f==1&&seg_set==5)
                    PORTB = 0; 
                else
                    PORTB = SegOne; 
                PORTD = seg_code[temp];               
                DELAY_ms1(1);

                temp = num / 1000;
                num = num % 1000;
                if(blink_seg_f==1&&seg_set==4)
                    PORTB = 0; 
                else
                    PORTB = SegTwo; 
                PORTD = seg_code[temp];
                DELAY_ms1(1);

                temp = num / 100;
                num = num % 100;
                if(blink_seg_f==1&&seg_set==3)
                    PORTB = 0; 
                else
                    PORTB = SegThree;
                PORTD = seg_code[temp];
                DELAY_ms1(1);

                temp = num / 10;
                if(blink_seg_f==1&&seg_set==2)
                    PORTB = 0; 
                else
                    PORTB = SegFour;
                PORTD = seg_code[temp];
                DELAY_ms1(1);    

                temp = num % 10;
                if(blink_seg_f==1 &&seg_set==1)
                    PORTB = 0; 
                else
                    PORTB = SegFive;
                        
                PORTD = seg_code[temp];
                DELAY_ms1(1);
                
                if(key==RESET_1)
                {
                    INTCONbits.INTE=0;
                    cnt=0;
                    GPIO_PinWrite(Relay,HIGH);
                    GPIO_PinWrite(LED,HIGH);
                    counter_mode1=0;
                    counter_mode2=0;
                    break;
                }
                
            }
        }
    }
}

void __interrupt() myIsr(void)
{
    if(INTCONbits.INTF==1)
    {
        cnt++;
        INTCONbits.INTF=0;
    }
    if(INTCONbits.TMR0IF==1)  //20ms 
    {
        timer0_cnt++;
        seg_blink_cnt++;
        if(seg_blink_cnt>=50)
        {   
            seg_blink_cnt=0;
            blink_seg_f=!blink_seg_f;
        }
        
        if(timer0_cnt>=10)
        {
            timer0_cnt=0;
            get_key_f=1;
        }
        TMR0 = 101;
        INTCONbits.TMR0IF=0;
    }
}


