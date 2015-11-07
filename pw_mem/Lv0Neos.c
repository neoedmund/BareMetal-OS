
#include <inttypes.h>


#include "U.h"
#include "Lv0.h"
#include "Lv1.h"
#include "Lv0Neos.h"

#include "std.h"

	/*public*/
 void neoe_mem_Lv0Neos_Init(neoe_mem_Lv0Neos* self, uint64_t* e820) {
		self->parent->realObj = self;
		neoe_mem_Lv0_Init(self->parent, e820);
		self->parent->write4   = neoe_mem_Lv0Neos_write4;
		self->parent->read4   = neoe_mem_Lv0Neos_read4;
		self->parent->copyRange   = neoe_mem_Lv0Neos_copyRange;
	}


	/*@Override*/
 void neoe_mem_Lv0Neos_write4(neoe_mem_Lv0* _self, uint64_t addr, uint64_t v) {
//		neoe_mem_Lv0Neos* self = _self->realObj;
		// write memory
		//uint64_t* p = ;
		* ((uint64_t*) addr) = v;
	}

	/*private*/
 void neoe_mem_Lv0Neos_debug(neoe_mem_Lv0Neos* self, char* s) {
		printf("[D]%s\n" , s);
	}

	/*@Override*/
 uint64_t neoe_mem_Lv0Neos_read4(neoe_mem_Lv0* _self, uint64_t addr) {
//		neoe_mem_Lv0Neos* self =   _self->realObj;
		return  * ((uint64_t*) addr);
	}

void asm_copyrange(uint64_t to, uint64_t from, uint64_t size);
	/*@Override*/
 void neoe_mem_Lv0Neos_copyRange(neoe_mem_Lv0* _self, uint64_t from, uint64_t to, uint64_t size) {
		//neoe_mem_Lv0Neos* self =  (neoe_mem_Lv0Neos*) _self->realObj;

		size = neoe_mem_U_alignSize(size);
		printf("copyrange[%lx→%lx=%lx]\n", from, to, size);

		asm_copyrange(to, from, size);
		// slow one: for (uint64_t i = 0; i < size; i++) {* ((uint64_t*) (to+i)) = * ((uint64_t*) (from+i));}
	}

	/*private*/
 void neoe_mem_Lv0Neos_warn(neoe_mem_Lv0Neos* self, char* s) {
		printf("[W]%s\n" , s);
	}



void testPrintf() {
	printf("Hello, this is test-printf.\n");
	uint64_t v = 123;
	printf("just test for printf  s1=%s,\n\tx1=%lx,\n\t d1=%ld  \n", "a-string", 1L, v );
}
#define MAX_MEM_ARR 100

static uint64_t FreeMemArr[MAX_MEM_ARR*2];
static int FreeMemArrCnt;

typedef struct  {
	uint64_t startAddr;
	uint64_t len;
	uint32_t usable;
	uint32_t ext;
	uint64_t padding;
} e820slot;




static void initE820Arr () {
	FreeMemArrCnt=0;
	uint64_t mp = 0x4000L;
	e820slot* slot = (e820slot*) mp;
	//uint64_t totalLen = 0;
	uint64_t MinAddr = 0x100000; // we do not use memory below 1MB
	while(1) {
		if ( slot->startAddr < MinAddr) {
			// skip
		}else {
			 // add to FreeMem
			if (slot->usable==1){
				if (FreeMemArrCnt >= MAX_MEM_ARR -1) {
					printf("no more slot 1!\n");
					exit(3);
				}
				FreeMemArr[FreeMemArrCnt*2] = slot->startAddr;
				FreeMemArr[FreeMemArrCnt*2+1] = slot->len;
				printf("free mem No.%d,  %lx Len:%lx\n", FreeMemArrCnt, slot->startAddr, slot->len);
				FreeMemArrCnt++;
				//totalLen += slot->len;
			}

			if (slot->usable == 0) {
				FreeMemArr[FreeMemArrCnt*2] = 0L;
				FreeMemArr[FreeMemArrCnt*2+1] = 0L;
				printf("free mem total slot %d\n", FreeMemArrCnt);
				break;
			}
		}
		// next
		int size = sizeof(e820slot);
		if (size!=32) {//assert
			printf("assert fail, size=%ld, expected=32\n", size);
		}
		mp += size;
		slot = (e820slot*) mp;// or slot++
	}

}

static neoe_mem_Lv0Neos lv0os;
static neoe_mem_Lv0 lv0;
static neoe_mem_Lv1 lv1;



static char waitkey() {
	while(1){
		char c = getchar();
		if (c!=0) return c;
	}	
}


static  void neoe_mem_Test1_testAllocFrees(neoe_mem_Lv1* lv1) {
		printf("neoe_mem_Test1_testAllocFrees\n");
		uint64_t p1, p2, p3, p4;
		printf("a1=%lx\n", p1 = neoe_mem_Lv1_alloc(lv1, 0x1000L));
		printf("a2=%lx\n", p2 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("a3=%lx\n", p3 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("a4=%lx\n", p4 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("-----\n");waitkey();
		printf("free a4=%lx\n" , neoe_mem_Lv1_free(lv1, p4));
		printf("a4=%lx\n", p4 = neoe_mem_Lv1_alloc(lv1, 0x1000000L + 1));
		printf("-----\n");waitkey();
		printf("free a4=%lx\n" , neoe_mem_Lv1_free(lv1, p4));
		printf("a4=%lx\n", p4 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("-----\n");waitkey();
		printf("free a3=%lx\n" , neoe_mem_Lv1_free(lv1, p3));
		printf("-----\n");waitkey();
		printf("a3=%lx\n", p3 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("free a3=%lx\n" , neoe_mem_Lv1_free(lv1, p3));
		printf("free a2=%lx\n" , neoe_mem_Lv1_free(lv1, p2));
		printf("-----\n");waitkey();
		printf("a2=%lx\n", p2 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("a3=%lx\n", p3 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("free a2=%lx\n" , neoe_mem_Lv1_free(lv1, p2));
		printf("free a3=%lx\n" , neoe_mem_Lv1_free(lv1, p3));
		printf("-----\n");waitkey();
		printf("a2=%lx\n", p2 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("a3=%lx\n", p3 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("free a1=%lx\n" , neoe_mem_Lv1_free(lv1, p1));
		printf("free a3=%lx\n" , neoe_mem_Lv1_free(lv1, p3));
		printf("free a2=%lx\n" , neoe_mem_Lv1_free(lv1, p2));
		printf("-----\n");waitkey();
		printf("free a4=%lx\n" , neoe_mem_Lv1_free(lv1, p4));
}

 void neoe_mem_Test1_testEnlarge(neoe_mem_Lv1* lv1) {
		printf("neoe_mem_Test1_testEnlarge\n");
		uint64_t p1, p2, p3, p4, p5;
		printf("a1=%lx\n", p1 = neoe_mem_Lv1_alloc(lv1, 0x1000L));
		printf("a2=%lx\n", p2 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("a3=%lx\n", p3 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("a4=%lx\n", p4 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("-----\n");waitkey();
		printf("free a4=%lx\n" , neoe_mem_Lv1_free(lv1, p4));
		printf("a4=%lx\n", p4 = neoe_mem_Lv1_alloc(lv1, 0x1000000L + 1));
		printf("a4=%lx\n", p4 = neoe_mem_Lv1_enlarge(lv1, p4, (0x1000000L + 5)));
		printf("a4=%lx\n", p4 = neoe_mem_Lv1_enlarge(lv1, p4, (0x2000000L)));
		printf("a1=%lx\n", p1 = neoe_mem_Lv1_enlarge(lv1, p1, (0x1000L + 100)));
		printf("a5=%lx\n", p5 = neoe_mem_Lv1_alloc(lv1, 0x1000L));

	}

static void initLVs() {
	lv0os.parent = &lv0;
	neoe_mem_Lv0Neos_Init(&lv0os, FreeMemArr);

	uint64_t maxKernelSize = 10*1024*1024;
	// low 10MB(0xa00000), kernel start from 1MB, so 10-1=9MB, too big current for kernel.
	neoe_mem_Lv0_reserve(&lv0, 0, maxKernelSize);
	neoe_mem_Lv0_format(&lv0);

	neoe_mem_Lv1_Init(&lv1, &lv0);

	printf("start testcases\n");
	neoe_mem_Test1_testAllocFrees(&lv1);
	neoe_mem_Test1_testEnlarge(&lv1);
	printf("test finished.");
}
static void initMM () {
	initE820Arr();
	initLVs();
}



int mm_main() {
	static int  mm_inited = 1;
	if (mm_inited == 1) {
		mm_inited = 2;
	} else {
		printf("MM already inited, \ndo not call more.\n");
		testPrintf();
		return 1;
	}

	initMM();
	return 0;
}
