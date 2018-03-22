#ifndef UTILITIES_H
#define UTILITIES_H

typedef enum {IDLE, PWM,ITEST,HOLD,TRACK} mode;

static volatile mode MODE;
void setMODE(mode newMODE);             //Function to set the mode
mode getMODE() ;                        //Function to get the current mode


#endif
