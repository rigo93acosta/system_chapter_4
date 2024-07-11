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
void uartTx(float value);

//##### Main #####
int main()
{   

    while (true) {
        temperatureLM35 = sensorLM35.read();
        uartTx(temperatureLM35);
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

void uartTx(float value){
    char str[100];
    int stringLength;
    sprintf(str, "Temperature: %.2f C\r\n", value);
    stringLength = strlen(str);
    uartUsb.write(str, stringLength);
}