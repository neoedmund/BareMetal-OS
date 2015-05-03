#include "libBareMetal.h"

// tested with testInt.c, enough or not!
typedef char    U8;
typedef short   U16;
typedef int     U32;
typedef long    U64;

// use rep stosq/scasq
#define fastmem

//----external----------
void os_dec_cursor();
U64  asm_writeU64(U64* loc, U64 cnt, U64 value);
U64  asm_verifyU64(U64* loc, U64 cnt, U64 value);
U64  asm_test(U64 a, U64 b, U64 c);
//--------------
void printe820();
void str_long2dec(U32 v, char* buf);
void memtest();
static char buf[100];
static int error =0 ;
//--------------------------------------------------

U32 memtest_main()
{
	b_output("(My) Real OS from here(C)!\n");
	printe820();

	U32* memsize = (U32*) 0x5020;
	char buf[100];
	str_long2dec( (*memsize), buf);
	b_output("memsize in MB:");
	b_output(buf);
	b_output("\n");
	memtest();
	U64 ret = asm_test(0x140000000,2,3);
	str_long2dec(ret, buf);
	b_output("ret=");
	b_output(buf);
	b_output("\n");
	return 0;
}

// filtered(from e820) free memory info
struct mempart {
	U64 startAddr;
	U64 len;
};

// 100 is enough
struct mempart FreeMemArr[100];
int FreeMemArrCnt=0;

struct e820slot {
	U64 startAddr;
	U64 len;
	U32 usable;
	U32 ext;
	U64 padding;
};
char HEX[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
void str_itoa(U64 v, char* buf, int len, int radix) {
	U8 x;
	int i=0;
	for (;i<len;i++){
		x = v % radix;
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
void str_long2dec(U32 v, char* buf) {
	U8 x;
	int radix = 10;
	int i=0;
	while(1){
		x = v % radix;
		v = v / radix;
		buf[i++]=HEX[x];
		if (v==0) break;
	}
	buf[i]=0;
	// reverse
	int len = i;
	int t;
	for (i=0;i<len/2;i++){
		t = buf[i];
		buf[i]=buf[len-i-1];
		buf[len-i-1] = t;
	}
}

void printe820slot(struct e820slot* slot) {
	//printf("%lx\t%ld\t%s\n", slot->startAddr, slot->len, slot->usable==1?"OK":"NG");
	char buf[100];
	str_long2hex(slot->startAddr, buf);
	b_output(buf);
	b_output("    ");
	str_long2hex(slot->len, buf);
	b_output(buf);
	b_output("    ");
	if ( slot->usable==1 ){
		b_output("OK");
	} else {
		b_output("NG");
	}
	b_output("\n");
}

void printe820(){
	FreeMemArrCnt=0;
	U64 mp = 0x0000000000004000L;
	struct e820slot* slot = (struct e820slot*) mp;
	while(1) {
		printe820slot(slot);
		{ // add to FreeMem
			if (slot->usable==1){
				FreeMemArr[FreeMemArrCnt].startAddr = slot->startAddr;
				FreeMemArr[FreeMemArrCnt].len = slot->len;
				FreeMemArrCnt++;
			}
		}
		int size = sizeof(struct e820slot);
		if (size!=32) {//assert
			char buf[100];
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
		j++;
		if (j==100) j=99;
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
	b_output("OK \n");
}



void verifyUnit(U64* unit, U64 total, U64 v) {
	U64 percent = total /100;
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
		j++;
		if (j==100) j=99;
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
	b_output("OK \n");
}
void doTestMem(struct mempart *mp){

	U64 total = mp->len / 8; //8byte each
	int step =1;

	U64  v = -1;
	U64* unit = (U64*) mp->startAddr;
	b_output("Step 1/4 write FF ");
	writeUnit(unit, total, v);

	step = 2;
	unit = (U64*) mp->startAddr;
	b_output("Step 2/4 verify FF ");
	verifyUnit(unit, total, v);

	step=3;
	v=0;
	unit = (U64*) mp->startAddr;
	b_output("Step 3/4 write 00 ");
	writeUnit(unit, total, v);

	step=4;
	unit = (U64*) mp->startAddr;
	b_output("Step 4/4 verify 00 ");
	verifyUnit(unit, total, v);

	b_output("OK \n");
}
void testFreeMem() {
	//asm_disable_cpu_cache();
	//TODO
	printTestingMempart();
	int i=0;
	int j=0;
	for (i=0;i<FreeMemArrCnt;i++) { 
		struct mempart *mp = & FreeMemArr[i];
		if (mp->len==0) continue;
		j++;
		b_output("Testing No.");
		str_long2dec(j, buf);
		b_output(buf);
		b_output("    ");
		str_long2hex(mp->startAddr, buf);
		b_output(buf);
		b_output("    ");
		str_long2hex(mp->len, buf);
		b_output(buf);
		b_output("\n");
		doTestMem(mp);
	}
	//asm_enable_cpu_cache();
}
void memtest() {
	b_output("memtest start...\n");
	struct mempart under2M;
	under2M.startAddr= 0;
	under2M.len= 0x200000;
	mempart_sub(&under2M);
	testFreeMem();
	b_output("memtest finish.\n");
}



