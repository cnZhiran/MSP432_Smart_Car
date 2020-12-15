#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <oled.h>
#include <motor.h>
#include <stdio.h>
#include <stdint.h>

#define SET_TIME    1
#define SET_ANGLE   2

unsigned int delay_count = 0,set_mode = SET_ANGLE;
int motor_l=0,motor_r=0;
uint16_t ta3_cnt_l = 0;
long speed_l=0;
unsigned char str[17];
Timer_A_PWMConfig pwm_S1 =
{
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_1,
        60000,
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        4650
};

void delay_10ms(unsigned int ms_time)
{
    delay_count = ms_time;
    while(delay_count);
}
void SYSTick_Config(){
    MAP_SysTick_enableModule();
    MAP_SysTick_setPeriod(480000);
    MAP_SysTick_enableInterrupt();
}
void STEER_Config(){
    MAP_Timer_A_generatePWM(TIMER_A2_BASE, &pwm_S1);
}
void TIMER_A_Config(){
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5, GPIO_PIN6,
            GPIO_PRIMARY_MODULE_FUNCTION);//Steer
    STEER_Config();
}

unsigned int k1_cnt=0,k2_cnt=0;
void KEY_Config(){
    /* Configuring GPIO1.1|1.4 as peripheral input for */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);

    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1|GPIO_PIN4);
    MAP_Interrupt_enableInterrupt(INT_PORT1);
}
Timer_A_ContinuousModeConfig encoder1=
{
         TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK,
         TIMER_A_CLOCKSOURCE_DIVIDER_1,
         TIMER_A_TAIE_INTERRUPT_ENABLE,
         TIMER_A_DO_CLEAR
};
void ENCODER_INIT() {
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P8, GPIO_PIN3,
                GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_Timer_A_configureContinuousMode(TIMER_A3_BASE, &encoder1);
    MAP_Interrupt_enableInterrupt(INT_TA3_N);
    MAP_Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_CONTINUOUS_MODE);
}
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_16);

	SYSTick_Config();
	KEY_Config();
	MOTOR_INIT();
	TIMER_A_Config();

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    OLED_init();
    OLED_clear();
    sprintf((char *)str,"角度设定:%2d度",0);
    OLED_Print_R(0,1,str);
    sprintf((char *)str,"时间设定:%2d秒",10);
    OLED_Print(2,1,str);
    sprintf((char *)str,"duty_l:%5d",motor_l);
    OLED_Print(4,1,str);
    MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_Interrupt_enableMaster();
	while(1){
        MAP_PCM_gotoLPM0();
	}
}
void PORT1_IRQHandler(void)
{
    uint32_t status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    if (status & GPIO_PIN1)
    {
        if(motor_l < 3000 - motor_dead) motor_l += 30;
        if(motor_r < 3000 - motor_dead) motor_r += 30;
    }
    if (status & GPIO_PIN4)
    {
        if(motor_l > -3000 + motor_dead) motor_l -= 30;
        if(motor_r > -3000 + motor_dead) motor_r -= 30;
    }
    MOTOR_Config(motor_l, motor_r);
    /*if (status & GPIO_PIN1)
        {
            if(pwm_S1.dutyCycle < 6000) pwm_S1.dutyCycle += 30;
        }
        if (status & GPIO_PIN4)
        {
            if(pwm_S1.dutyCycle > 3000) pwm_S1.dutyCycle -= 30;
        }
    STEER_Config();*/
    sprintf((char *)str,"duty_l:%5d",motor_l);
    OLED_Print(4,1,str);
}


void SysTick_Handler(void)
{
    if(delay_count) delay_count--;
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

void TA3_N_IRQHandler(void)
{
    MAP_Timer_A_clearInterruptFlag(TIMER_A0_BASE);
    speed_l += 0x10000;
}
