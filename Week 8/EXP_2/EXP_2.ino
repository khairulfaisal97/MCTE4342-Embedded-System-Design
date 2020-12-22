voidWriteByte(intaddress,chardata)
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
*data_register=data; //Stores the data in the EEDR
*control_register=4; //Enable Master Write
*control_register|=2; //Start writing
}
intmain()
{
Serial.begin(9600);
WriteByte(115,168); //Write a value of 168 to the memory location 115
unsignedchara =ReadByte(115); //Read the value at memory location 115
Serial.println(a); //Prints the value
