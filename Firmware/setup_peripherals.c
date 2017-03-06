/*
 * setup_peripherals.c
 *
 * Created: 12/7/2016 6:47:57 PM
 *  Author: Shiva and Rishabh
 */ 

#include <asf.h>
#include "main.h"

//! [setup]
static void configure_uart1(struct uart_module *uart1_instance)
{
	//! [setup_config]
	struct uart_config config_uart;
	//! [setup_config]
	//! [setup_config_defaults]
	uart_get_config_defaults(&config_uart);
	//! [setup_config_defaults]

	//! [setup_change_config]
	config_uart.baud_rate	= 9600;
	config_uart.pin_number_pad[0] = EDBG_CDC_PIN_PAD0;
	config_uart.pin_number_pad[1] = EDBG_CDC_PIN_PAD1;
	config_uart.pin_number_pad[2] = EDBG_CDC_PIN_PAD2;
	config_uart.pin_number_pad[3] = EDBG_CDC_PIN_PAD3;
	
	config_uart.pinmux_sel_pad[0] = EDBG_CDC_MUX_PAD0;
	config_uart.pinmux_sel_pad[1] = EDBG_CDC_MUX_PAD1;
	config_uart.pinmux_sel_pad[2] = EDBG_CDC_MUX_PAD2;
	config_uart.pinmux_sel_pad[3] = EDBG_CDC_MUX_PAD3;
	//! [setup_change_config]

	//! [setup_set_config]
	while (uart_init(uart1_instance,
	EDBG_CDC_MODULE, &config_uart) != STATUS_OK) {
	}

	//! [setup_set_config]
}

static void configure_uart3(struct uart_module *uart3_instance)
{
	//! [setup_config]
	struct uart_config config_uart;
	//! [setup_config]
	//! [setup_config_defaults]
	uart_get_config_defaults(&config_uart);
	//! [setup_config_defaults]

	//! [setup_change_config]
	config_uart.baud_rate = 9600;
	config_uart.pin_number_pad[0] = EXT3_UART_PIN_PAD0;
	config_uart.pin_number_pad[1] = EXT3_UART_PIN_PAD1;
	config_uart.pin_number_pad[2] = EDBG_CDC_PIN_PAD2;
	config_uart.pin_number_pad[3] = EDBG_CDC_PIN_PAD3;
	
	config_uart.pinmux_sel_pad[0] = EXT3_UART_MUX_PAD0;
	config_uart.pinmux_sel_pad[1] = EXT3_UART_MUX_PAD1;
	config_uart.pinmux_sel_pad[2] = EDBG_CDC_MUX_PAD2;
	config_uart.pinmux_sel_pad[3] = EDBG_CDC_MUX_PAD3;
	//! [setup_change_config]

	//! [setup_set_config]
	while (uart_init(uart3_instance,EXT3_UART_MODULE, &config_uart) != STATUS_OK) {
	}
	//! [setup_set_config]
}
//! [setup]

static void configure_gpio_pins(void)
{
	//! [setup_gpio_1]
	struct gpio_config config_gpio_pin;
	//! [setup_gpio_1]
	//! [setup_gpio_2]
	gpio_get_config_defaults(&config_gpio_pin);
	//! [setup_gpio_2]

	//! [setup_gpio_3]
	config_gpio_pin.direction = GPIO_PIN_DIR_OUTPUT;
	//! [setup_gpio_3]
	//! [setup_gpio_4]
	gpio_pin_set_config(PWRKEY_PIN, &config_gpio_pin);
	gpio_pin_set_output_level(PWRKEY_PIN,true);
	//! [setup_gpio_4]
	
}

static void configure_gpio_pin_input(void)
{
	//! [setup_gpio_1]
	struct gpio_config config_gpio_pin_in;
	//! [setup_gpio_1]
	//! [setup_gpio_2]
	gpio_get_config_defaults(&config_gpio_pin_in);
	//! [setup_gpio_2]

	//! [setup_gpio_3]
	config_gpio_pin_in.direction = GPIO_PIN_DIR_INPUT;
	config_gpio_pin_in.input_pull = GPIO_PIN_PULL_NONE;
	//! [setup_gpio_3]
	//! [setup_gpio_4]
	gpio_pin_set_config(SIM_PWRSTATE_PIN, &config_gpio_pin_in);
	//! [setup_gpio_4]
}

static void configure_gpio_rx_pin_input(void)
{
	//! [setup_gpio_1]
	struct gpio_config config_gpio_pin_in1;
	//! [setup_gpio_1]
	//! [setup_gpio_2]
	gpio_get_config_defaults(&config_gpio_pin_in1);
	//! [setup_gpio_2]

	//! [setup_gpio_3]
	config_gpio_pin_in1.direction = GPIO_PIN_DIR_INPUT;
	config_gpio_pin_in1.input_pull = GPIO_PIN_PULL_NONE;
	//! [setup_gpio_3]
	//! [setup_gpio_4]
	gpio_pin_set_config(PIN_LP_GPIO_2, &config_gpio_pin_in1);
	//! [setup_gpio_4]
}

static void configure_dualtimer(void)
{
	//! [setup_dualtimer_1]
	struct dualtimer_config config_dualtimer;
	//! [setup_dualtimer_1]

	//! [setup_dualtimer_2]
	dualtimer_get_config_defaults(&config_dualtimer);

	config_dualtimer.timer1.load_value = RECV_TIMEOUT;
	config_dualtimer.timer2.load_value = PWRKEY_TIME;
	
	dualtimer_init(&config_dualtimer);
	dualtimer_disable(RECVTIMEOUT_TIMER);
	dualtimer_disable(PWRKEY_TIMER);
	
	//dualtimer_enable(DUALTIMER_TIMER2);
	//! [setup_dualtimer_5]
}

static void configure_dualtimer_callback(void)
{
	//! [setup_register_callback]
	dualtimer_register_callback(DUALTIMER_TIMER1, recvTimeout_callback);
	dualtimer_register_callback(DUALTIMER_TIMER2, pwrkey_callback);
	//! [setup_register_callback]

	//! [enable_IRQ]
	NVIC_EnableIRQ(DUALTIMER0_IRQn);
	//! [enable_IRQ]
}

static void configure_gpio_wakeup(void)
{
	//! [setup_gpio_1]
	struct gpio_config config_gpio_pin_wakeup;
	//! [setup_gpio_1]
	gpio_get_config_defaults(&config_gpio_pin_wakeup);
	//! [setup_gpio_1]
	
	//! [setup_gpio_2]
	config_gpio_pin_wakeup.direction  = GPIO_PIN_DIR_INPUT;
	config_gpio_pin_wakeup.input_pull = GPIO_PIN_PULL_NONE;
	config_gpio_pin_wakeup.aon_wakeup = true;
	//! [setup_gpio_2]
	
	gpio_pin_set_config(WAKEUP_PIN, &config_gpio_pin_wakeup);
	/* Register callback function. */
	//! [callback_init]
	gpio_init();
	//! [callback_init]
	//! [callback_reg]
	gpio_register_callback(WAKEUP_PIN, wakeup_pin_callback, GPIO_CALLBACK_FALLING);
	//! [callback_reg]
	//! [callback_en]
	gpio_enable_callback(WAKEUP_PIN);
	//! [callback_en]
}

//! [callback_funcs]
static void aon_sleep_timer_callback(void)
{
	
}
//! [callback_funcs]
static void configure_aon_sleep_timer(void)
{
	//! [setup_6]
	struct aon_sleep_timer_config config_aon_sleep_timer;
	//! [setup_6]
	//! [setup_7]
	aon_sleep_timer_get_config_defaults(&config_aon_sleep_timer);
	//! [setup_7]
	//! [setup_8]
	config_aon_sleep_timer.mode = AON_SLEEP_TIMER_RELOAD_MODE;
	config_aon_sleep_timer.counter = AON_SLEEP_TIMEOUT;
	//! [setup_8]
	//! [setup_9]
	aon_sleep_timer_init(&config_aon_sleep_timer);
	//! [setup_9]

}

static void configure_aon_sleep_timer_callback(void)
{
	//! [setup_register_callback]
	aon_sleep_timer_register_callback(aon_sleep_timer_callback);
	//! [setup_register_callback]

	//! [enable_IRQ]
	NVIC_EnableIRQ(AON_SLEEP_TIMER0_IRQn);
	//! [enable_IRQ]
}

static void configure_timer(void)
{
	//! [setup_timer_1]
	struct timer_config config_timer;
	//! [setup_timer_1]
	//! [setup_timer_2]
	timer_get_config_defaults(&config_timer);
	//! [setup_timer_2]
	//! [setup_timer_3]
	config_timer.reload_value = MCU_SLEEP_TIMEOUT;
	//! [setup_timer_3]
	//! [setup_timer_4]
	timer_init(&config_timer);
	//! [setup_timer_4]
	//! [setup_timer_5]
	//! [setup_timer_5]
}

static void configure_timer_callback(void)
{
	//! [setup_register_callback]
	timer_register_callback(mcu_sleeptimer_callback);
	//! [setup_register_callback]
	
	//! [enable_IRQ]
	NVIC_EnableIRQ(TIMER0_IRQn);
	//! [enable_IRQ]
}
//! [setup]


static void setWatchdogTimeout(void)
{
	struct wdt_module dwt;
	dwt.hw = WDT0;
	struct wdt_config dwt_conf;
	wdt_get_config_defaults(&dwt_conf);
	dwt_conf.load_value = WATCHDOG_TIMEOUT;
	wdt_set_config(&dwt,WDT0,&dwt_conf);
	wdt_reset(&dwt);
}

void resetWatchdog()
{
	struct wdt_module dwt;
	dwt.hw = WDT0;
	wdt_reset(&dwt);
}

void simState_check_setup(void)
{
	configure_gpio_pin_input();
}

void initSimUart(void)
{
	configure_uart1(&sim_uart);
}

void disableUART_RX(void)
{
	configure_gpio_rx_pin_input();
}

void setup_mcu_wakeup(void)
{
	configure_gpio_wakeup();
}

void startMCU_SleepTimer(void)
{	
	timer_disable();
	timer_set_value(MCU_SLEEP_TIMEOUT);
	timer_enable();
}

void stopMCU_SleepTimer(void)
{
	timer_disable();
}

void setup_peripherals()
{
	initSimUart();
#if sim_uart_interrupt_mode
	uart_register_callback(&sim_uart, sim_uart_read_complete_callback,UART_RX_COMPLETE);
	uart_enable_callback(&sim_uart, UART_RX_COMPLETE);
#endif
#if debugPrint_enable
	configure_uart3(&debug_uart);
#endif
	configure_gpio_pins();
	simState_check_setup();
	configure_dualtimer();
	configure_dualtimer_callback();
	configure_timer();
	configure_timer_callback();
// 	configure_aon_sleep_timer();
// 	configure_aon_sleep_timer_callback();
// 	aon_sleep_timer_sleep_timer_active();
	setWatchdogTimeout();
}

//! [setup]
