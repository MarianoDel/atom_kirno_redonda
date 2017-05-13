/**
  ******************************************************************************
  * @file    Template_2/main.c
  * @author  Nahuel
  * @version V1.0
  * @date    22-August-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Use this template for new projects with stm32f0xx family.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_adc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//--- My includes ---//
#include "gpio.h"
#include "stm32f0x_tim.h"
#include "uart.h"
#include "hard.h"

#include "core_cm0.h"
#include "adc.h"
#include "flash_program.h"
#include "synchro.h"

//--- VARIABLES EXTERNAS ---//



// ------- Externals of USARTs  ----------
volatile unsigned char tx1buff[SIZEOF_DATA];
volatile unsigned char rx1buff[SIZEOF_DATA];
volatile unsigned char usart1_pckt_ready;
volatile unsigned char usart1_have_data;

volatile unsigned char tx2buff[SIZEOF_DATA];
volatile unsigned char rx2buff[SIZEOF_DATA];
volatile unsigned char usart2_pckt_ready;
volatile unsigned char usart2_have_data;

// ------- Externals of Messages -------
unsigned short mains_freq = 0;
unsigned short mains_var = 0;
unsigned char pwm = 0;
unsigned char debug_secs = 0;



//
//volatile unsigned char data1[SIZEOF_DATA1];
////static unsigned char data_back[10];
//volatile unsigned char data[SIZEOF_DATA];

// ------- Externals de los timers -------
//volatile unsigned short prog_timer = 0;
//volatile unsigned short mainmenu_timer = 0;
volatile unsigned short show_select_timer = 0;
volatile unsigned char switches_timer = 0;
volatile unsigned char acswitch_timer = 0;

volatile unsigned short scroll1_timer = 0;
volatile unsigned short scroll2_timer = 0;

volatile unsigned short standalone_timer;
volatile unsigned short standalone_enable_menu_timer;
//volatile unsigned short standalone_menu_timer;
volatile unsigned char grouped_master_timeout_timer;
volatile unsigned short take_temp_sample = 0;
volatile unsigned short minutes = 0;
volatile unsigned char timer_wifi_bright = 0;


unsigned char saved_mode;

// ------- para determinar igrid -------
volatile unsigned char igrid_timer = 0;
volatile unsigned char vgrid_timer = 0;

// ------- Externals de los switches -------
unsigned short s1;
unsigned short s2;
unsigned short sac;
unsigned char sac_aux;


//------ GLOBAL VARIABLES ------//
parameters_typedef param_struct;

// ------- de los timers -------
volatile unsigned short wait_ms_var = 0;
volatile unsigned short timer_standby;
//volatile unsigned char display_timer;
volatile unsigned char filter_timer;

//volatile unsigned char door_filter;
//volatile unsigned char take_sample;
//volatile unsigned char move_relay;
volatile unsigned short secs = 0;


// ------- de los filtros DMX -------
#define LARGO_F		32
#define DIVISOR_F	5
unsigned char vd0 [LARGO_F + 1];
unsigned char vd1 [LARGO_F + 1];
unsigned char vd2 [LARGO_F + 1];
unsigned char vd3 [LARGO_F + 1];
unsigned char vd4 [LARGO_F + 1];


//--- FUNCIONES DEL MODULO ---//
void TimingDelay_Decrement(void);
void Update_PWM (unsigned short);
void UpdatePackets (void);

// ------- del display -------


//--- FILTROS DE SENSORES ---//
#define LARGO_FILTRO 16
#define DIVISOR      4   //2 elevado al divisor = largo filtro
//#define LARGO_FILTRO 32
//#define DIVISOR      5   //2 elevado al divisor = largo filtro
unsigned short vtemp [LARGO_FILTRO + 1];
unsigned short vpote [LARGO_FILTRO + 1];

//--- FIN DEFINICIONES DE FILTRO ---//


//-------------------------------------------//
// @brief  Main program.
// @param  None
// @retval None
//------------------------------------------//
int main(void)
{
	unsigned char i,ii;
	unsigned char bytes_remain, bytes_read, need_ack = 0;
//	unsigned char resp = RESP_CONTINUE;
	unsigned short local_meas, local_meas_last;
	unsigned char main_state = 0;
	char s_lcd [20];
	unsigned char new_room = 0;
	unsigned char new_lamp = 0;
	unsigned char last_bright = 0;
	unsigned char show_ldr = 0;
	int dummy_resp = 0;
	unsigned char pps_one = 0;

#ifdef USE_PROD_PROGRAM
	unsigned char jump_the_menu = 0;
#endif
	parameters_typedef * p_mem_init;
	//!< At this stage the microcontroller clock setting is already configured,
    //   this is done through SystemInit() function which is called from startup
    //   file (startup_stm32f0xx.s) before to branch to application main.
    //   To reconfigure the default setting of SystemInit() function, refer to
    //   system_stm32f0xx.c file

	//GPIO Configuration.
	GPIO_Config();


	//ACTIVAR SYSTICK TIMER
	if (SysTick_Config(48000))
	{
		while (1)	/* Capture error */
		{
			if (LED)
				LED_OFF;
			else
				LED_ON;

			for (i = 0; i < 255; i++)
			{
				asm (	"nop \n\t"
						"nop \n\t"
						"nop \n\t" );
			}
		}
	}

	//--- Prueba de LED ---//
	while (1)
	{
		if (LED)
			LED_OFF;
		else
			LED_ON;

		Wait_ms(1000);
	}
	//--- Fin Prueba de LED ---//

	//--- Prueba de LED con SYNC ---//
	while (1)
	{
		if (SYNC)
			LED_ON;
		else
			LED_OFF;

	}
	//--- Fin Prueba de LED con SYNC ---//

	//--- Prueba de LED con RELAY ---//
	while (1)
	{
		if (RELAY)
		{
			LED_OFF;
			RELAY_OFF;
		}
		else
		{
			LED_ON;
			RELAY_ON;
		}
		Wait_ms(5000);
	}
	//--- Fin Prueba de LED y RELAY ---//

	//---------- Prueba USART1 Single Byte --------//
	USART1Config();
	while (1)
	{
		Usart1SendSingle('M');
		Wait_ms(3000);
	}
	//---------- Fin Prueba USART1 Single Byte --------//

	//---------- Prueba USART1 Multiple Bytes --------//
	USART1Config();
   while( 1 )
   {
		Usart1Send((char *) (const char *) "Kirno debug placa redonda\r\n");
		Wait_ms(3000);
   }
   //---------- Fin Prueba USART1 Single Bytes --------//


	//ADC Configuration
//	AdcConfig();

	//TIM Configuration.
	TIM_3_Init();
//	TIM_14_Init();
//	TIM_16_Init();		//para OneShoot() cuando funciona en modo master
//	TIM_17_Init();		//lo uso para el ADC de Igrid

//	EXTIOff ();


	//--- Welcome code ---//
	LED_OFF;
	RELAY_ON;
	//RELAY_OFF;


//	USART2Config();

	EXTIOff();






	//---------- Prueba temp --------//
	/*
	while (1)
	{
		local_meas = GetTemp();
		if (local_meas != local_meas_last)
		{
			LED_ON;
			local_meas_last = local_meas;
			LCD_2DO_RENGLON;
			LCDTransmitStr((const char *) "Brd Temp:       ");
			local_meas = ConvertTemp(local_meas);
			sprintf(s_lcd, "%d", local_meas);
			Lcd_SetDDRAM(0x40 + 10);
			LCDTransmitStr(s_lcd);
			LED_OFF;
		}

		UpdateTemp();
	}
	*/
	//---------- Fin prueba temp --------//

	//---------- Prueba 1 to 10V --------//
	/*
	local_meas = 0;
	while (1)
	{
		LCD_2DO_RENGLON;
		LCDTransmitStr((const char *) "1 to 10V:       ");
		fcalc = local_meas;
		fcalc = fcalc * K_1TO10;
		one_int = (short) fcalc;
		fcalc = fcalc - one_int;
		fcalc = fcalc * 10;
		one_dec = (short) fcalc;

		sprintf(s_lcd, "%02d.%01d V", one_int, one_dec);
		Lcd_SetDDRAM(0x40 + 10);
		LCDTransmitStr(s_lcd);

		Wait_ms (1000);
		if (local_meas <= 255)
			local_meas = 0;
		else
			local_meas++;
	}
	*/
	//---------- Fin prueba 1 to 10V --------//


    //---------- Programa de Produccion --------//
#ifdef USE_PROD_PROGRAM
	//--- PRUEBA FUNCION MAIN_MENU
	//leo la memoria, si tengo configuracion de modo
	//entro directo, sino a Main Menu
	if (saved_mode == 0xFF)	//memoria borrada
		main_state = MAIN_INIT;
	// else
	// 	jump_the_menu = RESP_YES;

	//Wait_ms(2000);
	while (1)
	{
		switch (main_state)
		{
			case MAIN_INIT:
				break;

			case MAIN_STAND_ALONE:
				break;

			case MAIN_GROUPED:
				break;

			default:
				main_state = MAIN_INIT;
				break;

		}

		// UpdateSwitches();
		// UpdateACSwitch();
		// UpdatePackets();
		ProcessMessages();
		UpdateMessages();
	}

	//--- FIN PRUEBA FUNCION MAIN_MENU
#endif
	//---------- Fin Programa de Procduccion --------//

	return 0;
}

//--- End of Main ---//



void TimingDelay_Decrement(void)
{
	if (wait_ms_var)
		wait_ms_var--;

//	if (display_timer)
//		display_timer--;

	if (timer_standby)
		timer_standby--;

	if (acswitch_timer)
		acswitch_timer--;

//	if (prog_timer)
//		prog_timer--;

	if (take_temp_sample)
		take_temp_sample--;

	if (filter_timer)
		filter_timer--;


	//cuenta de a 1 minuto
	if (secs > 59999)	//pasaron 1 min
	{
		minutes++;
		secs = 0;
	}
	else
		secs++;

}

//------ EOF -------//
