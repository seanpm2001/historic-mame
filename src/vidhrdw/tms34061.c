/****************************************************************************
 *																			*
 *	Functions to emulate the TMS34061 video controller						*
 *																			*
 *  Created by Zsolt Vasvari on 5/26/1998.									*
 *																			*
 *  This is far from complete. See the TMS34061 User's Guide available on	*
 *  www.spies.com/arcade													*
 *																			*
 ****************************************************************************/

#include "driver.h"
#include "vidhrdw/tms34061.h"

static int reg_address_mode;
static tms34061_getpixel_t get_pixel;
static tms34061_setpixel_t set_pixel;

#define REG_HORENDSYNC   0
#define REG_HORENDBLNK   1
#define REG_HORSTARTBLNK 2
#define REG_HORTOTAL     3
#define REG_VERENDSYNC   4
#define REG_VERENDBLNK   5
#define REG_VERSTARTBLNK 6
#define REG_VERTOTAL     7
#define REG_DISPUPDATE   8
#define REG_DISPSTART    9
#define REG_VERINT       10
#define REG_CONTROL1     11
#define REG_CONTROL2     12
#define REG_STATUS       13
#define REG_XYOFFSET     14
#define REG_XYADDRESS    15
#define REG_DISPADDRESS  16
#define REG_VERCOUNTER   17

static int regs[REG_VERCOUNTER+1];

static void tms34061_intcallback(int param);

static void* timer;

int tms34061_start(int reg_addr_mode,
				   int cpu,
                   tms34061_getpixel_t getpixel,
                   tms34061_setpixel_t setpixel)
{
	if (!Machine->drv->cpu[cpu].timed_interrupt)
	{
		if (errorlog)
			fprintf(errorlog, "TMS34061 Error - Timed interrupt handler must be defined for CPU #%d\n", cpu);

		return 1;
	}

	reg_address_mode = reg_addr_mode;
	get_pixel = getpixel;
	set_pixel = setpixel;

	// Initialize registers to their default values from the manual
	regs[REG_HORENDSYNC]   = 0x10;
	regs[REG_HORENDBLNK]   = 0x20;
	regs[REG_HORSTARTBLNK] = 0x1f0;
	regs[REG_HORTOTAL]     = 0x200;
	regs[REG_VERENDSYNC]   = 0x04;
	regs[REG_VERENDBLNK]   = 0x10;
	regs[REG_VERSTARTBLNK] = 0xf0;
	regs[REG_VERTOTAL]     = 0x100;
	regs[REG_DISPUPDATE]   = 0x00;
	regs[REG_DISPSTART]    = 0x00;
	regs[REG_VERINT]       = 0x00;
	regs[REG_CONTROL1]     = 0x7000;
	regs[REG_CONTROL2]     = 0x600;
	regs[REG_STATUS]       = 0x00;
	regs[REG_XYOFFSET]     = 0x10;
	regs[REG_XYADDRESS]    = 0x00;
	regs[REG_DISPADDRESS]  = 0x00;
	regs[REG_VERCOUNTER]   = 0x00;

	// Start vertical interrupt timer.
	timer = timer_pulse(TIME_IN_HZ (Machine->drv->frames_per_second),
	                    cpu, tms34061_intcallback);
	return 0;
}


void tms34061_stop(void)
{
	timer_remove(timer);
}


void tms34061_w(int offset, int data)
{
	int status = 0;		// Unsupported

	// Calculate which register and which half we're accessing
	int reg = offset >> 2;
	int hi  = offset & reg_address_mode;

	// Set register
	if (hi)
	{
		// Hi word
		regs[reg] = ((regs[reg] & 0xff) | (data << 8));
	}
	else
	{
		// Lo word
		regs[reg] = ((regs[reg] & 0xff00) | data);
	}

	switch (reg)
	{
	case REG_VERINT:
		// Set vertical interrupt timer
		timer_reset(timer, cpu_getscanlinetime(regs[reg]));

		// Fall through

	case REG_CONTROL1:
	case REG_CONTROL2:
	case REG_XYADDRESS:
		status = 1;		// Ok
		break;
	}

	if (!status && errorlog)
	{
		fprintf(errorlog, "Unsupported TMS34061 write. Reg #%02X=%04X - PC: %04X\n",
				reg, regs[reg], cpu_getpc());
	}
}

static void tms34061_intcallback(int param)
{
	// Reset timer for next frame
	timer_reset(timer, cpu_getscanlinetime(regs[REG_VERINT]));

	// Get out if vertical interrupts are disabled
    if (!(regs[REG_CONTROL1] & 0x400)) return;

	regs[REG_STATUS] |= 0x0001;

	cpu_cause_interrupt (param, Machine->drv->cpu[param].timed_interrupt());
}


int tms34061_r(int offset)
{
	int ret;

	int status = 0;		// Unsupported

	// Calculate which register and which half we're accessing
	int reg = offset >> 2;
	int hi  = offset & reg_address_mode;

	// Get register
	if (hi)
	{
		// Hi word
		ret = (regs[reg] >> 8);
	}
	else
	{
		// Lo word
		ret = (regs[reg] & 0xff);
	}

	switch (reg)
	{
	case REG_STATUS:
		// Need to clear status register now
		regs[reg] = 0;

		// Fall through

	case REG_CONTROL1:
	case REG_CONTROL2:
	case REG_XYADDRESS:
		status = 1;		// Ok
		break;
	}

	if (!status && errorlog)
	{
		fprintf(errorlog, "Unsupported TMS34061 read. Reg #%02X - PC: %04X\n",
				reg, cpu_getpc());
	}

	return ret;
}


static void adjust_xyaddress(int offset, int x, int y)
{
	// This an implementation of table on Page 4-15 of the User's Guide
	switch (offset & 0x06)
	{
	case 0x00:      break;
	case 0x02: x++; break;
	case 0x04: x--; break;
	case 0x06: x=0; break;
	}

	switch (offset & 0x18)
	{
	case 0x00:      break;
	case 0x08: y++; break;
	case 0x10: y--; break;
	case 0x18: y=0; break;
	}

	// Currently only implements when the X-Y addresses are 8 bits each
	// Case #7 on Page 4-18 in User's Guide

	regs[REG_XYADDRESS] = ((y & 0xff) << 8) | (x & 0xff);
}

void tms34061_xypixel_w(int offset, int data)
{
	// Currently only implements when the X-Y addresses are 8 bits each
	// Case #7 on Page 4-18 in User's Guide

	int x = regs[REG_XYADDRESS] & 0xff;
	int y = regs[REG_XYADDRESS] >> 8;

    set_pixel(x, y, data);

	if (offset) adjust_xyaddress(offset, x, y);
}


int tms34061_xypixel_r(int offset)
{
	// Currently only implements when the X-Y addresses are 8 bits each
	// Case #7 on Page 4-18 in User's Guide

	int x = regs[REG_XYADDRESS] & 0xff;
	int y = regs[REG_XYADDRESS] >> 8;

    int ret = get_pixel(x, y);

	if (offset) adjust_xyaddress(offset, x, y);

	return ret;
}


int tms34061_display_blanked(void)
{
	return (!(regs[REG_CONTROL2] & 0x2000));
}
