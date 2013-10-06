#include "cpu.h"
#define CYCLES(X) (c->c+=X)
#define WORD(X,Y) ((X<<8)|Y)
#define SET_N(val) (val |= SUBTRACT)
#define RESET_N(val) (val &= ~SUBTRACT)
#define SET_Z(val) (val |= ZERO)
#define RESET_Z(val) (val &= ~ZERO)
#define SET_C(val) (val |= CARRY)
#define RESET_C(val) (val &= ~CARRY)
#define SET_H(val) (val |= HALFCARRY)
#define RESET_H(val) (val &= ~HALFCARRY)

#define CHECK_CARRY_ADD(v,tmp) (v <= tmp)
#define CHECK_CARRY_SUB(v,tmp) (v >= tmp)
#define CHECK_HALFCARRY(v, tmp) ((v & 0xF0) != (tmp & 0xF0))

uint8_t inc(uint8_t* reg, uint8_t* flags)
{
	(*reg)++;
	if(!(*reg & 0x0F)) // Lower nibble overflown
	{
		*flags |= HALFCARRY;
	}
	if(!*reg) *flags |= ZERO;
	*flags &= ~SUBTRACT; /* Reset N flag */
	return 4;
}

uint8_t dec(uint8_t* reg, uint8_t* flags)
{
	(*reg)--;
	*flags |= SUBTRACT;
	/* H - Set if no borrow from bit 4, aka no overflow on lower nibble */
	if((*reg & 0x0F) != 0x0F) // Lower nibble not overflown
	{
		*flags |= HALFCARRY;
	}
	if(!*reg) *flags |= ZERO;
	return 4;
}
uint8_t LDrr_mm(CPU* c, MMU* m, uint8_t* regA, uint8_t* regB)
{
	/* Immediate values are in little-endian order, */
	/* so least significant bits are loaded first. */
	*regB=m[c->PC++];
	*regA=m[c->PC++];
	return 12;
}

uint8_t ADDrr_rr(CPU* c, uint8_t* ra1, uint8_t* ra2, uint8_t* rb1, uint8_t* rb2)
{
	c->reg.F &= ~SUBTRACT;
	uint16_t a1a2 = WORD(*ra1, *ra2);
	uint16_t result = a1a2 + WORD(*rb1, *rb2);
	if(result < a1a2) c->reg.F |= CARRY|HALFCARRY;
	else if((result & 0xF000) != (a1a2 & 0xF000))
	{// Carry from bit 11
		c->reg.F |= HALFCARRY;
	}
	return 8;
}

uint8_t ADDr_r(CPU* c, uint8_t* reg, uint8_t* reg2)
{
	uint8_t tmp=*reg;
	if(*reg2)
	{
		*reg += *reg2;
		if(*reg <= tmp)
		{
			c->reg.F |= CARRY;
			c->reg.F |= HALFCARRY;
		}
		else if((*reg & 0xF0) != (tmp & 0xF0))
		{
			c->reg.F |= HALFCARRY;
		}
	}
	if(!*reg) c->reg.F |= ZERO;
	c->reg.F &= ~SUBTRACT;
	return 4;
}

uint8_t ADCr_r(CPU* c, uint8_t* reg, uint8_t* reg2)
{
	uint8_t val=*reg2;
	if(c->reg.F & CARRY) val++;
	ADDr_r(c, reg, &val);
	return 4;
}

uint8_t SUBr_r(CPU* c, uint8_t* reg, uint8_t* reg2)
{
	uint8_t tmp=*reg;
	*reg -= *reg2;
	if(*reg2)
	{
		if(*reg >= tmp)
		{
			c->reg.F |= CARRY;
			c->reg.F |= HALFCARRY;
		}
		else if((*reg & 0xF0) != (tmp & 0xF0))
		{
			c->reg.F |= HALFCARRY;
		}
	}
	if(!*reg) c->reg.F |= ZERO;
	c->reg.F |= SUBTRACT;
	return 4;
}

/* 0 */

void NOP(CPU* c, MMU* m)
{
	CYCLES(4);
}

void LDBCnn(CPU* c, MMU* m)
{
	CYCLES(LDrr_mm(c, m, &c->reg.B, &c->reg.C));
}

void LDBCA(CPU* c, MMU* m)
{
	m[WORD(c->reg.B, c->reg.C)]=c->reg.A;
	CYCLES(8);
}

void INCBC(CPU* c, MMU* m)
{
	if(++c->reg.C == 0) c->reg.B++;
	CYCLES(8);
}

void INCB(CPU* c, MMU* m)
{
	CYCLES(inc(&c->reg.B, &c->reg.F));
}

void DECB(CPU* c, MMU* m)
{
	CYCLES(dec(&c->reg.B, &c->reg.F));
}

void LDBn(CPU* c, MMU* m)
{
	c->reg.B=m[c->PC++];
	CYCLES(8);
}

int RLCr(CPU* c, MMU* m, uint8_t* reg)
{
	/* Rotate register left */
	c->reg.F &= ~CARRY; // Reset carry flag
	if(*reg & 0x80)
	{
		c->reg.F |= CARRY; // Contains old bit 7 data
	}
	*reg <<= 1;
	if(c->reg.F & CARRY)
	{
		*reg |= 0x01;
	}
	if(!*reg) c->reg.F |= ZERO;
	return 4;
}

void RLCA(CPU* c, MMU* m)
{
	CYCLES(RLCr(c,m,&c->reg.A));
}

void LDnnSP(CPU* c, MMU* m)
{
	uint8_t lsb;
	uint8_t msb;
	lsb=m[c->PC++];
	msb=m[c->PC++];
	uint16_t imm=WORD(msb,lsb);
	m[imm]=c->SP;
	CYCLES(20);
}

void ADDHLBC(CPU* c, MMU* m)
{
	CYCLES(ADDrr_rr(c, &c->reg.H, &c->reg.L, &c->reg.B, &c->reg.C));
}

void LDABC(CPU* c, MMU* m)
{
	c->reg.A = m[WORD(c->reg.B, c->reg.C)];
	CYCLES(8);
}

void DECBC(CPU* c, MMU* m)
{
	if(--c->reg.C == 0xFF) c->reg.B--;
	CYCLES(8);
}

void INCC(CPU* c, MMU* m)
{
	CYCLES(inc(&c->reg.C, &c->reg.F));
}

void DECC(CPU* c, MMU* m)
{
	CYCLES(dec(&c->reg.C, &c->reg.F));
}

void LDCn(CPU* c, MMU* m)
{
	c->reg.C=m[c->PC++];
	CYCLES(8);
}

void RRCA(CPU* c, MMU* m)
{
	/* Rotate register right */
	c->reg.F &= ~CARRY; // Reset carry flag
	if(c->reg.A & 0x01)
	{
		c->reg.F |= CARRY; // Contains old bit 0 data
	}
	c->reg.A >>= 1;
	if(c->reg.F & CARRY)
	{
		c->reg.A |= 0x80;
	}
	if(!c->reg.A) c->reg.F |= ZERO;
	CYCLES(4);
}

/* 1 */

void STOP(CPU* c, MMU* m)
{
	c->stop=1;
	CYCLES(4);
}

void LDDEnn(CPU* c, MMU* m)
{
	CYCLES(LDrr_mm(c, m, &c->reg.D, &c->reg.E));
}

void LDDEA(CPU* c, MMU* m)
{
	m[WORD(c->reg.D, c->reg.E)]=c->reg.A;
	CYCLES(8);
}

void INCDE(CPU* c, MMU* m)
{
	if(++c->reg.E == 0) c->reg.D++;
	CYCLES(8);
}

void INCD(CPU* c, MMU* m)
{
	CYCLES(inc(&c->reg.D, &c->reg.F));
}

void DECD(CPU* c, MMU* m)
{
	CYCLES(dec(&c->reg.D, &c->reg.F));
}

void LDDn(CPU* c, MMU* m)
{
	c->reg.D=m[c->PC++];
	CYCLES(8);
}

void RLA(CPU* c, MMU* m)
{
	/* Rotate register left through carry flag */
	uint8_t old_carry = c->reg.F & CARRY;
	c->reg.F &= ~CARRY; // Reset carry flag
	if(c->reg.A & 0x80)
	{
		c->reg.F |= CARRY; // Contains old bit 7 data
	}
	c->reg.A <<= 1;
	if(old_carry)
	{
		c->reg.A |= 0x01;
	}
	if(!c->reg.A) c->reg.F |= ZERO;
	CYCLES(4);
}

void JRn(CPU* c, MMU* m)
{
	c->PC+=m[c->PC];
	CYCLES(8);
}

void ADDHLDE(CPU* c, MMU* m)
{
	CYCLES(ADDrr_rr(c, &c->reg.H, &c->reg.L, &c->reg.D, &c->reg.E));
}

void LDADE(CPU* c, MMU* m)
{
	c->reg.A = m[WORD(c->reg.D, c->reg.E)];
	CYCLES(8);
}

void DECDE(CPU* c, MMU* m)
{
	if(--c->reg.E == 0xFF) c->reg.D--;
	CYCLES(8);
}

void INCE(CPU* c, MMU* m)
{
	CYCLES(inc(&c->reg.E, &c->reg.F));
}

void DECE(CPU* c, MMU* m)
{
	CYCLES(dec(&c->reg.E, &c->reg.F));
}

void LDEn(CPU* c, MMU* m)
{
	c->reg.E=m[c->PC++];
	CYCLES(8);
}

void RRA(CPU* c, MMU* m)
{
	/* Rotate register right through carry flag */
	uint8_t old_carry = c->reg.F & CARRY;
	c->reg.F &= ~CARRY; // Reset carry flag
	if(c->reg.A & 0x01)
	{
		c->reg.F |= CARRY; // Contains old bit 0 data
	}
	c->reg.A >>= 1;
	if(old_carry)
	{
		c->reg.A |= 0x80;
	}
	if(!c->reg.A) c->reg.F |= ZERO;
	CYCLES(4);
}

/* 2 */

void JRNZn(CPU* c, MMU* m)
{
	if(!(c->reg.F & ZERO)) /* ZERO flag not set */
	{
		JRn(c,m); /* CYCLES(8) */
	}
}

void LDHLnn(CPU* c, MMU* m)
{
	CYCLES(LDrr_mm(c, m, &c->reg.H, &c->reg.L));
}

void LDIHLA(CPU* c, MMU* m)
{
	m[WORD(c->reg.H, c->reg.L)]=c->reg.A;
	if(++c->reg.L == 0) c->reg.H++;
	CYCLES(8);
}

void INCHL(CPU* c, MMU* m)
{
	if(++c->reg.L == 0) c->reg.H++;
	CYCLES(8);
}

void INCH(CPU* c, MMU* m)
{
	CYCLES(inc(&c->reg.H, &c->reg.F));
}

void DECH(CPU* c, MMU* m)
{
	CYCLES(dec(&c->reg.H, &c->reg.F));
}

void LDHn(CPU* c, MMU* m)
{
	c->reg.H=m[c->PC++];
	CYCLES(8);
}

void DAA(CPU* c, MMU* m)
{
	uint8_t h=c->reg.A >> 4;
	uint8_t l=c->reg.A & 0x0F;
	uint8_t addition=0x0;
	uint8_t result=0x0;
	if(c->reg.F & SUBTRACT)
	{// Last instruction was subtraction
		if(c->reg.F & CARRY)
		{
			if(c->reg.F & HALFCARRY)
			{
				if(h >= 0x06 && l >= 0x06)
				{
					addition=0x9A;
				}
			}
			else
			{
				if(h >= 0x07 && l <= 0x09)
				{
					addition=0xA0;
				}
			}
		}
		else
		{
			if(c->reg.F & HALFCARRY)
			{
				if(h <= 0x08 && l >= 0x06)
				{
					addition=0xFA;
				}
			}
		}
	}
	else
	{// Last instruction was addition
		if(c->reg.F & CARRY)
		{
			if(c->reg.F & HALFCARRY)
			{
				if(h <= 0x3 && l<= 0x3)
				{
					addition=0x66;
				}
			}
			else
			{
				if(h <= 0x02)
				{
					if(l <= 0x09)
					{
						addition=0x60;
					}
					else
					{
						addition=0x66;
					}
				}
			}
		}
		else
		{
			if(c->reg.F & HALFCARRY && l<= 0x03)
			{
				if(h <= 0x09) addition=0x06;
				else if(h >= 0xA)
				{
					addition=0x66;
					c->reg.F |= CARRY;
				}
			}
			else
			{
				if(h <= 0x08 && l >= 0x0A)
				{
					addition=0x06;
				}
				else if(h >= 0x0A && l <= 0x09)
				{
					addition=0x60;
					c->reg.F |= CARRY;
				}
				else if(h >= 0x09 && l >= 0x0A)
				{
					addition=0x66;
					c->reg.F |= CARRY;
				}
			}
		}
	}
	c->reg.F &= ~HALFCARRY;
	result = ((h << 4)|l)+addition;
	c->reg.A=result;
	if(!c->reg.A) c->reg.F |= ZERO;

	CYCLES(4);
}

void JRZn(CPU* c, MMU* m)
{
	if((c->reg.F & ZERO)) /* ZERO flag set */
	{
		JRn(c,m); /* CYCLES(8) */
	}
}

void ADDHLHL(CPU* c, MMU* m)
{
	CYCLES(ADDrr_rr(c, &c->reg.H, &c->reg.L, &c->reg.H, &c->reg.L));
}

void LDIAHL(CPU* c, MMU* m)
{
	c->reg.A = m[WORD(c->reg.H, c->reg.L)];
	if(++c->reg.L == 0) c->reg.H++;
	CYCLES(8);
}

void DECHL(CPU* c, MMU* m)
{
	if(--c->reg.L == 0xFF) c->reg.H--;
}

void INCL(CPU* c, MMU* m)
{
	CYCLES(inc(&c->reg.L, &c->reg.F));
}

void DECL(CPU* c, MMU* m)
{
	CYCLES(dec(&c->reg.L, &c->reg.F));
}

void LDLn(CPU* c, MMU* m)
{
	c->reg.L=m[c->PC++];
	CYCLES(8);
}

void CPL(CPU* c, MMU* m)
{
	c->reg.F |= SUBTRACT;
	c->reg.F |= HALFCARRY;
	c->reg.A =~ c->reg.A;
	CYCLES(4);
}

/* 3 */

void JRNCn(CPU* c, MMU* m)
{
	if(!(c->reg.F & CARRY)) /* CARRY flag not set */
	{
		JRn(c,m); /* CYCLES(8) */
	}
}

void LDSPnn(CPU* c, MMU* m)
{
	uint8_t lsb=m[c->PC++];
	uint8_t msb=m[c->PC++];
	c->SP = WORD(msb,lsb);
	CYCLES(12);
}

void LDDHLA(CPU* c, MMU* m)
{
	m[WORD(c->reg.H, c->reg.L)]=c->reg.A;
	if(--c->reg.L == 0xFF) c->reg.H--;
	CYCLES(8);
}

void INCSP(CPU* c, MMU* m)
{
	c->SP++;
	CYCLES(8);
}

void INCHL2(CPU* c, MMU* m)
{
	inc(&m[WORD(c->reg.H, c->reg.L)], &c->reg.F);
	CYCLES(12);
}

void DECHL2(CPU* c, MMU* m)
{
	dec(&m[WORD(c->reg.H, c->reg.L)], &c->reg.F);
	CYCLES(12);
}

void LDHLn(CPU* c, MMU* m)
{
	m[WORD(c->reg.H, c->reg.L)] = m[c->PC++];
	CYCLES(12);
}

void SCF(CPU* c, MMU* m)
{
	c->reg.F &= ~SUBTRACT;
	c->reg.F &= ~HALFCARRY;
	c->reg.F |= CARRY;
	CYCLES(4);
}

void JRCn(CPU* c, MMU* m)
{
	if((c->reg.F & CARRY)) /* CARRY flag set */
	{
		JRn(c,m); /* CYCLES(8) */
	}
}

void ADDHLSP(CPU* c, MMU* m)
{
	/* SP is given stupidly, but should work */
	CYCLES(ADDrr_rr(c,&c->reg.H, &c->reg.L, (uint8_t*)&c->SP, (uint8_t*)(&c->SP)+1));
}

void LDDAHL(CPU* c, MMU* m)
{
	c->reg.A = m[WORD(c->reg.H, c->reg.L)];
	if(--c->reg.L == 0xFF) c->reg.H--;
	CYCLES(8);
}

void DECSP(CPU* c, MMU* m)
{
	c->SP--;
	CYCLES(8);
}

void INCA(CPU* c, MMU* m)
{
	CYCLES(inc(&c->reg.A, &c->reg.F));
}

void DECA(CPU* c, MMU* m)
{
	CYCLES(dec(&c->reg.A, &c->reg.F));
}

void LDAn(CPU* c, MMU* m)
{
	c->reg.A=m[c->PC++];
	CYCLES(8);
}

void CCF(CPU* c, MMU* m)
{
	c->reg.F &= ~SUBTRACT;
	c->reg.F &= ~HALFCARRY;
	c->reg.F &= ~(c->reg.F & CARRY);
	CYCLES(4);
}

/* 4 */

void LDBB(CPU* c, MMU* m)
{
	c->reg.B=c->reg.B;
	CYCLES(4);
}

void LDBC(CPU* c, MMU* m)
{
	c->reg.B=c->reg.C;
	CYCLES(4);
}

void LDBD(CPU* c, MMU* m)
{
	c->reg.B=c->reg.D;
	CYCLES(4);
}

void LDBE(CPU* c, MMU* m)
{
	c->reg.B=c->reg.E;
	CYCLES(4);
}

void LDBH(CPU* c, MMU* m)
{
	c->reg.B=c->reg.H;
	CYCLES(4);
}

void LDBL(CPU* c, MMU* m)
{
	c->reg.B=c->reg.L;
	CYCLES(4);
}

void LDBHL(CPU* c, MMU* m)
{
	c->reg.B=m[WORD(c->reg.H, c->reg.L)];
	CYCLES(8);
}

void LDBA(CPU* c, MMU* m)
{
	c->reg.B=c->reg.A;
	CYCLES(4);
}

void LDCB(CPU* c, MMU* m)
{
	c->reg.C=c->reg.B;
	CYCLES(4);
}

void LDCC(CPU* c, MMU* m)
{
	c->reg.C=c->reg.C;
	CYCLES(4);
}

void LDCD(CPU* c, MMU* m)
{
	c->reg.C=c->reg.D;
	CYCLES(4);
}

void LDCE(CPU* c, MMU* m)
{
	c->reg.C=c->reg.E;
	CYCLES(4);
}

void LDCH(CPU* c, MMU* m)
{
	c->reg.C=c->reg.H;
	CYCLES(4);
}

void LDCL(CPU* c, MMU* m)
{
	c->reg.C=c->reg.L;
	CYCLES(4);
}

void LDCHL(CPU* c, MMU* m)
{
	c->reg.C=m[WORD(c->reg.H, c->reg.L)];
	CYCLES(8);
}

void LDCA(CPU* c, MMU* m)
{
	c->reg.C=c->reg.A;
	CYCLES(4);
}

/* 5 */

void LDDB(CPU* c, MMU* m)
{
	c->reg.D=c->reg.B;
	CYCLES(4);
}

void LDDC(CPU* c, MMU* m)
{
	c->reg.D=c->reg.C;
	CYCLES(4);
}

void LDDD(CPU* c, MMU* m)
{
	c->reg.D=c->reg.D;
	CYCLES(4);
}

void LDDE(CPU* c, MMU* m)
{
	c->reg.D=c->reg.E;
	CYCLES(4);
}

void LDDH(CPU* c, MMU* m)
{
	c->reg.D=c->reg.H;
	CYCLES(4);
}

void LDDL(CPU* c, MMU* m)
{
	c->reg.D=c->reg.L;
	CYCLES(4);
}

void LDDHL(CPU* c, MMU* m)
{
	c->reg.D=m[WORD(c->reg.H, c->reg.L)];
	CYCLES(8);
}

void LDDA(CPU* c, MMU* m)
{
	c->reg.D=c->reg.A;
	CYCLES(4);
}

void LDEB(CPU* c, MMU* m)
{
	c->reg.E=c->reg.B;
	CYCLES(4);
}

void LDEC(CPU* c, MMU* m)
{
	c->reg.E=c->reg.C;
	CYCLES(4);
}

void LDED(CPU* c, MMU* m)
{
	c->reg.E=c->reg.D;
	CYCLES(4);
}

void LDEE(CPU* c, MMU* m)
{
	c->reg.E=c->reg.E;
	CYCLES(4);
}

void LDEH(CPU* c, MMU* m)
{
	c->reg.E=c->reg.H;
	CYCLES(4);
}

void LDEL(CPU* c, MMU* m)
{
	c->reg.E=c->reg.L;
	CYCLES(4);
}

void LDEHL(CPU* c, MMU* m)
{
	c->reg.E=m[WORD(c->reg.H, c->reg.L)];
	CYCLES(8);
}

void LDEA(CPU* c, MMU* m)
{
	c->reg.E=c->reg.A;
	CYCLES(4);
}

/* 6 */

void LDHB(CPU* c, MMU* m)
{
	c->reg.H=c->reg.B;
	CYCLES(4);
}

void LDHC(CPU* c, MMU* m)
{
	c->reg.H=c->reg.C;
	CYCLES(4);
}

void LDHD(CPU* c, MMU* m)
{
	c->reg.H=c->reg.D;
	CYCLES(4);
}

void LDHE(CPU* c, MMU* m)
{
	c->reg.H=c->reg.E;
	CYCLES(4);
}

void LDHH(CPU* c, MMU* m)
{
	c->reg.H=c->reg.H;
	CYCLES(4);
}

void LDHL(CPU* c, MMU* m)
{
	c->reg.H=c->reg.L;
	CYCLES(4);
}

void LDHHL(CPU* c, MMU* m)
{
	c->reg.H=m[WORD(c->reg.H, c->reg.L)];
	CYCLES(8);
}

void LDHA(CPU* c, MMU* m)
{
	c->reg.H=c->reg.A;
	CYCLES(4);
}

void LDLB(CPU* c, MMU* m)
{
	c->reg.L=c->reg.B;
	CYCLES(4);
}

void LDLC(CPU* c, MMU* m)
{
	c->reg.L=c->reg.C;
	CYCLES(4);
}

void LDLD(CPU* c, MMU* m)
{
	c->reg.L=c->reg.D;
	CYCLES(4);
}

void LDLE(CPU* c, MMU* m)
{
	c->reg.L=c->reg.E;
	CYCLES(4);
}

void LDLH(CPU* c, MMU* m)
{
	c->reg.L=c->reg.H;
	CYCLES(4);
}

void LDLL(CPU* c, MMU* m)
{
	c->reg.L=c->reg.L;
	CYCLES(4);
}

void LDLHL(CPU* c, MMU* m)
{
	c->reg.L=m[WORD(c->reg.H, c->reg.L)];
	CYCLES(8);
}

void LDLA(CPU* c, MMU* m)
{
	c->reg.L=c->reg.A;
	CYCLES(4);
}

/* 7 */

void LDHLB(CPU* c, MMU* m)
{
	m[WORD(c->reg.H, c->reg.L)]=c->reg.B;
	CYCLES(8);
}

void LDHLC(CPU* c, MMU* m)
{
	m[WORD(c->reg.H, c->reg.L)]=c->reg.C;
	CYCLES(8);
}

void LDHLD(CPU* c, MMU* m)
{
	m[WORD(c->reg.H, c->reg.L)]=c->reg.D;
	CYCLES(8);
}

void LDHLE(CPU* c, MMU* m)
{
	m[WORD(c->reg.H, c->reg.L)]=c->reg.E;
	CYCLES(8);
}

void LDHLH(CPU* c, MMU* m)
{
	m[WORD(c->reg.H, c->reg.L)]=c->reg.H;
	CYCLES(8);
}

void LDHLL(CPU* c, MMU* m)
{
	m[WORD(c->reg.H, c->reg.L)]=c->reg.L;
	CYCLES(8);
}

void HALT(CPU* c, MMU* m)
{
	c->halt=1;
	CYCLES(4);
}

void LDHLA(CPU* c, MMU* m)
{
	m[WORD(c->reg.H, c->reg.L)]=c->reg.A;
	CYCLES(8);
}

void LDAB(CPU* c, MMU* m)
{
	c->reg.A=c->reg.B;
	CYCLES(4);
}

void LDAC(CPU* c, MMU* m)
{
	c->reg.A=c->reg.C;
	CYCLES(4);
}

void LDAD(CPU* c, MMU* m)
{
	c->reg.A=c->reg.D;
	CYCLES(4);
}

void LDAE(CPU* c, MMU* m)
{
	c->reg.A=c->reg.E;
	CYCLES(4);
}

void LDAH(CPU* c, MMU* m)
{
	c->reg.A=c->reg.H;
	CYCLES(4);
}

void LDAL(CPU* c, MMU* m)
{
	c->reg.A=c->reg.L;
	CYCLES(4);
}

void LDAHL(CPU* c, MMU* m)
{
	c->reg.A=m[WORD(c->reg.H, c->reg.L)];
	CYCLES(8);
}

void LDAA(CPU* c, MMU* m)
{
	c->reg.A=c->reg.A;
	CYCLES(4);
}

/* 8 */

void ADDAB(CPU* c, MMU* m)
{
	CYCLES(ADDr_r(c, &c->reg.A, &c->reg.B));
}

void ADDAC(CPU* c, MMU* m)
{
	CYCLES(ADDr_r(c, &c->reg.A, &c->reg.C));
}

void ADDAD(CPU* c, MMU* m)
{
	CYCLES(ADDr_r(c, &c->reg.A, &c->reg.D));
}

void ADDAE(CPU* c, MMU* m)
{
	CYCLES(ADDr_r(c, &c->reg.A, &c->reg.E));
}

void ADDAH(CPU* c, MMU* m)
{
	CYCLES(ADDr_r(c, &c->reg.A, &c->reg.H));
}

void ADDAL(CPU* c, MMU* m)
{
	CYCLES(ADDr_r(c, &c->reg.A, &c->reg.L));
}

void ADDAHL(CPU* c, MMU* m)
{
	ADDr_r(c, &c->reg.A, &m[WORD(c->reg.H, c->reg.L)]);
	CYCLES(8);
}

void ADDAA(CPU* c, MMU* m)
{
	CYCLES(ADDr_r(c, &c->reg.A, &c->reg.A));
}

void ADCAB(CPU* c, MMU* m)
{
	CYCLES(ADCr_r(c, &c->reg.A, &c->reg.B));
}

void ADCAC(CPU* c, MMU* m)
{
	CYCLES(ADCr_r(c, &c->reg.A, &c->reg.C));
}

void ADCAD(CPU* c, MMU* m)
{
	CYCLES(ADCr_r(c, &c->reg.A, &c->reg.D));
}

void ADCAE(CPU* c, MMU* m)
{
	CYCLES(ADCr_r(c, &c->reg.A, &c->reg.E));
}

void ADCAH(CPU* c, MMU* m)
{
	CYCLES(ADCr_r(c, &c->reg.A, &c->reg.H));
}

void ADCAL(CPU* c, MMU* m)
{
	CYCLES(ADCr_r(c, &c->reg.A, &c->reg.L));
}

void ADCAHL(CPU* c, MMU* m)
{
	ADCr_r(c, &c->reg.A, &m[WORD(c->reg.H, c->reg.L)]);
	CYCLES(8);
}

void ADCAA(CPU* c, MMU* m)
{
	CYCLES(ADCr_r(c, &c->reg.A, &c->reg.A));
}

/* 9 */

void SUBAB(CPU* c, MMU* m)
{
	CYCLES(SUBr_r(c, &c->reg.A, &c->reg.B));
}

void SUBAC(CPU* c, MMU* m)
{
	CYCLES(SUBr_r(c, &c->reg.A, &c->reg.C));
}

void SUBAD(CPU* c, MMU* m)
{
	CYCLES(SUBr_r(c, &c->reg.A, &c->reg.D));
}

void SUBAE(CPU* c, MMU* m)
{
	CYCLES(SUBr_r(c, &c->reg.A, &c->reg.E));
}

void SUBAH(CPU* c, MMU* m)
{
	CYCLES(SUBr_r(c, &c->reg.A, &c->reg.H));
}

void SUBAL(CPU* c, MMU* m)
{
	CYCLES(SUBr_r(c, &c->reg.A, &c->reg.L));
}

void SUBAHL(CPU* c, MMU* m)
{
	SUBr_r(c, &c->reg.A, &m[WORD(c->reg.H, c->reg.L)]);
	CYCLES(8);
}

void SUBAA(CPU* c, MMU* m)
{
	CYCLES(SUBr_r(c, &c->reg.A, &c->reg.A));
}

uint8_t SBCr_r(CPU* c, uint8_t* reg1, uint8_t* reg2)
{// Subtract n + carry flag from A
	SET_N(c->reg.F);
	if(*reg2)
	{
		uint8_t tmp=*reg1;
		*reg1 -= *reg2;
		if(CHECK_CARRY_SUB(*reg1, tmp))
		{
			SET_H(c->reg.F);
			SET_C(c->reg.F);
		}
		else if(CHECK_HALFCARRY(*reg1, tmp))
		{
			SET_H(c->reg.F);
		}
	}
	if(*reg1 == 0x0) SET_Z(c->reg.F);
	return 4;
}

void SBCAB(CPU* c, MMU* m)
{
	CYCLES(SBCr_r(c, &c->reg.A, &c->reg.B));
}

void SBCAC(CPU* c, MMU* m)
{
	CYCLES(SBCr_r(c, &c->reg.A, &c->reg.C));
}

void SBCAD(CPU* c, MMU* m)
{
	CYCLES(SBCr_r(c, &c->reg.A, &c->reg.D));
}

void SBCAE(CPU* c, MMU* m)
{
	CYCLES(SBCr_r(c, &c->reg.A, &c->reg.E));
}

void SBCAH(CPU* c, MMU* m)
{
	CYCLES(SBCr_r(c, &c->reg.A, &c->reg.H));
}

void SBCAL(CPU* c, MMU* m)
{
	CYCLES(SBCr_r(c, &c->reg.A, &c->reg.L));
}

void SBCAHL(CPU* c, MMU* m)
{
	SBCr_r(c, &c->reg.A, &m[WORD(c->reg.H, c->reg.L)]);
	CYCLES(8);
}

void SBCAA(CPU* c, MMU* m)
{
	CYCLES(SBCr_r(c, &c->reg.A, &c->reg.A));
}

/* A */

int ANDr_r(CPU* c, uint8_t* reg1, uint8_t* reg2)
{
	RESET_N(c->reg.F);
	SET_H(c->reg.F);
	RESET_C(c->reg.F);

	*reg1 = *reg1 & *reg2;

	if(*reg1 == 0x0) SET_Z(c->reg.F);

	return 4;
}

void ANDB(CPU* c, MMU* m)
{
	CYCLES(ANDr_r(c, &c->reg.A, &c->reg.B));
}

void ANDC(CPU* c, MMU* m)
{
	CYCLES(ANDr_r(c, &c->reg.A, &c->reg.C));
}

void ANDD(CPU* c, MMU* m)
{
	CYCLES(ANDr_r(c, &c->reg.A, &c->reg.D));
}

void ANDE(CPU* c, MMU* m)
{
	CYCLES(ANDr_r(c, &c->reg.A, &c->reg.E));
}

void ANDH(CPU* c, MMU* m)
{
	CYCLES(ANDr_r(c, &c->reg.A, &c->reg.H));
}

void ANDL(CPU* c, MMU* m)
{
	CYCLES(ANDr_r(c, &c->reg.A, &c->reg.L));
}

void ANDHL(CPU* c, MMU* m)
{
	ANDr_r(c, &c->reg.A, &m[WORD(c->reg.H, c->reg.L)]);
	CYCLES(8);
}

void ANDA(CPU* c, MMU* m)
{
	CYCLES(ANDr_r(c, &c->reg.A, &c->reg.A));
}

int XORr_r(CPU* c, uint8_t* reg1, uint8_t* reg2)
{
	RESET_N(c->reg.F);
	RESET_H(c->reg.F);
	RESET_C(c->reg.F);

	*reg1 = *reg1 ^ *reg2;

	if(*reg1 == 0x0) SET_Z(c->reg.F);

	return 4;
}

void XORB(CPU* c, MMU* m)
{
	CYCLES(XORr_r(c, &c->reg.A, &c->reg.B));
}

void XORC(CPU* c, MMU* m)
{
	CYCLES(XORr_r(c, &c->reg.A, &c->reg.C));
}

void XORD(CPU* c, MMU* m)
{
	CYCLES(XORr_r(c, &c->reg.A, &c->reg.D));
}

void XORE(CPU* c, MMU* m)
{
	CYCLES(XORr_r(c, &c->reg.A, &c->reg.E));
}

void XORH(CPU* c, MMU* m)
{
	CYCLES(XORr_r(c, &c->reg.A, &c->reg.H));
}

void XORL(CPU* c, MMU* m)
{
	CYCLES(XORr_r(c, &c->reg.A, &c->reg.L));
}

void XORHL(CPU* c, MMU* m)
{
	XORr_r(c, &c->reg.A, &m[WORD(c->reg.H, c->reg.L)]);
	CYCLES(8);
}

void XORA(CPU* c, MMU* m)
{
	CYCLES(XORr_r(c, &c->reg.A, &c->reg.A));
}

/* B */

int ORr_r(CPU* c, uint8_t* reg1, uint8_t* reg2)
{
	RESET_N(c->reg.F);
	RESET_H(c->reg.F);
	RESET_C(c->reg.F);

	*reg1 = *reg1 | *reg2;

	if(*reg1 == 0x0) SET_Z(c->reg.F);

	return 4;
}

void ORB(CPU* c, MMU* m)
{
	CYCLES(ORr_r(c, &c->reg.A, &c->reg.B));
}

void ORC(CPU* c, MMU* m)
{
	CYCLES(ORr_r(c, &c->reg.A, &c->reg.C));
}

void ORD(CPU* c, MMU* m)
{
	CYCLES(ORr_r(c, &c->reg.A, &c->reg.D));
}

void ORE(CPU* c, MMU* m)
{
	CYCLES(ORr_r(c, &c->reg.A, &c->reg.E));
}

void ORH(CPU* c, MMU* m)
{
	CYCLES(ORr_r(c, &c->reg.A, &c->reg.H));
}

void ORL(CPU* c, MMU* m)
{
	CYCLES(ORr_r(c, &c->reg.A, &c->reg.L));
}

void ORHL(CPU* c, MMU* m)
{
	ORr_r(c, &c->reg.A, &m[WORD(c->reg.H, c->reg.L)]);
	CYCLES(8);
}

void ORA(CPU* c, MMU* m)
{
	CYCLES(ORr_r(c, &c->reg.A, &c->reg.A));
}

int CPr_r(CPU* c, uint8_t* reg1, uint8_t* reg2)
{
	SET_N(c->reg.F);
	if(CHECK_HALFCARRY(*reg1, *reg2))
	{
		SET_H(c->reg.F);
	}
	if(*reg1 < *reg2) SET_C(c->reg.F);
	return 4;
}

void CPB(CPU* c, MMU* m)
{
	CYCLES(CPr_r(c, &c->reg.A, &c->reg.B));
}

void CPC(CPU* c, MMU* m)
{
	CYCLES(CPr_r(c, &c->reg.A, &c->reg.C));
}

void CPD(CPU* c, MMU* m)
{
	CYCLES(CPr_r(c, &c->reg.A, &c->reg.D));
}

void CPE(CPU* c, MMU* m)
{
	CYCLES(CPr_r(c, &c->reg.A, &c->reg.E));
}

void CPH(CPU* c, MMU* m)
{
	CYCLES(CPr_r(c, &c->reg.A, &c->reg.H));
}

void CP_L(CPU* c, MMU* m)
{
	CYCLES(CPr_r(c, &c->reg.A, &c->reg.L));
}

void CPHL(CPU* c, MMU* m)
{
	CPr_r(c, &c->reg.A, &m[WORD(c->reg.H, c->reg.L)]);
	CYCLES(8);
}

void CPA(CPU* c, MMU* m)
{
	CYCLES(CPr_r(c, &c->reg.A, &c->reg.A));
}

/* C */

void RETNZ(CPU* c, MMU* m)
{
	if(!(c->reg.F & ZERO)) RET(c,m);
	else CYCLES(8);
}

void POPr_r(CPU* c, MMU* m, uint8_t* reg1, uint8_t* reg2)
{
	*reg1=m[c->SP++];
	*reg2=m[c->SP++];
	CYCLES(12);
}

void POPBC(CPU* c, MMU* m)
{
	POPr_r(c,m,&c->reg.B,&c->reg.C);
}

void JPNZnn(CPU* c, MMU* m)
{
	if((c->reg.F & ZERO) == 0x0) JPnn(c,m);
	else CYCLES(12);
}

void JPnn(CPU* c, MMU* m)
{
	uint16_t addr=0;
	uint8_t lsb=m[c->PC++];
	uint8_t msb=m[c->PC++];
	addr=WORD(msb, lsb);
	c->PC=addr;
	CYCLES(12);
}

void CALLNZnn(CPU* c, MMU* m)
{
	if((c->reg.F & ZERO) == 0x0) CALLnn(c,m);
	else CYCLES(12);
}

void PUSHr_r(CPU* c, MMU* m, uint8_t* reg1, uint8_t* reg2)
{
	m[--c->SP]=*reg1;
	m[--c->SP]=*reg2;
	CYCLES(16);
}

void PUSHBC(CPU* c, MMU* m)
{
	PUSHr_r(c,m,&c->reg.B,&c->reg.C);
}

void ADDAn(CPU* c, MMU* m)
{
	ADDr_r(c, &c->reg.A, &m[c->PC++]);
	CYCLES(8);
}

void RST(CPU* c, MMU* m, uint8_t val)
{
	m[--c->SP]=c->PC;
	c->PC=val;
	CYCLES(32);
}

void RST0(CPU* c, MMU* m)
{
	RST(c,m,0);
}

void RETZ(CPU* c, MMU* m)
{
	if(c->reg.F & ZERO) RET(c,m);
	else CYCLES(8);
}

void RET(CPU* c, MMU* m)
{// Pop two bytes from stack & jump to that address
	c->PC=c->SP;
	c->SP+=2;
	CYCLES(8);
}

void JPZnn(CPU* c, MMU* m)
{
	if(c->reg.F & ZERO) JPnn(c,m);
	else CYCLES(12);
}

void Extops(CPU* c, MMU* m)
{
}

void CALLZnn(CPU* c, MMU* m)
{
	if(c->reg.F & ZERO) CALLnn(c,m);
	else CYCLES(12);
}

void CALLnn(CPU* c, MMU* m)
{
	c->SP-=2;
	uint8_t lsb=m[c->PC++];
	uint8_t msb=m[c->PC++];
	m[c->SP]=c->PC;
	c->PC=WORD(msb, lsb);
	CYCLES(12);
}

void ADCAn(CPU* c, MMU* m)
{
	ADCr_r(c, &c->reg.A, &m[c->PC++]);
	CYCLES(8);
}

void RST8(CPU* c, MMU* m)
{
	RST(c,m,0x8);
}

/* D */

void RETNC(CPU* c, MMU* m)
{
	if(!(c->reg.F & CARRY)) RET(c,m);
	else CYCLES(8);
}

void POPDE(CPU* c, MMU* m)
{
	POPr_r(c,m,&c->reg.D,&c->reg.E);
}

void JPNCnn(CPU* c, MMU* m)
{
	if((c->reg.F & CARRY) == 0x0) JPnn(c,m);
	else CYCLES(12);
}

void CALLNCnn(CPU* c, MMU* m)
{
	if((c->reg.F & CARRY) == 0x0) CALLnn(c,m);
	else CYCLES(12);
}

void PUSHDE(CPU* c, MMU* m)
{
	PUSHr_r(c,m,&c->reg.D,&c->reg.E);
}

void SUBAn(CPU* c, MMU* m)
{
	SUBr_r(c,&c->reg.A, &m[c->PC++]);
	CYCLES(8);
}

void RST10(CPU* c, MMU* m)
{
	RST(c,m,0x10);
}

void RETC(CPU* c, MMU* m)
{
	if(c->reg.F & CARRY) RET(c,m);
	else CYCLES(8);
}

void RETI(CPU* c, MMU* m)
{
	RET(c,m);
	// TODO: Enable interrupts
}

void JPCnn(CPU* c, MMU* m)
{
	if(c->reg.F & CARRY) JPnn(c,m);
	else CYCLES(12);
}

void CALLCnn(CPU* c, MMU* m)
{
	if(c->reg.F & CARRY) CALLnn(c,m);
	else CYCLES(12);
}

void SBCAn(CPU* c, MMU* m)
{
	SBCr_r(c, &c->reg.A, &m[c->PC++]);
	CYCLES(8);
}

void RST18(CPU* c, MMU* m)
{
	RST(c,m,0x18);
}

/* E */

void LDHnA(CPU* c, MMU* m)
{
	m[0xFF00 + m[c->PC++]]=c->reg.A;
	CYCLES(12);
}

void POPHL(CPU* c, MMU* m)
{
	POPr_r(c,m,&c->reg.H,&c->reg.L);
}

void LDHCA(CPU* c, MMU* m)
{
	m[0xFF00 + c->reg.C]=c->reg.A;
	CYCLES(8);
}

void PUSHHL(CPU* c, MMU* m)
{
	PUSHr_r(c,m,&c->reg.H,&c->reg.L);
}

void ANDn(CPU* c, MMU* m)
{
	ANDr_r(c, &c->reg.A, &m[c->PC++]);
	CYCLES(8);
}

void RST20(CPU* c, MMU* m)
{
	RST(c,m,0x20);
}

void ADDSPd(CPU* c, MMU* m)
{
	RESET_Z(c->reg.F);
	RESET_N(c->reg.F);
	uint8_t tmp=0;
	ADDrr_rr(c, (uint8_t*)&c->SP, (uint8_t*)(&c->SP)+1, &tmp, &m[c->PC++]);
	CYCLES(16);
}

void JPHL(CPU* c, MMU* m)
{
	c->PC=m[WORD(c->reg.H, c->reg.L)];
	CYCLES(4);
}

void LDnnA(CPU* c, MMU* m)
{
	uint8_t lsb=m[c->PC++];
	uint8_t msb=m[c->PC++];
	uint16_t imm=WORD(msb,lsb);
	m[imm]=c->reg.A;
	CYCLES(16);
}

void XORn(CPU* c, MMU* m)
{
	XORr_r(c, &c->reg.A, &m[c->PC++]);
	CYCLES(8);
}

void RST28(CPU* c, MMU* m)
{
	RST(c,m,0x28);
}

/* F */

void LDHAn(CPU* c, MMU* m)
{
	c->reg.A=m[0xFF00 + m[c->PC++]];
	CYCLES(12);
}

void POPAF(CPU* c, MMU* m)
{
	POPr_r(c,m,&c->reg.A,&c->reg.F);
}

void DI(CPU* c, MMU* m)
{
	/* Disable interrupts */
}

void PUSHAF(CPU* c, MMU* m)
{
	PUSHr_r(c,m,&c->reg.A,&c->reg.F);
}

void ORn(CPU* c, MMU* m)
{
	ORr_r(c, &c->reg.A, &m[c->PC++]);
	CYCLES(8);
}

void RST30(CPU* c, MMU* m)
{
	RST(c,m,0x30);
}

void LDHLSPd(CPU* c, MMU* m)
{
	// TODO: Check endianess
	RESET_Z(c->reg.F);
	RESET_N(c->reg.F);
	uint16_t val=c->SP+m[c->PC++];
	c->reg.H=val>>8;
	c->reg.L=val&0x00FF;
	CYCLES(12);
}

void LDSPHL(CPU* c, MMU* m)
{
	c->SP=WORD(c->reg.H, c->reg.L);
	CYCLES(8);
}

void LDAnn(CPU* c, MMU* m)
{
	uint8_t lsb=m[c->PC++];
	uint8_t msb=m[c->PC++];
	uint16_t imm=WORD(msb, lsb);
	c->reg.A=m[imm];
	CYCLES(16);
}

void EI(CPU* c, MMU* m)
{
	/* Enable interrupts */
}

void CPn(CPU* c, MMU* m)
{
	CPr_r(c, &c->reg.A, &m[c->PC++]);
	CYCLES(8);
}

void RST38(CPU* c, MMU* m)
{
	RST(c,m,0x38);
}

void RLCB(CPU* c, MMU* m)
{
	RLCr(c,m,&c->reg.B);
	CYCLES(8);
}

void RLCC(CPU* c, MMU* m)
{
	RLCr(c,m,&c->reg.C);
	CYCLES(8);
}

void RLCD(CPU* c, MMU* m)
{
	RLCr(c,m,&c->reg.D);
	CYCLES(8);
}

void RLCE(CPU* c, MMU* m)
{
	RLCr(c,m,&c->reg.E);
	CYCLES(8);
}

void RLCH(CPU* c, MMU* m)
{
	RLCr(c,m,&c->reg.H);
	CYCLES(8);
}

void RLCL(CPU* c, MMU* m)
{
	RLCr(c,m,&c->reg.L);
	CYCLES(8);
}

void RLCHL(CPU* c, MMU* m)
{
	RLCr(c,m,&m[WORD(c->reg.H, c->reg.L)]);
	CYCLES(16);
}

void RLCAext(CPU* c, MMU* m)
{
	RLCA(c,m);
	CYCLES(8);
}

void RRCB(CPU* c, MMU* m)
{
}

void RRCC(CPU* c, MMU* m)
{
}

void RRCD(CPU* c, MMU* m)
{
}

void RRCE(CPU* c, MMU* m)
{
}

void RRCH(CPU* c, MMU* m)
{
}

void RRCL(CPU* c, MMU* m)
{
}

void RRCHL(CPU* c, MMU* m)
{
}

void RRCAext(CPU* c, MMU* m)
{
}

void RLB(CPU* c, MMU* m)
{
}

void RLC(CPU* c, MMU* m)
{
}

void RLD(CPU* c, MMU* m)
{
}

void RLE(CPU* c, MMU* m)
{
}

void RLH(CPU* c, MMU* m)
{
}

void RLL(CPU* c, MMU* m)
{
}

void RLHL(CPU* c, MMU* m)
{
}

void RLAext(CPU* c, MMU* m)
{
}

void RRB(CPU* c, MMU* m)
{
}

void RRC(CPU* c, MMU* m)
{
}

void RRD(CPU* c, MMU* m)
{
}

void RRE(CPU* c, MMU* m)
{
}

void RRH(CPU* c, MMU* m)
{
}

void RRL(CPU* c, MMU* m)
{
}

void RRHL(CPU* c, MMU* m)
{
}

void RRAext(CPU* c, MMU* m)
{
}

void SLAB(CPU* c, MMU* m)
{
}

void SLAC(CPU* c, MMU* m)
{
}

void SLAD(CPU* c, MMU* m)
{
}

void SLAE(CPU* c, MMU* m)
{
}

void SLAH(CPU* c, MMU* m)
{
}

void SLAL(CPU* c, MMU* m)
{
}

void SLAHL(CPU* c, MMU* m)
{
}

void SLAA(CPU* c, MMU* m)
{
}

void SRAB(CPU* c, MMU* m)
{
}

void SRAC(CPU* c, MMU* m)
{
}

void SRAD(CPU* c, MMU* m)
{
}

void SRAE(CPU* c, MMU* m)
{
}

void SRAH(CPU* c, MMU* m)
{
}

void SRAL(CPU* c, MMU* m)
{
}

void SRAHL(CPU* c, MMU* m)
{
}

void SRAA(CPU* c, MMU* m)
{
}

void SWAPB(CPU* c, MMU* m)
{
}

void SWAPC(CPU* c, MMU* m)
{
}

void SWAPD(CPU* c, MMU* m)
{
}

void SWAPE(CPU* c, MMU* m)
{
}

void SWAPH(CPU* c, MMU* m)
{
}

void SWAPL(CPU* c, MMU* m)
{
}

void SWAPHL(CPU* c, MMU* m)
{
}

void SWAPA(CPU* c, MMU* m)
{
}

void SRLB(CPU* c, MMU* m)
{
}

void SRLC(CPU* c, MMU* m)
{
}

void SRLD(CPU* c, MMU* m)
{
}

void SRLE(CPU* c, MMU* m)
{
}

void SRLF(CPU* c, MMU* m)
{
}

void SRLH(CPU* c, MMU* m)
{
}

void SRLL(CPU* c, MMU* m)
{
}

void SRLHL(CPU* c, MMU* m)
{
}

void SRLA(CPU* c, MMU* m)
{
}

void BIT0B(CPU* c, MMU* m)
{
}

void BIT0C(CPU* c, MMU* m)
{
}

void BIT0D(CPU* c, MMU* m)
{
}

void BIT0E(CPU* c, MMU* m)
{
}

void BIT0H(CPU* c, MMU* m)
{
}

void BIT0L(CPU* c, MMU* m)
{
}

void BIT0HL(CPU* c, MMU* m)
{
}

void BIT0A(CPU* c, MMU* m)
{
}

void BIT1B(CPU* c, MMU* m)
{
}

void BIT1C(CPU* c, MMU* m)
{
}

void BIT1D(CPU* c, MMU* m)
{
}

void BIT1E(CPU* c, MMU* m)
{
}

void BIT1H(CPU* c, MMU* m)
{
}

void BIT1L(CPU* c, MMU* m)
{
}

void BIT1HL(CPU* c, MMU* m)
{
}

void BIT1A(CPU* c, MMU* m)
{
}

void BIT2B(CPU* c, MMU* m)
{
}

void BIT2C(CPU* c, MMU* m)
{
}

void BIT2D(CPU* c, MMU* m)
{
}

void BIT2E(CPU* c, MMU* m)
{
}

void BIT2H(CPU* c, MMU* m)
{
}

void BIT2L(CPU* c, MMU* m)
{
}

void BIT2HL(CPU* c, MMU* m)
{
}

void BIT2A(CPU* c, MMU* m)
{
}

void BIT3B(CPU* c, MMU* m)
{
}

void BIT3C(CPU* c, MMU* m)
{
}

void BIT3D(CPU* c, MMU* m)
{
}

void BIT3E(CPU* c, MMU* m)
{
}

void BIT3H(CPU* c, MMU* m)
{
}

void BIT3L(CPU* c, MMU* m)
{
}

void BIT3HL(CPU* c, MMU* m)
{
}

void BIT3A(CPU* c, MMU* m)
{
}

void BIT4B(CPU* c, MMU* m)
{
}

void BIT4C(CPU* c, MMU* m)
{
}

void BIT4D(CPU* c, MMU* m)
{
}

void BIT4E(CPU* c, MMU* m)
{
}

void BIT4H(CPU* c, MMU* m)
{
}

void BIT4L(CPU* c, MMU* m)
{
}

void BIT4HL(CPU* c, MMU* m)
{
}

void BIT4A(CPU* c, MMU* m)
{
}

void BIT5B(CPU* c, MMU* m)
{
}

void BIT5C(CPU* c, MMU* m)
{
}

void BIT5D(CPU* c, MMU* m)
{
}

void BIT5E(CPU* c, MMU* m)
{
}

void BIT5H(CPU* c, MMU* m)
{
}

void BIT5L(CPU* c, MMU* m)
{
}

void BIT5HL(CPU* c, MMU* m)
{
}

void BIT5A(CPU* c, MMU* m)
{
}

void BIT6B(CPU* c, MMU* m)
{
}

void BIT6C(CPU* c, MMU* m)
{
}

void BIT6D(CPU* c, MMU* m)
{
}

void BIT6E(CPU* c, MMU* m)
{
}

void BIT6H(CPU* c, MMU* m)
{
}

void BIT6L(CPU* c, MMU* m)
{
}

void BIT6HL(CPU* c, MMU* m)
{
}

void BIT6A(CPU* c, MMU* m)
{
}

void BIT7B(CPU* c, MMU* m)
{
}

void BIT7C(CPU* c, MMU* m)
{
}

void BIT7D(CPU* c, MMU* m)
{
}

void BIT7E(CPU* c, MMU* m)
{
}

void BIT7H(CPU* c, MMU* m)
{
}

void BIT7L(CPU* c, MMU* m)
{
}

void BIT7HL(CPU* c, MMU* m)
{
}

void BIT7A(CPU* c, MMU* m)
{
}

void RES0B(CPU* c, MMU* m)
{
}

void RES0C(CPU* c, MMU* m)
{
}

void RES0D(CPU* c, MMU* m)
{
}

void RES0E(CPU* c, MMU* m)
{
}

void RES0H(CPU* c, MMU* m)
{
}

void RES0L(CPU* c, MMU* m)
{
}

void RES0HL(CPU* c, MMU* m)
{
}

void RES0A(CPU* c, MMU* m)
{
}

void RES1B(CPU* c, MMU* m)
{
}

void RES1C(CPU* c, MMU* m)
{
}

void RES1D(CPU* c, MMU* m)
{
}

void RES1E(CPU* c, MMU* m)
{
}

void RES1H(CPU* c, MMU* m)
{
}

void RES1L(CPU* c, MMU* m)
{
}

void RES1HL(CPU* c, MMU* m)
{
}

void RES1A(CPU* c, MMU* m)
{
}

void RES2B(CPU* c, MMU* m)
{
}

void RES2C(CPU* c, MMU* m)
{
}

void RES2D(CPU* c, MMU* m)
{
}

void RES2E(CPU* c, MMU* m)
{
}

void RES2H(CPU* c, MMU* m)
{
}

void RES2L(CPU* c, MMU* m)
{
}

void RES2HL(CPU* c, MMU* m)
{
}

void RES2A(CPU* c, MMU* m)
{
}

void RES3B(CPU* c, MMU* m)
{
}

void RES3C(CPU* c, MMU* m)
{
}

void RES3D(CPU* c, MMU* m)
{
}

void RES3E(CPU* c, MMU* m)
{
}

void RES3H(CPU* c, MMU* m)
{
}

void RES3L(CPU* c, MMU* m)
{
}

void RES3HL(CPU* c, MMU* m)
{
}

void RES3A(CPU* c, MMU* m)
{
}

void RES4B(CPU* c, MMU* m)
{
}

void RES4C(CPU* c, MMU* m)
{
}

void RES4D(CPU* c, MMU* m)
{
}

void RES4E(CPU* c, MMU* m)
{
}

void RES4H(CPU* c, MMU* m)
{
}

void RES4L(CPU* c, MMU* m)
{
}

void RES4HL(CPU* c, MMU* m)
{
}

void RES4A(CPU* c, MMU* m)
{
}

void RES5B(CPU* c, MMU* m)
{
}

void RES5C(CPU* c, MMU* m)
{
}

void RES5D(CPU* c, MMU* m)
{
}

void RES5E(CPU* c, MMU* m)
{
}

void RES5H(CPU* c, MMU* m)
{
}

void RES5L(CPU* c, MMU* m)
{
}

void RES5HL(CPU* c, MMU* m)
{
}

void RES5A(CPU* c, MMU* m)
{
}

void RES6B(CPU* c, MMU* m)
{
}

void RES6C(CPU* c, MMU* m)
{
}

void RES6D(CPU* c, MMU* m)
{
}

void RES6E(CPU* c, MMU* m)
{
}

void RES6H(CPU* c, MMU* m)
{
}

void RES6L(CPU* c, MMU* m)
{
}

void RES6HL(CPU* c, MMU* m)
{
}

void RES6A(CPU* c, MMU* m)
{
}

void RES7B(CPU* c, MMU* m)
{
}

void RES7C(CPU* c, MMU* m)
{
}

void RES7D(CPU* c, MMU* m)
{
}

void RES7E(CPU* c, MMU* m)
{
}

void RES7H(CPU* c, MMU* m)
{
}

void RES7L(CPU* c, MMU* m)
{
}

void RES7HL(CPU* c, MMU* m)
{
}

void RES7A(CPU* c, MMU* m)
{
}

void SET0B(CPU* c, MMU* m)
{
}

void SET0C(CPU* c, MMU* m)
{
}

void SET0D(CPU* c, MMU* m)
{
}

void SET0E(CPU* c, MMU* m)
{
}

void SET0H(CPU* c, MMU* m)
{
}

void SET0L(CPU* c, MMU* m)
{
}

void SET0HL(CPU* c, MMU* m)
{
}

void SET0A(CPU* c, MMU* m)
{
}

void SET1B(CPU* c, MMU* m)
{
}

void SET1C(CPU* c, MMU* m)
{
}

void SET1D(CPU* c, MMU* m)
{
}

void SET1E(CPU* c, MMU* m)
{
}

void SET1H(CPU* c, MMU* m)
{
}

void SET1L(CPU* c, MMU* m)
{
}

void SET1HL(CPU* c, MMU* m)
{
}

void SET1A(CPU* c, MMU* m)
{
}

void SET2B(CPU* c, MMU* m)
{
}

void SET2C(CPU* c, MMU* m)
{
}

void SET2D(CPU* c, MMU* m)
{
}

void SET2E(CPU* c, MMU* m)
{
}

void SET2H(CPU* c, MMU* m)
{
}

void SET2L(CPU* c, MMU* m)
{
}

void SET2HL(CPU* c, MMU* m)
{
}

void SET2A(CPU* c, MMU* m)
{
}

void SET3B(CPU* c, MMU* m)
{
}

void SET3C(CPU* c, MMU* m)
{
}

void SET3D(CPU* c, MMU* m)
{
}

void SET3E(CPU* c, MMU* m)
{
}

void SET3H(CPU* c, MMU* m)
{
}

void SET3L(CPU* c, MMU* m)
{
}

void SET3HL(CPU* c, MMU* m)
{
}

void SET3A(CPU* c, MMU* m)
{
}

void SET4B(CPU* c, MMU* m)
{
}

void SET4C(CPU* c, MMU* m)
{
}

void SET4D(CPU* c, MMU* m)
{
}

void SET4E(CPU* c, MMU* m)
{
}

void SET4H(CPU* c, MMU* m)
{
}

void SET4L(CPU* c, MMU* m)
{
}

void SET4HL(CPU* c, MMU* m)
{
}

void SET4A(CPU* c, MMU* m)
{
}

void SET5B(CPU* c, MMU* m)
{
}

void SET5C(CPU* c, MMU* m)
{
}

void SET5D(CPU* c, MMU* m)
{
}

void SET5E(CPU* c, MMU* m)
{
}

void SET5H(CPU* c, MMU* m)
{
}

void SET5L(CPU* c, MMU* m)
{
}

void SET5HL(CPU* c, MMU* m)
{
}

void SET5A(CPU* c, MMU* m)
{
}

void SET6B(CPU* c, MMU* m)
{
}

void SET6C(CPU* c, MMU* m)
{
}

void SET6D(CPU* c, MMU* m)
{
}

void SET6E(CPU* c, MMU* m)
{
}

void SET6H(CPU* c, MMU* m)
{
}

void SET6L(CPU* c, MMU* m)
{
}

void SET6HL(CPU* c, MMU* m)
{
}

void SET6A(CPU* c, MMU* m)
{
}

void SET7B(CPU* c, MMU* m)
{
}

void SET7C(CPU* c, MMU* m)
{
}

void SET7D(CPU* c, MMU* m)
{
}

void SET7E(CPU* c, MMU* m)
{
}

void SET7H(CPU* c, MMU* m)
{
}

void SET7L(CPU* c, MMU* m)
{
}

void SET7HL(CPU* c, MMU* m)
{
}

void SET7A(CPU* c, MMU* m)
{
}

int main(){} // For compiling to check typos
