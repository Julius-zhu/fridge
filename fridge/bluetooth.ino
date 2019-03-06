void setup()
{
  Serial.begin(115200);
  pinMode(road1,OUTPUT);
  pinMode(road2,OUTPUT);
}

void loop()
{
  btread=Serial.read();
  switch(btread){
  case 0xFF:
  switch_all=1;
  break;
  case 0xEE:
  switch_all=0;
  break;
  case 0xDD:
  if(set)
  settemp++;
    else if(settemp==1){
    set--;
    set=1;}
    else
    settemp--;
  break;
  case 0xCC:
  if(settemp>0&&set)
  settemp--;
  else if(settemp==0){
    settemp++;
    set=0;}
    else
    settemp++;
  break;
  case 0xBB:
  switch(settemp/10){
  case 0:
  LED[0]=0;
  LED[1]=settemp;
  LED[2]=0;
  fg=set;
  break;
  case 1:
  LED[0]=0;
  LED[1]=settemp-10;    
  LED[2]=1;
  break;
  case 2:
  LED[0]=0;
  LED[1]=settemp-20;    
  LED[2]=2;
  break;
  }
  break;
  case 0x00:
   LED[0]=xiaoshu1;
   LED[1]=sdata%10;
   LED[2]=sdata/10;
  fg=(tempH<=0x7f);
  break;
  }
}
