#include <avr/sleep.h>
#include <avr/power.h>

#define LED_PIN (13)

volatile int f_timer=0;

ISR(TIMER1_OVF_vect)
{
  
}

void sleep(void)
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
  power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer2_disable();
  power_twi_disable();  
  /* Now enter sleep mode. */
  sleep_mode();
  /* The program will continue from here after the timer timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  /* Re-enable the peripherals. */
  power_all_enable();
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  /* Normal timer operation.*/
  TCCR1A = 0x00; 
  /* Clear the timer counter register.
   * You can pre-load this register with a value in order to 
   * reduce the timeout period, say if you wanted to wake up
   * every 4.0 seconds exactly.
   */
  TCNT1=0x0000; 
  /* Configure the prescaler for 1:1024, giving us a 
   * timeout of 4.09 seconds.
   */
  TCCR1B = 0x05;
  /* Enable the timer overlow interrupt. */
  TIMSK1=0x01;
}


void loop()
{
  
    sleep();
}
