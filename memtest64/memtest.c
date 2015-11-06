#include "libBareMetal.h"

// tested with testInt.c, enough or not!
typedef unsigned char    U8;
typedef unsigned short   U16;
typedef unsigned int     U32;
typedef unsigned long    U64;

// use rep stosq/scasq
#define fastmem

//----external----------
void os_dec_cursor();
U64  asm_writeU64(U64* loc, U64 cnt, U64 value);
U64  asm_verifyU64(U64* loc, U64 cnt, U64 value);
U64  asm_test(U64 a, U64 b, U64 c);
//--------------
void printe820();
void str_long2dec(U64 v, char* buf);
void memtest();
static char buf[100];
static int error =0 ;
//--------------------------------------------------

U32 memtest_main()
{
	//U32* memsize = (U32*) 0x5020;
	//str_long2dec( (*memsize), buf);
	//b_output("memsize MB:");
	//b_output(buf);
	//b_output("\n");
	memtest();
	return 0;
}

// filtered(from e820) free memory info
struct mempart {
	U64 startAddr;
	U64 len;
};

// 50  is enough?

#define MAX_MEM_ARR 100

static struct mempart FreeMemArr[MAX_MEM_ARR];
static int FreeMemArrCnt=0;

struct e820slot {
	U64 startAddr;
	U64 len;
	U32 usable;
	U32 ext;
	U64 padding;
};
static char HEX[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void str_itoa(U64 v, char* buf, int len, int radix) {
	int i=0;
	for (;i<len;i++){
		int x = v % radix;
		v = v / radix;
		buf[len-1-i]=HEX[x];
	}
	buf[i]=0;
}
void str_long2hex(U64 v, char* buf) {
	str_itoa(v, buf, 16, 16);
}
void str_int2hex(U32 v, char* buf) {
	str_itoa(v, buf, 8, 16);
}
char waitkey() {
	while(1){
		char c = b_input_key();
		if (c!=0) return c;
	}	
}
void str_long2dec(U64 v, char* buf) {
	int radix = 10;
	int i=0;
	while(1){
		int x = v % radix;
		v = v / radix;
		buf[i++]=HEX[x];
		if (v==0) break;
	}
	buf[i]=0;
	// reverse
	int len = i;
	for (i=0;i<len/2;i++){
		int t = buf[i];
		buf[i]=buf[len-i-1];
		buf[len-i-1] = t;
	}
}

void printe820slot(struct e820slot* slot) {
	//printf("%lx\t%ld\t%s\n", slot->startAddr, slot->len, slot->usable==1?"OK":"NG");
	str_long2hex(slot->startAddr, buf);
	b_output(buf);
	b_output("    ");
	str_long2hex(slot->len, buf);
	b_output(buf);
	b_output("    ");
	if ( slot->usable==1 ){
		b_output("O");
	} else {
		b_output("-");
	}
	b_output("\n");
}

void printe820(){
	FreeMemArrCnt=0;
	U64 mp = 0x0000000000004000L;
	struct e820slot* slot = (struct e820slot*) mp;
	U64 totalLen = 0;
	int i=0;
	while(1) {
		printe820slot(slot);
		i++;
		if (i>20) {
			b_output("press a key for more...");
			waitkey();
			i=0;
		}
		{ // add to FreeMem
			if (slot->usable==1){
				if (FreeMemArrCnt >= MAX_MEM_ARR) {
					b_output("no more slot 1!\n");
					while(1){};
				}
				FreeMemArr[FreeMemArrCnt].startAddr = slot->startAddr;
				FreeMemArr[FreeMemArrCnt].len = slot->len;
				FreeMemArrCnt++;
				totalLen += slot->len;
			}
		}
		int size = sizeof(struct e820slot);
		if (size!=32) {//assert
			b_output("assert fail, size=");
			str_long2hex(size, buf);
			b_output(buf);
			b_output("\n");
		}
		if (slot->usable == 0) {
			break;
		}
		mp += size;
		slot = (struct e820slot*) mp;// or slot++
	}

	b_output("Total size=");
	str_long2dec(totalLen, buf);
	b_output(buf);
	b_output("\n");
	if (i>0) {
		b_output("press a key to continue...");
		waitkey();
	}
}
void mempart_cutoff(struct mempart *target, struct mempart *mp) {
	U64 start = mp->startAddr;
	U64 stop = mp->startAddr + mp->len;
	U64 t0 = target->startAddr;
	U64 t1 = target->startAddr + target->len;
	// case 1/4
	if (start <= t0
		&& stop>t0
		&& stop<=t1 ) {
		target->startAddr = stop;
		target->len = t1 - stop;
		return;
	}
	// case 2/4
	if (start>=t0
		&& start<t1
		&& stop >= t1) {
		target->len = start-t0;
		return;
	}
	// case 3/4
	if (start<=t0 && stop>=t1) {
		target->len = 0;
		return;
	}
	// case 4/4
	if (start>t0 && stop<t1) {

		target->len = start-t0;
		// add one record
		if (FreeMemArrCnt >= MAX_MEM_ARR) {
			str_long2dec(FreeMemArrCnt, buf);
			b_output(buf);
			b_output("  ");
			b_output("no more slot 2!\n");
			while(1){};
		}
		FreeMemArr[FreeMemArrCnt].startAddr = stop;
		FreeMemArr[FreeMemArrCnt].len = t1-stop;
		FreeMemArrCnt++;
		return;
	}
}
void mempart_sub(struct mempart *mp) {
	for (int i=0 ; i< FreeMemArrCnt; i++) {
		struct mempart *target = & FreeMemArr[i];
		mempart_cutoff(target, mp);
	}
}
void printTestingMempart(){
	int i=0;
	int j=0;

	for (i=0;i<FreeMemArrCnt;i++) {
		struct mempart *mp = & FreeMemArr[i];
		if (mp->len==0) continue;
		j++;
		b_output("No.");
		str_long2dec(j, buf);
		b_output(buf);
		b_output("    ");
		str_long2hex(mp->startAddr, buf);
		b_output(buf);
		b_output("    ");
		str_long2hex(mp->len, buf);
		b_output(buf);
		b_output("\n");
	}
}
void writeUnit(U64* unit, U64 total, U64 v) {
	U64 percent = total /100;
//	if (percent%8!=0) {
//		b_output("adjust unit ");
//		str_long2dec(percent, buf);
//		b_output(buf);
//		b_output(" to ");
//		percent = percent/8*8;
//		str_long2dec(percent, buf);
//		b_output(buf);
//		b_output("\n");
//	}
	U64 cnt =0 ;
	int j=0;
#ifdef fastmem
	while(1){
		if (cnt+percent>total) {
			percent = total-cnt;
		}
		asm_writeU64(unit, percent, v);
		unit += percent;
		cnt += percent;
		if (cnt>=total) break;
		j= 100*cnt/total;
		if (j>=100) j=99;
		str_long2dec(j, buf);
		b_output(buf);
		if(j<10) b_output(" %");
		else b_output("%");
		os_dec_cursor();
		os_dec_cursor();
		os_dec_cursor();
	}
#else
	for (U64 i=0;i<total;i++) {
		*unit = v;
		unit++;
		if (i%percent==0){
			int j = i/percent;
		if (j==100) j=99;
		str_long2dec(j, buf);
		b_output(buf);
		if(j<10) b_output(" %");
		else b_output("%");
		os_dec_cursor();
		os_dec_cursor();
		os_dec_cursor();
		}
	}
#endif
	b_output("OK ");
}



void verifyUnit(U64* unit, U64 total, U64 v) {
	U64 percent = total /100;
//	if (percent%8!=0) {
//		b_output("adjust unit ");
//		str_long2dec(percent, buf);
//		b_output(buf);
//		b_output(" to ");
//		percent = percent/8*8;
//		str_long2dec(percent, buf);
//		b_output(buf);
//		b_output("\n");
//	}
	U64 cnt =0 ;
	int j=0;
#ifdef fastmem
	while(1) {
		if (cnt+percent>total) {
			percent = total-cnt;
		}
		int ret = asm_verifyU64(unit, percent, v);
		if (ret!=0) {
			error++;
			b_output("Error:");
			U64 loc = ((U64)unit) + sizeof(U64) *( percent - ret);
			str_long2hex(loc, buf);
			b_output(buf);
			b_output("\n");
		}
		unit += percent;
		cnt += percent;
		if (cnt>=total) break;
		j= 100*cnt/total;
		if (j>=100) j=99;
		str_long2dec(j, buf);
		b_output(buf);
		if(j<10) b_output(" %");
		else b_output("%");
		os_dec_cursor();
		os_dec_cursor();
		os_dec_cursor();
	}
#else
	for (U64 i=0;i<total;i++) {
		if ( *unit != v ){
			error++;
			b_output("Error:");
			str_long2hex( (U64) unit, buf);
			b_output(buf);
			b_output("\n");
		}
		unit++;
		if (i%percent==0){
			int j = i/percent;
			if (j==100) j=99;
			str_long2dec(j, buf);
			b_output(buf);
			if(j<10) b_output(" %");
			else b_output("%");
			os_dec_cursor();
			os_dec_cursor();
			os_dec_cursor();
		}
	}
#endif
	b_output("OK ");
}
void doTestMem(struct mempart *mp){

	U64 total = mp->len / 8; //8byte each
	int step =1;

	U64  v = -1;
	U64* unit = (U64*) mp->startAddr;
	b_output("1/4 write FF ");
	writeUnit(unit, total, v);

	step = 2;
	unit = (U64*) mp->startAddr;
	b_output(" 2/4 verify FF ");
	verifyUnit(unit, total, v);

	step=3;
	v=0;
	unit = (U64*) mp->startAddr;
	b_output(" 3/4 write 00 ");
	writeUnit(unit, total, v);

	step=4;
	unit = (U64*) mp->startAddr;
	b_output(" 4/4 verify 00 ");
	verifyUnit(unit, total, v);
	b_output(" OK \n");
}
void testFreeMem() {
	//asm_disable_cpu_cache();
	printTestingMempart();
	int i=0;
	int j=0;
	for (i=0;i<FreeMemArrCnt;i++) {
		struct mempart *mp = & FreeMemArr[i];
		if (mp->len==0) continue;
		j++;
		b_output("Test No.");
		str_long2dec(j, buf);
		b_output(buf);
		b_output("    ");
		str_long2hex(mp->startAddr, buf);
		b_output(buf);
		b_output("    ");
		str_long2hex(mp->len, buf);
		b_output(buf);
		b_output(" (");
		str_long2dec(mp->len, buf);
		b_output(buf);
		b_output(")\n");
		doTestMem(mp);
	}
	//asm_enable_cpu_cache();
}
void memtest() {
	b_output("memtest start\n");
	error = 0;
	printe820();
	struct mempart under2M;
	under2M.startAddr= 0;
	under2M.len= 0x200000;
	mempart_sub(&under2M);
	testFreeMem();
	b_output("memtest finish.\n");
	if (error>0) {
		b_output("Error count=");
		str_long2dec(error, buf);
		b_output(buf);
		b_output("\n");
	}
}

