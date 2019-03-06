void setup4()
{
  
}

void loop4()
{
	ReadTemperature();
        if(sdata<40){
        LED[0]=xiaoshu1;
    	LED[1]=sdata%10;
    	LED[2]=sdata/10;}
        delay(2000);
}
