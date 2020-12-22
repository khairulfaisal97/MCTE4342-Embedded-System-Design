charReadByte(intaddress)
{
char*data_register= (char*)0x40; //Points to EEDR
volatile char*control_register= (char*)0x3F; //Points to EECR
int*address_register= (int*)0x41; //Points to EEAR. Note that intis 16bit on ATmega328p
//This pointer points to both low byte and high byte
//of EEAR
while(((*control_register) &2))//If the data is being written (EEPE is high)
{
//do nothing
}
*address_register=address; //Stores the address in the EEAR
*control_register=1; //Set EERE (Initiate reading)
return*data_register; //Return the contents of the data register (EEDR)
}
intmain()
{
Serial.begin(9600);
chara =ReadByte(115); //Read the value of the EEPROM at memory address 115
Serial.println(a); //Prints the value of the EEPROM at memory address 115
