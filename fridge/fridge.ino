#define DQ A3
 char btread;
 boolean set=1;
 boolean switch_all=0;
 byte settemp=4;
 int road1=9,road2=10;
 int SCLK = 2;
 int RCLK = 3;
 int DIO = 4; //这里定义了那三个脚
 unsigned int LED_0F[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xbf};
 unsigned int LED[4];	//用于LED的4位显示缓存
 byte  tempL=0; 		//设全局变量
 byte tempH=0; 
 byte sdata;		//测量到的温度的整数部分
 byte xiaoshu1;		//小数第一位
 byte xiaoshu2;		//小数第二位
 byte xiaoshu;		//两位小数
 boolean fg=1;        	//温度正负标志

void Init_DS18B20()
{
   int x;
   pinMode(DQ,OUTPUT);
   digitalWrite(DQ, HIGH);  		//DQ先置高 
   delay(80); 			//稍延时
   digitalWrite(DQ, LOW); 		//发送复位脉冲 
   delayMicroseconds(500);      //延时（>480us) 
   digitalWrite(DQ, HIGH);  		//拉高数据线 
   delayMicroseconds(60); 	//等待（15~60us) 
   pinMode(DQ,INPUT);
   x=digitalRead(DQ);
   //用X的值来判断初始化有没有成功，18B20存在的话X=0，否则X=1
   pinMode(DQ,OUTPUT);
   digitalWrite(DQ, HIGH); 
   delayMicroseconds(300);
}

byte ReadOneChar()  			//主机数据线先从高拉至低电平1us以上，再使数据线升为高电平，从而产生读信号
{
	 byte i=0; 		//每个读周期最短的持续时间为60us，各个读周期之间必须有1us以上的高电平恢复期
	 byte dat=0;
	for (i=8;i>0;i--) 		//一个字节有8位 
	{
                dat>>=1;
		digitalWrite(DQ, LOW); 
                delayMicroseconds(1);
		digitalWrite(DQ,INPUT);
		if(digitalRead(DQ)) 
		dat|=0x80;
                Serial.print(digitalRead(DQ));
		delayMicroseconds(13);
	} 
	return(dat);
}

void WriteOneChar(byte dat) 
{ 
	byte i=0; 		        //数据线从高电平拉至低电平，产生写起始信号。15us之内将所需写的位送到数据线上，
        for(i=8;i>0;i--) 		//在15~60us之间对数据线进行采样，如果是高电平就写1，低写0发生。 
	{
                pinMode(DQ,OUTPUT);
                digitalWrite(DQ, LOW);	//在开始另一个写周期前必须有1us以上的高电平恢复期。 
		delayMicroseconds(1);
                if(dat&0x01)
                digitalWrite(DQ,HIGH);
                else
                digitalWrite(DQ, LOW);
		delayMicroseconds(50); 
		dat>>=1;
                digitalWrite(DQ, HIGH);
	} 
	delay(4);
}

void ReadTemperature(void) 
{ 
	Init_DS18B20(); 					//初始化
	WriteOneChar(0xcc); 				//跳过读序列号的操作
	WriteOneChar(0x44); 				//启动温度转换
	delay(1000); 						//转换需要一点时间，延时 
	pinMode(DQ,INPUT);
        Init_DS18B20(); 					//初始化
	WriteOneChar(0xcc); 				//跳过读序列号的操作 
	WriteOneChar(0xbe); 				//读温度寄存器（头两个值分别为温度的低位和高位） 
	tempL=ReadOneChar(); 				//读出温度的低位LSB
	tempH=ReadOneChar(); 				//读出温度的高位MSB	
	if(tempH>0x7f)      				//最高位为1时温度是负
	{
		tempL=~tempL;					//补码转换，取反加一
		tempH=~tempH;       
		fg=0;      						//读取温度为负时fg=0
	}
        else
        fg=1;
	sdata = tempL/16+tempH*16;      	//整数部分
	xiaoshu1 = (tempL&0x0f)*10/16; 		//小数第一位
	xiaoshu2 = (tempL&0x0f)*100/16%10;	//小数第二位
	xiaoshu=xiaoshu1*10+xiaoshu2; 		//小数两位
}

void LED4_Display (void)
{
    	unsigned int *led_table;          // 查表指针
    	unsigned int i;
    	//显示第1位
    	led_table = LED_0F + LED[0];
    	i = *led_table;
    	LED_OUT(i);			
    	LED_OUT(0x01);		
        digitalWrite(RCLK,LOW);
        digitalWrite(RCLK,HIGH);
    	//显示第2位
    	led_table = LED_0F + LED[1];
    	i = *led_table-0x80;
    	LED_OUT(i);		
    	LED_OUT(0x02);		
        digitalWrite(RCLK,LOW);
        digitalWrite(RCLK,HIGH);
    	//显示第3位
    	led_table = LED_0F + LED[2];
    	i = *led_table;
    	LED_OUT(i);			
    	LED_OUT(0x04);	
        digitalWrite(RCLK,LOW);
        digitalWrite(RCLK,HIGH);
    	//显示第4位
    	if(fg)
        LED_OUT(0xFF);
        else
        LED_OUT(0xbf);		
    	LED_OUT(0x08);
        digitalWrite(RCLK,LOW);
        digitalWrite(RCLK,HIGH);
}
    
void LED_OUT(unsigned int X)
{
    unsigned int i;
    for(i=8;i>=1;i--){
    	if (X&0x80) 
            digitalWrite(DIO,HIGH);
        else 
         
            digitalWrite(DIO,LOW);
    	X<<=1;
        digitalWrite(SCLK,LOW);
        digitalWrite(SCLK,HIGH);
    	}
}

