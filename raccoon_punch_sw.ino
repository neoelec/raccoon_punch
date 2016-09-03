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
 * File Name : raccoon_punch_sw_ino
 *
 * File Description :
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 03/Sep/2016
 * Version : Baby-Raccoon
 */

#include <avr/pgmspace.h>

#include "raccoon_punch.h"
#include "raccoon_punch_sw.h"

raccoon_punch_sw::raccoon_punch_sw()
{
}

raccoon_punch_sw *raccoon_punch_sw::get_instance(void)
{
	return singleton;
}

static raccoon_punch_sw __raccoon_punch_sw;

raccoon_punch_sw *raccoon_punch_sw::singleton = &__raccoon_punch_sw;

void raccoon_punch_sw::init(void)
{
	state = E_RACCOON_PUNCH_SW_STATE_IDLE;

	punch_hw->lcd_clear();
}

static const char __NUL[] PROGMEM = "NUL";
static const char __SOH[] PROGMEM = "SOH";
static const char __STX[] PROGMEM = "STX";
static const char __ETX[] PROGMEM = "ETX";
static const char __EOT[] PROGMEM = "EOT";
static const char __ENQ[] PROGMEM = "ENQ";
static const char __ACK[] PROGMEM = "ACK";
static const char __BEL[] PROGMEM = "BEL";
static const char __BS[] PROGMEM = "BS";
static const char __TAB[] PROGMEM = "TAB";
static const char __LF[] PROGMEM = "LF";
static const char __VT[] PROGMEM = "VT";
static const char __FF[] PROGMEM = "FF";
static const char __CR[] PROGMEM = "CR";
static const char __SO[] PROGMEM = "SO";
static const char __SI[] PROGMEM = "SI";
static const char __DLE[] PROGMEM = "DLE";
static const char __DC1[] PROGMEM = "DC1";
static const char __DC2[] PROGMEM = "DC2";
static const char __DC3[] PROGMEM = "DC3";
static const char __DC4[] PROGMEM = "DC4";
static const char __NAK[] PROGMEM = "NAK";
static const char __SYN[] PROGMEM = "SYN";
static const char __ETB[] PROGMEM = "ETB";
static const char __CAN[] PROGMEM = "CAN";
static const char __EM[] PROGMEM = "EM";
static const char __SUB[] PROGMEM = "SUB";
static const char __ESC[] PROGMEM = "ESC";
static const char __FS[] PROGMEM = "FS";
static const char __GS[] PROGMEM = "GS";
static const char __RS[] PROGMEM = "RS";
static const char __US[] PROGMEM = "US";

static const char * const __non_visible_char[] PROGMEM = {
	__NUL,
	__SOH,
	__STX,
	__ETX,
	__EOT,
	__ENQ,
	__ACK,
	__BEL,
	__BS,
	__TAB,
	__LF,
	__VT,
	__FF,
	__CR,
	__SO,
	__SI,
	__DLE,
	__DC1,
	__DC2,
	__DC3,
	__DC4,
	__NAK,
	__SYN,
	__ETB,
	__CAN,
	__EM,
	__SUB,
	__ESC,
	__FS,
	__GS,
	__RS,
	__US,
};

static const char __DEL[] PROGMEM = "DEL";
static const char __NA[] PROGMEM = "n/a";

void raccoon_punch_sw::loop(void)
{
	unsigned long timestamp = millis();
	const char *non_visible_char;
	uint8_t i;

	switch (state) {
	case E_RACCOON_PUNCH_SW_STATE_IDLE:
		return;
	case E_RACCOON_PUNCH_SW_STATE_BEGIN:
		timestamp_last = timestamp;
		data = 0x00;
		count = 0;
		state = E_RACCOON_PUNCH_SW_STATE_READ;
		break;
	case E_RACCOON_PUNCH_SW_STATE_1_BIT:
		timestamp_last = timestamp;
		data = (data << 1) | bit;
		punch_hw->lcd_print((char)(bit + '0'));
		if (8 == ++count)
			state = E_RACCOON_PUNCH_SW_STATE_DECODE;
		else
			state = E_RACCOON_PUNCH_SW_STATE_READ;
		break;
	case E_RACCOON_PUNCH_SW_STATE_READ:
		if (timestamp - timestamp_last > RACCOON_PUNCH_SW_TIMEOUT) {
			for (i = count; i < 8; i++)
				punch_hw->lcd_print('X');
			punch_hw->lcd_print(" / err\n");
			state = E_RACCOON_PUNCH_SW_STATE_IDLE;
			return;
		}
		break;
	case E_RACCOON_PUNCH_SW_STATE_DECODE:
		timestamp_last = timestamp;
		punch_hw->lcd_print(" = ");
		if (data < (uint8_t)' ') {
			non_visible_char = (const char *)pgm_read_word(&__non_visible_char[data]);
			punch_hw->lcd_print_P(non_visible_char);
		} else if (0x7F == data)	/* DEL */
			punch_hw->lcd_print_P(__DEL);
		else if (data > 0x7F)
			punch_hw->lcd_print_P(__NA);
		else
			punch_hw->lcd_print((char)data);
		punch_hw->lcd_print('\n');
		state = E_RACCOON_PUNCH_SW_STATE_END;
		break;
	case E_RACCOON_PUNCH_SW_STATE_END:
		if (timestamp - timestamp_last > RACCOON_PUNCH_SW_CARD_INTERVAL)
			state = E_RACCOON_PUNCH_SW_STATE_IDLE;
		break;
	case E_RACCOON_PUNCH_SW_STATE_CLEAR_LCD:
		punch_hw->lcd_clear();
		state = E_RACCOON_PUNCH_SW_STATE_IDLE;
		break;
	}
}

void raccoon_punch_sw::read_handler(void)
{
	uint8_t clk, data;

	clk = punch_hw->get_clk_pin_level();
	data = punch_hw->get_data_pin_level();

	switch (state) {
	case E_RACCOON_PUNCH_SW_STATE_END:
		break;
	case E_RACCOON_PUNCH_SW_STATE_IDLE:
		if (HIGH == clk && LOW == data)
			state = E_RACCOON_PUNCH_SW_STATE_BEGIN;
		break;
	default:
		if (LOW == clk) {
			bit = !!data;
			state = E_RACCOON_PUNCH_SW_STATE_1_BIT;
		}
	}
}

void raccoon_punch_sw::key_handler(void)
{
	if (state == E_RACCOON_PUNCH_SW_STATE_IDLE)
		state = E_RACCOON_PUNCH_SW_STATE_CLEAR_LCD;
}
