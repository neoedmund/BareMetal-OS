// package neoe.mem;

// public class Test1 {
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "Test1.h"
#include "TestLv0.h"
#include "Lv0.h"
#include "Lv1.h"

	/*public*/
 /*static*/
 int main(int argc, char** argv) {
		// 0000200000 len:BFDA000
		// 0000100000000 len: 140000000
		printf("start...\n");

		uint64_t e820[] =   {
				// No.0
				0x0000200000L, 0xBFDA000L,
				// No.1
				0x0000100000000L, 0x140000000L,
				// end
				0, 0 };
		neoe_mem_Lv0 _lv0;
		neoe_mem_Lv0* lv0 = &_lv0; 
		neoe_mem_TestLv0 _localvar_x1;
		neoe_mem_TestLv0* testlv0 =  &_localvar_x1;
		testlv0->parent = lv0;
		neoe_mem_TestLv0_Init(&_localvar_x1, e820);
		neoe_mem_Lv0_reserve(lv0, 0, 0x0000300000L);
		neoe_mem_Lv0_format(testlv0->parent);
		printf("format OK\n");
		neoe_mem_Lv1 _localvar_x2;
		neoe_mem_Lv1* lv1 =  &_localvar_x2;  
		neoe_mem_Lv1_Init(&_localvar_x2, testlv0->parent);
		printf("start testcases\n");
		neoe_mem_Test1_testAllocFrees(lv1);
		neoe_mem_Test1_testEnlarge(lv1);
		
		
				
		printf("---badcase--\n");
		//printf("free 0=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, 0));
		printf("free bad=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, 100));

	}

	/*private*/
 /*static*/
 void neoe_mem_Test1_testEnlarge(neoe_mem_Lv1* lv1) {
		uint64_t p1, p2, p3, p4, p5;
		printf("a1=%" PRIx64 "\n", p1 = neoe_mem_Lv1_alloc(lv1, 0x1000L));
		printf("a2=%" PRIx64 "\n", p2 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("a3=%" PRIx64 "\n", p3 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("a4=%" PRIx64 "\n", p4 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("-----\n");
		printf("free a4=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p4));
		printf("a4=%" PRIx64 "\n", p4 = neoe_mem_Lv1_alloc(lv1, 0x1000000L + 1));
		printf("a4=%" PRIx64 "\n", p4 = neoe_mem_Lv1_enlarge(lv1, p4, (0x1000000L + 5)));
		printf("a4=%" PRIx64 "\n", p4 = neoe_mem_Lv1_enlarge(lv1, p4, (0x2000000L)));
		printf("a1=%" PRIx64 "\n", p1 = neoe_mem_Lv1_enlarge(lv1, p1, (0x1000L + 100)));
		printf("a5=%" PRIx64 "\n", p5 = neoe_mem_Lv1_alloc(lv1, 0x1000L));

	}

	/*private*/
 /*static*/
 void neoe_mem_Test1_testAllocFrees(neoe_mem_Lv1* lv1) {
		uint64_t p1, p2, p3, p4, p5;
		printf("a1=%" PRIx64 "\n", p1 = neoe_mem_Lv1_alloc(lv1, 0x1000L));
		printf("a2=%" PRIx64 "\n", p2 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("a3=%" PRIx64 "\n", p3 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("a4=%" PRIx64 "\n", p4 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("-----\n");
		printf("free a4=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p4));
		printf("a4=%" PRIx64 "\n", p4 = neoe_mem_Lv1_alloc(lv1, 0x1000000L + 1));

		printf("-----\n");
		printf("free a4=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p4));
		printf("a4=%" PRIx64 "\n", p4 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("-----\n");
		printf("free a3=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p3));
		printf("-----\n");
		printf("a3=%" PRIx64 "\n", p3 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("free a3=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p3));
		printf("free a2=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p2));
		printf("-----\n");
		printf("a2=%" PRIx64 "\n", p2 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("a3=%" PRIx64 "\n", p3 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("free a2=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p2));
		printf("free a3=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p3));
		printf("-----\n");
		printf("a2=%" PRIx64 "\n", p2 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("a3=%" PRIx64 "\n", p3 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("free a1=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p1));
		printf("free a3=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p3));
		printf("free a2=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p2));
		printf("-----\n");
		printf("a1=%" PRIx64 "\n", p1 = neoe_mem_Lv1_alloc(lv1, 0x1000000L));
		printf("a2=%" PRIx64 "\n", p2 = neoe_mem_Lv1_alloc(lv1, 0x9fd8f28L));
		printf("a3=%" PRIx64 "\n", p3 = neoe_mem_Lv1_alloc(lv1, 0x100000000L));
		printf("a4=%" PRIx64 "\n", p4 = neoe_mem_Lv1_alloc(lv1, 0x30000000L));
		printf("a5=%" PRIx64 "\n", p5 = neoe_mem_Lv1_alloc(lv1, 0x10000000L));
		
		printf("-----\n");
		printf("free a4=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p4));
		printf("free a3=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p3));
		printf("a5=%" PRIx64 "\n", p5 = neoe_mem_Lv1_alloc(lv1, 0x10000000L));
		printf("free a5=%" PRIx64 "\n" , neoe_mem_Lv1_free(lv1, p5));

	}
// }/*cls*/


