#ifndef ADC_H
#define ADC_H

#define SAMPLE_TIME 10                                    // 10 core timer ticks = 250 ns
void adc_init(void);                                      // Function to initialise the ADC
unsigned int adc_sample_convert(int pin);                 // Function to read the ADC value

#endif
