#include "cpu.h"
#define CYCLES(X) (c->c+=X)
#define WORD(X,Y) ((X<<8)|Y)

void NOP(CPU* c, MMU* m)
{
	CYCLES(4);
}

void LDBCnn(CPU* c, MMU* m)
{
}

void LDBCA(CPU* c, MMU* m)
{
}

void INCBC(CPU* c, MMU* m)
{
}

void INCB(CPU* c, MMU* m)
{
}

void DECB(CPU* c, MMU* m)
{
}

void LDBn(CPU* c, MMU* m)
{
	c->reg.B=m[c->PC++];
	CYCLES(8);
}

void RLCA(CPU* c, MMU* m)
{
}

void LDnnSP(CPU* c, MMU* m)
{
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

/* --- */

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

/* --- */

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

/* -- */

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

/* -- */

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

void LDBCB(CPU* c, MMU* m)
{
}

void LDCC(CPU* c, MMU* m)
{
}

void LDCD(CPU* c, MMU* m)
{
}

void LDCE(CPU* c, MMU* m)
{
}

void LDCH(CPU* c, MMU* m)
{
}

void LDCL(CPU* c, MMU* m)
{
}

void LDCHL(CPU* c, MMU* m)
{
}

void LDCA(CPU* c, MMU* m)
{
}

/* -- */

void LDDB(CPU* c, MMU* m)
{
}

void LDDC(CPU* c, MMU* m)
{
}

void LDDD(CPU* c, MMU* m)
{
}

void LDDE(CPU* c, MMU* m)
{
}

void LDDH(CPU* c, MMU* m)
{
}

void LDDL(CPU* c, MMU* m)
{
}

void LDDHL(CPU* c, MMU* m)
{
}

void LDDA(CPU* c, MMU* m)
{
}

void LDEB(CPU* c, MMU* m)
{
}

void LDEC(CPU* c, MMU* m)
{
}

void LDED(CPU* c, MMU* m)
{
}

void LDEE(CPU* c, MMU* m)
{
}

void LDEH(CPU* c, MMU* m)
{
}

void LDEL(CPU* c, MMU* m)
{
}

void LDEHL(CPU* c, MMU* m)
{
}

void LDEA(CPU* c, MMU* m)
{
}

/* -- */

void LDHB(CPU* c, MMU* m)
{
}

void LDHC(CPU* c, MMU* m)
{
}

void LDHD(CPU* c, MMU* m)
{
}

void LDHE(CPU* c, MMU* m)
{
}

void LDHH(CPU* c, MMU* m)
{
}

void LDHL(CPU* c, MMU* m)
{
}

void LDHHL(CPU* c, MMU* m)
{
}

void LDHA(CPU* c, MMU* m)
{
}

void LDLB(CPU* c, MMU* m)
{
}

void LDLC(CPU* c, MMU* m)
{
}

void LDLD(CPU* c, MMU* m)
{
}

void LDLE(CPU* c, MMU* m)
{
}

void LDLH(CPU* c, MMU* m)
{
}

void LDLL(CPU* c, MMU* m)
{
}

void LDLHL(CPU* c, MMU* m)
{
}

void LDLA(CPU* c, MMU* m)
{
}

/* -- */

void LDHLB(CPU* c, MMU* m)
{
}

void LDHLC(CPU* c, MMU* m)
{
}

void LDHLD(CPU* c, MMU* m)
{
}

void LDHLE(CPU* c, MMU* m)
{
}

void LDHLH(CPU* c, MMU* m)
{
}

void LDHLL(CPU* c, MMU* m)
{
}

void LDHLHL(CPU* c, MMU* m)
{
}

void LDHLA(CPU* c, MMU* m)
{
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

/* -- */

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

/* -- */

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

/* -- */

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

/* -- */

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

/* -- */

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

/* -- */

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

/* -- */

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

/* -- */

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
