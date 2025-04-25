//
//
//	xr88c92.cpp
//
//	(C) E.Lind 2025
//

#include "xr88c92.h"
#include "bits.h"
#include <stdio.h>
// #include <corecrt_wstdio.h>

xr88c92::xr88c92(xr88c92_impl& impl, uint16_t interval)
	: impl(impl),
	  interval(interval),
	  IRQ(isr, 1, true)	// ~IRQ = SR bit 7
{
	reset();
}

xr88c92::~xr88c92()
{
}

void xr88c92::reset()
{
    mra0 = mrb0 = 0x00;
    mra1 = mrb1 = 0;
    mra2 = mrb2 = 0;
    sra = srb = 0x0C;
    csra = csrb = 0;
    cra = crb = 0;

	cycles = 0;

    mra_reg_active = 0;
    mrb_reg_active = 0;
}

void xr88c92::tick(uint8_t ticks)
{
	cycles += ticks;
	if (cycles < interval) return;
	cycles = 0;

	// Check for a received character if one isn't available
	
}

Byte xr88c92::read(Word offset)
{
    fprintf(stdout, "xr88c92: read: %02X\n", offset);
	switch (offset & 0x0f) {
        case 0x00:	// MRA0
            switch (mra_reg_active) {
                case 0x00:	// MRA0
                    mra_reg_active = 0x01;
                    return mra0;
                    break;
                case 0x01:	// MRA1
                    mra_reg_active = 0x02;
                    return mra1;
                    break;
                case 0x02:	// MRA2
                    mra_reg_active = 0x00;
                    return mra2;
                    break;
                default:
                    return 0;
            }
            break;
        case 0x01:	// SRA
            return sra;
            break;
        case 0x02:	// RXA
            if (rxa_queue.empty()) {
                return 0;
            }else{
                Byte rxa = rxa_queue.front();
                rxa_queue.pop();
                return rxa; 
            }
            break;

        default:
            fprintf(stderr, "xr88c92: read: unknown register %02X\n", offset);
            return 0;
    }
}


void xr88c92::write(Word offset, Byte val)
{
	// switch (offset & 1) {
	// 	case 0:	// control register
	// 		cr = val;
	// 		if ((cr & 0x03) == 0x03) {
	// 			reset();
	// 		}
	// 		break;
	// 	case 1:	// data register
	// 		td = val;
	// 		sr &= ~(IRQB | TDRE);
	// 		break;
	// }
}
