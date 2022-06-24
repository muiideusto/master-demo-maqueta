 #include "Ultrasonic.h"
 
    
    

  Ultrasonic::Ultrasonic(PinName trigger_pin, PinName echo_pin) : trigger(trigger_pin), echo(echo_pin)
    {
        trigger = 0;
        distance = -1;

        echo.rise(callback(this, &Ultrasonic::echo_in));    // Attach handler to the rising interruptIn edge
        echo.fall(callback(this, &Ultrasonic::echo_fall));  // Attach handler to the falling interruptIn edge
    }

    /**
     *  Start the background task to trigger sonar reads every 100ms
     */
    void Ultrasonic::start(void)
    {
        ticker.attach(callback(this, &Ultrasonic::background_read), 50ms);
    }

    /**
     *  Stop the background task that triggers sonar reads
     */
    void Ultrasonic::stop(void)
    {
        ticker.detach();
    }

    /**
     *  Interrupt pin rising edge interrupt handler. Reset and start timer
     */
    void Ultrasonic::echo_in(void)
    {
        timer.reset();
        timer.start();
        begin = timer.elapsed_time().count();
    }

    /**
     *  Interrupt pin falling edge interrupt handler. Read and disengage timer.
     *  Calculate raw echo pulse length
     */
    void Ultrasonic::echo_fall(void)
    {
        end = timer.elapsed_time().count();
        timer.stop();
        distance = end - begin;
    }

    /**
     *  Wrapper function to set the trigger pin low. Callbacks cannot take in both object and argument pointers.
     *  See use of this function in background_read().
     */
    void Ultrasonic::trigger_toggle(void)
    {
        trigger = 0;
    }

    /**
     *  Background callback task attached to the periodic ticker that kicks off sonar reads
     */
    void Ultrasonic::background_read(void)
    {
        trigger = 1;
        timeout.attach(callback(this, &Ultrasonic::trigger_toggle), 10us);
    }

    /**
     *  Public read function that returns the scaled distance result in cm
     */
    float Ultrasonic::read(void)
    {
        return distance / 58.0f; 
    }