intmain()//Just change this to void setup() for Arduino
{
DisableADCandComparator();
EnableExternalInterrupts();
for(;;)
{
DeepSleep();
char*pind=(char*)0x29;
boolred_pressed= ((*pind) & 2);//PD2
boolgreen_pressed= ((*pind) & 4);//PD3
if(red_pressed)
{
TransmitChannel1();
}
if(green_pressed)
{
TransmitChannel2();
}
}
}
ISR(INT0_vect)
{
}
ISR(IN1_vect)
{
}
void DisableADCandComparator()
{
char*adcsra = (char*)0x7A;
char*acsr = (char*) 0x50;
*adcsra= 0; //Disable ADC
*acsr= 1<< 7; //Disable comparator
}
voidEnableExternalInterrupts()
{
char*sreg=(char*)0x5F;
char*eimsk=(char*)0x3D;
char*eicra=(char*)0x69;
*sreg|=(1 <<7); //Enable interrupts
*eimsk=3; //Enable INT0 and INT1
*eicra=15; //Set trigger mode to LOW TO HIGH
}
voidTransmitChannel1()
{
//Transmit code for channel 1
}
voidTransmitChannel2()
{
//Transmit code for channel 2
}
