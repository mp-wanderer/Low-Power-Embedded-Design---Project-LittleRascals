/**
 * \file
 *
 * \brief SAM UART Quick Start
 *
 * Copyright (C) 2015-2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <string.h>
#include "main.h"
#include "platform.h"
#include "console_serial.h"
#include "ble_manager.h"

struct uart_module sim_uart;
struct uart_module debug_uart;

bool pwrTestFlag = false;
volatile bool ble_advertise_on = false;
volatile bool connected = false; 
volatile bool mcu_wakeup = false;
bool sim_ble_toggle = true;   /// false : SIM, true : BLE
bool useSIM = false;

//! [setup]

void debugPrint(const char str[])
{
#if debugPrint_enable

	char crlf[3] = "\r\n";
	uart_write_buffer_wait(&debug_uart, (uint8_t *)str, strlen(str));
	uart_write_buffer_wait(&debug_uart, (uint8_t *)crlf, strlen(crlf));	
#endif
}

// [callback_funcs]
void mcu_sleeptimer_callback(void)
{
	debugPrint("MCU Sleep Timer Callback");
	sim_ble_toggle = !sim_ble_toggle;
	if(sim_ble_toggle)
	{
		ble_advertise_on = true;
	}
	else
	{
		useSIM = true;	
	}
}
//! [callback_funcs]

void errorFunc(void)
{
	while(1);
}

int main(void)
{
	/**
	 * For make this QS work, disable the systick to stop task switch.
	 * Should not do it if you want the BLE functions.
	 */
	platform_driver_init();
	acquire_sleep_lock();
	serial_console_init();
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);

	setup_peripherals();
	initializeBLE();
//	[setup_init]
//! [setup_init]

	initializeAccl();
	simPwr(false);
	
	ble_advertise();
	while(!connected)
	{
		if(ble_event_task(1000) == AT_BLE_TIMEOUT)		// 1000 : 10 sec
		{
			debugPrint("\r\n1) BLE Event Timed Out");
		}
		debugPrint("\r\nwfi");
		asm volatile("wfi");
		asm volatile("nop");
	}
	ble_advertise_on = true;
// 	if(ble_event_task(1000) == AT_BLE_TIMEOUT)		// 1000 : 10 sec
// 	{
// 		debugPrint("\r\n2) BLE Event Timed Out");
// 	}
	int count_before_sleep = 0;
	bool already_advertising = false;
	while (1)
	{
		debugPrint("\r\nIn Main while");
		resetWatchdog();
		if(!connected)
		{
			if(ble_advertise_on)
			{
				ble_set_ulp_mode(BLE_ULP_MODE_CLEAR);
				debugPrint("\r\nCheck if in range");
				simPwr(false);
				//standbyAccl();
				ble_advertise();
				at_ble_err_status_t errStatus = ble_event_task(BLE_ADVERTISE_TIMEOUT);
				uint8_t eror = (uint8_t)errStatus;
				debugPrint("ble event wait err status : ");
				debugPrint(&eror);
				if(errStatus == AT_BLE_TIMEOUT)
				{
					debugPrint("\r\n4) BLE Event Timed Out");
					ble_set_ulp_mode(BLE_ULP_MODE_SET);
					ble_event_task(200);
					debugPrint("\r\nAdvertising Stopped");
					at_ble_adv_stop();
					ble_advertise_on = false;
					//count_before_sleep = 0;
					debugPrint("\r\nStarted MCU Sleep Timer");
					startMCU_SleepTimer();
					useSIM = true;
				}
				count_before_sleep = 0;
				debugPrint("\r\nafter event task");
			}
			else if(!ble_advertise_on && useSIM)
			{
				timer_disable();
				simPwr(false);
				simPwr(true);
				debugPrint("SIM ON!!!!!");
				initializeSIM();
				initializeSIMGPS();
				initializeHTTP();
				getLoc_PostData();
				setupAccl();
				simPwr(false);
				startMCU_SleepTimer();
				count_before_sleep = 0;
				useSIM = false;
			}
		}
		else
		{
			debugPrint("\r\nconnected");
			if(ble_event_task(1000) == AT_BLE_TIMEOUT)		// 1000 : 10 sec
			{
				debugPrint("\r\n3) BLE Event Timed Out");
			}
			already_advertising = false;
		}
		if(mcu_wakeup)
		{
			startMCU_SleepTimer();	
		}
		if(count_before_sleep > 3)
		{
			//ble_event_task(200);
			debugPrint("\r\nCalling wfi");
 			asm volatile ("wfi");
 			asm volatile ("nop");	
			count_before_sleep = 0;
		}
		count_before_sleep++;
	}
}
