/*
 * MMA8452.c
 *
 * Created: 12/8/2016 11:36:17 PM
 *  Author: Shiva and Rishabh
 */ 

#include "MMA8452.h"

/* Timeout counter. */
//! [timeout_counter]
uint16_t timeout = 0;
//! [timeout_counter]

uint8_t CTRL_REG1_Standby[2] = {
	0x2A, 0xF8
};

uint8_t CTRL_REG1_Active[2] = {
	0x2A, 0xF9
};

uint8_t FF_MT_CFG[2] = {
	0x15, 0x78
};

uint8_t FF_MT_THS[2] = {
	0x17, 0x10
};

uint8_t FF_MT_COUNT[2] = {
	0x18, 0x0A
};

uint8_t CTRL_REG4[2] = {
	0x2D, 0x04
};

uint8_t CTRL_REG5[2] = {
	0x2E, 0x04
};

uint8_t CTRL_REG2_Reset[2] = {
	0x2B, 0x40
};


/* Init software module. */
//! [dev_inst]
struct i2c_master_module i2c_master_instance;
//! [dev_inst]

void wakeup_pin_callback(void)
{
	//ble_advertise_on = true;
	mcu_wakeup = true;
	debugPrint("\r\n!!!!!!MCU WAKEUP!!!!!!!!");
}

void resetAccl(void)
{
	struct i2c_master_packet packet = {
		.address     = SLAVE_ADDRESS,
		.data_length = 2,
		.data        = CTRL_REG2_Reset,
	};
	//! [packet]
	//! [init]

	//! [main]
	/* Write buffer to slave until success. */
	//! [write_packet]
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) != STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
		}
	}
}

void setupAccl(void)
{
	struct i2c_master_packet packet = {
		.address     = SLAVE_ADDRESS,
		.data_length = 2,
		.data        = CTRL_REG1_Standby,
	};
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
		}
	}
	packet.data = FF_MT_CFG;
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
		}
	}
	packet.data = FF_MT_THS;
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
		}
	}
	packet.data = FF_MT_COUNT;
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
		}
	}
	packet.data = CTRL_REG4;
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
		}
	}
	packet.data = CTRL_REG5;
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
		}
	}
	packet.data = CTRL_REG1_Active;
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
		}
	}
}

void standbyAccl(void)
{
	struct i2c_master_packet packet = {
		.address     = SLAVE_ADDRESS,
		.data_length = 2,
		.data        = CTRL_REG1_Standby,
	};
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) !=
	STATUS_OK) {
		/* Increment timeout counter and check if timed out. */
		if (timeout++ == TIMEOUT) {
			break;
		}
	}
}

//! [initialize_i2c]
void configure_i2c_master(void)
{
	/* Initialize config structure and software module. */
	//! [init_conf]
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(&config_i2c_master);
	//! [init_conf]
	/* Initialize and enable device with config, and enable i2c. */
	//! [init_module]
	i2c_master_init(&i2c_master_instance, I2C0, &config_i2c_master);
	//! [init_module]
	
	//! [enable_module]
	i2c_enable(i2c_master_instance.hw);
	//! [enable_module]
}
//! [initialize_i2c]

void initializeAccl(void)
{
	setup_mcu_wakeup();
	configure_i2c_master();
	resetAccl();
	wait4_5seconds();
	setupAccl();
}
