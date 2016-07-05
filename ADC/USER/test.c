#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "beep.h"	 	 
#include "key.h"	 	 
#include "exti.h"	 	 
#include "wdg.h" 	 
#include "timer.h"		 	 
#include "tpad.h"
#include "oled.h"		 	 
#include "lcd.h"
#include "usmart.h"	
#include "rtc.h"	 	 
#include "wkup.h"	
#include "adc.h" 	 
//ALIENTEK战舰STM32开发板实验17
//ADC 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司   				 	

int main(void)
{	
	u16 adc[2]={0};
	u16 adcx,adcx2;
	float temp,temp2;
  	Stm32_Clock_Init(9);	//系统时钟设置
	uart_init(72,9600);	 	//串口初始化为9600
	delay_init(72);	   	 	//延时初始化 
	LED_Init();		  		//初始化与LED连接的硬件接口
 	LCD_Init();			   	//初始化LCD
	usmart_dev.init(72);	//初始化USMART				 	
 //	Adc_Init();		  		//ADC初始化	
	MYDMA_Config(DMA1_Channel1,(u32)&ADC1->DR,(u32)adc,2);//DMA1通道4,外设为串口1,存储器为SendBuff,长度2.
		Adc_Init();		  		//ADC初始化	
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(60,70,200,16,16,"ADC TEST");	
	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,200,16,16,"2012/9/7");	
	//显示提示信息
	POINT_COLOR=BLUE;//设置字体为蓝色
	LCD_ShowString(60,130,200,16,16,"ADC_CH0_VAL:");	      
	LCD_ShowString(60,150,200,16,16,"ADC_CH0_VOL:0.000V");	    
	LCD_ShowString(60,180,200,16,16,"ADC_CH0_VOL:0.000V");	 
	while(1)
	{
// 		adcx=Get_Adc_Average(ADC_CH1,10);
// 		adcx2=Get_Adc_Average(2,10);
		LCD_ShowxNum(156,130,adc[0],4,16,0);//显示ADC的值
		temp=(float)adc[0]*(3.3/4096);
		temp2=(float)adc[1]*(3.3/4096);
//		if(temp<1.35)	temp-=1.22;
		printf("当前1电压为：%dV\n",adc[0]);
		printf("当前2电压为：%dV\n",adc[1]);
		adcx=temp;
		adcx2=temp2;
		LCD_ShowxNum(156,150,adcx,1,16,0);//显示电压值
		LCD_ShowxNum(156,180,adcx2,1,16,0);//显示电压值
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(172,150,temp,3,16,0X80);
		temp2-=adcx2;
		temp2*=1000;
		LCD_ShowxNum(172,180,temp2,3,16,0X80);
//		printf("当前电压为：%dV",&adcx);
		LED0=!LED0;
		LED1=!LED1;
		delay_ms(250);	
		delay_ms(350);
	}
}









