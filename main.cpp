#include "mbed.h"
#include "arm_book_lib.h"

//##### Defines #####

//##### Public objects #####
DigitalInOut sirenPin(D8);

AnalogIn sensorLM35(A0);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//##### Public global variables #####
float temperatureLM35 = 0;

//##### Declaration public functions #####
void init();
void actionBuzzer();

//##### Main #####
int main()
{   
    char str[100];
    int stringLength;
    while (true) {
        temperatureLM35 = sensorLM35.read();
        sprintf(str, "Temperature: %.2f \xB0 C\r\n", temperatureLM35);
        stringLength = strlen(str);
        uartUsb.write(str, stringLength);
        delay(1000);
    }
}

//##### Implemention public function #####

void init(){
    sirenPin.mode(OpenDrain);
    sirenPin.input();
}

void actionBuzzer(){
    sirenPin.output();
    sirenPin = LOW;
    delay(2000);
    sirenPin.input();
}