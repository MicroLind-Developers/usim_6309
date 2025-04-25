//
//	mc6809.h
//
//	Class definition for Motorola MC6809 microprocessor
//
//	(C) R.P.Bellis 1993
//

#pragma once

#include <string>
#include "wiring.h"
#include "usim.h"
#include "bits.h"


class mc6309 : virtual public USimBE {

protected: // Processor addressing modes

	enum {
				immediate,
				direct,
				indexed,
				extended,
				inherent,
				relative
	} mode;

protected:	// Processor registers

	Word			        u, s;	// Stack pointers
	Word			        x, y;	// Index registers
	Byte			        dp;		// Direct Page register
    Word			        v;		// Transfer register

    union {
        Word                zero;
        struct {
            Byte            z0;	    // Zero
            Byte            z1;	    // Zero
        } bytes;
    } zero;

#ifdef BYTE_ORDER_MSB_FIRST
	union {
		DWord			    q;	    // Combined accumulator
        struct {
             union {
                Word        d;	    // Accumulator d
                struct {
                    Byte    a;      // Accumulator a
                    Byte    b;      // Accumulator b
                } h_word;	
            } upper;
            union {
                Word        w;	    // Accumulator w
                struct {
                    Byte    e;      // Accumulator e
                    Byte    f;      // Accumulator f
                } l_word;	
            } lower; 
        } acc_dword;
	} acc;
#else
    union {
        DWord			    q;	    // Combined accumulator
        struct {
            union {
                Word        w;	    // Accumulator w
                struct {
                    Byte    f;      // Accumulator f
                    Byte    e;      // Accumulator e
                } l_word;	
            } lower;
            union {
                Word        d;	    // Accumulator d
                struct {
                    Byte    b;      // Accumulator b
                    Byte    a;      // Accumulator a
                } h_word;	
            } upper;
        } acc_dword;
    } acc;
#endif
	Byte&			a;
	Byte&			b;
    Byte&			e;
    Byte&			f;
	Word&			d;
    Word&			w;
    DWord&		    q;
	union {
		Byte			all;	// Condition code register
		struct {
#ifdef BITFIELDS_LSB_FIRST
			Byte		c : 1;	// Carry
			Byte		v : 1;	// Overflow
			Byte		z : 1;	// Zero
			Byte		n : 1;	// Negative
			Byte		i : 1;	// IRQ disable
			Byte		h : 1;	// Half carry
			Byte		f : 1;	// FIRQ disable
			Byte		e : 1;	// Entire
#else
			Byte		e : 1;	// Entire
			Byte		f : 1;	// FIRQ disable
			Byte		h : 1;	// Half carry
			Byte		i : 1;	// IRQ disable
			Byte		n : 1;	// Negative
			Byte		z : 1;	// Zero
			Byte		v : 1;	// Overflow
			Byte		c : 1;	// Carry
#endif
		} bit;
	} cc;

    union {
		Byte			all;	// Mode register
		struct {
#ifdef BITFIELDS_LSB_FIRST
            Byte		nm : 1;	// Native mode, uses 6309 instructions and stacks W on interrupt
			Byte		fm : 1;	// FIRQ uses IRQ stacking
			Byte		u3 : 1;	// Unused
			Byte		u2 : 1;	// Unused
			Byte		u1 : 1;	// Unused
			Byte		u0 : 1;	// Unused
			Byte		il : 1;	// Illegal instruction Exception
			Byte		dz : 1;	// Divide by zero Exception
#else
			Byte		dz : 1;	// Divide by zero Exception
			Byte		il : 1;	// Illegal instruction Exception
			Byte		u0 : 1;	// Unused
			Byte		u1 : 1;	// Unused
			Byte		u2 : 1;	// Unused
			Byte		u3 : 1;	// Unused
			Byte		fm : 1;	// FIRQ uses IRQ stacking
			Byte		nm : 1;	// Native mode, uses 6309 instructions and stacks W on interrupt
#endif
		} bit;
	} md;

private:	// internal processor state
	bool			waiting_sync;
	bool			waiting_cwai;
	bool			nmi_previous;

private:	// instruction and operand fetch and decode
	Word&			ix_refreg(Byte);

	void			fetch_instruction();
	Byte			fetch_operand();
	Word			fetch_word_operand();
    DWord			fetch_dword_operand();
	Word			fetch_effective_address();
	Word			fetch_indexed_operand();
	void			execute_instruction();

	void			do_predecrement();
	void			do_postincrement();

private:	// instruction implementations
	void			abx();
	void			adca(), adcb();
    void            adcd(), adcr();
	void			adda(), addb(), adde(), addf();
    void            addd(), addw(), addr();      
    void            aim();
	void			anda(), andb(), andcc();
    void            andd(), andr();
    void            asla(), aslb(), asl();
    void            asld();
	void			asra(), asrb(), asr();
    void            asrd();
    void            band();
	void			bcc(), lbcc();
	void			bcs(), lbcs();
    void            beor();
	void			beq(), lbeq();
	void			bge(), lbge();
	void			bgt(), lbgt();
	void			bhi(), lbhi();
    void			bhs(), lbhs();
    void            biand();
    void            bieor();
    void			bior();
	void			bita(), bitb();
    void            bitd();
    void            bitmd();
	void			ble(), lble();
    void            blo(), lblo();
	void			bls(), lbls();
	void			blt(), lblt();
	void			bmi(), lbmi();
	void			bne(), lbne();
    void            bor();
	void			bpl(), lbpl();
	void			bra(), lbra();
	void			brn(), lbrn();
	void			bsr(), lbsr();
	void			bvc(), lbvc();
	void			bvs(), lbvs();
	void			clra(), clrb(), clre(), clrf(), clr();
    void            clrd(), clrw();
	void			cmpa(), cmpb(), cmpe(), cmpf();
	void			cmpd(), cmps(), cmpu(), cmpw(), cmpx(), cmpy();
    void            cmpr();
	void			coma(), comb(), come(), comf();
    void            comd(), comw();
    void            com();
	void			cwai();
	void			daa();
	void			deca(), decb(), dece(), decf();
    void			decd(), decw();
    void			dec();
    void            divd();
    void            divq();
    void            eim();
	void			eora(), eorb();
    void            eord(), eorr();
	void			exg();
	void			inca(), incb(), ince(), incf();
    void			incd(), incw(), inc();
	void			jmp();
	void			jsr();
	void			lda(), ldb(), lde(), ldf();
	void			ldd(), lds(), ldu(), ldw(), ldx(), ldy();
    void            ldbt(), ldmd(), ldq();
	void			leas(), leau(), leax(), leay(); 
	void			lsla(), lslb(), lsl();
    void            lsld();
	void			lsra(), lsrb(), lsr();
    void            lsrd(), lsrw();
	void			mul(), muld();
	void			nega(), negb(), negd(), neg();
	void			nop();
    void            oim();
	void			ora(), orb(), orcc();
    void            ord(), orr();
	void			pshs(), pshu();
    void            pshsw(), pshuw();
	void			puls(), pulu();
    void            pulsw(), puluw();
	void			rola(), rolb(), rol();
    void			rold(), rolw();
	void			rora(), rorb(), ror();
    void			rord(), rorw();
	void			rti(), rts();
	void			sbca(), sbcb();
    void            sbcd(), sbcr();
	void			sex(), sexw();
	void			sta(), stb(), ste(), stf();
	void			std(), sts(), stu(), stw(), stx(), sty();
    void            stbt();
    void            stq();
	void			suba(), subb(), sube(), subf();
	void			subd(), subw();
    void			subr();
	void			swi(), swi2(), swi3();
	void			sync();
    void            tfm();
	void			tfr();
    void            tim();
	void			tsta(), tstb(), tste(), tstf();
    void            tstd(), tstw(), tst();

protected:	// helper functions
	void			help_adc(Byte&);
    void            help_adc(Word&);
    void            help_adcr();
	void			help_add(Byte&);
    void            help_addr();
    void            help_add(Word&);
	void			help_and(Byte&);
    void			help_and(Word&);
    void            help_andr();
	void			help_asr(Byte&);
    void			help_asr(Word&);
	void			help_bit(Byte);
	void			help_clr(Byte&);
	void			help_cmp(Byte);
	void			help_cmp(Word);
	void			help_com(Byte&);
	void			help_dec(Byte&);
	void			help_eor(Byte&);
	void			help_inc(Byte&);
	void			help_ld(Byte&);
	void			help_ld(Word&);
    void            help_ld(DWord&);
	void			help_lsr(Byte&);
	void			help_lsl(Byte&);
    void			help_lsl(Word&);
	void			help_neg(Byte&);
	void			help_or(Byte&);
	void			help_psh(Byte, Word&, Word&);
	void			help_pul(Byte, Word&, Word&);
	void			help_ror(Byte&);
	void			help_rol(Byte&);
	void			help_sbc(Byte&);
	void			help_st(Byte);
	void			help_st(Word);
	void			help_sub(Byte&);
	void			help_sub(Word&);
	void			help_tst(Byte);

protected:	// overloadable functions (e.g. for breakpoints)
	virtual void		do_br(const char *, bool);
	virtual void		do_lbr(const char *, bool);

	virtual void		do_psh(Word& sp, Byte);
	virtual void		do_psh(Word& sp, Word);
	virtual void		do_pul(Word& sp, Byte&);
	virtual void		do_pul(Word& sp, Word&);

	virtual void		do_nmi();
	virtual void		do_firq();
	virtual void		do_irq();

	virtual void		pre_exec();
	virtual void		post_exec();

protected: 	// instruction tracing
	Word			insn_pc;
	const char*		insn;
	Byte			post;
	Word			operand;

	std::string		disasm_operand();
	std::string		disasm_indexed();

public:		// external signal pins
	InputPin		IRQ, FIRQ, NMI;

public:
// Registers
struct Registers {
    Word u, s; // Stack pointers
    Word x, y; // Index registers
    Byte dp;   // Direct Page register
    Word v;    // Transfer register

    union {
        Word zero;
        struct {
            Byte z0; // Zero
            Byte z1; // Zero
        } bytes;
    } zero;
    union {
        DWord q; // Combined accumulator
        struct {
            union {
                Word d; // Accumulator d
                struct {
                    Byte a; // Accumulator a
                    Byte b; // Accumulator b
                } h_word;
            } upper;
            union {
                Word w; // Accumulator w
                struct {
                    Byte e; // Accumulator e
                    Byte f; // Accumulator f
                } l_word;
            } lower;
        } acc_dword;
    } acc;
    Byte cc;
    Byte md;
    Byte pc;
};
					mc6309();		// public constructor
	virtual			~mc6309();		// public destructor

	virtual void	reset();		// CPU reset
	virtual void	tick();

	virtual void	print_regs();
    virtual struct Registers get_registers();

	Byte&			byterefreg(int);
	Word&			wordrefreg(int);


};

inline void mc6309::do_br(const char *mnemonic, bool test)
{
	(void)mnemonic;
	Word offset = extend8(fetch_operand());
	if (test) pc += offset;
	++cycles;
}

inline void mc6309::do_lbr(const char *mnemonic, bool test)
{
	(void)mnemonic;
	Word offset = fetch_word_operand();
	if (test) {
		pc += offset;
		++cycles;
	}
	++cycles;
}

inline void mc6309::do_psh(Word& sp, Byte val)
{
	write(--sp, val);
}

inline void mc6309::do_psh(Word& sp, Word val)
{
	write(--sp, (Byte)val);
	write(--sp, (Byte)(val >> 8));
}

inline void mc6309::do_pul(Word& sp, Byte& val)
{
	val = read(sp++);
}

inline void mc6309::do_pul(Word& sp, Word& val)
{
	val  = read(sp++) << 8;
	val |= read(sp++);
}
