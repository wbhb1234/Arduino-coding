//#include "Motor.h"
#include <Servo.h>
#include <PID_v1.h>

Servo duoji;			//����һ������ࣻ
#define startpos 93		//����м�ֵ
#define RightMAX 37	//��������ֵ
#define LeftMAX -37		//��������ֵ
int pos = startpos;		//�������ĽǶ�

int StartSS=14;		//��������һ����
int pos_val[5]={0};		//λ����Ϣ����
int position_now;		//��ǰλ����Ϣ
int position_old;		//��һʱ��λ����Ϣ

//������Ŷ���
const int ENA =7;
const int INA = 5;
const int INB = 6;
const int ENB =8;
//PID Control
double Setpoint, Input, Output;

PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

void sensorinit(int s1);	//��������ʼ��
void getpostion(void);		//��ȡλ����Ϣ
void changeservo(void);		//�ı����Ƕ�

void setup()
{
	/* add setup code here */
	//�����ʼ��
	duoji.attach(9);//ָ��һ��arduino���� ��������ָ����������9
	duoji.write(pos);//���ö����λ�����С�����ͨ���ı�pos ���ı����Ƕȡ�
	//��������ʼ��AN0~AN4
	sensorinit(StartSS);
	//�����ڼ������ٶȿ�����һ���ϵ�ֵ���ԡ�
	pinMode(ENA,OUTPUT);
	digitalWrite(ENA,HIGH);
	pinMode(ENB,OUTPUT);
	digitalWrite(ENB,HIGH);
	pinMode(INA,OUTPUT);
	pinMode(INB,OUTPUT);
	analogWrite(INA,0);
	analogWrite(INB,100);
	//���ڳ�ʼ�� 
	Serial.begin(9600);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}
	
	 getpostion();
	 Input = position_now;
	 Setpoint = 5;
	 myPID.SetMode(AUTOMATIC);
	 myPID.SetOutputLimits(LeftMAX, RightMAX);
	
}

void loop()
{
  /* add main program code here */
    getpostion();
	Input = position_now;
	myPID.Compute();
   // changeservo();
	pos = (int)Output+startpos;
	duoji.write(pos);
}
void sensorinit(int s1)
{
		int i;
		//������Ϊ����
		for (i=0;i<5;i++)
		{
			pinMode(s1+i,INPUT);
		}
		//��ȡ���ݵ�����

}
//ͨ����ȡ��������Ϣ����ȡ����λ����Ϣ��
void getpostion(void)
{
	int Fd;
	int Psd=0;
	int possum=0;
	int i,j;
	//����ѹ�������
	for (i=0;i<5;i++) 
	{
		pos_val[i] = digitalRead(StartSS+i);
	}
	possum = pos_val[0]+pos_val[1]+pos_val[2]+pos_val[3]+pos_val[4];
	//�Ӵ�ӡ
	Serial.print("IR:");
	Serial.print(pos_val[0]);Serial.print(" ");
	Serial.print(pos_val[1]);Serial.print(" ");
	Serial.print(pos_val[2]);Serial.print(" ");
	Serial.print(pos_val[3]);Serial.print(" ");
	Serial.println(pos_val[4]);
	
	//�˳���û��ѹ���ѹ�ĵ�������2�����ϵ����������������������λ����Ϣ���䡣
	if (possum ==5 || possum < 3)
	{
		position_now = position_old;
	}
	//����ѹһ���ѹ�����������Ľ��,����λ�á�
	else
	{
			for (j=0;j<5;j++)
			{
				if (Psd ==0 )
				{
					if (pos_val[j] == 0) //��ɫû�з��䣬Ϊ0
					{
						Fd = j;
						Psd = 1;
					}
				}
			}	
	}
	//����ѹһ�� ���ݹ�ʽ�����λ��
	if (possum == 4)
	{
		position_now = Fd*2+1;
	}
	//����ѹ���� ���ݹ�ʽ�����λ��
	else if(possum == 3) position_now = (Fd+1)*2;
	//������һ�μ������λ��
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