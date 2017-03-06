/*
 * main.h
 *
 * Created: 12/7/2016 6:48:32 PM
 *  Author: Shiva and Rishabh
 */ 


#ifndef MAIN_H_
#define MAIN_H_

extern struct uart_module sim_uart;
extern struct uart_module debug_uart;
extern volatile bool recvTimeoutFlag;
extern volatile bool ble_advertise_on;
extern volatile bool connected;
extern volatile bool mcu_wakeup; 


typedef enum {
	FAILURE = 0,
	SUCCESS = 1,
	RETRY = 2
}returnStatus;

// typedef enum {
// 	FirstConnect = 0,
// 	Disconnected = 1,
// 	Connected = 2
// }connectionStatus;

#define CLOCK_FREQ								26000000
#define One_Sec_Count							CLOCK_FREQ
#define WATCHDOG_TIMEOUT						One_Sec_Count*150
#define RECV_TIMEOUT							One_Sec_Count*2
#define PWRKEY_TIME								One_Sec_Count*1.5
#define AON_SLEEP_TIMEOUT						32000*6
#define MCU_SLEEP_TIMEOUT						One_Sec_Count*60
#define BLE_ADVERTISE_TIMEOUT					1000		// 10 seconds
//#define MCU_AWAKE_TIMEOUT						2
#define PWRKEY_PIN								PIN_LP_GPIO_20
//#define SIMSTATECHECK_PIN						PIN_LP_GPIO_2
#define WAKEUP_PIN								PIN_AO_GPIO_0
#define SIM_PWRSTATE_PIN						PIN_LP_GPIO_19
#define RECVTIMEOUT_TIMER						DUALTIMER_TIMER1
#define PWRKEY_TIMER							DUALTIMER_TIMER2
#define	sim_uart_interrupt_mode					false
#define debugPrint_enable						true

// Function definitions
void debugPrint(const char str[]);
void setup_peripherals(void);
void pwrkey_callback(void);
void recvTimeout_callback(void);
void sim_uart_read_complete_callback(struct uart_module *const module);
void wakeup_pin_callback(void);
void mcu_sleeptimer_callback(void);

void resetAccl(void);
void setupAccl(void);

void initializeAccl(void);
void initializeSIM(void);
void simPwr(bool pwr);
void simState_check_setup(void);
void initSimUart(void);
void disableUART_RX(void);
void initializeSIMGPS(void);
void getLoc_PostData(void);
void initializeHTTP(void);
void wait4_5seconds(void);
void setup_mcu_wakeup(void);
void startMCU_SleepTimer(void);
void stopMCU_SleepTimer(void);
void standbyAccl(void);

void ble_advertise(void);
void initializeBLE(void);

void resetWatchdog(void);
void errorFunc(void);


#endif /* MAIN_H_ */
