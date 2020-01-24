/*
* Petr Krehlik (xkrehl04)
Kompletni reseni projektu IMP
Upraveno: 99% - zustaly jen include
Posledni zmena: 16.12.2019
 */
 
/**
 * @file    MKL27Z64xxx4_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "fsl_port.h"
#include "clock_config.h"
#include "MKL27Z644.h"
#include "fsl_debug_console.h"


//define LEDs
#define S1_R_PORT PORTA
#define S1_R_GPIO GPIOA
#define S1_R_PIN 2u
#define S1_O_PORT PORTB
#define S1_O_GPIO GPIOB
#define S1_O_PIN 0u
#define S1_G_PORT PORTB
#define S1_G_GPIO GPIOB
#define S1_G_PIN 1u
#define S1R_G_PORT PORTA
#define S1R_G_GPIO GPIOA
#define S1R_G_PIN 1u

#define S2_R_PORT	PORTA
#define S2_R_GPIO GPIOA
#define S2_R_PIN 5u
#define S2_O_PORT PORTC
#define S2_O_GPIO GPIOC
#define S2_O_PIN 4u
#define S2_G_PORT PORTC
#define S2_G_GPIO GPIOC
#define S2_G_PIN 6u
#define S2R_G_PORT PORTE
#define S2R_G_GPIO GPIOE
#define S2R_G_PIN 31u

#define S3_R_PORT PORTC
#define S3_R_GPIO GPIOC
#define S3_R_PIN 7u
#define S3_O_PORT PORTC
#define S3_O_GPIO GPIOC
#define S3_O_PIN 5u
#define S3_G_PORT PORTD
#define S3_G_GPIO GPIOD
#define S3_G_PIN 6u
#define S3R_G_PORT PORTD
#define S3R_G_GPIO GPIOD
#define S3R_G_PIN 7u

#define P1_G_PORT PORTC
#define P1_G_GPIO GPIOC
#define P1_G_PIN 0u
#define P1_R_PORT PORTE
#define P1_R_GPIO GPIOE
#define P1_R_PIN 20u

#define P2_G_PORT PORTA
#define P2_G_GPIO GPIOA
#define P2_G_PIN 12u
#define P2_R_PORT PORTE
#define P2_R_GPIO GPIOE
#define P2_R_PIN 25u

#define P3_G_PORT PORTC
#define P3_G_GPIO GPIOC
#define P3_G_PIN 8u
#define P3_R_PORT PORTC
#define P3_R_GPIO GPIOC
#define P3_R_PIN 9u

#define P1_B_PORT PORTE
#define P1_B_GPIO GPIOE
#define P1_B_PIN 21u

#define P2_B_PORT PORTE
#define P2_B_GPIO GPIOE
#define P2_B_PIN 24u

#define P3_B_PORT PORTE
#define P3_B_GPIO GPIOE
#define P3_B_PIN 16u

int P1_set=0;
int P2_set=0;
int P3_set=0;
volatile uint32_t g_systickCounter;
void SysTick_Handler(void)
{
	int tmp=GPIO_PinRead(P1_B_GPIO,P1_B_PIN);
	if(tmp==1 && P1_set==0)
	{
		P1_set=1;

	}
	tmp=GPIO_PinRead(P2_B_GPIO,P2_B_PIN);
	if(tmp==1 && P2_set==0)
	{
		P2_set=1;

	}
	tmp=GPIO_PinRead(P3_B_GPIO,P3_B_PIN);
	if(tmp==1 && P3_set==0)
	{
		P3_set=1;

	}
	if (g_systickCounter != 0U)
	{
		g_systickCounter--;
	}
}

void SysTick_DelayTicks(uint32_t n)
{
	g_systickCounter = n;
	while (g_systickCounter != 0U)
	{
	}
}

void P_Check()
{
	if(P1_set==1)
	{
		P1_set=0;
		GPIO_PinWrite(S2R_G_GPIO, S2R_G_PIN, 1);
		GPIO_PinWrite(P1_R_GPIO, P1_R_PIN, 0);
		GPIO_PinWrite(P1_G_GPIO, P1_G_PIN, 1);
		SysTick_DelayTicks(5000U);
		GPIO_PinWrite(S2R_G_GPIO, S2R_G_PIN, 0);
		GPIO_PinWrite(P1_R_GPIO, P1_R_PIN, 1);
		GPIO_PinWrite(P1_G_GPIO, P1_G_PIN, 0);
		SysTick_DelayTicks(2000U);
	}
	if(P2_set==1)
	{
		P2_set=0;
		GPIO_PinWrite(S3R_G_GPIO, S3R_G_PIN, 1);
		GPIO_PinWrite(P2_R_GPIO, P2_R_PIN, 0);
		GPIO_PinWrite(P2_G_GPIO, P2_G_PIN, 1);
		SysTick_DelayTicks(5000U);
		GPIO_PinWrite(S3R_G_GPIO, S3R_G_PIN, 0);
		GPIO_PinWrite(P2_R_GPIO, P2_R_PIN, 1);
		GPIO_PinWrite(P2_G_GPIO, P2_G_PIN, 0);
		SysTick_DelayTicks(2000U);
	}
	if(P3_set==1)
	{
		P3_set=0;
		GPIO_PinWrite(S1R_G_GPIO, S1R_G_PIN, 1);
		GPIO_PinWrite(P3_R_GPIO, P3_R_PIN, 0);
		GPIO_PinWrite(P3_G_GPIO, P3_G_PIN, 1);
		SysTick_DelayTicks(5000U);
		GPIO_PinWrite(S1R_G_GPIO, S1R_G_PIN, 0);
		GPIO_PinWrite(P3_R_GPIO, P3_R_PIN, 1);
		GPIO_PinWrite(P3_G_GPIO, P3_G_PIN, 0);
		SysTick_DelayTicks(2000U);
	}
}


int main(void) {

	gpio_pin_config_t led_config = {
	        kGPIO_DigitalOutput, 0,
	    };

	gpio_pin_config_t button_config = {
			kGPIO_DigitalInput, 0,
		};
  	/* Init board hardware. */
    BOARD_InitBootPins();

    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortB);
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortD);
    CLOCK_EnableClock(kCLOCK_PortE);

    //Init buttons

    PORT_SetPinMux(P1_B_PORT, P1_B_PIN, kPORT_MuxAsGpio);
    GPIO_PinInit(P1_B_GPIO,P1_B_PIN, &button_config);

    PORT_SetPinMux(P2_B_PORT, P2_B_PIN, kPORT_MuxAsGpio);
    GPIO_PinInit(P2_B_GPIO,P2_B_PIN, &button_config);

    PORT_SetPinMux(P3_B_PORT, P3_B_PIN, kPORT_MuxAsGpio);
    GPIO_PinInit(P3_B_GPIO,P3_B_PIN, &button_config);


    //Init LEDs

	PORT_SetPinMux(S1_R_PORT, S1_R_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(S1_O_PORT, S1_O_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(S1_G_PORT, S1_G_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(S1R_G_PORT, S1R_G_PIN, kPORT_MuxAsGpio);

    GPIO_PinInit(S1_R_GPIO,S1_R_PIN, &led_config);
    GPIO_PinWrite(S1_R_GPIO, S1_R_PIN, 1);


    GPIO_PinInit(S1_O_GPIO,S1_O_PIN, &led_config);
    GPIO_PinWrite(S1_O_GPIO, S1_O_PIN, 0);


    GPIO_PinInit(S1_G_GPIO,S1_G_PIN, &led_config);
    GPIO_PinWrite(S1_G_GPIO, S1_G_PIN, 0);


    GPIO_PinInit(S1R_G_GPIO,S1R_G_PIN, &led_config);
    GPIO_PinWrite(S1R_G_GPIO, S1R_G_PIN, 0);

    PORT_SetPinMux(S2_R_PORT, S2_R_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(S2_O_PORT, S2_O_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(S2_G_PORT, S2_G_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(S2R_G_PORT, S2R_G_PIN, kPORT_MuxAsGpio);

	GPIO_PinInit(S2_R_GPIO,S2_R_PIN, &led_config);
	GPIO_PinWrite(S2_R_GPIO, S2_R_PIN, 1);


	GPIO_PinInit(S2_O_GPIO,S2_O_PIN, &led_config);
	GPIO_PinWrite(S2_O_GPIO, S2_O_PIN, 0);


	GPIO_PinInit(S2_G_GPIO,S2_G_PIN, &led_config);
	GPIO_PinWrite(S2_G_GPIO, S2_G_PIN, 0);


	GPIO_PinInit(S2R_G_GPIO,S2R_G_PIN, &led_config);
	GPIO_PinWrite(S2R_G_GPIO, S2R_G_PIN, 0);

	PORT_SetPinMux(S3_R_PORT, S3_R_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(S3_O_PORT, S3_O_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(S3_G_PORT, S3_G_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(S3R_G_PORT, S3R_G_PIN, kPORT_MuxAsGpio);

	GPIO_PinInit(S3_R_GPIO,S3_R_PIN, &led_config);
	GPIO_PinWrite(S3_R_GPIO, S3_R_PIN, 1);


	GPIO_PinInit(S3_O_GPIO,S3_O_PIN, &led_config);
	GPIO_PinWrite(S3_O_GPIO, S3_O_PIN, 0);


	GPIO_PinInit(S3_G_GPIO,S3_G_PIN, &led_config);
	GPIO_PinWrite(S3_G_GPIO, S3_G_PIN, 0);


	GPIO_PinInit(S3R_G_GPIO,S3R_G_PIN, &led_config);
	GPIO_PinWrite(S3R_G_GPIO, S3R_G_PIN, 0);

	PORT_SetPinMux(P1_G_PORT, P1_G_PIN, kPORT_MuxAsGpio);
    GPIO_PinInit(P1_G_GPIO,P1_G_PIN, &led_config);
    GPIO_PinWrite(P1_G_GPIO, P1_G_PIN, 0);

    PORT_SetPinMux(P1_R_PORT, P1_R_PIN, kPORT_MuxAsGpio);
	GPIO_PinInit(P1_R_GPIO,P1_R_PIN, &led_config);
	GPIO_PinWrite(P1_R_GPIO, P1_R_PIN, 1);

	PORT_SetPinMux(P2_G_PORT, P2_G_PIN, kPORT_MuxAsGpio);
    GPIO_PinInit(P2_G_GPIO,P2_G_PIN, &led_config);
    GPIO_PinWrite(P2_G_GPIO, P2_G_PIN, 0);

    PORT_SetPinMux(P2_R_PORT, P2_R_PIN, kPORT_MuxAsGpio);
	GPIO_PinInit(P2_R_GPIO,P2_R_PIN, &led_config);
	GPIO_PinWrite(P2_R_GPIO, P2_R_PIN, 1);

	PORT_SetPinMux(P3_G_PORT, P3_G_PIN, kPORT_MuxAsGpio);
    GPIO_PinInit(P3_G_GPIO,P3_G_PIN, &led_config);
    GPIO_PinWrite(P3_G_GPIO, P3_G_PIN, 0);

    PORT_SetPinMux(P3_R_PORT, P3_R_PIN, kPORT_MuxAsGpio);
	GPIO_PinInit(P3_R_GPIO,P3_R_PIN, &led_config);
	GPIO_PinWrite(P3_R_GPIO, P3_R_PIN, 1);

	//GPIO_PortToggle(S1R_G_GPIO, 1u << S1R_G_PIN);
    if (SysTick_Config(SystemCoreClock / 1000U))
	{
		while (1)
		{
		}
	}

	while (1)
	{
		//Check cross
		SysTick_DelayTicks(1000U);
		P_Check();

		//GO: S1+S3+P2
		GPIO_PinWrite(S1_O_GPIO, S1_O_PIN, 1);
		GPIO_PinWrite(S3_O_GPIO, S3_O_PIN, 1);
		GPIO_PinWrite(P2_R_GPIO, P2_R_PIN, 0);
		GPIO_PinWrite(P2_G_GPIO, P2_G_PIN, 1);
		SysTick_DelayTicks(2000U);
		GPIO_PinWrite(S1_R_GPIO, S1_R_PIN, 0);
		GPIO_PinWrite(S3_R_GPIO, S3_R_PIN, 0);
		GPIO_PinWrite(S1_O_GPIO, S1_O_PIN, 0);
		GPIO_PinWrite(S3_O_GPIO, S3_O_PIN, 0);
		GPIO_PinWrite(S1_G_GPIO, S1_G_PIN, 1);
		GPIO_PinWrite(S3_G_GPIO, S3_G_PIN, 1);
		SysTick_DelayTicks(3000U);

		//STOP: S1+S3+P2
		GPIO_PinWrite(P2_R_GPIO, P2_R_PIN, 1);
		GPIO_PinWrite(P2_G_GPIO, P2_G_PIN, 0);
		GPIO_PinWrite(S1_G_GPIO, S1_G_PIN, 0);
		GPIO_PinWrite(S3_G_GPIO, S3_G_PIN, 0);
		GPIO_PinWrite(S1_O_GPIO, S1_O_PIN, 1);
		GPIO_PinWrite(S3_O_GPIO, S3_O_PIN, 1);
		SysTick_DelayTicks(2000U);
		GPIO_PinWrite(S1_O_GPIO, S1_O_PIN, 0);
		GPIO_PinWrite(S3_O_GPIO, S3_O_PIN, 0);
		GPIO_PinWrite(S1_R_GPIO, S1_R_PIN, 1);
		GPIO_PinWrite(S3_R_GPIO, S3_R_PIN, 1);
		SysTick_DelayTicks(2000U);

		//Check cross
		P_Check();

		//GO: S2+P1+P3+S1R
		GPIO_PinWrite(S2_O_GPIO, S2_O_PIN, 1);
		GPIO_PinWrite(P1_R_GPIO, P1_R_PIN, 0);
		GPIO_PinWrite(P1_G_GPIO, P1_G_PIN, 1);
		GPIO_PinWrite(P3_R_GPIO, P3_R_PIN, 0);
		GPIO_PinWrite(P3_G_GPIO, P3_G_PIN, 1);
		SysTick_DelayTicks(2000U);
		GPIO_PinWrite(S2_R_GPIO, S2_R_PIN, 0);
		GPIO_PinWrite(S2_O_GPIO, S2_O_PIN, 0);
		GPIO_PinWrite(S2_G_GPIO, S2_G_PIN, 1);
		SysTick_DelayTicks(3000U);

		//STOP: S2+P1+P3+S1R
		GPIO_PinWrite(S2_G_GPIO, S2_G_PIN, 0);
		GPIO_PinWrite(S2_O_GPIO, S2_O_PIN, 1);
		GPIO_PinWrite(P1_R_GPIO, P1_R_PIN, 1);
		GPIO_PinWrite(P1_G_GPIO, P1_G_PIN, 0);
		GPIO_PinWrite(P3_R_GPIO, P3_R_PIN, 1);
		GPIO_PinWrite(P3_G_GPIO, P3_G_PIN, 0);
		SysTick_DelayTicks(2000U);
		GPIO_PinWrite(S2_O_GPIO, S2_O_PIN, 0);
		GPIO_PinWrite(S2_R_GPIO, S2_R_PIN, 1);
		SysTick_DelayTicks(2000U);


	}
    return 0 ;
}
