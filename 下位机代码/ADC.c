#include <STC15F2K60S2.H>
#include "ADC.h"
#include "intrins.h"
#define ADC_POWER   0x80            //ADC?????
#define ADC_FLAG    0x10            //ADC????
#define ADC_START   0x08            //ADC?????
#define ADC_SPEEDLL 0x00            //540???
#define ADC_SPEEDL  0x20            //360???
#define ADC_SPEEDH  0x40            //180???
#define ADC_SPEEDHH 0x60            //90???
sbit PM25_LED = P5^4;
void Del0ay(int n)
{
	int x;
	while (n--)
	{
		x = 5000;
		while (x--);
	}
}
void Delay280us()		//@20.000MHz
{
	unsigned char i, j;

	i = 6;
	j = 111;
	do
	{
		while (--j);
	} while (--i);
}
void Delay9680us()		//@20.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 1;
	j = 189;
	k = 78;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Delay70us()		//@20.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 89;
	do
	{
		while (--j);
	} while (--i);
}
void ADC_Init()
{
	P1ASF = 0xf0;    
	ADC_RES = 0;                  
  ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
	Del0ay(2);
}
unsigned char GetADCResult(unsigned char ch)
{
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;
	_nop_();                     
	_nop_();
	_nop_();
	_nop_();
	while (!(ADC_CONTR & ADC_FLAG));
	ADC_CONTR &= ~ADC_FLAG;         
	return ADC_RES;                 
}
unsigned char GatValue_PM25()
{
	unsigned char Value;
	PM25_LED = 0;
	Delay280us();
	Value = GetADCResult(4);
	Delay70us();
	PM25_LED = 1;
	Delay9680us();
	return Value;
}
unsigned char GatValue_MQ7()
{
	return GetADCResult(5);
}
unsigned char GatValue_MQ135()
{
	return GetADCResult(6);
}