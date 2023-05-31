# AHT10 sensor on FRDM-KL05Z board
## Demo
This was a project I did for microcontroller class. It uses the PIT counter to periodically get a temperature and humidity reading from the AHT10 sensor. It then sends formatted information by UART.
## AHT10.h and AHT10.c
This two files implement all the function relevant to AHT10 and I2C communication. Here's an example of usage:
`uint32_t read_t,read_h;
double temp=0.0,hum=0.0;

AHT10_Init();
AHT10_GetRawReadings(&read_t,&read_h);
AHT10_ConvertReadings(&temp,&hum,read_t,read_h);`
The AHT10_Init() function "wakes up" the sensor by sending the initialization command. There's a 500ms delay which may not be necessary but I found that the sensor is more reliable when it's present. 
The AHT10_GetRawReadings() function will begin the measurement and return the raw data sent by the sensor. There's an 80ms delay between sending the command and reading the data which once again may have to be adjusted depending on the device - I found 80 working best for me. That data can be converted into Celsius and humidity percentage by calling the AHT10_ConvertReadings() function.
