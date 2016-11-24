//#include "Motor.h"
#include <Servo.h>

Servo duoji;			//创建一个舵机类；
#define startpos 90		//舵机中间值
#define RightMAX 125	//舵机→最大值
#define LeftMAX 45		//舵机←最大值
int pos = startpos;		//保存舵机的角度

int StartSS=14;		//传感器第一引脚
int pos_val[5]={0};		//位置信息保存
int position_now;		//当前位置信息
int position_old;		//上一时刻位置信息

//电机引脚定义
const int ENA =7;
const int INA = 5;
const int INB = 6;
const int ENB =8;

void sensorinit(int s1);	//传感器初始化
void getpostion(void);		//获取位置信息
void changeservo(void);		//改变舵机角度

void setup()
{
	/* add setup code here */
	//舵机初始化
	duoji.attach(9);//指定一个arduino引脚 ，这里我指定的是引脚9
	duoji.write(pos);//先让舵机归位，回中。后面通过改变pos 来改变舵机角度。
	//传感器初始化AN0~AN4
	sensorinit(StartSS);
	//测试期间电机的速度可以设一个较低值测试。
	pinMode(INA,OUTPUT);
	digitalWrite(INA,HIGH);
	pinMode(INB,OUTPUT);
	digitalWrite(INB,HIGH);
	pinMode(ENA,OUTPUT);
	pinMode(ENB,OUTPUT);
	analogWrite(ENA,0);
	analogWrite(ENB,100);
	//串口初始化 
	Serial.begin(9600);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}
	
}

void loop()
{
  /* add main program code here */
   // getpostion();
   delay(3000);
   // changeservo();
   for (pos = LeftMAX; pos <= RightMAX; pos += 1) { // goes from 0 degrees to 180 degrees
	     // in steps of 1 degree
	     duoji.write(pos);              // tell servo to go to position in variable 'pos'
	     delay(15);                       // waits 15ms for the servo to reach the position
   }
   for (pos = RightMAX; pos >= LeftMAX; pos -= 1) { // goes from 180 degrees to 0 degrees
	     duoji.write(pos);              // tell servo to go to position in variable 'pos'
	     delay(15);                       // waits 15ms for the servo to reach the position
   }
  
}
void sensorinit(int s1)
{
		int i;
		//引脚设为输入
		for (i=0;i<5;i++)
		{
			pinMode(s1+i,INPUT);
		}
		//读取数据到数组

}
//通过读取传感器信息，获取车身位置信息。
void getpostion(void)
{
	int Fd;
	int Psd=0;
	int possum=0;
	int i,j;
	//计算压点的数量
	for (i=0;i<5;i++) 
	{
		pos_val[i] = digitalRead(StartSS+i);
	}
	possum = pos_val[0]+pos_val[1]+pos_val[2]+pos_val[3]+pos_val[4];
	//加打印
	Serial.print("IR:");
	Serial.print(pos_val[0]);Serial.print(" ");
	Serial.print(pos_val[1]);Serial.print(" ");
	Serial.print(pos_val[2]);Serial.print(" ");
	Serial.print(pos_val[3]);Serial.print(" ");
	Serial.println(pos_val[4]);
	
	//滤除掉没有压点和压的点数超过2点以上的情况。如果遇到这种情况，位置信息不变。
	if (possum ==5 || possum < 3)
	{
		position_now = position_old;
	}
	//黑线压一点和压两点是正常的结果,计算位置。
	else
	{
			for (j=0;j<5;j++)
			{
				if (Psd ==0 )
				{
					if (pos_val[j] == 0) //黑色没有反射，为0
					{
						Fd = j;
						Psd = 1;
					}
				}
			}	
	}
	//黑线压一点 根据公式计算出位置
	if (possum == 4)
	{
		position_now = Fd*2+1;
	}
	//黑线压两点 根据公式计算出位置
	else if(possum == 3) position_now = (Fd+1)*2;
	//保存上一次计算出的位置
	position_old = position_now;
	Serial.print("Position:");
	Serial.println(position_now);
}

void changeservo(void)
{
	
	
	switch(position_now)
	{
		case 1:
		{
			pos = LeftMAX;
			duoji.write(pos);
		}break;
		case 2:
		case 3:
		case 4:
		{
			pos = 90 + (position_now-5)*4;
			duoji.write(pos);
		}break;
		case 5:
		{
			pos = startpos;
			duoji.write(pos);
		}break;
		case 6:
		case 7:
		case 8:
		{
			pos = 90 + (position_now-5)*4;
			duoji.write(pos);
		}break;
		case 9:
		{
			pos = RightMAX;
			duoji.write(pos);
		}break;
	}
	
	
	
}