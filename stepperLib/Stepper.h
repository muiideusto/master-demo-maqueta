#include "mbed.h"


class Stepper {
    DigitalOut   dir;
    DigitalOut   step;
    DigitalOut   enable;


public:

    Stepper(PinName step_pin, PinName dir_pin, PinName enable_pin);
   

    /**
     *  
     */
    void enableStepper(void);

    /**
     *  
     */
    void disableStepper(void);



    /**
     *  
     */
    void doStepForward();

    /**
     *  
     */
    void doStepBackward();

        /**
     *  
     */
    void doStepsForward(int steps);

    /**
     *  
     */
    void doStepsBackward(int steps);

            /**
     *  
     */
    void goForward();

    /**
     *  
     */
    void goBackward();

private:    
    /**
     *  
     */
    void doStep(int direcction);

    /**
     *  
     */
    void doSteps(int direcction, int steps);

    /**
     *  
     */
    void go(int direcction);

};