/**
 * Copyright (C) 2016. Joo, Young Jin <neoelec@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/**
 * Project Name : Raccoon's Punch Card Reader
 *
 * Project Description :
 *
 * Comments : tabstop = 8, shiftwidth = 8, noexpandtab
 */

/**
 * File Name : raccoon_punch_sw.h
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 03/Sep/2016
 * Version : Baby-Raccoon
 */

#ifndef __RACCOON_PUNCH_SW_H__
#define __RACCOON_PUNCH_SW_H__

#include <stdint.h>

#define RACCOON_PUNCH_SW_TIMEOUT	3000
#define RACCOON_PUNCH_SW_CARD_INTERVAL	1000

typedef enum {
	E_RACCOON_PUNCH_SW_STATE_IDLE = 0,
	E_RACCOON_PUNCH_SW_STATE_BEGIN,
	E_RACCOON_PUNCH_SW_STATE_1_BIT,
	E_RACCOON_PUNCH_SW_STATE_READ,
	E_RACCOON_PUNCH_SW_STATE_DECODE,
	E_RACCOON_PUNCH_SW_STATE_END,
	E_RACCOON_PUNCH_SW_STATE_CLEAR_LCD,
} raccoon_punch_state_t;

class raccoon_punch_sw {
public:
	raccoon_punch_sw();
	static raccoon_punch_sw *get_instance();

	void init(void);
	void loop(void);
	void read_handler(void);
	void key_handler(void);

private:
	static raccoon_punch_sw *singleton;

	raccoon_punch_state_t state;
	unsigned long timestamp_last;
	uint8_t data;
	uint8_t bit;
	uint8_t count;
};

#endif /* __RACCOON_PUNCH_SW_H__ */
