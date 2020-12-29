//�ȶ���ӿ�
#include <reg52.h>
#include <intrins.h>  
#include <math.h>   
#include <stdio.h>   

#define uchar unsigned char
#define uint unsigned int
	

//LCD����
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
			   ��������
---------------------------------------*/
void LCD_init(void);                          //��ʼ������ 
void LCD_write_command(uchar command);        //дָ��� 
void LCD_write_data(uchar dat);               //д���ݺ���  
void LCD_disp_char(uchar x,uchar y,uchar dat);//��ĳ����Ļλ������ʾһ���ַ�,X��0-15),y(1-2)  
void LCD_disp_str(uchar x,uchar y,uchar *str); //LCD1602��ʾ�ַ������� 
void delay_n10us(uint n);                     //��ʱ����
/*-------------------------------------
			   LCD1602�ӳ���
---------------------------------------*/
void LCD_init(void) 
{  
	delay_n10us(10);  
	LCD_write_command(0x38);//����8λ��ʽ��2�У�5x7 
	delay_n10us(10);  LCD_write_command(0x0c);//����ʾ���ع�꣬����˸ 
	delay_n10us(10);  LCD_write_command(0x06);//�趨���뷽ʽ����������λ 
	delay_n10us(10);  LCD_write_command(0x01);//�����Ļ��ʾ  
	delay_n10us(100);       //��ʱ��������ʱ��������ʱԼn��10us 
}
//********дָ���************
void LCD_write_command(uchar dat) 
{  
	delay_n10us(10);  
	LCD_RS=0;         //ָ�� 
	LCD_RW=0;         //д�� 
	LCD_E=1;          //���� 
	LCD_DB=dat;  delay_n10us(10);  //ʵ��֤�����ҵ�LCD1602�ϣ���forѭ��1�ξ��������ͨдָ� 
	LCD_E=0;  delay_n10us(10);  //ʵ��֤�����ҵ�LCD1602�ϣ���forѭ��1�ξ��������ͨдָ� 
}
//********д���ݺ���*************
void LCD_write_data(uchar dat) 
{  
	delay_n10us(10);  
	LCD_RS=1;          //���� 
	LCD_RW=0;		   //д��
	LCD_E=1;           //���� 
	LCD_DB=dat; delay_n10us(10);
	 LCD_E=0;  delay_n10us(10); 
}
//*******��ʾһ���ַ�����*********
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

void delay_n10us(uint n)  //��ʱ 
{                 
	uint i;                    
	for(i=n;i>0;i--)            
	 {         
	 nop_();_nop_();_nop_();_nop_();_nop_();_nop_();    
	 } 
}

/*-------------------------------------
			    �������
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
			    Һ����ʾ
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
			    ������
---------------------------------------*/	
void main(void)
{
	
	LCD_init();//LCD��ʼ��              
	LCD_disp_str(0,1,"designer lpx khr"); //��ʾ 
	LCD_disp_str(0,2,"   2020-10-08   "); //��ʾ 
	while(1)
	{
		key();
		display();
	}
}