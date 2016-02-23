//�������200 p/r Ҳ���ǲ����1.8��
//������� 400r/r ���Ƚ� 0.9��

#include <reg52.h>
#include <intrins.h>
#include <stdio.h>
#include <stdarg.h>

#define FOSC 11059200L //�������ã�Ĭ��ʹ��11.0592M Hz
//#define FOSC 12000000L //�������ã�ʹ��12M Hz
//#define FOSC 24000000L //�������ã�ʹ��24M Hz
#define BAUD 9600
#define TIME_MS    65075 //(65536-FOSC/12/1000*0.1) //�趨��ʱʱ�� 0.5ms ,��11.0592M�����£����׳���60ms

//IO�ӿڶ���
//�����ӿ� ����Ч
sbit key_plus = P2^1;
sbit key_minus = P2^0;
//����ܽӿ� ����Ч
sbit we1 = P2^3;
sbit we2 = P2^2;
#define LED_PORT P0 
//�����������IO
sbit motor_dcy1 = P1^0;
sbit motor_dcy2 = P1^1;//����˥��ģʽѡ��
sbit motor_m1 = P1^2;
sbit motor_m2 = P1^3;//����ģʽѡ��ѡ���Ƿ��Ƶ
sbit motor_pro = P1^4;//�������� input
sbit motor_m0 = P1^5;//��ʼ״̬���� input
sbit motor_tq1 = P1^6;
sbit motor_tq2 = P1^7;//����������ã�����ֵԽ������Խǿ
sbit motor_step = P3^4;//�����������   ���������
sbit motor_dir = P3^5;//���Ƶ��ת������  ���������
sbit motor_en = P3^6;//���ʹ��  ���������
//������IO
sbit coder_a = P3^2;
sbit coder_b = P3^3;

//ȫ�ֱ�������
//LED��ʾ��ģ 0-F ����ģʽ
unsigned code table[]= {0Xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
//���Ҳ�����0-2�У����0.9��
unsigned int code sintable[401]={
1000,1016,1031,1047,1063,1078,1094,1110,1125,1141,1156,1172,1187,1203,1218,1233,1249,1264,1279,1294,1309,1324,1339,1353,1368,1383,1397,1412,1426,1440,1454,1468,1482,1495,1509,1522,1536,1549,1562,1575,
1588,1600,1613,1625,1637,1649,1661,1673,1685,1696,1707,1718,1729,1740,1750,1760,1771,1780,1790,1800,1809,1818,1827,1836,1844,1853,1861,1869,1876,1884,1891,1898,1905,1911,1918,1924,1930,1935,1941,1946,
1951,1956,1960,1965,1969,1972,1976,1979,1982,1985,1988,1990,1992,1994,1996,1997,1998,1999,2000,2000,2000,2000,2000,1999,1998,1997,1996,1994,1992,1990,1988,1985,1982,1979,1976,1972,1969,1965,1960,1956,
1951,1946,1941,1935,1930,1924,1918,1911,1905,1898,1891,1884,1876,1869,1861,1853,1844,1836,1827,1818,1809,1800,1790,1780,1771,1760,1750,1740,1729,1718,1707,1696,1685,1673,1661,1649,1637,1625,1613,1600,
1588,1575,1562,1549,1536,1522,1509,1495,1482,1468,1454,1440,1426,1412,1397,1383,1368,1353,1339,1324,1309,1294,1279,1264,1249,1233,1218,1203,1187,1172,1156,1141,1125,1110,1094,1078,1063,1047,1031,1016,
1000,984,969,953,937,922,906,890,875,859,844,828,813,797,782,767,751,736,721,706,691,676,661,647,632,617,603,588,574,560,546,532,518,505,491,478,464,451,438,425,
412,400,387,375,363,351,339,327,315,304,293,282,271,260,250,240,229,220,210,200,191,182,173,164,156,147,139,131,124,116,109,102,95,89,82,76,70,65,59,54,
49,44,40,35,31,28,24,21,18,15,12,10,8,6,4,3,2,1,0,0,0,0,0,1,2,3,4,6,8,10,12,15,18,21,24,28,31,35,40,44,
49,54,59,65,70,76,82,89,95,102,109,116,124,131,139,147,156,164,173,182,191,200,210,220,229,240,250,260,271,282,293,304,315,327,339,351,363,375,387,400,
412,425,438,451,464,478,491,505,518,532,546,560,574,588,603,617,632,647,661,676,691,706,721,736,751,767,782,797,813,828,844,859,875,890,906,922,937,953,969,984,
1000
};

char ratio_value=0;//0-99
unsigned int motor_count=0;
unsigned int coder_count=0;//0 - 3600 ÿ����һ�������9��������3600�͹�0

unsigned int time1s_tick=0;
unsigned int t_flag=0;

char putchar(char ch)
{ 
	/* Place your implementation of fputc here */ 
	SBUF=(unsigned char)ch; //�����յ������ݷ��뵽���ͼĴ���
	while(!TI);		  //�ȴ������������
	TI=0;		 //���������ɱ�־λ	
	return ch;
}

void Delayms(unsigned int ms)
{
	unsigned int i,j;
	for(i=0;i<ms;i++)
	#if FOSC == 11059200L
	for(j=0;j<114;j++);
	#elif FOSC == 12000000L
	for(j=0;j<123;j++);
	#elif FOSC == 24000000L
	for(j=0;j<249;j++);
	#else
	for(j=0;j<114;j++);
	#endif
}

void Timer0Init()
{	
	TMOD |=  0x01; //���ö�ʱ��0������ʽΪ1
	TH0=TIME_MS/256;
	TL0=TIME_MS%256;
	ET0=1; //������ʱ��0�ж�
	TR0=0;	//������ʱ��	
	EA=1;  //�����ж�
}

//�ⲿ�ж�0��ʼ�������ձ���������
void Exit0Init()
{
	EX0 = 1;	//ʹ�� INT1 �ⲿ�ж�
	IT0 = 1;	// ������ʽΪ���帺���ش���
	EA = 1;//���ж�
}  

void UsartConfiguration()
{
	SCON = 0X50;			//����Ϊ������ʽ1	10λ�첽�շ���
	TMOD |= 0x20; //���ü�����������ʽ2  8λ�Զ���װ������	
	PCON = 0X80;//�����ʼӱ�	SMOD = 1  28800
	TH1 = 256 -(FOSC/12/32/(BAUD/2)); //���������
	TL1 = 256 -(FOSC/12/32/(BAUD/2));
	TR1 = 1; //�򿪶�ʱ��	
	EA = 1;//�����ж�
}

void SystemInit()
{
	key_plus = 1;
	key_minus = 1;
	LED_PORT = 0xff;
	we1 = 1;
	we2 = 1;
	
	//�����������
	motor_dcy1 = 1;
	motor_dcy2 = 0;//25%˥��ģʽ
	motor_m1 = 0;
	motor_m2 = 0;//����
	motor_tq1 = 0;
	motor_tq2 = 1;//50%����
	motor_dir = 0;//����
	motor_en = 0;//ʹ��
}

void KeyScan() 
{
	if(key_plus==0)  //�ж��Ƿ��¼���
	{
		Delayms(5); //��ʱ,���ȥ����
		if(key_plus==0)   //ȷ�ϰ�������
		{
			ratio_value++;     //����������1
			if(ratio_value == 100) //��8�����¼���
			{
				ratio_value = 99;    //��count����
			}
			printf("ratio:%d\r\n",(int)ratio_value);
		}
		while(key_plus==0);//��������,ÿ��һ��countֻ��1.
	}
	
	if(key_minus==0)  //�ж��Ƿ��¼���
	{
		Delayms(5); //��ʱ,���ȥ����
		if(key_minus==0)   //ȷ�ϰ�������
		{
			ratio_value--;     //����������1
			if(ratio_value == -1) //��8�����¼���
			{
				ratio_value = 0;    //��count����
			}
			printf("ratio:%d\r\n",(int)ratio_value);
		}
		while(key_minus==0);//��������,ÿ��һ��countֻ��1.
	}	
}

void Display(char value)
{
	char ge,shi;
	ge = value%10;
	shi = value%100/10;
	
	we1 = 0;
	LED_PORT = table[shi];
	Delayms(2);
	LED_PORT = 0xff;
	we1 = 1;
	
	we2 = 0;
	LED_PORT = table[ge];
	Delayms(2);
	LED_PORT = 0xff;
	we2 = 1;
}

/*******************************************************************************
* �� �� �� ��main
* �������� ��������
* ��    �� ����
* ��    �� ����
*******************************************************************************/
void main()
{
	UsartConfiguration();
	printf("system begin\r\n");
	Timer0Init();
	Exit0Init();
	SystemInit();	
	
	while(1)
	{
		KeyScan();
		Display(ratio_value);
//		if(motor_count >= 5000)//ÿ��5s��תһ��
//		{
//			motor_count = 0;
//			motor_dir = !motor_dir;
//			printf("system switch dir\r\n");
//		}
		if(time1s_tick>= 2000)
		{
			time1s_tick = 0;
			printf("%d %d\r\n",coder_count,motor_count);
		}
	}	
}

//500us
void Timer0Int() interrupt 1
{
	TH0=TIME_MS/256;
	TL0=TIME_MS%256;

	time1s_tick++;
	if(t_flag == 1)
	{
		t_flag = 0;
		
	}
	if(coder_count > motor_count)//��ת
	{
		motor_dir = 0;
		motor_step = !motor_step;
		if(motor_step == 3600) motor_step = 0;
		motor_count += 9;
	}
	if(coder_count < motor_count)//��ת
	{
		motor_dir = 1;
		motor_step = !motor_step;
		if(motor_step == 0) motor_step = 3600;
		motor_count -= 9;		
	}
}

void Exit0Int() interrupt 0 
{
	
	if(coder_b == 1)//��ת
	{
		if(coder_count == 3600) 
		{
			coder_count = 0;
			t_flag = 1;
		}
		coder_count += 9;	
		motor_dir = 0;	
		motor_step = !motor_step;
	}else
	{
		if(coder_count == 0) 
		{	
			coder_count = 3600;
			t_flag = 1;
		}
		coder_count -= 9;//��ת
		motor_dir = 1;	
		motor_step = !motor_step;
	}
}
