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
 * File Name : raccoon_punch_hw_uno.h
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 03/Sep/2016
 * Version : Baby-Raccoon
 */

#ifndef __RACCOON_PUNCH_HW_UNO_H__
#define __RACCOON_PUNCH_HW_UNO_H__

#include <stdint.h>
#include <LiquidCrystal_I2C.h>

#include "raccoon_punch.h"

#define RACCOON_PUNCH_HW_UNO_LCD_SLAVE		0x27
#define RACCOON_PUNCH_HW_UNO_LCD_COL		16
#define RACCOON_PUNCH_HW_UNO_LCD_ROW		2
#define RACCOON_PUNCH_HW_UNO_LCD_NR_CHAR	\
	((RACCOON_PUNCH_HW_UNO_LCD_COL) * (RACCOON_PUNCH_HW_UNO_LCD_ROW))

class raccoon_punch_hw_uno : public raccoon_punch_hw {
public:
	raccoon_punch_hw_uno();
	static raccoon_punch_hw_uno *get_instance(void);

	virtual void init(void);

	virtual uint8_t get_clk_pin_level(void);
	virtual uint8_t get_clk_pin_interrupt(void);

	virtual uint8_t get_data_pin_level(void);

	virtual uint8_t get_key_pin_interrupt(void);

	virtual void lcd_clear(void);
	virtual void lcd_clear_row(uint8_t row);
	virtual void lcd_set_cursor(uint8_t col, uint8_t row);
	virtual void lcd_print(char ch);
	virtual void lcd_print(const char *str);
	virtual void lcd_print_P(const char *str);

private:
	void lcd_scroll(void);

	static raccoon_punch_hw_uno *singleton;

	uint8_t clk_pin;
	uint8_t data_pin;
	uint8_t key_pin;

	LiquidCrystal_I2C *lcd;
	uint8_t lcd_row_max, lcd_col_max;
	uint8_t lcd_row, lcd_col;
	char **lcd_line_buf;
};

#endif /* __RACCOON_PUNCH_HW_UNO_H__ */
