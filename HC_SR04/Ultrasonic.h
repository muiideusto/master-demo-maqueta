#include "mbed.h"


class Ultrasonic {
    DigitalOut   trigger;
    InterruptIn  echo;     // calls a callback when a pin changes
    Timer        timer;
    Timeout      timeout;  // calls a callback once when a timeout expires
    Ticker       ticker;   // calls a callback repeatedly with a timeout
    int32_t      begin;
    int32_t      end;
    float        distance;

public:

    Ultrasonic(PinName trigger_pin, PinName echo_pin);
   

    /**
     *  Start the background task to trigger sonar reads every 100ms
     */
    void start(void);


    /**
     *  Stop the background task that triggers sonar reads
     */
    void stop(void);


    /**
     *  Interrupt pin rising edge interrupt handler. Reset and start timer
     */
    void echo_in(void);


    /**
     *  Interrupt pin falling edge interrupt handler. Read and disengage timer.
     *  Calculate raw echo pulse length
     */
    void echo_fall(void);


    /**
     *  Wrapper function to set the trigger pin low. Callbacks cannot take in both object and argument pointers.
     *  See use of this function in background_read().
     */
    void trigger_toggle(void);
 

    /**
     *  Background callback task attached to the periodic ticker that kicks off sonar reads
     */
    void background_read(void);


    /**
     *  Public read function that returns the scaled distance result in cm
     */
    float read(void);

};