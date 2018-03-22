#include "NU32.h"
#include "timer.h"

void timer2init(void){
T2CONbits.TCKPS = 0;     // Timer2 prescaler 3 1:4 5kHz
PR2 = 15999;
TMR2 = 0;


IPC2bits.T2IP = 5;                // step 4: interrupt priority
IFS0bits.T2IF = 0;                // step 5: clear CT interrupt flag
IEC0bits.T2IE = 1;                // step 6: enable core timer interrupt
T2CONbits.ON = 1;        // turn on Timer2

}


void timer4init(void){
// 200hz timer
T4CONbits.TCKPS = 4;
TMR4 = 0;
PR4 = 24999;
IEC0bits.T4IE = 1;
IFS0bits.T4IF = 0;
IPC4bits.T4IP = 4;
T4CONbits.ON =1;
}
