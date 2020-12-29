//先定义接口
#include <reg52.h>
#include <intrins.h>  
#include <math.h>   
#include <stdio.h>   

#define uchar unsigned char
#define uint unsigned int
	

//LCD定义
#define LCD_DB P0 
sbit LCD_RS=P2^6;   
sbit LCD_RW=P2^5;  
sbit LCD_E=P2^7;  


sbit K1=P3^1; 
sbit K2=P3^0; 
sbit K4=P3^3; 
bit flag=1;

uint Year=2020;
uint Mon=10;
uint Day=8;
uchar Mode=0;
/*-------------------------------------
			   函数申明
---------------------------------------*/
void LCD_init(void);                          //初始化函数 
void LCD_write_command(uchar command);        //写指令函数 
void LCD_write_data(uchar dat);               //写数据函数  
void LCD_disp_char(uchar x,uchar y,uchar dat);//在某个屏幕位置上显示一个字符,X（0-15),y(1-2)  
void LCD_disp_str(uchar x,uchar y,uchar *str); //LCD1602显示字符串函数 
void delay_n10us(uint n);                     //延时函数
/*-------------------------------------
			   LCD1602子程序
---------------------------------------*/
void LCD_init(void) 
{  
	delay_n10us(10);  
	LCD_write_command(0x38);//设置8位格式，2行，5x7 
	delay_n10us(10);  LCD_write_command(0x0c);//开显示，关光标，不闪烁 
	delay_n10us(10);  LCD_write_command(0x06);//设定输入方式，增量不移位 
	delay_n10us(10);  LCD_write_command(0x01);//清除屏幕显示  
	delay_n10us(100);       //延时清屏，延时函数，延时约n个10us 
}
//********写指令函数************
void LCD_write_command(uchar dat) 
{  
	delay_n10us(10);  
	LCD_RS=0;         //指令 
	LCD_RW=0;         //写入 
	LCD_E=1;          //允许 
	LCD_DB=dat;  delay_n10us(10);  //实践证明，我的LCD1602上，用for循环1次就能完成普通写指令。 
	LCD_E=0;  delay_n10us(10);  //实践证明，我的LCD1602上，用for循环1次就能完成普通写指令。 
}
//********写数据函数*************
void LCD_write_data(uchar dat) 
{  
	delay_n10us(10);  
	LCD_RS=1;          //数据 
	LCD_RW=0;		   //写入
	LCD_E=1;           //允许 
	LCD_DB=dat; delay_n10us(10);
	 LCD_E=0;  delay_n10us(10); 
}
//*******显示一个字符函数*********
void LCD_disp_char(uchar x,uchar y,uchar dat) 
{    
	uchar address;   
	if(y==1)           
	address=0x80+x;  
	else           
	address=0xc0+x;    
	LCD_write_command(address);   
	LCD_write_data(dat); 
}

void LCD_disp_str(uchar x,uchar y,uchar *str) 
{    
	uchar address;   
	if(y==1)           
	address=0x80+x;   
	else           
	address=0xc0+x;    
	LCD_write_command(address);   
	while(*str!='\0')   
		{       
		LCD_write_data(*str);
		str++;
		}
}

void delay_n10us(uint n)  //延时 
{                 
	uint i;                    
	for(i=n;i>0;i--)            
	 {         
	 nop_();_nop_();_nop_();_nop_();_nop_();_nop_();    
	 } 
}

/*-------------------------------------
			    按键检测
---------------------------------------*/	
void key()
{
	if(K1==0)
	{
		if(flag==1)
		{
			flag=0;
			if(Mode==0)
			{
				if(Year<9999)
				{
					Year++;
				}
			}	
			else if(Mode==1)
			{
				if(Mon<12)
				{
					Mon++;
				}
			}
			else if(Mode==2)
			{
				if(Day<31)
				{
					Day++;
				}
			}	
		}
	}
	else if(K2==0)
	{
		if(flag==1)
		{
			flag=0;
			if(Mode==0)
			{
				if(Year>0)
				{
					Year--;
				}
			}
			else if(Mode==1)
			{
				if(Mon>1)
				{
					Mon--;
				}
			}		
			else if(Mode==2)
			{
				if(Day>1)
				{
					Day--;
				}
			}
		}
	}
	else if(K4==0)
	{
		if(flag==1)
		{
			flag=0;
			if(Mode<2)
			{
				Mode++;
			}
			else
			{
				Mode=0;
			}
		}
	}
	else
	{
		flag=1;
	}
}
/*-------------------------------------
			    液晶显示
---------------------------------------*/	
void display()
{
	LCD_disp_char(3,2,Year/1000+'0'); 
	LCD_disp_char(4,2,Year/100%10+'0'); 
	LCD_disp_char(5,2,Year/10%10+'0'); 
	LCD_disp_char(6,2,Year%10+'0'); 
	
	LCD_disp_char(8,2,Mon/10+'0'); 
	LCD_disp_char(9,2,Mon%10+'0'); 
	
	LCD_disp_char(11,2,Day/10+'0'); 
	LCD_disp_char(12,2,Day%10+'0'); 
}
/*-------------------------------------
			    主函数
---------------------------------------*/	
void main(void)
{
	
	LCD_init();//LCD初始化              
	LCD_disp_str(0,1,"designer lpx khr"); //显示 
	LCD_disp_str(0,2,"   2020-10-08   "); //显示 
	while(1)
	{
		key();
		display();
	}
}