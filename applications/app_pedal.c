#include "ch.h" // ChibiOS
#include "hal.h" // ChibiOS HAL
#include "mc_interface.h" // Motor control functions
#include "hw.h" // Pin mapping on this hardware
#include "timeout.h" // To reset the timeout
 
// Example thread
static THD_FUNCTION(example_thread, arg);
static THD_WORKING_AREA(example_thread_wa, 2048); // 2kb stack for this thread
void app_pedal_init(void) {
 
    // Start the example thread
    chThdCreateStatic(example_thread_wa, sizeof(example_thread_wa),
        NORMALPRIO, example_thread, NULL);
}
 
static THD_FUNCTION(example_thread, arg) {
    (void)arg;
    static float MAX_CURRENT=80;
 
    chRegSetThreadName("APP_EXAMPLE");
 
    for(;;) {
        float pot = (float)ADC_Value[ADC_IND_EXT];
        pot /= 4095.0;
        if(pot>=0.15){
            mc_interface_set_current((pot-0.15)*2.19*MAX_CURRENT);
        }
	else{
	    mc_interface_set_pid_speed(0);
	}
 
        // Run this loop at 500Hz
        chThdSleepMilliseconds(2);
 
        // Reset the timeout
        timeout_reset();
    }
}
