
#include <stdio.h>
#include <inttypes.h>

#include "U.h"
#include "Lv0.h"
#include "Lv0Neos.h"



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
		//TODO
	}

	/*private*/
 void neoe_mem_Lv0Neos_debug(neoe_mem_Lv0Neos* self, char* s) {
		printf("[D]%s\n" , s);
	}

	/*@Override*/
 uint64_t neoe_mem_Lv0Neos_read4(neoe_mem_Lv0* _self, uint64_t addr) {
//		neoe_mem_Lv0Neos* self =   _self->realObj;
		
		//TODO
		return 0;
	}

	/*@Override*/
 void neoe_mem_Lv0Neos_copyRange(neoe_mem_Lv0* _self, uint64_t from, uint64_t to, uint64_t size) {
		//neoe_mem_Lv0Neos* self =  (neoe_mem_Lv0Neos*) _self->realObj;
		
		//TODO
		size = neoe_mem_U_alignSize(size);
		//neoe_mem_Lv0Neos_debug(self, String.format("copyrange[%lx→%lx=%lx]", from, to, size));
		printf("copyrange[%lx→%lx=%lx]\n", from, to, size);
		for (uint64_t i = 0; i < size; i++) {
			uint64_t v = neoe_mem_Lv0Neos_read4(_self, from + i);
			neoe_mem_Lv0Neos_write4(_self, to + i, v);
		}
	}

	/*private*/
 void neoe_mem_Lv0Neos_warn(neoe_mem_Lv0Neos* self, char* s) {
		printf("[W]%s\n" , s);
	}



void testPrintf() {
	printf("Hello, this is test-printf.\n");
	uint64_t v = 123;
	printf("just test for printf  s1-%s ,x1=%lx, d1=%ld  \n", "a-string", 1L, v );
}
int mm_main() {
	//int whaterver[100];
	static int mm_inited = 0;
	if (mm_inited == 0) {
		mm_inited = 1;
	} else {
		printf("MM already inited, do not call more.\n");
		testPrintf();
		return 1;
	}
	testPrintf();
	return 0;
}
