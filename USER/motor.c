#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <motor.h>

Timer_A_PWMConfig pwm1 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        3000,
        TIMER_A_CAPTURECOMPARE_REGISTER_4,
        TIMER_A_OUTPUTMODE_RESET_SET,
        0
};
Timer_A_PWMConfig pwm2 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        3000,
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        0
};
Timer_A_PWMConfig pwm1_n =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        3000,
        TIMER_A_CAPTURECOMPARE_REGISTER_3,
        TIMER_A_OUTPUTMODE_RESET_SET,
        0
};
Timer_A_PWMConfig pwm2_n =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        3000,
        TIMER_A_CAPTURECOMPARE_REGISTER_2,
        TIMER_A_OUTPUTMODE_RESET_SET,
        0
};
void MOTOR_INIT() {
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7,
                GPIO_PRIMARY_MODULE_FUNCTION);
    MOTOR_Config(0,0);
}
void MOTOR_Config(int l,int r) {
    if(l>=0) {
        pwm1_n.dutyCycle = 0;
        pwm1.dutyCycle = l + motor_dead;
        MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwm1_n);
        MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwm1);
    }else {
        pwm1.dutyCycle = 0;
        pwm1_n.dutyCycle = -l + motor_dead;
        MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwm1);
        MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwm1_n);
    }
    if(r>=0) {
        pwm2_n.dutyCycle = 0;
        pwm2.dutyCycle = r + motor_dead;
        MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwm2_n);
        MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwm2);
    }else {
        pwm2.dutyCycle = 0;
        pwm2_n.dutyCycle = -r + motor_dead;
        MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwm2);
        MAP_Timer_A_generatePWM(TIMER_A1_BASE, &pwm2_n);
    }
}
