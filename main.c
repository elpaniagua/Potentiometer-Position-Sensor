/*Robinson Paniagua
 * Position Sensor using a potentiometer
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"


uint32_t ADCValue[1];
uint32_t DegValue;



 int main(void) {

/********************************* GPIO Port F Configuration***********************************/
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN); //Initializes the Clock SysDiv_5, 40MHZ
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); // Initializes ADC Module with the Clock
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); //Initializes the GPIO Ports with the Clock
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
/***************Sets the Outputs for the Ports*****************************************************/
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,GPIO_PIN_2|GPIO_PIN_1);
	GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_3);
/**********Configures the ADC Module to Scan for Changes*****************************************/
	ADCSequenceConfigure(ADC0_BASE,1,ADC_TRIGGER_PROCESSOR,0);
	ADCSequenceStepConfigure(ADC0_BASE,1,0,ADC_CTL_CH0);
	ADCSequenceEnable(ADC0_BASE,1);
/*****************************Continuos Loop to Scan for Changes and Change LED Status************/

	while (1) {
		ADCIntClear(ADC0_BASE, 1); //Clears any interrupts associated with the ADC Module
		ADCProcessorTrigger(ADC0_BASE, 1); //Continuous scan for changes in Voltages
		ADCSequenceDataGet(ADC0_BASE, 1, ADCValue);
		DegValue = ADCValue[1] / 15.6; //Voltage to Degree Conversion


/**************************Detects position of the Switch and turns on the respective LED***************************/
		if (DegValue == 0) {
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x02);
		} else {
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x0);
		}

		if ((DegValue > 5) && (DegValue < 90)) {
			GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x04);
		} else {
			GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x00);
		}

		if ((DegValue > 90) && (DegValue < 180)) {
			GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x02);
		}

		else {
			GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x00);
		}

		if ((DegValue > 180) && (DegValue < 270)) {
			GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0x80);
		}

		else {
			GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0x00);
		}

	}
}
