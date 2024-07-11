#include "mbed.h"
#include "arm_book_lib.h"

//##### Defines #####

//##### Public objects #####
DigitalInOut sirenPin(D8);

AnalogIn sensorLM35(A0);
AnalogIn alarmGas(A1); //Empleo Potenciometro no tengo ese sensor

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//##### Public global variables #####
float temperatureLM35 = 0;
float valueGas = 0;  //Empleo Potenciometro no tengo ese sensor

//##### Declaration public functions #####
void init();
void actionBuzzer();
void uartTx(float value, int flag);

//##### Main #####
int main()
{   

    while (true) {
        temperatureLM35 = sensorLM35.read();
        uartTx(temperatureLM35, 1);
        delay(1000);
        valueGas = alarmGas.read();
        uartTx(valueGas, 2);
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

void uartTx(float value, int flag){
    char str[100];
    int stringLength;
    switch (flag) {
        case 1: //Flag 1 Temperatura
            sprintf(str, "Temperature: %.2f C\r\n", value);
            stringLength = strlen(str);
            uartUsb.write(str, stringLength);
        break;

        case 2: //Flag 2 Gas
            sprintf(str, "Gas: %.2f C\r\n", value);
            stringLength = strlen(str);
            uartUsb.write(str, stringLength);
            break;

        default:
            sprintf(str, "Opcion incorrecta \r\n");
            stringLength = strlen(str);
            uartUsb.write(str, stringLength);
            break;

    }        
}