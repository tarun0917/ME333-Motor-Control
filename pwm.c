#include "NU32.h"
#include "pwm.h"
#include "utilities.h"
#include "encoder.h"

#include <stdio.h>


void pwminit(void){
OC1CONbits.OCTSEL = 1;   //enable timer 3   1:1  20kHz
T3CONbits.TCKPS = 1;     // Timer3 prescaler N=4 (1:4)
PR3 = 1999;              // period = (PR2+1) * N * 12.5 ns = 100 us, 10 kHz
TMR3 = 0;                // initial TMR2 count is 0
OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
OC1RS = 500;             // duty cycle = OC1RS/(PR2+1) = 25%
OC1R = 500;              // initialize before turning OC1 on; afterward it is read-only
T3CONbits.ON = 1;        // turn on Timer3
OC1CONbits.ON = 1;       // turn on OC1
TRISEbits.TRISE0 = 0;    //configure pin as output
LATEbits.LATE0 = 0;      // set default value, phase pin
TRISEbits.TRISE1 = 0;    //motor enable to pin E1
}


void setspeed(void){
  setMODE(PWM);
  NU32_ReadUART3(buffer, BUF_SIZE);
  sscanf(buffer, "%f", & pwm);
  OC1RS = (unsigned int) abs(pwm) * 20;
  if (pwm < 0) {
    LATEbits.LATE0 = 0;
  } else {
    LATEbits.LATE0 = 1;
  }
  sprintf(buffer, "%f\r\n", pwm);
  NU32_WriteUART3(buffer);
}


void setidle(void){
  setMODE(IDLE);
  LATEbits.LATE0 = 0;
  OC1RS = 0;
  sprintf(buffer, "%d\r\n", OC1RS);
  NU32_WriteUART3(buffer);
}
