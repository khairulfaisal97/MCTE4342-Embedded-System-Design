classEEPROM
{
public:
voidWriteByte(intaddress,chardata)
{
//Code for Writing
}
charReadByte(intaddress)
{
//Code for Writing
}
};
intmain()
{
EEPROM eeprom;//Creates an instance of EEPROM class
Serial.begin(9600);
eeprom.WriteByte(115,168); //Write a value of 168 to the memory location 115
unsignedchar a =eeprom.ReadByte(115); //Read the value at memory location 115
Serial.println(a); //Prints the value
}
