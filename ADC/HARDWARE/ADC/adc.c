#include "adc.h"
#include "delay.h"					   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//ADC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
	   
//初始化ADC
//这里我们仅以规则通道为例
//我们默认仅开启通道1																	   
void  Adc_Init(void)
{    
	//先初始化IO口
 	RCC->APB2ENR|=1<<2;    //使能PORTA口时钟 
	GPIOA->CRL&=0XFFFFF00F;//PA1 anolog输入
	//通道10/11设置			 
	RCC->APB2ENR|=1<<9;    //ADC1时钟使能	  
	RCC->APB2RSTR|=1<<9;   //ADC1复位
	RCC->APB2RSTR&=~(1<<9);//复位结束	    
	RCC->CFGR&=~(3<<14);   //分频因子清零	
//	SYSCLK/DIV2=12M //ADC时钟设置为12M,ADC最大时钟不能超过14M!
	//否则将导致ADC准确度下降! 
	RCC->CFGR|=2<<14;      	 
	ADC1->CR1&=0XF0FFFF;   //工作模式清零
	ADC1->CR1|=0<<16;      //独立工作模式  
	ADC1->CR1|=1<<13;
//	ADC1->CR1&=~(1<<8);    //非扫描模式	  
	ADC1->CR1|=(1<<8);    //扫描模式	
	ADC1->CR2|=(1<<1);    //连续模式
	ADC1->CR2&=~(7<<17);	   
	ADC1->CR2|=7<<17;	   //软件控制转换  
	ADC1->CR2|=1<<20;      //使用用外部触发(SWSTART)!!!	必须使用一个事件来触发
	ADC1->CR2&=~(1<<11);   //右对齐	 
	ADC1->CR2|=1<<8;//使用DMA模式
	ADC1->SQR1&=~(0XF<<20);
	ADC1->SQR1|=1<<20;     //2个转换在规则序列中 也就是只转换规则序列1 			
	ADC1->SQR3|=1<<0;
	ADC1->SQR3|=2<<5;	
	//设置通道1的采样时间
	ADC1->SMPR2&=~(7<<3);  //通道1采样时间清空	  
 	ADC1->SMPR2|=7<<3;     //通道1  239.5周期,提高采样时间可以提高精确度	 
	   
	//设置通道1的采样时间
//	ADC1->SMPR2&=~(7<<6);  //通道1采样时间清空	  
 	ADC1->SMPR2|=7<<6;     //通道1  239.5周期,提高采样时间可以提高精确度	
	ADC1->CR2|=1<<0;	   //开启AD转换器	 
	ADC1->CR2|=1<<3;       //使能复位校准  
	while(ADC1->CR2&1<<3); //等待校准结束 			 
    //该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。 		 
	ADC1->CR2|=1<<2;        //开启AD校准	   
	while(ADC1->CR2&1<<2);  //等待校准结束
	//该位由软件设置以开始校准，并在校准结束时由硬件清除  
	ADC1->CR2|=1<<22;       //启动规则转换通道
}				  
//获得ADC值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	//设置转换序列	  		 
	ADC1->SQR3&=0XFFFFFFE0;//规则序列1 通道ch
	ADC1->SQR3|=ch;		  			    
	ADC1->CR2|=1<<22;       //启动规则转换通道 
	while(!(ADC1->SR&1<<1));//等待转换结束	 	   
	return ADC1->DR;		//返回adc值	
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 
	 
u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 		    
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量  
void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	RCC->AHBENR|=1<<0;			//开启DMA1时钟
	delay_ms(5);				//等待DMA时钟稳定
	DMA_CHx->CPAR=cpar; 	 	//DMA1 外设地址 
	DMA_CHx->CMAR=(u32)cmar; 	//DMA1,存储器地址
	DMA1_MEM_LEN=cndtr;      	//保存DMA传输数据量
	DMA_CHx->CNDTR=cndtr;    	//DMA1,传输数据量
	DMA_CHx->CCR=0X00000000;	//复位
//	DMA_CHx->CCR|=1<<4;  		//从存储器读
	DMA_CHx->CCR&=~(1<<4);  		//从外设读
//	DMA_CHx->CCR|=0<<5;  		//普通模式
	DMA_CHx->CCR|=1<<5;  		//循环模式
	DMA_CHx->CCR|=0<<6; 		//外设地址非增量模式
	DMA_CHx->CCR|=1<<7; 	 	//存储器增量模式
//	DMA_CHx->CCR&=~(1<<7); 	 	//存储器非增量模式
	DMA_CHx->CCR|=1<<8; 	 	//外设数据宽度为16位
	DMA_CHx->CCR|=1<<10; 		//存储器数据宽度16位
	DMA_CHx->CCR|=1<<12; 		//中等优先级
	DMA_CHx->CCR|=0<<14; 		//非存储器到存储器模式	

	DMA_CHx->CCR|=1<<0;          //开启DMA传输
//	DMA_CHx->CNDTR=2; //DMA1,传输数据量 
	
} 
//开启一次DMA传输
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{
	DMA_CHx->CCR&=~(1<<0);       //关闭DMA传输 
	DMA_CHx->CNDTR=DMA1_MEM_LEN; //DMA1,传输数据量 
	DMA_CHx->CCR|=1<<0;          //开启DMA传输
}








