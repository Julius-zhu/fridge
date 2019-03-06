void setup2()
{ 
   pinMode(SCLK,OUTPUT);
   pinMode(RCLK,OUTPUT);
   pinMode(DIO,OUTPUT); //让三个脚都是输出状态
   Serial.begin(115200);

}
void loop2()
{
    LED4_Display ();
    if((set<(tempH<=0x7f)||(!set&&settemp>=sdata)||((tempH<=0x7f)&&(settemp<=sdata)))&&switch_all){
    digitalWrite(road1,HIGH);
    digitalWrite(road2,HIGH);}
    else{
    digitalWrite(road1,LOW);
    digitalWrite(road2,LOW);
    }
}
