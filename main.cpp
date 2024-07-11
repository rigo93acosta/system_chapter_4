#include "mbed.h"
#include "arm_book_lib.h"

//##### Defines #####

//##### Public objects #####
DigitalInOut sirenPin(D8);

//##### Public global variables #####

//##### Declaration public functions #####



//##### Main #####
int main()
{   
    sirenPin.mode(OpenDrain);
    sirenPin.input();
    while (true) {
        sirenPin.output();
        sirenPin = LOW;
        delay(2000);
        sirenPin.input();
    }
}

//##### Implemention public function #####

