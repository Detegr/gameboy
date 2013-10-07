#ifndef TAPIBOYCPU
#define TAPIBOYCPU

#include <stdint.h>

#define ZERO		0x80 /* Z - Last math operation is zero or two values match when using CP */
#define SUBTRACT	0x40 /* N - Subtraction was performed in the last math instruction */
#define HALFCARRY	0x20 /* H - Carry occurred from the lower nibble in the last math operation */
#define CARRY		0x10 /* C - Carry occurred from the last math operation or if A is smaller value when executing CP */

#define ROM_START 0x100 /* Rom starting location in memory */

typedef uint8_t MMU;
typedef struct Z80_8BitRegisters
{
	uint8_t A;
	uint8_t B;
	uint8_t C;
	uint8_t D;
	uint8_t E;
	uint8_t H;
	uint8_t L;
	uint8_t F; /* Flags register */
} Z80Reg;

typedef struct Z80CPU
{
	Z80Reg reg;
	/* 16 bit registers */
	uint16_t SP; /* Stack Pointer */
	uint16_t PC; /* Program Counter, set to 0x100 on power up */

	uint8_t ime; /* Interrupt master enable */

	uint8_t halt; /* Is the CPU halted? */
	uint8_t stop; /* Is the CPU stopped? */
	unsigned int c; /* Total time in clock cycles (*4 of machine cycles) */
	MMU MMU[65536];
} CPU;

typedef void(*OpCode)(CPU*, MMU*);

void NOP(CPU* c, MMU* m);
void LDBCnn(CPU* c, MMU* m);
void LDBCA(CPU* c, MMU* m);
void INCBC(CPU* c, MMU* m);
void INCB(CPU* c, MMU* m);
void DECB(CPU* c, MMU* m);
void LDBn(CPU* c, MMU* m);
void RLCA(CPU* c, MMU* m);
void LDnnSP(CPU* c, MMU* m);
void ADDHLBC(CPU* c, MMU* m);
void LDABC(CPU* c, MMU* m);
void DECBC(CPU* c, MMU* m);
void INCC(CPU* c, MMU* m);
void DECC(CPU* c, MMU* m);
void LDCn(CPU* c, MMU* m);
void RRCA(CPU* c, MMU* m);

void STOP(CPU* c, MMU* m);
void LDDEnn(CPU* c, MMU* m);
void LDDEA(CPU* c, MMU* m);
void INCDE(CPU* c, MMU* m);
void INCD(CPU* c, MMU* m);
void DECD(CPU* c, MMU* m);
void LDDn(CPU* c, MMU* m);
void RLA(CPU* c, MMU* m);
void JRn(CPU* c, MMU* m);
void ADDHLDE(CPU* c, MMU* m);
void LDADE(CPU* c, MMU* m);
void DECDE(CPU* c, MMU* m);
void INCE(CPU* c, MMU* m);
void DECE(CPU* c, MMU* m);
void LDEn(CPU* c, MMU* m);
void RRA(CPU* c, MMU* m);

void JRNZn(CPU* c, MMU* m);
void LDHLnn(CPU* c, MMU* m);
void LDIHLA(CPU* c, MMU* m);
void INCHL(CPU* c, MMU* m);
void INCH(CPU* c, MMU* m);
void DECH(CPU* c, MMU* m);
void LDHn(CPU* c, MMU* m);
void DAA(CPU* c, MMU* m);
void JRZn(CPU* c, MMU* m);
void ADDHLHL(CPU* c, MMU* m);
void LDIAHL(CPU* c, MMU* m);
void DECHL(CPU* c, MMU* m);
void INCL(CPU* c, MMU* m);
void DECL(CPU* c, MMU* m);
void LDLn(CPU* c, MMU* m);
void CPL(CPU* c, MMU* m);


void JRNCn(CPU* c, MMU* m);
void LDSPnn(CPU* c, MMU* m);
void LDDHLA(CPU* c, MMU* m);
void INCSP(CPU* c, MMU* m);
void INCHL2(CPU* c, MMU* m);
void DECHL2(CPU* c, MMU* m);
void LDHLn(CPU* c, MMU* m);
void SCF(CPU* c, MMU* m);
void JRCn(CPU* c, MMU* m);
void ADDHLSP(CPU* c, MMU* m);
void LDDAHL(CPU* c, MMU* m);
void DECSP(CPU* c, MMU* m);
void INCA(CPU* c, MMU* m);
void DECA(CPU* c, MMU* m);
void LDAn(CPU* c, MMU* m);
void CCF(CPU* c, MMU* m);

void LDBB(CPU* c, MMU* m);
void LDBC(CPU* c, MMU* m);
void LDBD(CPU* c, MMU* m);
void LDBE(CPU* c, MMU* m);
void LDBH(CPU* c, MMU* m);
void LDBL(CPU* c, MMU* m);
void LDBHL(CPU* c, MMU* m);
void LDBA(CPU* c, MMU* m);
void LDCB(CPU* c, MMU* m);
void LDCC(CPU* c, MMU* m);
void LDCD(CPU* c, MMU* m);
void LDCE(CPU* c, MMU* m);
void LDCH(CPU* c, MMU* m);
void LDCL(CPU* c, MMU* m);
void LDCHL(CPU* c, MMU* m);
void LDCA(CPU* c, MMU* m);

void LDDB(CPU* c, MMU* m);
void LDDC(CPU* c, MMU* m);
void LDDD(CPU* c, MMU* m);
void LDDE(CPU* c, MMU* m);
void LDDH(CPU* c, MMU* m);
void LDDL(CPU* c, MMU* m);
void LDDHL(CPU* c, MMU* m);
void LDDA(CPU* c, MMU* m);
void LDEB(CPU* c, MMU* m);
void LDEC(CPU* c, MMU* m);
void LDED(CPU* c, MMU* m);
void LDEE(CPU* c, MMU* m);
void LDEH(CPU* c, MMU* m);
void LDEL(CPU* c, MMU* m);
void LDEHL(CPU* c, MMU* m);
void LDEA(CPU* c, MMU* m);

void LDHB(CPU* c, MMU* m);
void LDHC(CPU* c, MMU* m);
void LDHD(CPU* c, MMU* m);
void LDHE(CPU* c, MMU* m);
void LDHH(CPU* c, MMU* m);
void LDHL(CPU* c, MMU* m);
void LDHHL(CPU* c, MMU* m);
void LDHA(CPU* c, MMU* m);
void LDLB(CPU* c, MMU* m);
void LDLC(CPU* c, MMU* m);
void LDLD(CPU* c, MMU* m);
void LDLE(CPU* c, MMU* m);
void LDLH(CPU* c, MMU* m);
void LDLL(CPU* c, MMU* m);
void LDLHL(CPU* c, MMU* m);
void LDLA(CPU* c, MMU* m);

void LDHLB(CPU* c, MMU* m);
void LDHLC(CPU* c, MMU* m);
void LDHLD(CPU* c, MMU* m);
void LDHLE(CPU* c, MMU* m);
void LDHLH(CPU* c, MMU* m);
void LDHLL(CPU* c, MMU* m);
void HALT(CPU* c, MMU* m);
void LDHLA(CPU* c, MMU* m);
void LDAB(CPU* c, MMU* m);
void LDAC(CPU* c, MMU* m);
void LDAD(CPU* c, MMU* m);
void LDAE(CPU* c, MMU* m);
void LDAH(CPU* c, MMU* m);
void LDAL(CPU* c, MMU* m);
void LDAHL(CPU* c, MMU* m);
void LDAA(CPU* c, MMU* m);

void ADDAB(CPU* c, MMU* m);
void ADDAC(CPU* c, MMU* m);
void ADDAD(CPU* c, MMU* m);
void ADDAE(CPU* c, MMU* m);
void ADDAH(CPU* c, MMU* m);
void ADDAL(CPU* c, MMU* m);
void ADDAHL(CPU* c, MMU* m);
void ADDAA(CPU* c, MMU* m);
void ADCAB(CPU* c, MMU* m);
void ADCAC(CPU* c, MMU* m);
void ADCAD(CPU* c, MMU* m);
void ADCAE(CPU* c, MMU* m);
void ADCAH(CPU* c, MMU* m);
void ADCAL(CPU* c, MMU* m);
void ADCAHL(CPU* c, MMU* m);
void ADCAA(CPU* c, MMU* m);

void SUBAB(CPU* c, MMU* m);
void SUBAC(CPU* c, MMU* m);
void SUBAD(CPU* c, MMU* m);
void SUBAE(CPU* c, MMU* m);
void SUBAH(CPU* c, MMU* m);
void SUBAL(CPU* c, MMU* m);
void SUBAHL(CPU* c, MMU* m);
void SUBAA(CPU* c, MMU* m);
void SBCAB(CPU* c, MMU* m);
void SBCAC(CPU* c, MMU* m);
void SBCAD(CPU* c, MMU* m);
void SBCAE(CPU* c, MMU* m);
void SBCAH(CPU* c, MMU* m);
void SBCAL(CPU* c, MMU* m);
void SBCAHL(CPU* c, MMU* m);
void SBCAA(CPU* c, MMU* m);

void ANDB(CPU* c, MMU* m);
void ANDC(CPU* c, MMU* m);
void ANDD(CPU* c, MMU* m);
void ANDE(CPU* c, MMU* m);
void ANDH(CPU* c, MMU* m);
void ANDL(CPU* c, MMU* m);
void ANDHL(CPU* c, MMU* m);
void ANDA(CPU* c, MMU* m);
void XORB(CPU* c, MMU* m);
void XORC(CPU* c, MMU* m);
void XORD(CPU* c, MMU* m);
void XORE(CPU* c, MMU* m);
void XORH(CPU* c, MMU* m);
void XORL(CPU* c, MMU* m);
void XORHL(CPU* c, MMU* m);
void XORA(CPU* c, MMU* m);

void ORB(CPU* c, MMU* m);
void ORC(CPU* c, MMU* m);
void ORD(CPU* c, MMU* m);
void ORE(CPU* c, MMU* m);
void ORH(CPU* c, MMU* m);
void ORL(CPU* c, MMU* m);
void ORHL(CPU* c, MMU* m);
void ORA(CPU* c, MMU* m);
void CPB(CPU* c, MMU* m);
void CPC(CPU* c, MMU* m);
void CPD(CPU* c, MMU* m);
void CPE(CPU* c, MMU* m);
void CPH(CPU* c, MMU* m);
void CP_L(CPU* c, MMU* m);
void CPHL(CPU* c, MMU* m);
void CPA(CPU* c, MMU* m);

void RETNZ(CPU* c, MMU* m);
void POPBC(CPU* c, MMU* m);
void JPNZnn(CPU* c, MMU* m);
void JPnn(CPU* c, MMU* m);
void CALLNZnn(CPU* c, MMU* m);
void PUSHBC(CPU* c, MMU* m);
void ADDAn(CPU* c, MMU* m);
void RST0(CPU* c, MMU* m);
void RETZ(CPU* c, MMU* m);
void RET(CPU* c, MMU* m);
void JPZnn(CPU* c, MMU* m);
void Extops(CPU* c, MMU* m);
void CALLZnn(CPU* c, MMU* m);
void CALLnn(CPU* c, MMU* m);
void ADCAn(CPU* c, MMU* m);
void RST8(CPU* c, MMU* m);

void RETNC(CPU* c, MMU* m);
void POPDE(CPU* c, MMU* m);
void JPNCnn(CPU* c, MMU* m);
void CALLNCnn(CPU* c, MMU* m);
void PUSHDE(CPU* c, MMU* m);
void SUBAn(CPU* c, MMU* m);
void RST10(CPU* c, MMU* m);
void RETC(CPU* c, MMU* m);
void RETI(CPU* c, MMU* m);
void JPCnn(CPU* c, MMU* m);
void CALLCnn(CPU* c, MMU* m);
void SBCAn(CPU* c, MMU* m);
void RST18(CPU* c, MMU* m);

void LDHnA(CPU* c, MMU* m);
void POPHL(CPU* c, MMU* m);
void LDHCA(CPU* c, MMU* m);
void PUSHHL(CPU* c, MMU* m);
void ANDn(CPU* c, MMU* m);
void RST20(CPU* c, MMU* m);
void ADDSPd(CPU* c, MMU* m);
void JPHL(CPU* c, MMU* m);
void LDnnA(CPU* c, MMU* m);
void XORn(CPU* c, MMU* m);
void RST28(CPU* c, MMU* m);

void LDHAn(CPU* c, MMU* m);
void POPAF(CPU* c, MMU* m);
void DI(CPU* c, MMU* m);
void PUSHAF(CPU* c, MMU* m);
void ORn(CPU* c, MMU* m);
void RST30(CPU* c, MMU* m);
void LDHLSPd(CPU* c, MMU* m);
void LDSPHL(CPU* c, MMU* m);
void LDAnn(CPU* c, MMU* m);
void EI(CPU* c, MMU* m);
void CPn(CPU* c, MMU* m);
void RST38(CPU* c, MMU* m);

void RLCB(CPU* c, MMU* m);
void RLCC(CPU* c, MMU* m);
void RLCD(CPU* c, MMU* m);
void RLCE(CPU* c, MMU* m);
void RLCH(CPU* c, MMU* m);
void RLCL(CPU* c, MMU* m);
void RLCHL(CPU* c, MMU* m);
void RLCAext(CPU* c, MMU* m);
void RRCB(CPU* c, MMU* m);
void RRCC(CPU* c, MMU* m);
void RRCD(CPU* c, MMU* m);
void RRCE(CPU* c, MMU* m);
void RRCH(CPU* c, MMU* m);
void RRCL(CPU* c, MMU* m);
void RRCHL(CPU* c, MMU* m);
void RRCAext(CPU* c, MMU* m);

void RLB(CPU* c, MMU* m);
void RLC(CPU* c, MMU* m);
void RLD(CPU* c, MMU* m);
void RLE(CPU* c, MMU* m);
void RLH(CPU* c, MMU* m);
void RLL(CPU* c, MMU* m);
void RLHL(CPU* c, MMU* m);
void RLAext(CPU* c, MMU* m);
void RRB(CPU* c, MMU* m);
void RRC(CPU* c, MMU* m);
void RRD(CPU* c, MMU* m);
void RRE(CPU* c, MMU* m);
void RRH(CPU* c, MMU* m);
void RRL(CPU* c, MMU* m);
void RRHL(CPU* c, MMU* m);
void RRAext(CPU* c, MMU* m);

void SLAB(CPU* c, MMU* m);
void SLAC(CPU* c, MMU* m);
void SLAD(CPU* c, MMU* m);
void SLAE(CPU* c, MMU* m);
void SLAH(CPU* c, MMU* m);
void SLAL(CPU* c, MMU* m);
void SLAHL(CPU* c, MMU* m);
void SLAA(CPU* c, MMU* m);
void SRAB(CPU* c, MMU* m);
void SRAC(CPU* c, MMU* m);
void SRAD(CPU* c, MMU* m);
void SRAE(CPU* c, MMU* m);
void SRAH(CPU* c, MMU* m);
void SRAL(CPU* c, MMU* m);
void SRAHL(CPU* c, MMU* m);
void SRAA(CPU* c, MMU* m);

void SWAPB(CPU* c, MMU* m);
void SWAPC(CPU* c, MMU* m);
void SWAPD(CPU* c, MMU* m);
void SWAPE(CPU* c, MMU* m);
void SWAPH(CPU* c, MMU* m);
void SWAPL(CPU* c, MMU* m);
void SWAPHL(CPU* c, MMU* m);
void SWAPA(CPU* c, MMU* m);
void SRLB(CPU* c, MMU* m);
void SRLC(CPU* c, MMU* m);
void SRLD(CPU* c, MMU* m);
void SRLE(CPU* c, MMU* m);
void SRLH(CPU* c, MMU* m);
void SRLL(CPU* c, MMU* m);
void SRLHL(CPU* c, MMU* m);
void SRLA(CPU* c, MMU* m);

void BIT0B(CPU* c, MMU* m);
void BIT0C(CPU* c, MMU* m);
void BIT0D(CPU* c, MMU* m);
void BIT0E(CPU* c, MMU* m);
void BIT0H(CPU* c, MMU* m);
void BIT0L(CPU* c, MMU* m);
void BIT0HL(CPU* c, MMU* m);
void BIT0A(CPU* c, MMU* m);
void BIT1B(CPU* c, MMU* m);
void BIT1C(CPU* c, MMU* m);
void BIT1D(CPU* c, MMU* m);
void BIT1E(CPU* c, MMU* m);
void BIT1H(CPU* c, MMU* m);
void BIT1L(CPU* c, MMU* m);
void BIT1HL(CPU* c, MMU* m);
void BIT1A(CPU* c, MMU* m);

void BIT2B(CPU* c, MMU* m);
void BIT2C(CPU* c, MMU* m);
void BIT2D(CPU* c, MMU* m);
void BIT2E(CPU* c, MMU* m);
void BIT2H(CPU* c, MMU* m);
void BIT2L(CPU* c, MMU* m);
void BIT2HL(CPU* c, MMU* m);
void BIT2A(CPU* c, MMU* m);
void BIT3B(CPU* c, MMU* m);
void BIT3C(CPU* c, MMU* m);
void BIT3D(CPU* c, MMU* m);
void BIT3E(CPU* c, MMU* m);
void BIT3H(CPU* c, MMU* m);
void BIT3L(CPU* c, MMU* m);
void BIT3HL(CPU* c, MMU* m);
void BIT3A(CPU* c, MMU* m);

void BIT4B(CPU* c, MMU* m);
void BIT4C(CPU* c, MMU* m);
void BIT4D(CPU* c, MMU* m);
void BIT4E(CPU* c, MMU* m);
void BIT4H(CPU* c, MMU* m);
void BIT4L(CPU* c, MMU* m);
void BIT4HL(CPU* c, MMU* m);
void BIT4A(CPU* c, MMU* m);
void BIT5B(CPU* c, MMU* m);
void BIT5C(CPU* c, MMU* m);
void BIT5D(CPU* c, MMU* m);
void BIT5E(CPU* c, MMU* m);
void BIT5H(CPU* c, MMU* m);
void BIT5L(CPU* c, MMU* m);
void BIT5HL(CPU* c, MMU* m);
void BIT5A(CPU* c, MMU* m);

void BIT6B(CPU* c, MMU* m);
void BIT6C(CPU* c, MMU* m);
void BIT6D(CPU* c, MMU* m);
void BIT6E(CPU* c, MMU* m);
void BIT6H(CPU* c, MMU* m);
void BIT6L(CPU* c, MMU* m);
void BIT6HL(CPU* c, MMU* m);
void BIT6A(CPU* c, MMU* m);
void BIT7B(CPU* c, MMU* m);
void BIT7C(CPU* c, MMU* m);
void BIT7D(CPU* c, MMU* m);
void BIT7E(CPU* c, MMU* m);
void BIT7H(CPU* c, MMU* m);
void BIT7L(CPU* c, MMU* m);
void BIT7HL(CPU* c, MMU* m);
void BIT7A(CPU* c, MMU* m);

void RES0B(CPU* c, MMU* m);
void RES0C(CPU* c, MMU* m);
void RES0D(CPU* c, MMU* m);
void RES0E(CPU* c, MMU* m);
void RES0H(CPU* c, MMU* m);
void RES0L(CPU* c, MMU* m);
void RES0HL(CPU* c, MMU* m);
void RES0A(CPU* c, MMU* m);
void RES1B(CPU* c, MMU* m);
void RES1C(CPU* c, MMU* m);
void RES1D(CPU* c, MMU* m);
void RES1E(CPU* c, MMU* m);
void RES1H(CPU* c, MMU* m);
void RES1L(CPU* c, MMU* m);
void RES1HL(CPU* c, MMU* m);
void RES1A(CPU* c, MMU* m);

void RES2B(CPU* c, MMU* m);
void RES2C(CPU* c, MMU* m);
void RES2D(CPU* c, MMU* m);
void RES2E(CPU* c, MMU* m);
void RES2H(CPU* c, MMU* m);
void RES2L(CPU* c, MMU* m);
void RES2HL(CPU* c, MMU* m);
void RES2A(CPU* c, MMU* m);
void RES3B(CPU* c, MMU* m);
void RES3C(CPU* c, MMU* m);
void RES3D(CPU* c, MMU* m);
void RES3E(CPU* c, MMU* m);
void RES3H(CPU* c, MMU* m);
void RES3L(CPU* c, MMU* m);
void RES3HL(CPU* c, MMU* m);
void RES3A(CPU* c, MMU* m);

void RES4B(CPU* c, MMU* m);
void RES4C(CPU* c, MMU* m);
void RES4D(CPU* c, MMU* m);
void RES4E(CPU* c, MMU* m);
void RES4H(CPU* c, MMU* m);
void RES4L(CPU* c, MMU* m);
void RES4HL(CPU* c, MMU* m);
void RES4A(CPU* c, MMU* m);
void RES5B(CPU* c, MMU* m);
void RES5C(CPU* c, MMU* m);
void RES5D(CPU* c, MMU* m);
void RES5E(CPU* c, MMU* m);
void RES5H(CPU* c, MMU* m);
void RES5L(CPU* c, MMU* m);
void RES5HL(CPU* c, MMU* m);
void RES5A(CPU* c, MMU* m);

void RES6B(CPU* c, MMU* m);
void RES6C(CPU* c, MMU* m);
void RES6D(CPU* c, MMU* m);
void RES6E(CPU* c, MMU* m);
void RES6H(CPU* c, MMU* m);
void RES6L(CPU* c, MMU* m);
void RES6HL(CPU* c, MMU* m);
void RES6A(CPU* c, MMU* m);
void RES7B(CPU* c, MMU* m);
void RES7C(CPU* c, MMU* m);
void RES7D(CPU* c, MMU* m);
void RES7E(CPU* c, MMU* m);
void RES7H(CPU* c, MMU* m);
void RES7L(CPU* c, MMU* m);
void RES7HL(CPU* c, MMU* m);
void RES7A(CPU* c, MMU* m);

void SET0B(CPU* c, MMU* m);
void SET0C(CPU* c, MMU* m);
void SET0D(CPU* c, MMU* m);
void SET0E(CPU* c, MMU* m);
void SET0H(CPU* c, MMU* m);
void SET0L(CPU* c, MMU* m);
void SET0HL(CPU* c, MMU* m);
void SET0A(CPU* c, MMU* m);
void SET1B(CPU* c, MMU* m);
void SET1C(CPU* c, MMU* m);
void SET1D(CPU* c, MMU* m);
void SET1E(CPU* c, MMU* m);
void SET1H(CPU* c, MMU* m);
void SET1L(CPU* c, MMU* m);
void SET1HL(CPU* c, MMU* m);
void SET1A(CPU* c, MMU* m);

void SET2B(CPU* c, MMU* m);
void SET2C(CPU* c, MMU* m);
void SET2D(CPU* c, MMU* m);
void SET2E(CPU* c, MMU* m);
void SET2H(CPU* c, MMU* m);
void SET2L(CPU* c, MMU* m);
void SET2HL(CPU* c, MMU* m);
void SET2A(CPU* c, MMU* m);
void SET3B(CPU* c, MMU* m);
void SET3C(CPU* c, MMU* m);
void SET3D(CPU* c, MMU* m);
void SET3E(CPU* c, MMU* m);
void SET3H(CPU* c, MMU* m);
void SET3L(CPU* c, MMU* m);
void SET3HL(CPU* c, MMU* m);
void SET3A(CPU* c, MMU* m);

void SET4B(CPU* c, MMU* m);
void SET4C(CPU* c, MMU* m);
void SET4D(CPU* c, MMU* m);
void SET4E(CPU* c, MMU* m);
void SET4H(CPU* c, MMU* m);
void SET4L(CPU* c, MMU* m);
void SET4HL(CPU* c, MMU* m);
void SET4A(CPU* c, MMU* m);
void SET5B(CPU* c, MMU* m);
void SET5C(CPU* c, MMU* m);
void SET5D(CPU* c, MMU* m);
void SET5E(CPU* c, MMU* m);
void SET5H(CPU* c, MMU* m);
void SET5L(CPU* c, MMU* m);
void SET5HL(CPU* c, MMU* m);
void SET5A(CPU* c, MMU* m);

void SET6B(CPU* c, MMU* m);
void SET6C(CPU* c, MMU* m);
void SET6D(CPU* c, MMU* m);
void SET6E(CPU* c, MMU* m);
void SET6H(CPU* c, MMU* m);
void SET6L(CPU* c, MMU* m);
void SET6HL(CPU* c, MMU* m);
void SET6A(CPU* c, MMU* m);
void SET7B(CPU* c, MMU* m);
void SET7C(CPU* c, MMU* m);
void SET7D(CPU* c, MMU* m);
void SET7E(CPU* c, MMU* m);
void SET7H(CPU* c, MMU* m);
void SET7L(CPU* c, MMU* m);
void SET7HL(CPU* c, MMU* m);
void SET7A(CPU* c, MMU* m);

OpCode OpCodes[256]={
    &NOP, &LDBCnn, &LDBCA, &INCBC, &INCB, &DECB, &LDBn, &RLCA, &LDnnSP, &ADDHLBC, &LDABC, &DECBC, &INCC, &DECC, &LDCn, &RRCA,
    &STOP, &LDDEnn, &LDDEA, &INCDE, &INCD, &DECD, &LDDn, &RLA, &JRn, &ADDHLDE, &LDADE, &DECDE, &INCE, &DECE, &LDEn, &RRA,
    &JRNZn, &LDHLnn, &LDIHLA, &INCHL, &INCH, &DECH, &LDHn, &DAA, &JRZn, &ADDHLHL, &LDIAHL, &DECHL, &INCL, &DECL, &LDLn, &CPL,
    &JRNCn, &LDSPnn, &LDDHLA, &INCSP, &INCHL2, &DECHL2, &LDHLn, &SCF, &JRCn, &ADDHLSP, &LDDAHL, &DECSP, &INCA, &DECA, &LDAn, &CCF, 
    &LDBB, &LDBC, &LDBD, &LDBE, &LDBH, &LDBL, &LDBHL, &LDBA, &LDCB, &LDCC, &LDCD, &LDCE, &LDCH, &LDCL, &LDCHL, &LDCA, 
    &LDDB, &LDDC, &LDDD, &LDDE, &LDDH, &LDDL, &LDDHL, &LDDA, &LDEB, &LDEC, &LDED, &LDEE, &LDEH, &LDEL, &LDEHL, &LDEA, 
    &LDHB, &LDHC, &LDHD, &LDHE, &LDHH, &LDHL, &LDHHL, &LDHA, &LDLB, &LDLC, &LDLD, &LDLE, &LDLH, &LDLL, &LDLHL, &LDLA, 
    &LDHLB, &LDHLC, &LDHLD, &LDHLE, &LDHLH, &LDHLL, &HALT, &LDHLA, &LDAB, &LDAC, &LDAD, &LDAE, &LDAH, &LDAL, &LDAHL, &LDAA, 
    &ADDAB, &ADDAC, &ADDAD, &ADDAE, &ADDAH, &ADDAL, &ADDAHL, &ADDAA, &ADCAB, &ADCAC, &ADCAD, &ADCAE, &ADCAH, &ADCAL, &ADCAHL, &ADCAA, 
    &SUBAB, &SUBAC, &SUBAD, &SUBAE, &SUBAH, &SUBAL, &SUBAHL, &SUBAA, &SBCAB, &SBCAC, &SBCAD, &SBCAE, &SBCAH, &SBCAL, &SBCAHL, &SBCAA, 
    &ANDB, &ANDC, &ANDD, &ANDE, &ANDH, &ANDL, &ANDHL, &ANDA, &XORB, &XORC, &XORD, &XORE, &XORH, &XORL, &XORHL, &XORA, 
    &ORB, &ORC, &ORD, &ORE, &ORH, &ORL, &ORHL, &ORA, &CPB, &CPC, &CPD, &CPE, &CPH, &CP_L, &CPHL, &CPA, 
    &RETNZ, &POPBC, &JPNZnn, &JPnn, &CALLNZnn, &PUSHBC, &ADDAn, &RST0, &RETZ, &RET, &JPZnn, &Extops, &CALLZnn, &CALLnn, &ADCAn, &RST8, 
    &RETNC, &POPDE, &JPNCnn, 0, &CALLNCnn, &PUSHDE, &SUBAn, &RST10, &RETC, &RETI, &JPCnn, 0, &CALLCnn, 0, &SBCAn, &RST18, 
    &LDHnA, &POPHL, &LDHCA, 0, 0, &PUSHHL, &ANDn, &RST20, &ADDSPd, &JPHL, &LDnnA, 0, 0, 0, &XORn, &RST28, 
    &LDHAn, &POPAF, 0, &DI, 0, &PUSHAF, &ORn, &RST30, &LDHLSPd, &LDSPHL, &LDAnn, &EI, 0, 0, &CPn, &RST38
};

OpCode ExtOps[256]={
	&RLCB, &RLCC, &RLCD, &RLCE, &RLCH, &RLCL, &RLCHL, &RLCAext, &RRCB, &RRCC, &RRCD, &RRCE, &RRCH, &RRCL, &RRCHL, &RRCAext,
	&RLB, &RLC, &RLD, &RLE, &RLH, &RLL, &RLHL, &RLAext, &RRB, &RRC, &RRD, &RRE, &RRH, &RRL, &RRHL, &RRAext,
	&SLAB, &SLAC, &SLAD, &SLAE, &SLAH, &SLAL, &SLAHL, &SLAA, &SRAB, &SRAC, &SRAD, &SRAE, &SRAH, &SRAL, &SRAHL, &SRAA,
	&SWAPB, &SWAPC, &SWAPD, &SWAPE, &SWAPH, &SWAPL, &SWAPHL, &SWAPA, &SRLB, &SRLC, &SRLD, &SRLE, &SRLH, &SRLL, &SRLHL, &SRLA,
	&BIT0B, &BIT0C, &BIT0D, &BIT0E, &BIT0H, &BIT0L, &BIT0HL, &BIT0A, &BIT1B, &BIT1C, &BIT1D, &BIT1E, &BIT1H, &BIT1L, &BIT1HL, &BIT1A,
	&BIT2B, &BIT2C, &BIT2D, &BIT2E, &BIT2H, &BIT2L, &BIT2HL, &BIT2A, &BIT3B, &BIT3C, &BIT3D, &BIT3E, &BIT3H, &BIT3L, &BIT3HL, &BIT3A,
	&BIT4B, &BIT4C, &BIT4D, &BIT4E, &BIT4H, &BIT4L, &BIT4HL, &BIT4A, &BIT5B, &BIT5C, &BIT5D, &BIT5E, &BIT5H, &BIT5L, &BIT5HL, &BIT5A,
	&BIT6B, &BIT6C, &BIT6D, &BIT6E, &BIT6H, &BIT6L, &BIT6HL, &BIT6A, &BIT7B, &BIT7C, &BIT7D, &BIT7E, &BIT7H, &BIT7L, &BIT7HL, &BIT7A,
	&RES0B, &RES0C, &RES0D, &RES0E, &RES0H, &RES0L, &RES0HL, &RES0A, &RES1B, &RES1C, &RES1D, &RES1E, &RES1H, &RES1L, &RES1HL, &RES1A,
	&RES2B, &RES2C, &RES2D, &RES2E, &RES2H, &RES2L, &RES2HL, &RES2A, &RES3B, &RES3C, &RES3D, &RES3E, &RES3H, &RES3L, &RES3HL, &RES3A,
	&RES4B, &RES4C, &RES4D, &RES4E, &RES4H, &RES4L, &RES4HL, &RES4A, &RES5B, &RES5C, &RES5D, &RES5E, &RES5H, &RES5L, &RES5HL, &RES5A,
	&RES6B, &RES6C, &RES6D, &RES6E, &RES6H, &RES6L, &RES6HL, &RES6A, &RES7B, &RES7C, &RES7D, &RES7E, &RES7H, &RES7L, &RES7HL, &RES7A,
	&SET0B, &SET0C, &SET0D, &SET0E, &SET0H, &SET0L, &SET0HL, &SET0A, &SET1B, &SET1C, &SET1D, &SET1E, &SET1H, &SET1L, &SET1HL, &SET1A,
	&SET2B, &SET2C, &SET2D, &SET2E, &SET2H, &SET2L, &SET2HL, &SET2A, &SET3B, &SET3C, &SET3D, &SET3E, &SET3H, &SET3L, &SET3HL, &SET3A,
	&SET4B, &SET4C, &SET4D, &SET4E, &SET4H, &SET4L, &SET4HL, &SET4A, &SET5B, &SET5C, &SET5D, &SET5E, &SET5H, &SET5L, &SET5HL, &SET5A,
	&SET6B, &SET6C, &SET6D, &SET6E, &SET6H, &SET6L, &SET6HL, &SET6A, &SET7B, &SET7C, &SET7D, &SET7E, &SET7H, &SET7L, &SET7HL, &SET7A
};

#endif
