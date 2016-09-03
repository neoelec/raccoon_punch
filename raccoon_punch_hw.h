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
 * File Name : raccoon_punch_hw.h
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 03/Sep/2016
 * Version : Baby-Raccoon
 */

#ifndef __RACCOON_PUNCH_HW_H__
#define __RACCOON_PUNCH_HW_H__

#include <stdint.h>

class raccoon_punch_hw {
public:
	virtual void init(void) = 0;

	virtual uint8_t get_clk_pin_level(void) = 0;
	virtual uint8_t get_clk_pin_interrupt(void) = 0;

	virtual uint8_t get_data_pin_level(void) = 0;

	virtual uint8_t get_key_pin_interrupt(void) = 0;

	virtual void lcd_clear(void) = 0;
	virtual void lcd_clear_row(uint8_t row) = 0;
	virtual void lcd_set_cursor(uint8_t col, uint8_t row) = 0;
	virtual void lcd_print(char ch) = 0;
	virtual void lcd_print(const char *str) = 0;
	virtual void lcd_print_P(const char *str) = 0;
};

#endif /* __RACCOON_PUNCH_HW_H__ */
