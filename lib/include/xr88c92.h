//
//
//	xr88c92.h
//
//	(C) E.Lind 2025
//

#pragma once

#include "device.h"
#include "wiring.h"
#include <stdint.h>
#include <queue>

class xr88c92_impl {

public:
	virtual bool		poll_read() = 0;
	virtual bool		poll_write() { return true; };

public:
	virtual Byte		read() = 0;
	virtual void		write(Byte) = 0;

public:
                        xr88c92_impl() = default;
	virtual				~xr88c92_impl() = default;
};

class xr88c92 : virtual public ActiveMappedDevice {

protected:

// Internal registers
protected:

	Byte				mra0, mra1, mra2, sra, csra, cra, rxa, txa, ipcr, acr, isr, imr, cur, ctpu, clr, ctpl, mrb0, mrb1, mrb2, srb, csrb, crb, rxb, txb, gpr, ipr, opcr, stcr, sopr, spcr, ropr;

// Access to real IO device
    xr88c92_impl&		impl;
	uint16_t			interval;	// how often to poll
	uint16_t			cycles;		// cycles since last poll

// Initialisation functions

protected:
	virtual void		tick(uint8_t);
	virtual void		reset();

// Read and write functions
public:

	virtual Byte		read(Word offset);
	virtual void		write(Word offset, Byte val);

// Other exposed interfaces
public:
	OutputPinReg		IRQ;

private:
// Internal state
    uint8_t         mra_reg_active;
    uint8_t         mrb_reg_active;
    
    std::queue<Byte>        txa_queue;
    std::queue<Byte>        txb_queue;
    std::queue<Byte>        rxa_queue;
    std::queue<Byte>        rxb_queue;

// Public constructor and destructor
public:
                        xr88c92(xr88c92_impl& impl, uint16_t interval = 1000);
	virtual				~xr88c92();

};
