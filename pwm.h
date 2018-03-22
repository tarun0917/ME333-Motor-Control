#ifndef PWM_H
#define PWM_H

volatile float pwm = 0;
void pwminit(void);           //Function to initialise the PWM
void setspeed(void);          //Function to set the pwm value which sets the speed of the motor
void setidle(void);           //Function to set the pwm to 0 which sets the speed of the motor to 0 

#endif
