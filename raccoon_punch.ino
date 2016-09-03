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
 * File Name : raccoon_punch.ino
 *
 * File Description : This project inspired from 'Arduino Punched Card Reader'
 *  https://arduining.com/2012/06/10/arduino-punched-card-reader/
 *
 * Author : Joo, Young Jin <neoelec@gmail.com>
 * Dept : Raccoon's Cave
 * Created Date : 03/Sep/2016
 * Version : Baby-Raccoon
 */

#include <avr/pgmspace.h>

#include "raccoon_punch.h"
#include "raccoon_punch_hw.h"
#include "raccoon_punch_hw_uno.h"
#include "raccoon_punch_sw.h"

raccoon_punch_hw *punch_hw;
raccoon_punch_sw *punch_sw;

static void read_handler(void)
{
	punch_sw->read_handler();
}

static void key_handler(void)
{
	punch_sw->key_handler();
}

static const char __punch_banner[] PROGMEM =
		"Raccoon's Punch "
		"     Card Reader";

void setup(void)
{
	// put your setup code here, to run once:
	punch_hw = raccoon_punch_hw_uno::get_instance();
	punch_hw->init();
	punch_hw->lcd_print_P(__punch_banner);

	delay(1000);

	punch_sw = raccoon_punch_sw::get_instance();
	punch_sw->init();

	attachInterrupt(punch_hw->get_clk_pin_interrupt(), read_handler, CHANGE);
	attachInterrupt(punch_hw->get_key_pin_interrupt(), key_handler, FALLING);
}

void loop(void)
{
	// put your main code here, to run repeatedly:
	punch_sw->loop();
}
