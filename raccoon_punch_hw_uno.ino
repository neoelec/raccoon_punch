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
 * File Name : raccoon_punch_hw_uno.ino
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 03/Sep/2016
 * Version : Baby-Raccoon
 */

#include <avr/pgmspace.h>
#include <Wire.h>

#include "raccoon_punch_hw_uno.h"

static LiquidCrystal_I2C __raccoon_punch_hw_uno_lcd(
		RACCOON_PUNCH_HW_UNO_LCD_SLAVE,
		RACCOON_PUNCH_HW_UNO_LCD_COL,
		RACCOON_PUNCH_HW_UNO_LCD_ROW);

static char __raccoon_punch_hw_uno_lcd_buf[RACCOON_PUNCH_HW_UNO_LCD_NR_CHAR];
static char *__raccoon_punch_hw_uno_lcd_line_buf[RACCOON_PUNCH_HW_UNO_LCD_ROW];

raccoon_punch_hw_uno::raccoon_punch_hw_uno()
		: clk_pin(3), data_pin(4), key_pin(2),
		lcd(&__raccoon_punch_hw_uno_lcd),
		lcd_row_max(RACCOON_PUNCH_HW_UNO_LCD_ROW - 1),
		lcd_col_max(RACCOON_PUNCH_HW_UNO_LCD_COL - 1),
		lcd_line_buf(__raccoon_punch_hw_uno_lcd_line_buf)
{
}

static raccoon_punch_hw_uno __raccoon_punch_hw_uno;

raccoon_punch_hw_uno *raccoon_punch_hw_uno::singleton =
		&__raccoon_punch_hw_uno;

raccoon_punch_hw_uno *raccoon_punch_hw_uno::get_instance(void)
{
	return singleton;
}

void raccoon_punch_hw_uno::init(void)
{
	uint8_t i;

	pinMode(clk_pin, INPUT);
	pinMode(data_pin, INPUT);
	pinMode(key_pin, INPUT);

	lcd->begin();
	lcd->backlight();
	lcd_row = lcd_col = 0;

	for (i = 0; i <= lcd_row_max; i++)
		lcd_line_buf[i] = &__raccoon_punch_hw_uno_lcd_buf[(lcd_col_max + 1) * i];
}

uint8_t raccoon_punch_hw_uno::get_clk_pin_level(void)
{
	return digitalRead(clk_pin) ? HIGH : LOW;
}

uint8_t raccoon_punch_hw_uno::get_clk_pin_interrupt(void)
{
	return digitalPinToInterrupt(clk_pin);
}

uint8_t raccoon_punch_hw_uno::get_data_pin_level(void)
{
	return digitalRead(data_pin) ? HIGH : LOW;
}

uint8_t raccoon_punch_hw_uno::get_key_pin_interrupt(void)
{
	return digitalPinToInterrupt(key_pin);
}

void raccoon_punch_hw_uno::lcd_clear(void)
{
	uint8_t row, col;

	lcd->clear();
	lcd->home();

	for (row = 0; row <= lcd_row_max; row++)
		for (col = 0; col <= lcd_col_max; col++)
			lcd_line_buf[row][col] = ' ';

	lcd_row = 0;
	lcd_col = 0;
}

void raccoon_punch_hw_uno::lcd_clear_row(uint8_t row)
{
	uint8_t col;

	lcd->setCursor(0, row);
	for (col = 0; col <= lcd_col_max; col++) {
		lcd->print(' ');
		lcd_line_buf[row][col] = ' ';
	}
	lcd->setCursor(0, row);
}

void raccoon_punch_hw_uno::lcd_set_cursor(uint8_t col, uint8_t row)
{
	if (col > lcd_col_max || row > lcd_col_max)
		return;

	lcd->setCursor(col, row);
	lcd_row = row;
	lcd_col = col;
}

void raccoon_punch_hw_uno::lcd_scroll(void)
{
	uint8_t row, col;
	char *line_buf_tmp = lcd_line_buf[0];

	for (row = 0; row < lcd_row_max; row++) {
		lcd_line_buf[row] = lcd_line_buf[row + 1];
		lcd->setCursor(0, row);
		for (col = 0; col <= lcd_col_max; col++)
			lcd->print(lcd_line_buf[row][col]);
	}

	lcd->setCursor(0, lcd_row_max);
	for (col = 0; col <= lcd_col_max; col++) {
		lcd->print(' ');
		line_buf_tmp[col] = ' ';
	}

	lcd_line_buf[lcd_row_max] = line_buf_tmp;
	lcd_set_cursor(0, lcd_row_max);
}

void raccoon_punch_hw_uno::lcd_print(char ch)
{
	if ('\n' == ch || '\r' == ch) {
		lcd_row++;
		lcd_col = 0;

		if (lcd_row > lcd_row_max)
			lcd_scroll();
		lcd->setCursor(lcd_col, lcd_row);
		return;
	}

	if (lcd_col > lcd_col_max) {
		lcd_col = 0;
		if (++lcd_row > lcd_row_max)
			lcd_scroll();
		lcd->setCursor(lcd_col, lcd_row);
	}

	lcd->print(ch);
	lcd_line_buf[lcd_row][lcd_col] = ch;

	lcd_col++;
}

void raccoon_punch_hw_uno::lcd_print(const char *str)
{
	while (*str)
		lcd_print(*str++);
}

void raccoon_punch_hw_uno::lcd_print_P(const char *str)
{
	char ch;

	while (ch = pgm_read_byte(str++))
		lcd_print(ch);
}
