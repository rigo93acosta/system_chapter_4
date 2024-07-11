#include "mbed.h"
#include "arm_book_lib.h"

//##### Defines #####
#define NUMBER_OF_AVG_SAMPLES 10
#define OVER_TEMP_LEVEL 50

#define POTENTIOMETER_OVER_GAS_LEVEL 0.5


//##### Public objects #####
DigitalInOut sirenPin(D8);
DigitalIn buttomTestAlarm(BUTTON1);

DigitalOut incorrectLed(D9); //Led Azul
DigitalOut systemBlockedLed(D10); //Led Rojo


AnalogIn sensorLM35(A0);
AnalogIn alarmGas(A1); //Empleo Potenciometro no tengo ese sensor

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//##### Public global variables #####
int lm35SampleIndex = 0;
float lm35ReadingsArray [NUMBER_OF_AVG_SAMPLES];
float lm35ReadingsSum = 0.0;
float lm35ReadingsAverage = 0.0;
float lm35TempC = 0.0;

float valueGas = 0;  //Empleo Potenciometro no tengo ese sensor

//##### Declaration public functions #####
void inputsInit();
void outputsInit();
void actionBuzzer();
void uartTx(float value, int flag);
void testAlarm();

float analogReadingScaledWithTheLM35Formula( float analogReading );
float celsiusToFahrenheit( float tempInCelsiusDegrees );

void lm35ReadingsArrayInit();
void readTemperature();

//##### Main #####
int main()
{   

    inputsInit();
    outputsInit();
    void lm35ReadingsArrayInit();

    while (true) {
        testAlarm();
        readTemperature();
        uartTx(lm35TempC, 1);
        thread_sleep_for(1000);
        valueGas = alarmGas.read();
        uartTx(valueGas, 2);
        thread_sleep_for(1000);
    }
}

//##### Implemention public function #####
void inputsInit(){
    buttomTestAlarm.mode(PullDown);
    sirenPin.mode(OpenDrain);
    sirenPin.input();
}

void outputsInit(){
    incorrectLed = OFF;
    systemBlockedLed = OFF;
}

void testAlarm(){
    if(!buttomTestAlarm){
        actionBuzzer();
    }
}

void actionBuzzer(){
    sirenPin.output();
    sirenPin = LOW;
    thread_sleep_for(1000);
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

void readTemperature(){

    static int lm35SampleIndex = 0;
    lm35ReadingsArray[lm35SampleIndex] = sensorLM35.read();
    lm35SampleIndex++;
    if (lm35SampleIndex >= NUMBER_OF_AVG_SAMPLES) {
      lm35SampleIndex = 0;
    }

    lm35ReadingsSum = 0.0;
    for (int i = 0; i < NUMBER_OF_AVG_SAMPLES; i++) {
      lm35ReadingsSum = lm35ReadingsSum + lm35ReadingsArray[i];
    }
    lm35ReadingsAverage = lm35ReadingsSum / NUMBER_OF_AVG_SAMPLES;
    lm35TempC = analogReadingScaledWithTheLM35Formula ( lm35ReadingsAverage );
}


float analogReadingScaledWithTheLM35Formula(float analogReading) {
  return (analogReading * 3.3 / 0.01);
}

float celsiusToFahrenheit(float tempInCelsiusDegrees) {
  return (tempInCelsiusDegrees * 9.0 / 5.0 + 32.0);
}

void lm35ReadingsArrayInit() {
  int i;
  for (i = 0; i < NUMBER_OF_AVG_SAMPLES; i++) {
    lm35ReadingsArray[i] = 0;
  }
}