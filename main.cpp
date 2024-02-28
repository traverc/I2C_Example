#include "mbed.h"
#include "arm_book_lib.h"

//I2C pin definitions
#define I2C1_SDA  PB_9
#define I2C1_SCL  PB_8

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

I2C  digitalPot(I2C1_SDA, I2C1_SCL);
const int addr7bit = 0x28;      // 7 bit I2C address
const int addr8bit = 0x28 << 1; // 8bit I2C address, 0x50
char midiMessage[3] = {0x80, 60, 100};

#define TIME_INCREMENT_MS  1 //Loop delay
int main() {
    digitalPot.frequency(400000); //400KHz
    char data[] = {0x02, 0x80};  //Initialize to use 0x02 control register to set 0x80 mode 1
                                 //Mode 1 means writes go to Wiper register only
    digitalPot.write(addr8bit, data, 2); //send the i2c address and set control reg to mode 1
    data[0] = 0x00;  // now change the register to the wiper register
    data[1] = 0;  //we will start with a wiper value of 0 to set VRW=0
    digitalPot.write(addr8bit, data, 2);  
    delay(10000); //hold it here for 10 seconds so we can measure it
    while (true) {
    //now we will change the wiper value to 128/2 to get VRW of 3.3/2V  (about 1.65V)
        data[1] = 64;
        digitalPot.write(addr8bit, data, 2); //write the 64 value to WR
        delay(TIME_INCREMENT_MS);
    }
}
