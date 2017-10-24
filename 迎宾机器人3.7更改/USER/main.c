#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "rs485.h"
#include "hc05.h"
#include "usart3.h"
#include "timer.h"
#include "pid.h"
#include "pwm.h"
#include "led.h"
#include "uart4.h"
#include "math.h"
#include "route.h"
/*函数声明*/

void system_init(void);
void lanyakongzhi(void);
void lanyafashuju(void);
void shengyinkongzhi(void);
void InitVar(int walk,int terminus,int seeter,int beginselect,int overselect);
/*函数声明*/

u8 rs485qianjinbuf[8] = {0x01,0x06,0x00,0xc8,0x00,0xa1,0xc9,0x8c};	
u8 rs485zuozhuanbuf[8] = {0x01,0x06,0x00,0xc8,0x00,0xa2,0x89,0x8d};	
u8 rs485youzhuanbuf[8] = {0x01,0x06,0x00,0xc8,0x00,0xa3,0x48,0x4d};	
u8 rs485houtuibuf[8] = {0x01,0x06,0x00,0xc8,0x00,0xa4,0x09,0x8f};	
u8 rs485tingzhibuf[8] = {0x01,0x06,0x00,0xc8,0x00,0xff,0x48,0x74};	
u8 rs485buzoubuf[8] = {0x01,0x06,0x00,0xc8,0x00,0xb1,0xc8,0x40};	
/*变量声明*/
extern u16 distance;//超声波相关
extern u8 time_flag;//超声波相关
extern int waydirection;//支路选择的方向 1左 -1右 0只有一条
u8 bitt=0;
u8 reclen=0; 
u8 reclen4=0;
u8 fflag = 0;
u8 xuhao = 0;
u8 cishu = 0;
int xpo = 0;
int ypo = 0;
int zpo = 0;
u8 zhongjian = 0;
u16 xxian,yxian;
//这五个标志位用来使能中断
int walkstate=0,terminusstate=0;
#define seekfootON walkstate=1
#define seekfootOFF walkstate=0
#define seekpointON terminusstate=1
#define seekpointOFF terminusstate=0
int seeterminus=0;//标记寻脚印过程中检测到的终点
//支路选择标志位
int selectbegin=0,selectover=0;
#define seekwayON selectbegin=1
#define seekwayOFF selectbegin=0
#define seekwayON selectbegin=1
#define seekwayON selectbegin=1
int numb,numc;
extern int zxpo,zypo;
/**/

//[m3]  女平常
//[m51] 智障男
//[m52] 深情男

//测试可以，待优化
void pointTopoint(int nowpoint,int nextpoint,int roadtime,int pointenddistance)
{
	InitVar(0,0,0,0,0);
	kongzhidong(0,0);
	closetopoint(pointenddistance,2,150);
	chooseroad(nowpoint,nextpoint);
	kongzhidong(0,0);
	delay_ms(1000);
	ontheroad(roadtime,10);
}


int main(void)
{
	system_init();
	pointTopoint(302,401,6,25);
	pointTopoint(401,501,6,25);

}

void InitVar(int walk,int terminus,int seeter,int beginselect,int overselect)
{
	walkstate=walk;//寻脚印
	terminusstate=terminus;//寻起点终点
	seeterminus=seeter;//终点检测标志位
	selectbegin=beginselect;//执行支路选择函数 
	selectover=overselect;//选择支路完成标志
}


void system_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组
	delay_init(168);  //初始化延时函数
  usart3_init(9600);
	RS485_Init(9600);
	usart4_init(9600);
	//HMISendstart();
	uart_init(115200);//初始化串口波特率为115200
	LED2_Init();
	TIM13_PWM_Init(1000-1,12-1); //84000000/12/1000 =7KHZ
	TIM4_PWM_Init(1000-1,12-1);  //84000000/12/1000 =7KHZ
	USART3_RX_STA=0;
	USART4_RX_STA=0;
	
	
	TIM_SetCompare1(TIM14,0);
	TIM_SetCompare2(TIM3,0);
}



void lanyafashuju(void)
{
	u3_printf("x:%d",xpo);
	u3_printf("y:%d",ypo);
	u3_printf("//");
}

void shengyinkongzhi(void)
{
	if(USART4_RX_STA&0X8000)			//接收到一次数据了
	{
		reclen4=USART4_RX_STA&0X7FFF;	//得到数据长度
		USART4_RX_BUF[reclen4]=0;	 	//加入结束符
		USART4_RX_STA=0;	 
//		printf("%s/t",USART3_RX_BUF);
//		printf("reci/t");
		if(USART4_RX_BUF[0] == 1)
		{
		
			RS485_Send_Data(rs485qianjinbuf,8);
			
		}
		else if(USART4_RX_BUF[0] == 2)
		{
			RS485_Send_Data(rs485zuozhuanbuf,8);
		}
		else if(USART4_RX_BUF[0] == 3)
		{
			RS485_Send_Data(rs485youzhuanbuf,8);
		}
		else if(USART4_RX_BUF[0] == 4)
		{
			RS485_Send_Data(rs485houtuibuf,8);
		}
		else if(USART4_RX_BUF[0] == 5)
		{
			RS485_Send_Data(rs485tingzhibuf,8);
		}
		else if(USART4_RX_BUF[0] == 6)   //xx过来
		{
		
		}
		else if(USART4_RX_BUF[0] == 7)
		{
			
		}
		delay_ms(10);
		usartt4 = 0;
	}	 			
	
}



