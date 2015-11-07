// package neoe.mem;

// public class Envelop {
#include <stdio.h>
#include <stdlib.h> //exit()
#include <inttypes.h>
#include "Envelop.h"
#include "DummyLv0.h"
	/*public*/
 /*static*/
 uint64_t neoe_mem_Envelop_fromUserAddress(  uint64_t addr) {
		return addr - 56 - neoe_mem_Envelop_patch;
	}

	/*public*/
 /*static*/
 uint64_t neoe_mem_Envelop_toUserAddress(   uint64_t start) {
		return start + 56 + neoe_mem_Envelop_patch;
	}

 

	/*public*/
 void neoe_mem_Envelop_Init(neoe_mem_Envelop* self, neoe_mem_Lv0* lv0, uint64_t start) {
		self->lv0 = lv0;
		self->start = start;
		neoe_mem_Envelop_checkSignature(self );
	}

	/*public*/
 uint64_t neoe_mem_Envelop_alignLow(neoe_mem_Envelop* self, uint64_t size) {
		return size >> neoe_mem_Lv0_alignInByteShift << neoe_mem_Lv0_alignInByteShift;
	}
static neoe_mem_Lv0 dummyLv0;
	/*private*/
void neoe_mem_Envelop_checkSignature(neoe_mem_Envelop* self) {
//	printf("Env start=%" PRIx64 "\n", self->start);
	if (self->start == 0 || neoe_mem_Envelop_getSignature(self) != neoe_mem_Envelop_Signature) {
		/*klog*/printf("[E]bad sig!\n"); exit(1);
		DummyLv0_Init(&dummyLv0);
		self->lv0 =  &dummyLv0;
	}
}
 
	/*public*/
 uint64_t neoe_mem_Envelop_getCurrentDataLen(neoe_mem_Envelop* self) {
		return neoe_mem_Envelop_getP2(self) - neoe_mem_Envelop_getP1(self);
	}

	/*public*/
 uint64_t neoe_mem_Envelop_getDataAddr(neoe_mem_Envelop* self) {
		return self->start + 56 + neoe_mem_Envelop_patch;
	}

	/*public*/
 uint64_t neoe_mem_Envelop_getMaxDataLen(neoe_mem_Envelop* self) {
		return self->lv0->read4(self->lv0,self->start + 32 + neoe_mem_Envelop_patch);
	}

	/*public*/
 uint64_t neoe_mem_Envelop_getNextPos(neoe_mem_Envelop* self) {
		return self->lv0->read4(self->lv0,self->start + 16 + neoe_mem_Envelop_patch);
	}

	/*public*/
 uint64_t neoe_mem_Envelop_getP1(neoe_mem_Envelop* self) {
		return self->lv0->read4(self->lv0,self->start + 40 + neoe_mem_Envelop_patch);
	}

	/*public*/
 uint64_t neoe_mem_Envelop_getP2(neoe_mem_Envelop* self) {
		return self->lv0->read4(self->lv0,self->start + 48 + neoe_mem_Envelop_patch);
	}

	/*public*/
 uint64_t neoe_mem_Envelop_getPrevPos(neoe_mem_Envelop* self) {
		return self->lv0->read4(self->lv0,self->start + 8 + neoe_mem_Envelop_patch);
	}

	/*private*/
 uint64_t neoe_mem_Envelop_getSignature(neoe_mem_Envelop* self) {
		return self->lv0->read4(self->lv0,self->start + 8);
	}

	/*public*/
 uint64_t neoe_mem_Envelop_getTotalSize1(neoe_mem_Envelop* self) {
		return self->lv0->read4(self->lv0,self->start);
	}

	/*public*/
 uint64_t neoe_mem_Envelop_getType(neoe_mem_Envelop* self) {
		return self->lv0->read4(self->lv0,self->start + 24 + neoe_mem_Envelop_patch);
	}

	/*public*/
 void neoe_mem_Envelop_reload(neoe_mem_Envelop* self, uint64_t p1) {
		self->start = p1;
		neoe_mem_Envelop_checkSignature(self);
	}

	/*private*/
 void neoe_mem_Envelop_setMaxDataLen(neoe_mem_Envelop* self, uint64_t v) {
		self->lv0->write4(self->lv0,self->start + 32 + neoe_mem_Envelop_patch, v);

	}

	/*public*/
 void neoe_mem_Envelop_setNextPos(neoe_mem_Envelop* self, uint64_t v) {
		self->lv0->write4(self->lv0,self->start + 16 + neoe_mem_Envelop_patch, v);

	}

	/*public*/
 void neoe_mem_Envelop_setP1(neoe_mem_Envelop* self, uint64_t v) {
		self->lv0->write4(self->lv0,self->start + 40 + neoe_mem_Envelop_patch, v);

	}

	/*public*/
 void neoe_mem_Envelop_setP2(neoe_mem_Envelop* self, uint64_t v) {
		self->lv0->write4(self->lv0,self->start + 48 + neoe_mem_Envelop_patch, v);
	}

	/*private*/
 void neoe_mem_Envelop_setTotalSize1(neoe_mem_Envelop* self, uint64_t v) {
		self->lv0->write4(self->lv0,self->start, v);
	}

	/*private*/
 void neoe_mem_Envelop_setTotalSize2(neoe_mem_Envelop* self, uint64_t v) {
		self->lv0->write4(self->lv0,self->start + neoe_mem_Envelop_getTotalSize1(self) - 8, v);
	}

	/*public*/
 void neoe_mem_Envelop_setType(neoe_mem_Envelop* self, uint64_t type) {
		self->lv0->write4(self->lv0,self->start + 24 + neoe_mem_Envelop_patch, type);
	}

	/*public*/
 void neoe_mem_Envelop_setSize(neoe_mem_Envelop* self, uint64_t size) {
		neoe_mem_Envelop_setTotalSize1(self, size);
		neoe_mem_Envelop_setTotalSize2(self, size);
		neoe_mem_Envelop_setMaxDataLen(self, size - neoe_mem_Envelop_OverheadSize);
	}

// }/*cls*/


