/*
 * MMA8452.h
 *
 * Created: 12/8/2016 11:37:36 PM
 *  Author: Shiva and Rishabh
 */ 


#ifndef MMA8452_H_
#define MMA8452_H_

#include <asf.h>
#include "main.h"

//! [address]
#define SLAVE_ADDRESS 0x1D
//! [address]

/* Number of times to try to send packet if failed. */
//! [timeout]
#define TIMEOUT 1000
//! [timeout]

void configure_i2c_master(void);



#endif /* MMA8452_H_ */
