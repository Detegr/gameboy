#include "cpu.h"
#define CYCLES(X) (c->c+=X)
#define WORD(X,Y) ((X<<8)|Y)

/* 0 */

void NOP(CPU* c, MMU* m)
{
	CYCLES(4);
}

void LDBCnn(CPU* c, MMU* m)
{
	/* Immediate values are in little-endian order, */
	/* so least significant bits are loaded first. */
	c->reg.C=m[c->PC++];
	c->reg.B=m[c->PC++];
	CYCLES(12);
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
	c->reg.B++;
	if(!(c->reg.B & 0x0F)) // Lower nibble overflown
	{
		c->reg.F |= HALFCARRY;
	}
	if(!c->reg.B) c->reg.F |= ZERO;
	c->reg.F &= ~SUBTRACT; /* Reset N flag */
	CYCLES(4);
}

void DECB(CPU* c, MMU* m)
{
	c->reg.B--;
	c->reg.F |= SUBTRACT;
	/* H - Set if no borrow from bit 4, aka no overflow on lower nibble */
	if((c->reg.B & 0x0F) != 0x0F) // Lower nibble not overflown
	{
		c->reg.F |= HALFCARRY;
	}
	if(!c->reg.B) c->reg.F |= ZERO;
	CYCLES(4);
}

void LDBn(CPU* c, MMU* m)
{
	c->reg.B=m[c->PC++];
	CYCLES(8);
}

void RLCA(CPU* c, MMU* m)
{
	/* Rotate register left */
	c->reg.F &= ~CARRY; // Reset carry flag
	if(c->reg.A & 0x80)
	{
		c->reg.F |= CARRY; // Contains old bit 7 data
	}
	c->reg.A <<= 1;
	if(c->reg.F & CARRY)
	{
		c->reg.A |= 0x01;
	}
	CYCLES(4);
}

void LDnnSP(CPU* c, MMU* m)
{
	uint16_t addr;
	(&addr)[0] = m[c->PC++];
	(&addr)[1] = m[c->PC++];
	m[addr]=c->SP;
	CYCLES(20);
}

void ADDHLBC(CPU* c, MMU* m)
{
}

void LDABC(CPU* c, MMU* m)
{
}

void DECBC(CPU* c, MMU* m)
{
}

void INCC(CPU* c, MMU* m)
{
}

void DECC(CPU* c, MMU* m)
{
}

void LDCn(CPU* c, MMU* m)
{
	c->reg.C=m[c->PC++];
	CYCLES(8);
}

void RRCA(CPU* c, MMU* m)
{
}

/* 1 */

void STOP(CPU* c, MMU* m)
{
}

void LDDEnn(CPU* c, MMU* m)
{
}

void LDDEA(CPU* c, MMU* m)
{
}

void INCDE(CPU* c, MMU* m)
{
}

void INCD(CPU* c, MMU* m)
{
}

void DECD(CPU* c, MMU* m)
{
}

void LDDn(CPU* c, MMU* m)
{
	c->reg.D=m[c->PC++];
	CYCLES(8);
}

void RLA(CPU* c, MMU* m)
{
}

void JRn(CPU* c, MMU* m)
{
}

void ADDHLDE(CPU* c, MMU* m)
{
}

void LDADE(CPU* c, MMU* m)
{
}

void DECDE(CPU* c, MMU* m)
{
}

void INCE(CPU* c, MMU* m)
{
}

void DECE(CPU* c, MMU* m)
{
}

void LDEn(CPU* c, MMU* m)
{
	c->reg.E=m[c->PC++];
	CYCLES(8);
}

void RRA(CPU* c, MMU* m)
{
}

/* 2 */

void JRNZn(CPU* c, MMU* m)
{
}

void LDHLnn(CPU* c, MMU* m)
{
}

void LDIHLA(CPU* c, MMU* m)
{
}

void INCHL(CPU* c, MMU* m)
{
}

void INCH(CPU* c, MMU* m)
{
}

void DECH(CPU* c, MMU* m)
{
}

void LDHn(CPU* c, MMU* m)
{
	c->reg.H=m[c->PC++];
	CYCLES(8);
}

void DAA(CPU* c, MMU* m)
{
}

void JRZn(CPU* c, MMU* m)
{
}

void ADDHLHL(CPU* c, MMU* m)
{
}

void LDIAHL(CPU* c, MMU* m)
{
}

void DECHL(CPU* c, MMU* m)
{
}

void INCL(CPU* c, MMU* m)
{
}

void DECL(CPU* c, MMU* m)
{
}

void LDLn(CPU* c, MMU* m)
{
	c->reg.L=m[c->PC++];
	CYCLES(8);
}

void CPL(CPU* c, MMU* m)
{
}

/* 3 */

void JRNCn(CPU* c, MMU* m)
{
}

void LDSPnn(CPU* c, MMU* m)
{
}

void LDDHLA(CPU* c, MMU* m)
{
}

void INCSP(CPU* c, MMU* m)
{
}

void INCHL2(CPU* c, MMU* m)
{
}

void DECHL2(CPU* c, MMU* m)
{
}

void LDHLn(CPU* c, MMU* m)
{
}

void SCF(CPU* c, MMU* m)
{
}

void JRCn(CPU* c, MMU* m)
{
}

void ADDHLSP(CPU* c, MMU* m)
{
}

void LDDAHL(CPU* c, MMU* m)
{
}

void DECSP(CPU* c, MMU* m)
{
}

void INCA(CPU* c, MMU* m)
{
}

void DECA(CPU* c, MMU* m)
{
}

void LDAn(CPU* c, MMU* m)
{
}

void CCF(CPU* c, MMU* m)
{
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
}

void ADDAC(CPU* c, MMU* m)
{
}

void ADDAD(CPU* c, MMU* m)
{
}

void ADDAE(CPU* c, MMU* m)
{
}

void ADDAH(CPU* c, MMU* m)
{
}

void ADDAL(CPU* c, MMU* m)
{
}

void ADDAHL(CPU* c, MMU* m)
{
}

void ADDAA(CPU* c, MMU* m)
{
}

void ADCAB(CPU* c, MMU* m)
{
}

void ADCAC(CPU* c, MMU* m)
{
}

void ADCAD(CPU* c, MMU* m)
{
}

void ADCAE(CPU* c, MMU* m)
{
}

void ADCAH(CPU* c, MMU* m)
{
}

void ADCAL(CPU* c, MMU* m)
{
}

void ADCAHL(CPU* c, MMU* m)
{
}

void ADCAA(CPU* c, MMU* m)
{
}

/* 9 */

void SUBAB(CPU* c, MMU* m)
{
}

void SUBAC(CPU* c, MMU* m)
{
}

void SUBAD(CPU* c, MMU* m)
{
}

void SUBAE(CPU* c, MMU* m)
{
}

void SUBAH(CPU* c, MMU* m)
{
}

void SUBAL(CPU* c, MMU* m)
{
}

void SUBAHL(CPU* c, MMU* m)
{
}

void SUBAA(CPU* c, MMU* m)
{
}

void SBCAB(CPU* c, MMU* m)
{
}

void SBCAC(CPU* c, MMU* m)
{
}

void SBCAD(CPU* c, MMU* m)
{
}

void SBCAE(CPU* c, MMU* m)
{
}

void SBCAH(CPU* c, MMU* m)
{
}

void SBCAL(CPU* c, MMU* m)
{
}

void SBCAHL(CPU* c, MMU* m)
{
}

void SBCAA(CPU* c, MMU* m)
{
}

/* A */

void ANDB(CPU* c, MMU* m)
{
}

void ANDC(CPU* c, MMU* m)
{
}

void ANDD(CPU* c, MMU* m)
{
}

void ANDE(CPU* c, MMU* m)
{
}

void ANDH(CPU* c, MMU* m)
{
}

void ANDL(CPU* c, MMU* m)
{
}

void ANDHL(CPU* c, MMU* m)
{
}

void ANDA(CPU* c, MMU* m)
{
}

void XORB(CPU* c, MMU* m)
{
}

void XORC(CPU* c, MMU* m)
{
}

void XORD(CPU* c, MMU* m)
{
}

void XORE(CPU* c, MMU* m)
{
}

void XORH(CPU* c, MMU* m)
{
}

void XORL(CPU* c, MMU* m)
{
}

void XORHL(CPU* c, MMU* m)
{
}

void XORA(CPU* c, MMU* m)
{
}

/* B */

void ORB(CPU* c, MMU* m)
{
}

void ORC(CPU* c, MMU* m)
{
}

void ORD(CPU* c, MMU* m)
{
}

void ORE(CPU* c, MMU* m)
{
}

void ORH(CPU* c, MMU* m)
{
}

void ORL(CPU* c, MMU* m)
{
}

void ORHL(CPU* c, MMU* m)
{
}

void ORA(CPU* c, MMU* m)
{
}

void CPB(CPU* c, MMU* m)
{
}

void CPC(CPU* c, MMU* m)
{
}

void CPD(CPU* c, MMU* m)
{
}

void CPE(CPU* c, MMU* m)
{
}

void CPH(CPU* c, MMU* m)
{
}

void CP_L(CPU* c, MMU* m)
{
}

void CPHL(CPU* c, MMU* m)
{
}

void CPA(CPU* c, MMU* m)
{
}

/* C */

void ETNZ(CPU* c, MMU* m)
{
}

void POPBC(CPU* c, MMU* m)
{
}

void JPNZnn(CPU* c, MMU* m)
{
}

void JPnn(CPU* c, MMU* m)
{
}

void CALLNZnn(CPU* c, MMU* m)
{
}

void PUSHBC(CPU* c, MMU* m)
{
}

void ADDAn(CPU* c, MMU* m)
{
}

void RST0(CPU* c, MMU* m)
{
}

void RETZ(CPU* c, MMU* m)
{
}

void RET(CPU* c, MMU* m)
{
}

void JPZnn(CPU* c, MMU* m)
{
}

void Extops(CPU* c, MMU* m)
{
}

void CALLZnn(CPU* c, MMU* m)
{
}

void CALLnn(CPU* c, MMU* m)
{
}

void ADCAn(CPU* c, MMU* m)
{
}

void RST8(CPU* c, MMU* m)
{
}

/* D */

void RETNC(CPU* c, MMU* m)
{
}

void POPDE(CPU* c, MMU* m)
{
}

void JPNCnn(CPU* c, MMU* m)
{
}

void CALLNCnn(CPU* c, MMU* m)
{
}

void PUSHDE(CPU* c, MMU* m)
{
}

void SUBAn(CPU* c, MMU* m)
{
}

void RST10(CPU* c, MMU* m)
{
}

void RETC(CPU* c, MMU* m)
{
}

void RETI(CPU* c, MMU* m)
{
}

void JPCnn(CPU* c, MMU* m)
{
}

void CALLCnn(CPU* c, MMU* m)
{
}

void SBCAn(CPU* c, MMU* m)
{
}

void RST18(CPU* c, MMU* m)
{
}

/* E */

void LDHnA(CPU* c, MMU* m)
{
}

void POPHL(CPU* c, MMU* m)
{
}

void LDHCA(CPU* c, MMU* m)
{
}

void PUSHHL(CPU* c, MMU* m)
{
}

void ANDn(CPU* c, MMU* m)
{
}

void RST20(CPU* c, MMU* m)
{
}

void ADDSPd(CPU* c, MMU* m)
{
}

void JPHL(CPU* c, MMU* m)
{
}

void LDnnA(CPU* c, MMU* m)
{
}

void XORn(CPU* c, MMU* m)
{
}

void RST28(CPU* c, MMU* m)
{
}

/* F */

void LDHAn(CPU* c, MMU* m)
{
}

void POPAF(CPU* c, MMU* m)
{
}

void DI(CPU* c, MMU* m)
{
}

void PUSHAF(CPU* c, MMU* m)
{
}

void ORn(CPU* c, MMU* m)
{
}

void RST30(CPU* c, MMU* m)
{
}

void LDHLSPd(CPU* c, MMU* m)
{
}

void LDSPHL(CPU* c, MMU* m)
{
}

void LDAnn(CPU* c, MMU* m)
{
}

void EI(CPU* c, MMU* m)
{
}

void CPn(CPU* c, MMU* m)
{
}

void RST38(CPU* c, MMU* m)
{
}

int main(){} // For compiling to check typos
