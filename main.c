/*This is a motor contol project. I developed this code as a part of ME333's final project.
The code has been built in a modular fashion. I have built the various modules according to
the types such as timers, pwm etc*/

#include "NU32.h" 
#include <stdio.h>
#include "encoder.h"
#include "adc.h"
#include "timer.h"
#include "pwm.h"
#include "currentcontrol.h"
#include "positioncontrol.h"
#include "utilities.h"


static volatile int adcval = 0;
static volatile int counter = 0;



#define VOLTS_PER_COUNT (3.3/1024)
#define CORE_TICK_TIME 25    // nanoseconds between core ticks
#define SAMPLE_TIME 10       // 10 core timer ticks = 250 ns
#define DELAY_TICKS 20000000 // delay 1/2 sec, 20 M core ticks, between messages
// isr for 5khz

void __ISR(_TIMER_4_VECTOR, IPL4SOFT) motorController(void) {

    switch (getMODE()) {
    //
    case HOLD:
    {
        motorangle = (encoder_counts() - 32768) * 0.2;
        errorangle = motorangle - userangle;
        errorangleint += kimotor * errorangle;
        umotor = kpmotor * (errorangle) + errorangleint + kdmotor * (motorangle - prevangle);
        if (umotor > 400.0) {
            umotor = 400.0;
        } else if (umotor < -400) {
            umotor = -400;
        }
        prevangle = motorangle;
        break;
    }
    case TRACK:
    {
        if (trackcount < tracklength) {
            trackmotorangle[trackcount] = (encoder_counts() - 32768) * 0.2;
            trackerrorangle = trackmotorangle[trackcount] - trackuserangle[trackcount];
            errorangleint += kimotor * trackerrorangle;
            umotortrack = kpmotor * (trackerrorangle) + errorangleint + kdmotor * (trackmotorangle[trackcount] - prevangle);
            if (umotortrack > 400.0) {
                umotortrack = 400.0;
            } else if (umotortrack < -400) {
                umotortrack = -400;
            }
            prevangle = trackmotorangle[trackcount];
            trackcount++;
        } else {
            trackcount = 0;
            userangle = (float) trackuserangle[tracklength - 1];
            errorangleint = 0;
            prevangle = userangle;
            setMODE(HOLD);
        }
        break;
    }

    default:
    {
        break;
    }
    }
    IFS0bits.T4IF = 0; // clear CT int flag
}

void __ISR(_TIMER_2_VECTOR, IPL5SOFT) Controller(void) {

    adcval = 2 * adc_sample_convert(7) - 1023; // sample and convert pin 14
    switch (getMODE()) {
    case IDLE:
    {
        OC1RS = 0;
        errorangle = 0;

        motorangle = 0;
        errorcurrent = 0;
        break;
    }

    case HOLD:
    {

        errorcurrent = adcval - umotor;
        errorcurrentint += errorcurrent;
        ucurrent = kpcurrent * (errorcurrent) + kicurrent * (errorcurrentint);
        ucurrentnew = ucurrent;

        if (ucurrentnew > 100.0) {
            ucurrentnew = 100.0;
        } else if (ucurrentnew < -100) {
            ucurrentnew = -100;
        }

        pwm = ucurrentnew;
        OC1RS = (unsigned int) abs(pwm) * 20;
        if (pwm < 0) {
            LATEbits.LATE0 = 1; // setting motor direction, phase
        } else {
            LATEbits.LATE0 = 0;
        }
        break;
    }

    case ITEST:
    {
        errorcurrent = -Waveform[counter] + adcval;
        errorcurrentint += errorcurrent;
        ucurrent = kpcurrent * (errorcurrent) + kicurrent * (errorcurrentint);
        ucurrentnew = ucurrent;
        if (ucurrentnew > 100.0) {
            ucurrentnew = 100.0;
        } else if (ucurrentnew < -100) {
            ucurrentnew = -100;
        }
        pwm = ucurrentnew;
        OC1RS = (unsigned int) abs(pwm) * 20;
        if (pwm < 0) {
            LATEbits.LATE0 = 1; // setting motor direction, phase
        } else {
            LATEbits.LATE0 = 0;
        }

        if (counter < NUMSAMPS) {
            currentvalue[counter] = adcval;
            counter++;

        } else {
            counter = 0;
            setMODE(IDLE);
        }
        break;
    }

    case TRACK:
    {

        errorcurrent = adcval - umotortrack;
        errorcurrentint += errorcurrent;
        ucurrent = kpcurrent * (errorcurrent) + kicurrent * (errorcurrentint);
        ucurrentnew = ucurrent;

        if (ucurrentnew > 100.0) {
            ucurrentnew = 100.0;
        } else if (ucurrentnew < -100) {
            ucurrentnew = -100;
        }

        pwm = ucurrentnew;
        OC1RS = (unsigned int) abs(pwm) * 20;
        if (pwm < 0) {
            LATEbits.LATE0 = 1; // setting motor direction, phase
        } else {
            LATEbits.LATE0 = 0;
        }
        break;
    }
    default:
    {
        break;
    }
    }

    IFS0bits.T2IF = 0; // clear CT int flag
}

int main() {

    float kpcurrenttemp = 0, kicurrenttemp = 0;
    float kpmotortemp = 0, kdmotortemp = 0, kimotortemp = 0;
    unsigned int a = 0;
    char buffer[BUF_SIZE];
    NU32_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
    encoder_init();
    adc_init();
    NU32_LED1 = 1; // turn off the LEDs
    NU32_LED2 = 1; //turn off the LEDs

    __builtin_disable_interrupts();
    //timer initalisation
    timer2init();
    timer4init();
    __builtin_enable_interrupts();
    pwminit();

    OC1RS = 0;

    //generate the reference waveform
    makeWaveform();

    while (1) {
        NU32_ReadUART3(buffer, BUF_SIZE);
        NU32_LED2 = 1;
        switch (buffer[0]) {
        case 'a':
        {
            getcurrentcounts();
            break;
        }

        case 'b':
        {
            getcurrentma();
            break;
        }

        case 'c':
        {
            sprintf(buffer, "%d\r\n", encoder_counts());
            NU32_WriteUART3(buffer);
            break;
        }

        case 'd':
        {
            encoder_degree();
            break;
        }

        case 'e':
        {
            sprintf(buffer, "%d\r\n", encoder_reset());
            NU32_WriteUART3(buffer);
            break;
        }

        case 'f':
        {
            setspeed();
            break;
        }
        case 'g':
        {
            readcurrentgain();
            break;
        }
        case 'h':
        {
            writecurrentgain();
            break;
        }

        case 'i':
        {
            readpositiongain();
            break;
        }

        case 'j':
        {
            writepositiongain();
            break;
        }


        case 'k':
        {
            counter = 0;
            setMODE(ITEST);
            while (counter < 99) {
                ;
            }
            sendcurrentdata();
            break;
        }
        case 'l':
        {
            readuserangle();
            break;
        }

        case 'm':
        {

            readstep();
            break;
        }

        case 'n':
        {
            readcubic();
            break;
        }

        case 'o':
        {
            trackerrorangle = 0;
            errorcurrentint = 0;
            trackcount =0;
            setMODE(TRACK);
            while(getMODE()==4) {
                ;
            }
            sendmotordata();
            break;
        }

        case 'p':
        {
            setidle();
            break;
        }

        case 'q':
        {
            setMODE(IDLE);
            break;
        }


        case 'r':
        {
            sprintf(buffer, "%d\r\n", getMODE());
            NU32_WriteUART3(buffer);
            break;
        }


        default:
        {
            NU32_LED2 = 0; // turn on LED2 to indicate an error
            break;
        }
        }
    }
    return 0;
}
